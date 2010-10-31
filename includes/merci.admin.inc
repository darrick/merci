<?php
// $Id$ //

/**
 * @file
 * merci admin settings 
 */

/**
 * Builds the MERCI admininstration settings form.
 */
function merci_admin_settings($form, &$form_state) {
  $form = array();

  $statuses = array();

  $statuses[MERCI_STATUS_UNCONFIRMED] = t('Unconfirmed');
  $statuses[MERCI_STATUS_PENDING] = t('Pending');
  $statuses[MERCI_STATUS_CHECKED_OUT] = t('Checked Out');

  $form['merci_content_types'] = array(
    '#type' => 'fieldset',
    '#title' => t('Content types'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $header = array(t('Type'), t('Usage'), t('Operations'));
  $rows = array();

  $merci_types = array();
  $merci_type_result = db_query("SELECT type, type_setting FROM {merci_node_type} WHERE type_setting = :type_setting OR  type_setting = :type_setting", array(':type_setting' => 'resource', ':type_setting' => 'bucket'));

  while ($merci_type = db_fetch_object($merci_type_result)) {
    $merci_types[$merci_type->type] = $merci_type->type_setting;
  }
  // while

  foreach (node_type_get_types() as $type) {

    $type_url_str = str_replace('_', '-', $type->type);

    $usage = t('Not used by MERCI');
    if (isset($merci_types[$type->type])) {
      $usage = 'MERCI ' . $merci_types[$type->type];
    }
    // if

    $operations = l(t('Edit'), 'admin/content/node-type/' . $type_url_str, array('query' => drupal_get_destination()));

    if (module_exists('merci_inventory') && merci_inventory_node_type_menu_access($type->type)) {

      $operations .= ' | ' . l(t('Inventory Sync'), 'admin/content/node-type/' . $type_url_str . '/merci_inventory', array('query' => drupal_get_destination()));
    }
    // if

    $rows[] = array(
      $type->name,
      $usage,
      $operations,
    );
  }
  // foreach

  $form['merci_content_types']['content_types_table'] = array(
    '#value' => theme('table', array('header' => $header, 'rows' => $rows)),
  );

  $form['merci_general'] = array(
    '#type' => 'fieldset',
    '#title' => t('General settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['merci_general']['merci_default_reservation_status'] = array(
    '#type' => 'select',
    '#title' => t('Default Reservation Status'),
    '#options' => $statuses,
    '#default_value' => variable_get('merci_default_reservation_status', ''),
    '#description' => t('New Reservations will be set to this by default.  Use Checked Out if you are using MERCI to manage the checkout, but do not allow actual Reservations.'),
  );

  $form['merci_general']['merci_max_days_advance_reservation'] = array(
    '#type' => 'textfield',
    '#title' => t('Max Advance'),
    '#size' => 11,
    '#maxlength' => 11,
    '#default_value' => variable_get('merci_max_days_advance_reservation', '0'),
    '#description' => t('Maximum number of days a Reservation can be made in advance.  Set to 0 if to allow Reservations to be made an unlimitted time into the future.'),
  );

  /*
   $form['merci_general']['merci_email_receipt'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send Email Receipt'),
    '#default_value' => variable_get('merci_email_receipt', 0),
    '#description' => t('Use when default Reservation Status is Unconfirmed'),
  );

  $form['merci_general']['merci_email_confirmation'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send Email Confirmation'),
    '#default_value' => variable_get('merci_email_confirmation', 0),
    '#description' => t('Add link to change reservations from Unconfirmed to Confirmed'),
  );
   */



  // Weekend became push in 2.x

  $form['merci_general']['merci_saturday_is_weekend'] = array(
    '#type' => 'checkbox',
    '#title' => t('Push forward if checkout is closed when Reservation ends on Saturday'),
    '#default_value' => variable_get('merci_saturday_is_weekend', 1),
  );
  $form['merci_general']['merci_sunday_is_weekend'] = array(
    '#type' => 'checkbox',
    '#title' => t('Push forward if checkout is closed when Reservation ends on Sunday'),
    '#default_value' => variable_get('merci_sunday_is_weekend', 1),
  );
  $form['merci_general']['merci_monday_is_weekend'] = array(
    '#type' => 'checkbox',
    '#title' => t('Push forward if checkout is closed when Reservation ends on Monday'),
    '#default_value' => variable_get('merci_monday_is_weekend', 0),
  );
  $form['merci_general']['merci_tuesday_is_weekend'] = array(
    '#type' => 'checkbox',
    '#title' => t('Push forward if checkout is closed when Reservation ends on Tuesday'),
    '#default_value' => variable_get('merci_tuesday_is_weekend', 0),
  );
  $form['merci_general']['merci_wednesday_is_weekend'] = array(
    '#type' => 'checkbox',
    '#title' => t('Push forward if checkout is closed when Reservation ends on Wednesday'),
    '#default_value' => variable_get('merci_wednesday_is_weekend', 0),
  );
  $form['merci_general']['merci_thursday_is_weekend'] = array(
    '#type' => 'checkbox',
    '#title' => t('Push forward if checkout is closed when Reservation ends on Thursday'),
    '#default_value' => variable_get('merci_thursday_is_weekend', 0),
  );
  $form['merci_general']['merci_friday_is_weekend'] = array(
    '#type' => 'checkbox',
    '#title' => t('Push forward if checkout is closed when Reservation ends on Friday'),
    '#default_value' => variable_get('merci_friday_is_weekend', 0),
  );

  // Hours of operation

  $hours_description = t('<div>Enter military time for both opening and closing time, separated by a dash, in the format <em>hh:mm-hh:mm</em></div>ex. <em>09:00-17:00</em> would be open at 9AM, close at 5PM. Leave blank to indicate not being open.');

  $form['merci_general']['merci_hours_mon'] = array(
    '#type' => 'textfield',
    '#title' => t('Monday hours'),
    '#size' => 11,
    '#maxlength' => 11,
    '#default_value' => variable_get('merci_hours_mon', ''),
    '#description' => $hours_description,
  );
  $form['merci_general']['merci_hours_tue'] = array(
    '#type' => 'textfield',
    '#title' => t('Tuesday hours'),
    '#size' => 11,
    '#maxlength' => 11,
    '#default_value' => variable_get('merci_hours_tue', ''),
    '#description' => $hours_description,
  );
  $form['merci_general']['merci_hours_wed'] = array(
    '#type' => 'textfield',
    '#title' => t('Wednesday hours'),
    '#size' => 11,
    '#maxlength' => 11,
    '#default_value' => variable_get('merci_hours_wed', ''),
    '#description' => $hours_description,
  );
  $form['merci_general']['merci_hours_thu'] = array(
    '#type' => 'textfield',
    '#title' => t('Thursday hours'),
    '#size' => 11,
    '#maxlength' => 11,
    '#default_value' => variable_get('merci_hours_thu', ''),
    '#description' => $hours_description,
  );
  $form['merci_general']['merci_hours_fri'] = array(
    '#type' => 'textfield',
    '#title' => t('Friday hours'),
    '#size' => 11,
    '#maxlength' => 11,
    '#default_value' => variable_get('merci_hours_fri', ''),
    '#description' => $hours_description,
  );
  $form['merci_general']['merci_hours_sat'] = array(
    '#type' => 'textfield',
    '#title' => t('Saturday hours'),
    '#size' => 11,
    '#maxlength' => 11,
    '#default_value' => variable_get('merci_hours_sat', ''),
    '#description' => $hours_description,
  );
  $form['merci_general']['merci_hours_sun'] = array(
    '#type' => 'textfield',
    '#title' => t('Sunday hours'),
    '#size' => 11,
    '#maxlength' => 11,
    '#default_value' => variable_get('merci_hours_sun', ''),
    '#description' => $hours_description,
  );

  $form['merci_general']['merci_hours_admin'] = array(
    '#type' => 'textfield',
    '#title' => t('Admin hours'),
    '#size' => 11,
    '#maxlength' => 11,
    '#default_value' => variable_get('merci_hours_admin', ''),
    '#description' => t('This setting controls the hours shown on the conflict grid for users with Adminster MERCI permessions.  The conflict grid is normally limited to the days and hours the set above.  Because users with Admister MERCI rights can create Resevervations outside the normal hours of operation, they need to see conflicts during those times as well.'),
  );

  $form['merci_general']['merci_closed_dates'] = array(
    '#type' => 'textarea',
    '#title' => t('Closed dates'),
    '#rows' => 10,
    // TODO: this doesn't seem to work...
    '#cols' => 5,
    '#default_value' => variable_get('merci_closed_dates', ''),
    '#description' => t('<div>Enter dates which are closed regardless of the day of the week, one date per line, in the format <em>mm-dd</em></div>ex. <em>07-04</em> would mean July 4th is always closed, regardless of what day of the week it falls on.'),
  );

  $form['merci_general']['merci_membership_discount'] = array(
    '#type' => 'textfield',
    '#title' => t('Membership discount'),
    '#size' => 11,
    '#maxlength' => 11,
    '#default_value' => variable_get('merci_membership_discount', ''),
    '#description' => t('Percent comercial rate is reduced for users in a role with member discount permission. Enter as decimal.  60% would be .60.  An item with a commercial rate of $100 would cost a member $40 for any hours beyond the fee free hours for that reservation.'),
  );

  $form['merci_general']['merci_contract_header'] = array(
    '#type' => 'textarea',
    '#title' => t('Contract header'),
    '#rows' => 10,
    // TODO: this doesn't seem to work...
    '#cols' => 5,
    '#default_value' => variable_get('merci_contract_header', ''),
    '#description' => t('Header portion of printable contract.  Allows HTML.'),
  );

  $form['merci_general']['merci_contract_boilerplate'] = array(
    '#type' => 'textarea',
    '#title' => t('Contract boilerplate'),
    '#rows' => 10,
    // TODO: this doesn't seem to work...
    '#cols' => 5,
    '#default_value' => variable_get('merci_contract_boilerplate', ''),
    '#description' => t('Legalese that makes the contract legally binding.'),
  );

  $form['merci_general']['merci_contract_footer'] = array(
    '#type' => 'textarea',
    '#title' => t('Contract footer'),
    '#rows' => 10,
    // TODO: this doesn't seem to work...
    '#cols' => 5,
    '#default_value' => variable_get('merci_contract_footer', ''),
    '#description' => t('Footer portion of printable contract. Normally includes signature lines. HTML allowed.'),
  );

  if (module_exists('token')) {
    $form['merci_general']['token_help'] = array(
      '#title' => t('Replacement patterns'),
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    // TODO Please change this theme call to use an associative array for the $variables parameter.
    $form['merci_general']['token_help']['help'] = array(
      '#value' => theme('token_help', 'node'),
    );
  }

  return system_settings_form($form);
}

