<?php

/**
 * @file
 * MERCI - Managed Equipment Reservation Checkout and Inventory
 */


/**
 * Implementation of hook_init().
 */
function merci_init() {
  drupal_add_css(drupal_get_path('module', 'merci') . '/merci.css');
}


/**
 * Implementation of hook_prepare().
 */
function merci_prepare(&$node) {
  if (!isset($node->merci_reservation_status)) {
    $node->merci_reservation_status = variable_get('merci_default_reservation_status', strval(MERCI_STATUS_UNCONFIRMED));
  }
}

/**
 * writes additional info to log to aid in troubleshoot configuration
 */
function merci_verbose_logging($string) {

  if (variable_get('merci_verbose_logging', 0)) {
    watchdog('merci', $string);
  }
}


function merci_validate_merci_selected_items($form, &$form_state) {
  $node = (object) $form_state['values'];

  $choices = $node->merci_reservation_items;

  $reservation_nid = $node->nid;

  // ****
  // Build date objects we'll need for our different validations.
  // ****
  $start              = $node->field_merci_date[0]['value'];
  $end                = $node->field_merci_date[0]['value2'];

  // Do this even for reservations with merci status of canceled and checked in.
  $choice_counts = array();
  $total_counts = array();
  $messages = array();
  $conflicts = array();
  foreach ($choices as $did => $item) {

    if (is_array($item)) {
      $value = $item['merci_item_nid'];
    }
    else {
      $value = $item;
    }

    if (is_numeric($did) and !$value) {
      $value = $item['type'];
    }

    if ($value) {
      // Only include active buckets, and content types the user
      // can reserve.
      $messages[$did] = '';
      if (is_numeric($value)) {
        $new_item = node_load($value);
        $title    = $new_item->title;
        $type     = $new_item->type;
      }
      else {
        $title = node_get_types('name', $value);
        $type = $value;
      }

      $content_settings = merci_load_item_settings($type);

      // Is this content type active?
      if ($content_settings->merci_active_status != MERCI_STATUS_ACTIVE) {
        $messages[$did] = '<div> ' . t("%name is not active.", array('%name' => $title)) . '</div>';
        continue;
      }

      // Does the user have access to manage reservations or this content type?
      if (!user_access('manage reservations') && !merci_check_content_type_user_permissions($type)) {
        $messages[$did] = '<div> ' . t("You do not have permission to reserve %name.", array('%name' => $title)) . '</div>';
        continue;
      }

      // Do we have the item available at this time?
      if (!isset($total_counts[$type])) {
        if (is_numeric($value)) {
          $item_options = merci_get_reservable_items($type, $start, $end, $reservation_nid);
          if (merci_type_setting($type) == 'bucket') {
            $total_counts[$type][$type] = $choice_counts[$type][$type] = merci_get_available_bucket_count($type, $start, $end, $node->vid) - $content_settings->merci_spare_items;
          }
          foreach ($item_options as $nid => $item_nid) {
            $total_counts[$type][$nid] = $choice_counts[$type][$nid] = 1;
          }
        }
        else {
          $total_counts[$type][$value] = $choice_counts[$type][$value] = merci_get_available_bucket_count($type, $start, $end, $node->vid) - $content_settings->merci_spare_items;
        }
      }
      if (!$choice_counts[$type][$value] and $total_counts[$type][$value]) {
        $messages[$did] = '<div> ' . t("You've selected too many %name's.  We only have %amount available at the times you've selected.", array('%name' => $title, '%amount' => $total_counts[$type][$value])) . '</div>';
      }
      elseif (!$choice_counts[$type][$value]) {
        $overdue_items_array = merci_overdue_items($type, $start, $reservation_nid = NULL);
        if (!empty($overdue_items_array)) {
          $messages[$did] = '<div> ' . t("%name is not available because it is still checked out by:", array('%name' => $title)) . '</div>';
          foreach ($overdue_items_array as $reservations) {
            foreach (array_keys($reservations) as $nid) {
              $overdue = node_load($nid);
              $messages[$did] .= '<div> ' . l($overdue->title, 'node/' . $overdue->nid) . '</div>';
            }
          }
        }
        else {
          //TODO: should not be doing theming here.
          $messages[$did] .= merci_theme_conflict_grid($type, $title, $start, $end, $value, $node->vid);
        }
      }
      else {
        //drupal_set_message(t('There are no time conflicts with this Reservation.'));
      }
      $choice_counts[$type][$value]--;
      if (is_numeric($value) ) {
        $choice_counts[$type][$type]--;
      }

      // Check item restrictions.  max hours, etc.
      $restrictions = merci_check_content_type_restrictions($type, $start, $end);

      if (!empty($restrictions)) {
        foreach ($restrictions as $restriction) {
          $messages[$did] .= '<div>' . t($restriction, array('%name' => $title)) . '</div>';
        }
      }
      if (!empty($messages[$did])) {
        form_set_error("merci_reservation_items][$did][merci_item_nid", $messages[$did]);
      }
    }
  }
  return $messages;
}

function merci_validate_merci_reservation_date($form, &$form_state) {
  $node = (object) $form_state['values'];

  // ****
  // Build date objects we'll need for our different validations.
  // ****
  $start              = $node->field_merci_date[0]['value'];
  $end                = $node->field_merci_date[0]['value2'];
  $start_object       = merci_create_local_date_object($start);
  $end_object         = merci_create_local_date_object($end);
  $hours_of_operation = merci_load_hours_of_operation();
  $start_day_of_week  = (int) date_format($start_object, 'w');
  $end_day_of_week    = (int) date_format($end_object, 'w');
  $start_month_day    = date_format($start_object, 'm-d');
  $end_month_day      = date_format($end_object, 'm-d');
  $start_hours        = $hours_of_operation[$start_day_of_week];
  $end_hours          = $hours_of_operation[$end_day_of_week];
  $start_date         = date_format($start_object, 'm-d-Y');
  $max_days           = variable_get("merci_max_days_advance_reservation", '0');

  // Hours of operation restrictions, max days, and closed dates checks
  // Users in role with Administer MERCI permssion or outside hours of operation skip these checks
  if (user_access('create reservations outside hours of operation')) {

    merci_verbose_logging('SKIP Hours of Operation Check, Max Days Check, and Closed Dates Check because user has create reservations outside hours of operation permission');

    //check to see if warning should be displayed
    if (strtotime(date('G:i', strtotime($start . ' UTC'))) < strtotime($start_hours['open']) || strtotime($start_hours['close']) < strtotime(date('G:i', strtotime($end . ' UTC')))) {

      drupal_set_message('<b>' . t('You are making a Reservation outside the normal hours of operation.  This may impact access to the items you are reserving.') . '</b>');
    }
  }
  else {
    // Reservation start date cannot exceed the max advance
    merci_verbose_logging('CHECKING Max Days');
    if ($max_days) {
      $max_date = new DateTime("+$max_days day");
      //$max_date = date('m-d-Y', mktime(0, 0, 0, date("m"), date("d")+$max_days, date("Y")));
      if ($start_object > $max_date) {
        form_set_error('field_merci_date][0][value][date', t('You cannot make a Reservation more than %days days in advance. Start the Reservation before %date.', array('%days' => $max_days, '%date' => date_format($max_date, 'm-d-Y'))));
      }
    }

    // Can't start or end a reservation on days that are
    // closed dates.
    merci_verbose_logging('CHECKING Closed Dates');
    if (in_array($start_month_day, $hours_of_operation['closed_days'])) {
      $name = date_format($start_object, 'F jS');
      form_set_error('field_merci_date][0][value][date', t('Sorry, but we are closed on %day for a holiday or special event.', array('%day' => $name)));
    }
    if (in_array($end_month_day, $hours_of_operation['closed_days'])) {
      $name = date_format($end_object, 'F jS');
      form_set_error('field_merci_date][0][value2][date', t('Sorry, but we are closed on %day for a holiday or special event.', array('%day' => $name)));
    }

    // Can't start or end a reservation on a day the facility
    // has no hours of operation, or outside hours of operation.
    merci_verbose_logging('CHECKING Hours of Operation');
    $start_name = date_format($start_object, 'l');
    if (!$hours_of_operation[$start_day_of_week]) {
      form_set_error('field_merci_date][0][value][date', t('Reservations cannot start on a %day.', array('%day' => $start_name)));
    }
    else {
      $start_time = date_format($start_object, 'H:i');
      if ($start_time < $start_hours['open']) {
        form_set_error('field_merci_date][0][value][time', t('Reservations cannot start at a time before %start.',
          array('%start' => merci_format_time($start_hours['open']))));
      }
      elseif ($start_time > $start_hours['close']) {
        form_set_error('field_merci_date][0][value][time', t('Reservations cannot start at a time after %end.',
          array('%end' => merci_format_time($start_hours['close']))));
      }
    }

    $end_name = date_format($end_object, 'l');
    if (!$hours_of_operation[$end_day_of_week]) {
      form_set_error('field_merci_date][0][value2][date', t('Reservations cannot end on a %day.', array('%day' => $end_name)));
    }
    else {
      $end_time = date_format($end_object, 'H:i');
      if ($end_time < $end_hours['open']) {
        form_set_error('field_merci_date][0][value2][time', t('Reservations cannot end at a time before %start.',
          array('%start' => merci_format_time($end_hours['open']))));
      }
      elseif ($end_time > $end_hours['close']) {

        form_set_error('field_merci_date][0][value2][time', t('Reservations cannot end at a time after %end.',
          array('%end' => merci_format_time($end_hours['close']))));
      }
    }
  } // Hours of operation restrictions, max days, and closed dates checks
}


function merci_validate_status($form, &$form_state) {
  $node = (object) $form_state['values'];
  // Reservations with a checked out status.
  if ($node->merci_reservation_status == MERCI_STATUS_CHECKED_OUT) {
    // Make sure all existing bucket reservations have an item assigned.
    if (empty($node->merci_reservation_items)) {
      form_set_error('merci_reservation_status', t('You can not finalize a reservation that has no reserved items.'));
    }
    else {
      foreach ($node->merci_reservation_items as $did => $item) {
        if ($item['merci_item_nid'] == "0") {
          form_set_error("merci_reservation_items][$did][merci_item_nid", t("The reservation for %title must have an item associated with it for finalized reservations.", array('%title' => $item['name'])));
        }
        // Can't add a bucket item and finalize at the same time.
        if (!is_numeric($item['merci_item_nid']) and strlen($item['merci_item_nid'])) {
          form_set_error("merci_reservation_items][$did][merci_item_nid", t("You cannot finalize a reservation while adding a bucket item."));
        }
      }
    }
  }
  // Prevent status changes on reservations that have past.
  $current_status = $node->merci_original_reservation_status;
  if ($current_status && $current_status != $node->merci_reservation_status && time() > strtotime($node->field_merci_date[0]['value2']) && !in_array((int) $node->merci_reservation_status, array(MERCI_STATUS_CANCELLED, MERCI_STATUS_CHECKED_IN, MERCI_STATUS_DENIED))) {
    $statuses = merci_record_status();
    form_set_error('merci_reservation_status', t('You cannot change the status to %status for a reservation that has past.', array('%status' => $statuses[$node->merci_reservation_status])));
  }
}

function merci_validate_empty_reservation_items($form, &$form_state) {
  $node = (object) $form_state['values'];

  $choices = $node->merci_reservation_items;

  $unselected = 0;

  foreach ($choices as $did => $item) {
    if (is_array($item)) {
      $value = $item['merci_item_nid'];
    }
    else {
      $value = $item;
    }

    if (is_numeric($did) and !$value) {
      $value = $item['type'];
    }

    if (!$value) {
      $unselected++;
    }
  }
  if ($unselected == count($choices)) {
    $first = array_shift(array_keys($choices));
    form_set_error("merci_reservation_items][$first][merci_item_nid", t("You cannot create a reservation without any items selected."));
  }
}
/**
 * Implementation of hook_validate().
 */
function merci_node_validate($form, &$form_state) {

  if (!empty($form_state['ahah_submission'])) {
    return;
  }

  // No validation necessary on deletion.
  if ($form_state['clicked_button']['#id'] == 'edit-delete') {
    return;
  }

  // Do no validation if their errors from the main validation function.
  if (form_get_errors()) {
    return;
  }

  merci_validate_status($form, $form_state);
  merci_validate_merci_reservation_date($form, $form_state);

  // Tests for existing items.

  //merci_validate_existing_items($form, $form_state);
  merci_validate_empty_reservation_items($form, $form_state);
  merci_validate_merci_selected_items($form, $form_state);
  //merci_validate_merci_choices($form, $form_state);

}


/**
 * Implementation of hook_insert().
 */
function merci_insert($node) {
  if ($node->type == 'merci_reservation') {
    drupal_write_record("merci_reservation", $node);
    merci_add_reservation_items($node);
  }
}

/**
 * Implementation of hook_update().
 */
function merci_update($node) {
  if ($node->type == 'merci_reservation') {
    if ($node->revision) {
      drupal_write_record("merci_reservation", $node);
    }
    else {
      drupal_write_record("merci_reservation", $node, "vid");
    }
    merci_add_reservation_items($node);
  }

}

/**
 * Implementation of hook_delete().
 */
function merci_delete($node) {
  if ($node->type == 'merci_reservation') {

    if ($node->merci_reservation_status == MERCI_STATUS_CHECKED_OUT) {
      foreach ($node->merci_reservation_items as $item) {
        $update = array(
          'nid' => $item['merci_item_nid'],
          'merci_item_status' => MERCI_ITEM_STATUS_AVAILABLE,
        );
        drupal_write_record('merci_reservation_item_node', $update, 'nid');
        node_delete($item['merci_placeholder_nid']);
      }
    }

    merci_delete_record('merci_reservation', $node, 'nid');
    merci_delete_record('merci_reservation_detail', $node, 'nid');

  }
}

/**
 * Implementation of hook_view().
 */
function merci_view($node, $teaser = FALSE, $page = FALSE) {
  // TODO: should we fix node previews?
  if ($node->type == 'merci_reservation' && !isset($node->preview)) {
    $node->content['merci_reservation_status'] = array(
      '#value' => drupal_get_form('merci_display_reservation_status', merci_record_status($node->merci_reservation_status)),
      '#weight' => 0,
    );
    if ($page) {
      $reservation_table = drupal_get_form('merci_build_reservation_table_form', $node);
      $node = node_prepare($node, $teaser);
      $node->content['merci_reservation_items'] = array(
        '#value' => $reservation_table,
        '#weight' => 1,
      );
    }
  }
  return $node;
}

/**
 * Implementation of hook_form().
 */
function merci_form(&$node, $form_state) {

  merci_check_default_timezone();
  $form = node_content_form($node, $form_state);

  if (isset($form_state['node'])) {
    $node = $form_state['node'] + (array) $node;
  }
  $node = (object) $node;

  // Add a wrapper for the choices and more button.
  $form['choice_wrapper'] = array(
    '#tree' => FALSE,
    '#prefix' => '<div class="clear-block" id="merci-choice-wrapper">',
    '#suffix' => '</div>',
  );

  // Build existing reserved items table on existing reservations.
  //if (isset($node->nid)) {
  $form['choice_wrapper']['merci_reservation_items'] = merci_build_reservation_table_form($form_state, $node, TRUE);
  //}

  // Choice adding code mostly stolen from poll module.
  $choice_count = empty($node->choice_count) ? 3 : $node->choice_count;

  if ($form_state['post']['op'] == "Add more items") {
    $choice_count += 3;
  }

  $form['choice_wrapper']['choice_count'] = array(
    '#type' => 'value',
    '#value' => $choice_count,
  );

  // Add the current choices to the form.
  for ($delta = 1; $delta <= $choice_count; $delta++) {
    $default = isset($node->merci_reservation_items["choice_" . $delta]['merci_item_nid']) ?
      $node->merci_reservation_items["choice_" . $delta]['merci_item_nid'] : '';

    $form['choice_wrapper']['merci_reservation_items']["choice_" . $delta]['merci_item_nid'] =
      _merci_choice_form($node, $form_state, $delta, $default);
  }

  $options = array();
  for ($i = 1; $i < 20; $i++) {
    $options[$i] = $i;
  }

  // We name our button 'merci_more' to avoid conflicts with other modules using
  // AHAH-enabled buttons with the id 'more'.
  $form['choice_wrapper']['merci_more'] = array(
    '#type' => 'submit',
    '#value' => t('Add more items'),
    '#description' => t("If the number of items above isn't enough, click here to add more items."),
    '#weight' => 1,
    '#submit' => array('merci_more_choices_submit'),
    // If no javascript action.
    '#ahah' => array(
      'path' => 'merci/js',
      'wrapper' => 'merci-choice-wrapper',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );

  if (user_access('manage reservations')) {
    $form['merci_reservation_status'] = array(
      '#title' => t('Status'),
      '#type' => 'radios',
      '#options' => merci_record_status(),
      '#default_value' => $node->merci_reservation_status,
      '#description' => t('Finalized bookings can not have time conflicts with each other.'),
    );
  }
  else {
    $form['merci_reservation_status'] = array(
      '#type' => 'value',
      '#value' => $node->merci_reservation_status,
    );
  }
  $form['merci_original_reservation_status'] = array(
    '#type' => 'value',
    '#value' => $node->merci_original_reservation_status ? $node->merci_original_reservation_status : $node->merci_reservation_status,
  );

  // Since hook_validate is broken in 6.x, we add our own
  // custom validation here.
  // TODO check if this fixed.
  $form['#validate'][] = 'merci_node_validate';

  $form['#cache'] = TRUE; // Make sure the form is cached.

  // Pull the correct action out of form_state if it's there to avoid AHAH+Validation action-rewrite.
  if (isset($form_state['action'])) {
    $form['#action'] = $form_state['action'];
  }

  return $form;
}

/**
 * Implementation of hook_form_alter().
 */
function merci_form_alter(&$form, $form_state, $form_id) {

  // Node add/edit forms.

  $type = $form['type']['#value'];

  switch ($form_id) {
    // Node settings form.

    case $type . '_node_form':
      if (merci_is_merci_type($type)) {

        $node = (object) $form['#node'];

        $sub_type = $node->merci_sub_type ? $node->merci_sub_type : MERCI_SUB_TYPE_ITEM;
        $default_availability = $node->merci_default_availability ? $node->merci_default_availability : MERCI_AVA_F;

        if ($sub_type == MERCI_SUB_TYPE_ITEM) {
          if (empty($form['merci'])) {
            $form['merci'] = array(
              '#type' => 'fieldset',
              '#title' => t('MERCI settings'),
              '#collapsible' => TRUE,
              '#collapsed' => TRUE,
            );
          }
          $form['merci']['merci_default_availability'] = array(
            '#title' => t('Default booking availability'),
            '#type' => 'radios',
            '#options' => merci_item_status(),
            '#description' => t('If no availability information is defined for a given time, the resource falls back onto this setting.'),
            '#default_value' => $default_availability,
            '#element_validate' => array('merci_validate_default_availability'),
          );
        }
        $form['merci_sub_type'] = array(
          '#type' => 'value',
          '#value' => $sub_type,
        );

        merci_add_settings_form($form, $form_state);
      }
      else if ($type == 'merci_reservation') {
        $form['#after_build'][] = '_merci_after_build';
      }
      break;

    case 'node_type_form':
      // Reservation content type can't used for other MERCI functionality.
      if (isset($form['#node_type']->type) && $form['#node_type']->type == 'merci_reservation') {
        return;
      }
      $type     = $form['old_type']['#value'];

      // If any nodes have already been created, lock the type setting.
      if ($type) {
        $nodes = merci_nodes_for_type_count($type);
        $settings = merci_load_item_settings($type);
      }

      if (empty($settings) and $nodes) {
        return;
      }

      $warning  = '<div>' . t('<strong> WARNING:</strong> changing this setting has no effect on existing reserved items.') . '</div>';

      $options  = array(
        'disabled' => t('Disabled'),
        'bucket' => t('Bucket'),
        'resource' => t('Resource'),
      );

      $form['merci']       = array(
        '#type' => 'fieldset',
        '#title' => t('MERCI settings'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );

      // If any nodes have already been created, lock the type setting.
      if ($nodes) {
        $form['merci']['merci_type_setting'] = array(
          '#type' => 'value',
          '#value' => $settings->merci_type_setting,
        );
        $form['merci']['merci_type_setting_display'] = array(
          '#type' => 'item',
          '#title' => t('Reservable item type'),
          '#value' => $options[$settings->merci_type_setting],
          '#description' => t('The setting can not be changed because content already exists for this type.'),
        );
      }
      else {
        $description_items = array(
          t('Resource:  Use this content type to create unique items that can be reserved.'),
          t('Bucket:  Use this content type to create interchangable items that can be reserved (ex. Camera). Buckets reference interchangable items. The actual item does not have to be chosen until the reservation is checked out.'),
        );
        $form['merci']['merci_type_setting'] = array(
          '#type' => 'radios',
          '#title' => t('Reservable item type'),
          '#options' => $options,
          '#default_value' => $settings ? $settings->merci_type_setting : 'disabled',
          '#description_items' => $description_items,
          '#theme' => 'merci_type_setting',
        );
      }

      $status = array(
        MERCI_STATUS_ACTIVE => t('Active'),
        MERCI_STATUS_INACTIVE => t('Inactive'),
      );
      $form['merci']['merci_active_status'] = array(
        '#type' => 'radios',
        '#title' => t('Status'),
        '#options' => $status,
        '#default_value' => isset($settings->merci_active_status) ? intval($settings->merci_active_status) : MERCI_STATUS_ACTIVE,
        '#description' => t('Set to active to allow this type to be reserved.'),
      );

      // This setting is only valid for buckets.
      if (!isset($settings->merci_type_setting) || $settings->merci_type_setting == 'bucket') {
        $form['merci']['merci_spare_items'] = array(
          '#type' => 'textfield',
          '#title' => t('Spare items'),
          '#size' => 10,
          '#default_value' => $settings ? $settings->merci_spare_items : 0,
          '#element_validate' => array('merci_is_numeric_validate'),
          '#description' => filter_xss(t("Set this to the number of items of this type that should always be unavailable and thus unreservable.  This way you'll still have enough items for future reservations in case something breaks.") . $warning),
        );
        $form['merci']['merci_auto_assign_bucket_item'] = array(
          '#type' => 'checkbox',
          '#title' => t('Automatically assign a bucket item'),
          '#default_value' => $settings ? $settings->merci_auto_assign_bucket_item : 0,
          '#description' => t('Automatically assign the best fit bucket item when reserving a new bucket item.'),
        );
      }

      $vid = variable_get('merci_equipment_grouping_vid', 0);

      $form['merci']['merci_grouping'] = taxonomy_form(
      $vid,
      variable_get('merci_grouping_' . $type, 0),
      t('This will alter order the content types are displayed to users reserving items from buckets.  Terms added to the MERCI Equipment Groupings taxonomy will appear here.'),
      t('Grouping')
    );

      $form['merci']['merci_max_hours_per_reservation'] = array(
        '#type' => 'textfield',
        '#title' => t('Maximum hours per reservation'),
        '#size' => 10,
        '#default_value' => $settings ? $settings->merci_max_hours_per_reservation : 0,
        '#element_validate' => array('merci_is_numeric_validate'),
        '#description' => filter_xss(t('The maximum hours the item can be reserved for in one reservation. Set to zero for no limit.') . $warning),
      );
      $form['merci']['merci_allow_overnight'] = array(
        '#type' => 'checkbox',
        '#title' => t('Allow overnight reservation'),
        '#default_value' => $settings ? $settings->merci_allow_overnight : 0,
        '#description' => filter_xss(t('Allow a reservation to continue over multiple days.  If this is not checked, items in this content type must be returned before the checkout closes.') . $warning),
      );
      $form['merci']['merci_allow_weekends'] = array(
        '#type' => 'checkbox',
        '#title' => t('Allow weekend reservation'),
        '#default_value' => $settings ? $settings->merci_allow_weekends : 0,
        '#description' => filter_xss(t('Allow a reservation to be made over days defined as weekend.') . $warning),
      );
      merci_add_settings_form($form, $form_state);

      $form['#validate'][] = 'merci_node_type_save_validate';
      $form['#submit'][]   = 'merci_node_type_save_submit';

      break;

    case 'node_delete_confirm':
      $node = node_load((int) arg(1));
      if (!merci_delete_item_validate($node)) {
        unset($form['actions']['submit']);
      }
      break;

    case 'node_type_delete_confirm':
      $type = str_replace('-', '_', arg(3));
      merci_delete_node_type_validate($form);
      break;

    case 'node_admin_content':
      if (!isset($form['#validate'])) {
        $form['#validate'] = array();
      }
      $form['#validate'][] = 'merci_node_admin_delete_validate';
      break;
  }
}
/**
 * Implementation of hook_content_extra_fields.
 */
function merci_content_extra_fields() {
  $extras['merci'] = array(
    'label' => t('MERCI Settings'),
    'description' => t('Allows user to select Reservation status.'),
    'weight' => 100,
  );
  $extras['choice_wrapper'] = array(
    'label' => t('MERCI Choices'),
    'description' => t('Child items included in the Reservation.'),
    'weight' => 80,
  );
  return $extras;
}

/**
 * Implementation of hook_node_operations().
 */
function merci_node_operations($return = NULL) {
  $operations = array(
    'merci_update' => array(
      'label' => t('Confirm Reservation(s)'),
      'callback' => 'merci_operations_update',
    ),
  );
  return $operations;
}

/**
 * Implementation of hook_token_list().
 *
 */
function merci_token_list($type = 'all') {
  if ($type == 'node' || $type == 'all') {
    //@TODO: Fix This Token
    //$tokens['node']['merci_resources'] = t('Reserved resource');
    return $tokens;
  }
}

/**
 * Implementation of hook_token_values().
 * @see {merci_token_list}
 */
function merci_token_values($type, $object = NULL, $options = array()) {
  switch ($type) {
    case 'node':
      $node = merci_load($object);
      if ($node) {
        $values['merci_resources'] = '';

        // We want these timestamps generated in UTC.
        $old_timezone = date_default_timezone_get();
        date_default_timezone_set('UTC');
        $starthour = strtotime($node->field_merci_date[0]['value']);
        $endhour = strtotime($node->field_merci_date[0]['value2']);
        date_default_timezone_set($old_timezone);
        $hours = round(($endhour - $starthour) / 3600, 2);
        $titles = array();
        foreach ($node->merci_reservation_items as $item) {
          $item_node = node_load($item['merci_placeholder_nid']);
          if ($item['item_title'] != '') {
            $titles[] = $item['item_title'];
          }
          else {
            $titles[] = $item['merci_placeholder_title'];
          }
        }

        $values['merci_resources'] = check_plain(implode(", ", $titles));
        return $values;
      }
  }
}

/**
 * Implementation of hook_views_api().
 */
function merci_views_api() {
  return array(
    'api' => 2,
    'path' => drupal_get_path('module', 'merci'),
  );
}

/**
 * Implementation of hook_views_handlers().
 */
function merci_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'merci') . '/handlers',
    ),
    'handlers' => array(
      'views_handler_field_item_node_nid' => array(
        'parent' => 'views_handler_field_prerender_list',
        'file' => 'views_handler_field_item_node_nid.inc',
      ),
      'merci_handler_field_merci_node_type_type_setting' => array(
        'parent' => 'views_handler_field',
        'file' => 'merci_handler_field.inc',
      ),
      'merci_handler_filter_merci_node_type_type_setting' => array(
        'parent' => 'views_handler_filter_in_operator',
        'file' => 'merci_handler_filter_in_operator.inc',
      ),
      'merci_handler_field_merci_node_type_status' => array(
        'parent' => 'views_handler_field',
        'file' => 'merci_handler_field.inc',
      ),
      'merci_handler_filter_merci_node_type_status' => array(
        'parent' => 'views_handler_filter_in_operator',
        'file' => 'merci_handler_filter_in_operator.inc',
      ),
      'merci_handler_field_merci_reservation_status' => array(
        'parent' => 'views_handler_field',
        'file' => 'merci_handler_field.inc',
      ),
      'merci_handler_filter_merci_reservation_status' => array(
        'parent' => 'views_handler_filter_in_operator',
        'file' => 'merci_handler_filter_in_operator.inc',
      ),
      'merci_handler_field_merci_bucket_resource_node_default_availability' => array(
        'parent' => 'views_handler_field',
        'file' => 'merci_handler_field.inc',
      ),
      'merci_handler_filter_merci_bucket_resource_node_default_availability' => array(
        'parent' => 'views_handler_filter_in_operator',
        'file' => 'merci_handler_filter_in_operator.inc',
      ),
      'merci_handler_field_merci_bucket_resource_node_sub_type' => array(
        'parent' => 'views_handler_field',
        'file' => 'merci_handler_field.inc',
      ),
      'merci_handler_filter_merci_bucket_resource_node_sub_type' => array(
        'parent' => 'views_handler_filter_in_operator',
        'file' => 'merci_handler_filter_in_operator.inc',
      ),
      // filter
      'merci_views_handler_filter_reserved_item_nid' => array(
        'parent' => 'views_handler_filter_many_to_one',
      ),
      'merci_handler_field_merci_reservation_item_node_item_status' => array(
        'parent' => 'views_handler_field',
        'file' => 'merci_handler_field.inc',
      ),
      'merci_handler_filter_merci_reservation_item_node_item_status' => array(
        'parent' => 'views_handler_filter_in_operator',
        'file' => 'merci_handler_filter_in_operator.inc',
      ),
    ),
  );
}

/**
 * Submit handler to add more choices to a reservation form. This handler is used when
 * javascript is not available. It makes changes to the form state and the
 * entire form is rebuilt during the page reload.
 */
function merci_more_choices_submit($form, &$form_state) {
  // Set the form to rebuild and run submit handlers.
  node_form_submit_build_node($form, $form_state);

}

/**
 * Submit handler to date filter items on a reservation form.
 * It makes changes to the form state and the entire form is
 * rebuilt during the page reload.
 */
function merci_date_filter($form, &$form_state) {
  // Set the form to rebuild and run submit handlers.
  node_form_submit_build_node($form, $form_state);
}

/**
 * Builds the list of all currently reservable items, filtered by date.
 *
 * @param $node
 *   The reservation node object.
 * @param $form_state
 *   Current form state array.
 * @param $reservation_nid
 *   (Optional) The nid of a reservation to ignore in the options exclusions.
 *
 * @return
 *   An associative array with the following key/value pairs:
 *     'options'      => An array of available items, in the format used
 *                       for the item selector.
 */
function merci_build_reservable_items($node, $form_state, $reservation_nid = NULL) {

  // Newly set dates take precedence.
  if (isset($form_state['values']['field_merci_date'])) {
    $start = $form_state['values']['field_merci_date'][0]['value'];
    $end = $form_state['values']['field_merci_date'][0]['value2'];
  }
  // Dates loaded from the reservation are next.
  elseif (isset($node->nid)) {
    $date_info = $node->field_merci_date[0];
    $start     = $date_info['value'];
    $end       = $date_info['value2'];
  }
  // New reservation, so no date filtering.
  else {
    $is_new = TRUE;
  }

  $options = array();
  $options['options'] = array('' => t('<Select>'));

  $merci_types = merci_content_types();
  $bucket_options = array();

  // This array holds all reservable items the user may reserve.
  // Loop through each bucket type.

  foreach ($merci_types as $type => $value) {
    if ($value['merci_active_status'] != MERCI_STATUS_ACTIVE) {
      continue;
    }
    if (!merci_check_content_type_user_permissions($type)) {
      continue;
    }
    if (empty($is_new)) {
      $restrictions = merci_check_content_type_restrictions($type, $start, $end);
      if (!empty($restrictions)) {
        continue;
      }
    }
    if ($value['merci_type_setting'] == 'bucket') {
      // Check for available items in the bucket.
      $available_bucket_items = merci_get_available_bucket_count($type, $start, $end, $reservation_nid)
        - $value['merci_spare_items'];

      if ($available_bucket_items) {
        $bucket_options[$type] = $value['type_name'];
      }
    }
    else if ($value['merci_type_setting'] == 'resource') {
      // No date filtering for new reservations.
      $item_options = merci_get_reservable_items($type, $start, $end, $reservation_nid);

      if (!empty($item_options)) {
        $options['options'][$value['type_name']] = $item_options;
      }
    }
  }

  // FIX - NEED TO CHANGE FORMATTING HERE
  if ($bucket_options) {
    $options['options'][t('Buckets')] = $bucket_options;
  }

  return $options;
}

/**
 * Checks for reservation restrictions for a content type.
 *
 * These include maximum hours per reservation, and if the bucket/resource
 * is reservable overnight and/or on weekends.
 *
 * @param $content_type
 *   The content type to be checked.
 * @param $start
 *   The start date of the reservation in DATETIME format and UTC timezone.
 * @param $end
 *   The end date of the reservation in DATETIME format and UTC timezone.
 *
 * @return
 *   An array of warning messages for any restrictions found.
 */


function merci_check_content_type_restrictions($content_type, $start, $end) {
  if (!user_access("manage reservations")) {

    //TODO I don't like this.
    $type_settings = merci_load_item_settings($content_type);
    $return = array();

    // Convert start/end dates to local time.
    // TODO clean this up.
    $start_object = merci_create_local_date_object($start);
    $end_object = merci_create_local_date_object($end);

    // We want these timestamps generated in UTC.
    $old_timezone = date_default_timezone_get();
    date_default_timezone_set('UTC');
    $start_timestamp = strtotime($start);
    $end_timestamp = strtotime($end);
    date_default_timezone_set($old_timezone);

    $reserved_hours    = ($end_timestamp - $start_timestamp) / (60 * 60);
    $start_day_of_week = date_format($start_object, 'w');
    $end_day_of_week   = date_format($end_object, 'w');

    // Make sure max hours aren't exceeded.
    if ($type_settings->merci_max_hours_per_reservation && $reserved_hours > $type_settings->merci_max_hours_per_reservation) {

      // Override max_hours_per_reservation if we can reserve this over the weekend

      // Validate allow_weekend.
      if (user_access('override max hours over closed days') || $type_settings->merci_allow_weekends) {
        $closed_days = array();
        // Do we allow extending this reservation over days checked as a weekend in addition to days we are closed?
        if ($type_settings->merci_allow_weekends) {
          $i = 0;
          foreach (array('sunday', 'monday', 'tuesday', 'wednesday', 'thursday', 'friday', 'saturday') as $day) {
            if (variable_get('merci_' . $day . '_is_weekend', 0)) {
              $closed_days[$i] = TRUE;
            }
            $i++;
          }
        }
        // Do we allow extending a reservtion over days we are closed?
        if (user_access('override max hours over closed days')) {
          $hours_of_operation = merci_load_hours_of_operation($content_type);
          for ($i = 1; $i <= 6; $i++) {
            if (empty($hours_of_operation[$i])) {
              $closed_days[$i] = TRUE;
            }
          }
        }

        // Only extend if the following day is closed/weekend.
        // TODO check that the end time is not the same day and within the hours of being open.
        if (!$closed_days[date('w', $start_timestamp + 86400)]) {
          $return[] = t('%name cannot be reserved for more than %hours hours.', array('%hours' => $type_settings->merci_max_hours_per_reservation));
        }

        // Only extend the max time if the default max time falls on a weekend.
        if ($closed_days[date('w', $start_timestamp + $type_settings->merci_max_hours_per_reservation * 60 * 60)]) {

          //Find the next day we are open.
          for ($i = 1; $i <= 6; $i++) {
            if (!$closed_days[date('w', $start_timestamp + $i * 86400)]) {

              // Does the end_day fall here?
              // TODO force time to be exactly when open.
              if ($end_day_of_week != date('w', $start_timestamp + $i * 86400)) {
                $return[] = t('%name cannot be reserved more then one day after a weekend.', array('%hours' => $type_settings->merci_max_hours_per_reservation));
              }
              break;
            }
          }
        }
      }
      else {
        $return[] = t('%name cannot be reserved for more than %hours hours.', array('%hours' => $type_settings->merci_max_hours_per_reservation));
      }
    }

    // Validate allow_overnight.
    if (!$type_settings->merci_allow_overnight) {
      // Need the 48 hour check in case somebody starts and ends their
      // reservation on the same day.
      if ($start_day_of_week != $end_day_of_week || $reserved_hours > 48) {
        $return[] = t('%name cannot be reserved overnight.');
      }
    }
  }
  return $return;
}
// merci_check_content_type_restrictions

/**
 * Ensures the user has 'edit own [type] content' and 'delete own [type] content'
 * permissions, otherwise they are not allowed to reserve the content type.
 *
 * @return TRUE if the user has access to reserve the content type, FALSE
 *   otherwise.
 */
function merci_check_content_type_user_permissions($type) {
  return user_access("edit own $type content") && user_access("delete own $type content");
}

function merci_is_merci_type($type) {
  return merci_type_setting($type) != 'disabled';
}

function merci_type_setting($type) {
  return variable_get('merci_type_setting_' . $type, 'disabled');
}
/**
 * Return a list of all merci content types.
 *
 * @param $content_type_name
 *   If set, return information on just this type.
 *
 * Do some type checking and set up empty arrays for missing
 * info to avoid foreach errors elsewhere in the code.
 */

function merci_content_types($type_name = NULL) {
  // handle type name with either an underscore or a dash
  $type_name = !empty($type_name) ? str_replace('-', '_', $type_name) : NULL;

  $info = _merci_content_type_info();
  if (!isset($type_name)) {
    return $info;
  }
  else {
    return $info[$type_name];
  }
}

function merci_convert_date_popup($dates, $date_format   = 'm/d/Y g:ia') {
  module_load_include('inc', 'date_api', 'date_api_elements');

  $date_timezone = date_default_timezone_name();
  $start         = array(
    '#value' => array(
      'date' => $dates['value']['date'],
      'time' => $dates['value']['time'],
    ),
    '#date_timezone' => $date_timezone,
    '#date_format' => $date_format,
  );
  $end = array(
    '#value' => array(
      'date' => $dates['value2']['date'],
      'time' => $dates['value2']['time'],
    ),
    '#date_timezone' => $date_timezone,
    '#date_format' => $date_format,
  );
  $start = date_popup_input_value($start);
  $end = date_popup_input_value($end);
  $start = date_make_date($start);
  $end   = date_make_date($end);
  date_timezone_set($start, timezone_open('UTC'));
  date_timezone_set($end, timezone_open('UTC'));
  $start = date_convert($start, DATE_OBJECT, DATE_DATETIME);
  $end = date_convert($end, DATE_OBJECT, DATE_DATETIME);
  return array('value' => $start, 'value2' => $end);
}

function merci_node_type_status($code = NULL) {
  $statuses = array(
    MERCI_STATUS_ACTIVE => t('Active'),
    MERCI_STATUS_INACTIVE => t('Inactive'),
  );
  if (isset($code)) {
    return $statuses[$code];
  }
  else {
    return $statuses;
  }
}

/**
 * Return the name of a type code.
 *
 * @param string|int $code
 *  if int, will return translated name of the code.
 *  if NULL, returns array of codes as keys, and translated strings as value
 *
 * @return string|int
 */
function merci_item_status($code = NULL) {
  $statuses = array(
    MERCI_AVA_F => t('Available'),
    MERCI_UNA_F => t('Unavailable'),
    MERCI_AVA_T => t('Template Only'),
    MERCI_UNA_S => t('No Longer in Inventory'),
  );
  if (isset($code)) {
    return $statuses[$code];
  }
  else {
    return $statuses;
  }
}


function merci_item_reservation_status($code = NULL) {
  // Item status for reservations.
  $statuses = array(
    MERCI_ITEM_STATUS_AVAILABLE => t('Available'),
    MERCI_ITEM_STATUS_CHECKED_OUT => t('Checked Out'),
  );
  if (isset($code)) {
    return $statuses[$code];
  }
  else {
    return $statuses;
  }
}
/**
 * Return the name of a status code.
 *
 * @param string|int $code
 *  if int, will return translated name of the code.
 *  if NULL, returns array of codes as keys, and translated strings as value
 *
 * @return string|int
 */
function merci_record_status($code = NULL) {
  $types = array(
    MERCI_STATUS_UNCONFIRMED => t('Unconfirmed'),
    MERCI_STATUS_PENDING => t('Confirmed'),
    MERCI_STATUS_CHECKED_OUT => t('Checked out'),
    MERCI_STATUS_CHECKED_IN => t('Checked in'),
    MERCI_STATUS_CANCELLED => t('Cancelled'),
    MERCI_STATUS_DENIED => t('Denied'),
    MERCI_STATUS_DENIED => t('No Show'),
  );
  if (isset($code)) {
    return $types[$code];
  }
  else {
    return $types;
  }
}

/**
 * Submit handler for saving MERCI node type data.
 */
function merci_node_type_save_submit($form, &$form_state) {
  if ($form_state['clicked_button']['#value'] == t('Save content type')) {
    $settings = $form_state['values'];
    //$existing = db_result(db_query("SELECT type FROM {merci_node_type} WHERE type = '%s'",$settings['type']));
    $existing = merci_node_type_existing($settings['type']);
    if (!$existing) {
      $return = drupal_write_record('merci_node_type', $settings);
    }
    else {
      $return = drupal_write_record('merci_node_type', $settings, 'type');
    }
    cache_clear_all('merci_' . $settings['type'] . '_data', 'cache');
    cache_clear_all('merci_content_type_info', 'cache');
    // This hack is necessary because the node type form submit
    // automatically saves all remaining form items to {variable}
    // We're doing custom storage, so remove these.
    // Don't delete merci_type_setting_

    $variables = array(
      'merci_max_hours_per_reservation_',
      'merci_allow_overnight_',
      'merci_allow_weekends_',
      'merci_late_fee_per_hour_',
      'merci_rate_per_hour_',
      'merci_fee_free_hours_',
      'merci_status_',
      'merci_spare_items_',
      'merci_min_cancel_hours_',
      'merci_autocheckout_',
      'merci_autocheckin_',
      'merci_selfcheckout_',
    );
    foreach ($variables as $variable) {
      variable_del($variable . $settings['type']);
    }
  }

}

/**
 * Validation for numeric textfields.
 */
function merci_is_numeric_validate($form) {
  if ($form['#value'] && !is_numeric($form['#value'])) {
    form_set_error($form['#name'], t('%title must be a number.', array('%title' => $form['#title'])));
  }
}

function merci_build_accessory_form($form_state, $node, $did) {

  $vocabularies = taxonomy_get_vocabularies($node->type);

  $form = array();

  foreach ($node->taxonomy as $tid => $term) {
    $value[] = $tid;
  }
  foreach ($vocabularies as $vocabulary) {

    $taxonomy_form   = taxonomy_form($vocabulary->vid, $value);
    $taxonomy_form['#title'] = '';
    unset($taxonomy_form['#theme']);
    $taxonomy_form['#ahah'] = array(
      'path' => 'merci/taxonomy/' . $node->nid . '/' . $vocabulary->vid . '/' . $did,
      'wrapper' => 'merci-accessories-' . $node->nid,
      'method' => 'prepend',
      'effect' => 'fade',
    );
    $form[$vocabulary->vid] = $taxonomy_form;
  }
  return $form;
}

/**
 * Pulls items available to assign to a bucket for a reservation.
 *
 * @param $node
 *   The reservation node.
 * @param $bucket_type
 *   The bucket type.
 *
 * @return
 *   An array of available items, in select options format.
 */
function merci_get_available_bucket_items($node, $bucket_type) {

  $date_info = $node->field_merci_date[0];
  $start     = $date_info['value'];
  $end       = $date_info['value2'];

  $options = merci_get_reservable_items($bucket_type, $start, $end, $node->nid);

  return $options;
}

function merci_get_suggested_bucket_item($content_type, $start, $end, $items = array()) {
  $total_items_array = merci_reserved_bucket_items($content_type, $start, $end );

  foreach ($total_items_array as $item_nid => $node) {
    if (empty($total_items_array[$item_nid]) && !in_array($item_nid, $items)) {
      return $item_nid;
    }

  }

  return 0;
}

/**
 * Builds an array representing the hours of operation for the facility.
 *
 * @return
 *   An associative array with the following key/value pairs:
 *     [php_day_of_week_number_as_in_date_function] => An associative
 *       array with the following key/values pairs:
 *         'open'  => Opening time (military).
 *         'close' => Closing time (military).
 *     'closed_days' => An array of closed dates in mm-dd format.
 */
function merci_load_hours_of_operation($content_type = '') {

  $days_of_the_week = array(
    'sun',
    'mon',
    'tue',
    'wed',
    'thu',
    'fri',
    'sat',
  );

  $hours_of_operation = array();
  foreach ($days_of_the_week as $num => $day) {
    $hours = variable_get("merci_hours_$day", '');
    if (drupal_strlen($hours) == 11) {
      $parts = explode('-', $hours);
      if (count($parts == 2)) {
        $hours_of_operation[$num] = array(
          'open' => $parts[0],
          'close' => $parts[1],
        );
      }
      else {
        $hours_of_operation[$num] = FALSE;
      }
    }
    else {
      $hours_of_operation[$num] = FALSE;
    }
  }

  $closed_days_raw = variable_get('merci_closed_dates', '');
  $hours_of_operation['closed_days'] = array();
  $parts = explode("\n", $closed_days_raw);
  foreach ($parts as $date) {
    $date = trim($date);
    if (drupal_strlen($date) == 5) {
      $hours_of_operation['closed_days'][] = $date;
    }
  }

  return $hours_of_operation;
}

function merci_hours_str_to_array($str) {

  if (drupal_strlen($str) == 11) {
    $parts = explode('-', ($str));
    if (count($parts) == 2) {
      return array(
        'open' => $parts[0],
        'close' => $parts[1],
      );
    }
  }

  return FALSE;
}
// merci_hours_str_to_array

function merci_check_default_timezone() {
  $default = variable_get('date_default_timezone_name', '');
  if (empty($default)) {
    drupal_set_message(t('No site wide default timezone configured.  Please <a href="@link">configure</a> one in order for the MERCI reservation system to operate correctly.', array('@link' => url('admin/settings/date-time'))));
  }
}
/**
 * Creates a date object based on the site's local timezone.
 *
 * @param $datetime
 *   A date in DATETIME format, UTC timezone.
 *
 * @return
 *   A php date object in the site's timezone.
 */
function merci_create_local_date_object($datetime) {
  $date_object = date_create($datetime, timezone_open('UTC'));
  date_timezone_set($date_object, timezone_open(date_default_timezone_name()));

  return $date_object;
}

/**
 * Custom validation function to protect merci nodes from mass deletion.
 */
function merci_node_admin_delete_validate($form, &$form_state) {

  // Look only for delete op.
  $operation = $form_state['values']['operation'];
  if ($operation != 'delete') {
    return;
  }
  // Get the checked nodes.
  $nids = array_filter($form_state['values']['nodes']);

  // Perform the check for each submitted node.
  foreach ($nids as $nid) {
    $node = node_load($nid);

    // Check to see if any of the nodes should not be deleted.
    if (!merci_delete_item_validate($node)) {
      // If so, then unset the checked node so it will not be processed, and display a warning.
      // Note that the array element has to be completely removed here in order to prevent the
      // node from being deleted, due to the nature of the mass deletion callback.
      unset($form_state['values']['nodes'][$nid]);
      unset($nids[$nid]);
    }
  }

  // If we've unset all of the nodes that were checked, then don't continue with the form processing.
  if (!count($nids)) {
    drupal_set_message(t('No nodes selected.'), 'error');
    drupal_goto('admin/content/node');
  }
}


/**
 * Sort by vid
 *
 * @param $a
 *   The first object.
 * @param $b
 *   The second object
 *
 * @return
 *   0,1, or -1 indicating which object has a higher VID
 */
function merci_by_vid() {

  if ($a->vid == $b->vid) {

    return 0;

  }
  return ($a->vid > $b->vid) ? -1 : 1;
}
// merci_by_vid

/**
 * Calculates the short hour/minute time format based on the site settings.
 */
function merci_time_format() {
  static $time_only_format = NULL;
  if (empty($time_only_format)) {
    $short_date_format = variable_get('date_format_short', 'm/d/Y - H:i');
    $time_only_format = date_limit_format($short_date_format, array('hour', 'minute'));
  }
  return $time_only_format;
}

/**
 * Formats a time value into the site's preferred format.
 *
 * @param object $hours_minutes
 *   A string of the form 'H:MM' or 'HH:MM'
 *
 * @return
 *   A string in 12- or 24-hour format with no leading zero.
 */
function merci_format_time($hours_minutes) {
  $return = date(merci_time_format(), strtotime($hours_minutes));
  if ($return[0] == '0') {
    return substr($return, 1);
  }
  return $return;
}



/**
 * Callback function for updating Reservation status from VBO.
 */
function merci_operations_update($nodes) {
  foreach ($nodes as $nid) {
    merci_confirm_reservation($nid);
  }
}

/**
 * Callback function for updating Reservation status.
 */
function merci_confirm_reservation($nid) {
  $node = node_load($nid);

  //only update if MERCI Status is Unconfirmed
  if ($node->merci_reservation_status == MERCI_STATUS_UNCONFIRMED) {
    $node->merci_reseravation_status = MERCI_STATUS_PENDING;
    node_save($node);
    return TRUE;
  }

}



function merci_add_settings_form(&$form, $form_state) {

  // Only admin can edit these values.
  if (!user_access('administer MERCI')) {
    return;
  }

  $type = array_key_exists('old_type', $form) ? $form['old_type']['#value'] : $form['type']['#value'];
  //$merci_settings = mnerci_content_types($type);
  //if(!$merci_settings) return;
  if ($form['#id'] == 'node-type-form') {
    $node = merci_load_item_settings($type);
    $node = (object) $node;
  }
  else {
    $node = (object) $form['#node'];
  }
  //merci_load_item_settings($node);
  // New nodes are always sub type item.
  $sub_type = $node->merci_sub_type ? $node->merci_sub_type : MERCI_SUB_TYPE_ITEM;

  if (empty($form['merci'])) {
    $form['merci'] = array(
      '#type' => 'fieldset',
      '#title' => t('MERCI settings'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
  }

  // Bucket item nodes have no individual pricing, so just zero these values out.
  // Althought you can override them on the reserervation nodes.
  if ($node->merci_type_setting == 'resource' or $form['#id'] == 'node-type-form' or $sub_type == MERCI_SUB_TYPE_RESERVATION ) {
    $form['merci']['merci_rate_per_hour'] = array(
      '#type' => 'textfield',
      '#title' => t('Rate per hour'),
      '#size' => 10,
      '#default_value' => $node->merci_rate_per_hour,
      '#element_validate' => array('merci_is_numeric_validate'),
      '#description' => t('The per hour rental fee for the item.'),
    );
    $form['merci']['merci_late_fee_per_hour'] = array(
      '#type' => 'textfield',
      '#title' => t('Late fee per hour'),
      '#size' => 10,
      '#default_value' => $node->merci_late_fee_per_hour,
      '#element_validate' => array('merci_is_numeric_validate'),
      '#description' => t('The per hour fee for returning the item late.'),
    );
    $form['merci']['merci_fee_free_hours'] = array(
      '#type' => 'textfield',
      '#title' => t('Fee free hours'),
      '#size' => 10,
      '#default_value' => $node->merci_fee_free_hours,
      '#element_validate' => array('merci_is_numeric_validate'),
      '#description' => t('The number of hours the item can be used before fees are charged.'),
    );
  }

  if (($node->merci_type_setting == 'resource' and $sub_type == MERCI_SUB_TYPE_ITEM) or $form['#id'] == 'node-type-form') {

    $form['merci']['merci_min_cancel_hours'] = array(
      '#type' => 'textfield',
      '#title' => t('Minimum hours for cancelation without No Show'),
      '#size' => 10,
      '#default_value' => $node->merci_min_cancel_hours,
      '#element_validate' => array('merci_is_numeric_validate'),
      '#description' => t('Minimum number of hours before the start time a user may cancel a reservation for the item.'),
    );
    $form['merci']['merci_autocheckout'] = array(
      '#type' => 'checkbox',
      '#title' => t('Auto checkout'),
      '#default_value' => $node->merci_autocheckout,
      '#description' => t('Automatically check this item out when the Reservation starts.'),
    );
    $form['merci']['merci_autocheckin'] = array(
      '#type' => 'checkbox',
      '#title' => t('Auto checkin'),
      '#default_value' => $node->merci_autocheckin,
      '#description' => t('Automatically check this item in when the Reservation ends.'),
    );
    $form['merci']['merci_selfcheckout'] = array(
      '#type' => 'checkbox',
      '#title' => t('Self checkout'),
      '#default_value' => $node->merci_selfcheckout,
      '#description' => t('Manage checkout with additional code.'),
    );
  }
}

function _merci_after_build($form, &$form_state) {

  $weight = ($form['field_merci_date']['#weight'] + 1);
  $form['field_merci_date'][0]['field_merci_date_button'] = array(
    '#type' => 'submit',
    '#value' => t('Limit Lists to Available Items'),
    '#weight' => $weight,
    '#submit' => array('merci_date_filter'),
    '#ahah' => array(
      'path' => 'merci/js',
      'wrapper' => 'merci-choice-wrapper',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );
  return $form;
}
// Loads the current settings for reservable item nodes.
/* If you just want the content type settings just pass only node->type.
 */

function merci_load_item_settings($object) {

  if (is_string($object)) {
    $type = $object;
  }
  else {
    $node = (array) $object;
    $type = $node['type'];
  }

  $item_settings = array();
  // Settings from the content type edit page.
  $content_settings = merci_content_types($type);
  if (empty($content_settings)) {
    $content_settings = array();
  }

  if ($node['nid']) {
    // Settings common to all merci item nodes.
    // resource or bucket.
    $merci_type = $content_settings['merci_type_setting'];

    $item_settings = merci_reservation_item_node_settings($vid);
    switch ($merci_type) {
      case 'bucket':
        // TODO: move to seperate module.
        if ($item_settings['merci_sub_type'] == MERCI_SUB_TYPE_RESERVATION) {
          unset($item_settings['merci_default_availability']);
          unset($item_settings['merci_item_status']);
          $item_settings += merci_bucket_node_settings($vid);
        }
        break;

      case 'resource':
        // TODO: move to seperate module.
        $item_settings += merci_resource_node_settings($vid);
        break;
    }
  }

  if ($item_settings) {
    return (object) ($item_settings + $content_settings);
  }
  else {
    return (object) ($content_settings);
  }
}
/**
 * Validates the state change of a reservable item.
 *
 * @param $node
 *   The item node.
 */
function merci_validate_default_availability($element, &$form_state) {

  // Only perform the check if the item is set to an unavailable state.
  if (in_array((int) $element['#value'], array(MERCI_UNA_F, MERCI_UNA_S))) {

    $bad_reservations = merci_incomplete_reservations_for_item_nid($form_state['values']['nid']);
    if (!empty($bad_reservations)) {
      $output = '<ul>'
      foreach ($bad_reservations as $node) {
        $output .= '<li>' . $node . '</li>';
      }
      $output .= '</ul>'
      form_set_error('merci_default_availability', t('%title can not be set to an unavailable status until it is removed from the following reservations:', array('%title' => $form_state['values']['title'])) . $output);
      
    }
  }
}
