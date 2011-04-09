<?php

/**
 * @file
 * MERCI - Managed Equipment Reservation Checkout and Inventory
 */


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
        $type = $value;
      }

      $content_settings = merci_load_item_settings($type);

      $title = $title ? $title : $content_settings['type_name'];

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
      $output = '<ul>';
      foreach ($bad_reservations as $node) {
        $output .= '<li>' . $node . '</li>';
      }
      $output .= '</ul>';
      form_set_error('merci_default_availability', t('%title can not be set to an unavailable status until it is removed from the following reservations:', array('%title' => $form_state['values']['title'])) . $output);

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
 * writes additional info to log to aid in troubleshoot configuration
 */
function merci_verbose_logging($string) {

  if (variable_get('merci_verbose_logging', 0)) {
    watchdog('merci', $string);
  }
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

  // Group the buckets.
  $vid = variable_get('merci_equipment_grouping_vid', 0);
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
        $options['options'][$value['merci_item_grouping']][$type] = $value['type_name'];
      }
    }
    else if ($value['merci_type_setting'] == 'resource') {
      // No date filtering for new reservations.
      $item_options = merci_get_reservable_items($type, $start, $end, $reservation_nid);

      if (!empty($item_options)) {
        foreach ($item_options as $key => $item) {
          $options['options'][$value['merci_item_grouping']][$key] = $item;
        }
      }
    }
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
 * Adds items to reservation on creation/update.
 *
 * @param $node
 *   The reservation node.
 */
function merci_add_reservation_items($node) {

  $member_total = 0;
  $commercial_total = 0;

  $hours = round(strtotime($node->field_merci_date[0]['value2']) - strtotime($node->field_merci_date[0]['value'])) / 3600;

  $exempt_items = array();
  // Update existing items or add new ones.
  if (isset($node->merci_reservation_items)) {
    foreach ($node->merci_reservation_items as $did => $item) {

      if (empty($item['merci_item_nid']) and !$item['type']) {
        continue;
      }

      // If we are copying a reservation.  I.e. via node_repeat
      if ($item['merci_placeholder_nid'] and $node->is_new) {
        $placeholder_node = node_load($item['merci_placeholder_nid']);
        $placeholder_node->nid = NULL;
        $placeholder_node->vid = NULL;
        $placeholder_node = node_submit($placeholder_node);
        node_save($placeholder_node);
        $item['merci_placeholder_nid'] = $placeholder_node->nid;
      }
      // Create a placeholder node if we don't have one yet.
      if (!$item['merci_placeholder_nid']) {
        // Resource.
        if (is_numeric($item['merci_item_nid'])) {
          $item_node        = node_load($item['merci_item_nid']);
          $item['type']        = $item_node->type;
          $item['item_title']       = $item_node->title;
          $settings = $item_node;
        }
        // Bucket.
        elseif ($item['merci_item_nid']) {

          $item['type']        = $item['merci_item_nid'];
          $settings = merci_load_item_settings($item['type']);
          $item['name']  = $settings['type_name'];;
          if ($settings->merci_auto_assign_bucket_item) {
            $date_info = $node->field_merci_date[0];
            $start     = $date_info['value'];
            $end       = $date_info['value2'];

            $item['merci_item_nid'] = merci_get_suggested_bucket_item($item['type'], $start, $end, $exempt_items);
            $exempt_items[] = $item['merci_item_nid'];
            $item_node        = node_load($item['merci_item_nid']);
            $item['item_title'] = $item_node->title;
            $settings = $item_node;
          }
        }
        else {
          break;
        }
        $title = $item['item_title'] ? $item['item_title']    : $item['name'];

        // Build the item's placeholder node.
        $reservation = new stdClass();
        $reservation->type = $item['type'];
        $reservation->name = $node->name;
        $reservation->uid = $node->uid;
        $reservation->title = "$title " . t('(Reservation)');
        $reservation->body = '';
        $reservation->status = 0;
        $reservation->promote = 0;
        $reservation->sticky = 0;

        // MERCI specific data.
        $reservation->merci_default_availability = MERCI_AVA_F;
        $reservation->merci_sub_type = MERCI_SUB_TYPE_RESERVATION;

        // Use the item specific accounting data if an item is assigned,
        // otherwise fall back to the content type defaults.
        // TODO move to nodeapi insert and update ops.
        //Add to commerical rate
        //print '<pre>';
        //print_r();
        $rate = $settings->merci_rate_per_hour;
        $commercial_total = $commercial_total + ($hours * $rate);
        $reservation->merci_late_fee_per_hour = $settings->merci_late_fee_per_hour;
        $reservation->merci_rate_per_hour = $settings->rate_per_hour;
        $reservation->merci_fee_free_hours = $settings->fee_free_hours;
        $reservation = node_submit($reservation);
        node_save($reservation);

        $item['merci_placeholder_nid'] = $reservation->nid;
      }

      // Update the state of all items with associations.
      switch ((int) $node->merci_reservation_status) {
        case MERCI_STATUS_UNCONFIRMED:
        case MERCI_STATUS_PENDING:
          $item_status = MERCI_ITEM_STATUS_RESERVED;
          break;

        case MERCI_STATUS_CHECKED_OUT:
          $item_status = MERCI_ITEM_STATUS_CHECKED_OUT;
          break;

        case MERCI_STATUS_CHECKED_IN:
          $item_status = MERCI_ITEM_STATUS_CHECKED_IN;
          break;

        case MERCI_STATUS_CANCELLED:
          $item_status = MERCI_ITEM_STATUS_CANCELED;
          break;
        case MERCI_STATUS_DENIED:
          $item_status = MERCI_ITEM_STATUS_AVAILABLE;
          break;
      }

      // If we have an item assigned.  Set status to reserved.
      $item['merci_item_status'] = $item_status;

      if ($node->revision or is_string($did) or $node->is_new) {
        $item['nid'] = $node->nid;
        $item['vid'] = $node->vid;
        drupal_write_record('merci_reservation_detail', $item);
      }
      else {
        $item['did'] = $did;
        drupal_write_record('merci_reservation_detail', $item, 'did');
      }

      $item_status = $item_status == MERCI_ITEM_STATUS_CHECKED_OUT ? MERCI_ITEM_STATUS_CHECKED_OUT : MERCI_ITEM_STATUS_AVAILABLE;
      if (is_numeric($item['merci_item_nid']) and $item['merci_item_nid'] > 0) {
        $update = array();
        $update['nid'] = $item['merci_item_nid'];
        $update['merci_item_status'] = $item_status;
        drupal_write_record('merci_reservation_item_node', $update, 'nid');
      }
    }
  }
}
