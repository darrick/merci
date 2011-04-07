<?php

/**
 * @file
 * MERCI - Managed Equipment Reservation Checkout and Inventory
 */


/**
 * Builds the MERCI admininstration settings form.
 */
function merci_admin_settings() {
  $form = array();

  $statuses = array();

  $statuses[MERCI_STATUS_UNCONFIRMED] = t('Unconfirmed');
  $statuses[MERCI_STATUS_PENDING] = t('Confirmed');
  $statuses[MERCI_STATUS_CHECKED_OUT] = t('Checked Out');


  $form['merci_default_reservation_status'] = array(
    '#type' => 'select',
    '#title' => t('Default Reservation Status'),
    '#options' => $statuses,
    '#default_value' => variable_get('merci_default_reservation_status', ''),
    '#description' => t('New Reservations will be set to this by default.  Use Checked Out if you are using MERCI to manage the checkout, but do not allow actual Reservations.'),
  );

  $form['merci_max_days_advance_reservation'] = array(
    '#type' => 'textfield',
    '#title' => t('Max Advance'),
    '#size' => 11,
    '#maxlength' => 11,
    '#default_value' => variable_get('merci_max_days_advance_reservation', '0'),
    '#description' => t('Maximum number of days a Reservation can be made in advance.  Set to 0 if to allow Reservations to be made an unlimitted time into the future.'),
  );

  // Weekend

  $form['merci_saturday_is_weekend'] = array(
    '#type' => 'checkbox',
    '#title' => t('Saturday is a considered weekend'),
    '#default_value' => variable_get('merci_saturday_is_weekend', 1),
  );
  $form['merci_sunday_is_weekend'] = array(
    '#type' => 'checkbox',
    '#title' => t('Sunday is a considered weekend'),
    '#default_value' => variable_get('merci_sunday_is_weekend', 1),
  );
  $form['merci_monday_is_weekend'] = array(
    '#type' => 'checkbox',
    '#title' => t('Monday is considered a weekend'),
    '#default_value' => variable_get('merci_monday_is_weekend', 0),
  );
  $form['merci_tuesday_is_weekend'] = array(
    '#type' => 'checkbox',
    '#title' => t('Tuesday is considered a weekend'),
    '#default_value' => variable_get('merci_tuesday_is_weekend', 0),
  );
  $form['merci_wednesday_is_weekend'] = array(
    '#type' => 'checkbox',
    '#title' => t('Wednesday is considered a weekend'),
    '#default_value' => variable_get('merci_wednesday_is_weekend', 0),
  );
  $form['merci_thursday_is_weekend'] = array(
    '#type' => 'checkbox',
    '#title' => t('Thursday is considered a weekend'),
    '#default_value' => variable_get('merci_thursday_is_weekend', 0),
  );
  $form['merci_friday_is_weekend'] = array(
    '#type' => 'checkbox',
    '#title' => t('Firday is a considered weekend'),
    '#default_value' => variable_get('merci_friday_is_weekend', 0),
  );

  // Hours of operation

  $hours_description = '<div>' . t('Enter military time for both opening and closing time, separated by a dash, in the format') . ' <em>hh:mm-hh:mm</em></div>' . t('ex.') . ' <em>09:00-17:00</em> ' . t('would be open at 9AM, close at 5PM. Leave blank to indicate not being open.');

  $form['merci_hours_mon'] = array(
    '#type' => 'textfield',
    '#title' => t('Monday hours'),
    '#size' => 11,
    '#maxlength' => 11,
    '#default_value' => variable_get('merci_hours_mon', '09:00-17:00'),
    '#description' => $hours_description,
  );
  $form['merci_hours_tue'] = array(
    '#type' => 'textfield',
    '#title' => t('Tuesday hours'),
    '#size' => 11,
    '#maxlength' => 11,
    '#default_value' => variable_get('merci_hours_tue', '09:00-17:00'),
    '#description' => $hours_description,
  );
  $form['merci_hours_wed'] = array(
    '#type' => 'textfield',
    '#title' => t('Wednesday hours'),
    '#size' => 11,
    '#maxlength' => 11,
    '#default_value' => variable_get('merci_hours_wed', '09:00-17:00'),
    '#description' => $hours_description,
  );
  $form['merci_hours_thu'] = array(
    '#type' => 'textfield',
    '#title' => t('Thursday hours'),
    '#size' => 11,
    '#maxlength' => 11,
    '#default_value' => variable_get('merci_hours_thu', '09:00-17:00'),
    '#description' => $hours_description,
  );
  $form['merci_hours_fri'] = array(
    '#type' => 'textfield',
    '#title' => t('Friday hours'),
    '#size' => 11,
    '#maxlength' => 11,
    '#default_value' => variable_get('merci_hours_fri', '09:00-17:00'),
    '#description' => $hours_description,
  );
  $form['merci_hours_sat'] = array(
    '#type' => 'textfield',
    '#title' => t('Saturday hours'),
    '#size' => 11,
    '#maxlength' => 11,
    '#default_value' => variable_get('merci_hours_sat', ''),
    '#description' => $hours_description,
  );
  $form['merci_hours_sun'] = array(
    '#type' => 'textfield',
    '#title' => t('Sunday hours'),
    '#size' => 11,
    '#maxlength' => 11,
    '#default_value' => variable_get('merci_hours_sun', ''),
    '#description' => $hours_description,
  );

  $form['merci_hours_admin'] = array(
    '#type' => 'textfield',
    '#title' => t('Admin hours'),
    '#size' => 11,
    '#maxlength' => 11,
    '#default_value' => variable_get('merci_hours_admin', '07:00-22:00'),
    '#description' => t('This setting controls the hours shown on the conflict grid for users with Adminster MERCI permessions.  The conflict grid is normally limited to the days and hours the set above.  Because users with Admister MERCI rights can create Resevervations outside the normal hours of operation, they need to see conflicts during those times as well.'),
  );

  $form['merci_closed_dates'] = array(
    '#type' => 'textarea',
    '#title' => t('Closed dates'),
    '#rows' => 10,
    // TODO: this doesn't seem to work...
    '#cols' => 5,
    '#default_value' => variable_get('merci_closed_dates', ''),
    '#description' => '<div>' . t('Enter dates which are closed regardless of the day of the week, one date per line, in the format') . ' <em>mm-dd</em></div>' . t('ex.') . ' <em>07-04</em> ' . t('would mean July 4th is always closed, regardless of what day of the week it falls on.'),
  );

  $form['merci_advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['merci_advanced']['merci_verbose_logging'] = array(
    '#type' => 'checkbox',
    '#title' => t('Verbose Logging Mode'),
    '#default_value' => variable_get('merci_verbose_logging', 0),
    '#description' => t('This option will add log messages at several points in the resevervation process making it easier to diagnose configuration issues.'),
  );

  //check to see if the fields are locked
  $result = db_query("SELECT locked FROM {content_node_field} WHERE field_name = '%s'", 'field_merci_date');

  $message = t('MERCI requires a Date field to function. This field is locked by default.  Altering or deleting the Date field could cause serious problems.  Always backup before altering this field!') . '<br /><br />';

  $field_state = db_fetch_object($result);
  if ($field_state->locked) {
    $value = t('Core MERCI fields are currently locked.') . ' ' . l(t('Unlock them.'), 'admin/merci/unlock',   array('query' => drupal_get_destination()));

  }
  else {
    $value = t('Core MERCI fields are currently unlocked.') . ' ' . l(t('Lock them.'), 'admin/merci/lock',   array('query' => drupal_get_destination()));
  }

  //TODO: Change this to AHAH call
  $form['merci_advanced']['merci_unlock'] = array(
    '#type' => 'markup',
    '#title' => t('Unlock Fields'),
    '#value' => '<div>' . $message . $value . '</div>',
  );

  return system_settings_form($form);
}



function merci_unlock() {
  db_query("UPDATE {content_node_field} SET locked = 0 WHERE field_name = 'field_merci_date'");
  $return = $_GET["destination"];
  if (!$return) {
    $return = 'admin/settings/merci';
  }
  drupal_goto($return);
}

function merci_lock() {
  db_query("UPDATE {content_node_field} SET locked = 1 WHERE field_name = 'field_merci_date'");
  $return = $_GET["destination"];
  if (!$return) {
    $return = 'admin/settings/merci';
  }
  drupal_goto($return);
}

