<?php


/**
 * @file
 * Admin section functions for MERCI Role Rule Override
 */

/**
 * Menu callback; rule override administration.
 */
function merci_rro_admin_page($type_name = NULL) {

  if ($_GET['delete'] != '') {

    $html .= drupal_get_form('merci_rro_override_delete', $type_name, $_GET['delete']);
  }
  // if
  else {

    $html .= '<h2>Active Overrides:</h2>';

    $overrides = merci_rro_content_type_overrides($type_name);
    $exclude_roles = array();

    if (count($overrides)) {

      foreach ($overrides as $override) {

        $html .= theme('merci_rro_override', $override);
        $exclude_roles[] = $override->rid;
      }
      // foreach
    }
    // if
    else {

      $html .= '<p><i>No active overrides.</i></p>';
    }
    // else

    $html .= '<h2>Add New Override</h2>';
    $html .= drupal_get_form('merci_rro_add_new_override', $type_name, $exclude_roles);
  }
  // else

  return $html;
}
// merci_rro_admin_page
function merci_rro_override_delete(&$form_state, $type_name, $rid) {

  $role = merci_rro_role_name_from_rid($rid);
  $form = array();

  $form['merci_rro_type'] = array(
    '#type' => 'hidden',
    '#value' => $type_name,
  );
  $form['merci_rro_rid'] = array(
    '#type' => 'hidden',
    '#value' => $rid,
  );

  $form['merci_rro_sure'] = array(
    '#type' => 'markup',
    '#value' => '<p>'. t('Are you sure you want to delete the override for role ') .'<i>'. check_plain($role) .'</i>'. t(' on type ') .'<i>'. check_plain($type_name) .'</i>?</p>',
  );

  $form['merci_rro_yes'] = array('#type' => 'submit', '#value' => t('Delete'));
  $form['merci_rro_no'] = array('#type' => 'submit', '#value' => t('Cancel'));
  $form['#submit'] = array('merci_rro_delete_submit');

  return $form;
}
// merci_rro_override_delete
function merci_rro_delete_submit($form_id, $form_values) {

  $values = $form_values['values'];

  $sql = "DELETE FROM {merci_rro}
  WHERE rid = %d
  AND type = '%s'";

  db_query($sql, $values['merci_rro_rid'], $values['merci_rro_type']);

  drupal_set_message(t('Override deleted for @role', array('@role' => merci_rro_role_name_from_rid($values['merci_rro_rid']))));
}
// merci_rro_delete_submit
function merci_rro_add_new_override(&$form_state, $type_name, $exclude_roles) {

  $form = array();
  $roles = user_roles();

  foreach ($exclude_roles as $role) {

    unset($roles[$role]);
  }
  // foreach

  $form['merci_rro_type'] = array(
    '#type' => 'hidden',
    '#value' => $type_name,
  );
  $form['merci_rro_roles'] = array(
    '#type' => 'select',
    '#title' => t('Role'),
    '#options' => $roles,
    '#required' => TRUE,
    '#description' => t('Override settings will be applied to any users with this role.'),
  );
  $form['merci_rro_max_hours_per_reservation'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum hours per reservation'),
    '#size' => 10,
    '#default_value' => 0,
    '#element_validate' => array('merci_is_numeric_validate'),
    '#description' => t('The maximum hours the item can be reserved for in one reservation. Set to zero for no limit.') . $warning,
  );
  $form['merci_rro_allow_overnight'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow overnight reservation'),
    '#default_value' => 0,
    '#description' => t('Allow a reservation to continue over multiple days.') . $warning,
  );
  $form['merci_rro_allow_weekends'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow weekend reservation'),
    '#default_value' => 0,
    '#description' => t('Allow a reservation to be made over days defined as weekend.') . $warning,
  );
  $form['merci_rro_rate_per_hour'] = array(
    '#type' => 'textfield',
    '#title' => t('Rate per hour'),
    '#size' => 10,
    '#default_value' => 0,
    '#element_validate' => array('merci_is_numeric_validate'),
    '#description' => t('The per hour rental fee for the item.'),
  );
  $form['merci_rro_late_fee_per_hour'] = array(
    '#type' => 'textfield',
    '#title' => t('Late fee per hour'),
    '#size' => 10,
    '#default_value' => 0,
    '#element_validate' => array('merci_is_numeric_validate'),
    '#description' => t('The per hour fee for returning the item late.'),
  );
  $form['merci_rro_fee_free_hours'] = array(
    '#type' => 'textfield',
    '#title' => t('Fee free hours'),
    '#size' => 10,
    '#default_value' => 0,
    '#element_validate' => array('merci_is_numeric_validate'),
    '#description' => t('The number of hours the item can be used before fees are charged.'),
  );
  $form['merci_rro_min_cancel_hours'] = array(
    '#type' => 'textfield',
    '#title' => t('Minimum hours for cancelation without No Show'),
    '#size' => 10,
    '#default_value' => 0,
    '#element_validate' => array('merci_is_numeric_validate'),
    '#description' => t('Minimum number of hours a user can cancel a reservation for the item.'),
  );

  $hours_description = t('<div>Enter military time for both opening and closing time, separated by a dash, in the format <em>hh:mm-hh:mm</em></div>ex. <em>09:00-17:00</em> would be open at 9AM, close at 5PM. Leave blank to indicate not being open.');

  $form['merci_hours_mon'] = array(
    '#type' => 'textfield',
    '#title' => t('Monday hours'),
    '#size' => 11,
    '#maxlength' => 11,
    '#default_value' => variable_get('merci_hours_mon', ''),
    '#description' => $hours_description,
  );
  $form['merci_hours_tue'] = array(
    '#type' => 'textfield',
    '#title' => t('Tuesday hours'),
    '#size' => 11,
    '#maxlength' => 11,
    '#default_value' => variable_get('merci_hours_tue', ''),
    '#description' => $hours_description,
  );
  $form['merci_hours_wed'] = array(
    '#type' => 'textfield',
    '#title' => t('Wednesday hours'),
    '#size' => 11,
    '#maxlength' => 11,
    '#default_value' => variable_get('merci_hours_wed', ''),
    '#description' => $hours_description,
  );
  $form['merci_hours_thu'] = array(
    '#type' => 'textfield',
    '#title' => t('Thursday hours'),
    '#size' => 11,
    '#maxlength' => 11,
    '#default_value' => variable_get('merci_hours_thu', ''),
    '#description' => $hours_description,
  );
  $form['merci_hours_fri'] = array(
    '#type' => 'textfield',
    '#title' => t('Friday hours'),
    '#size' => 11,
    '#maxlength' => 11,
    '#default_value' => variable_get('merci_hours_fri', ''),
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

  $form['merci_rro_submit'] = array('#type' => 'submit', '#value' => t('Add'));
  $form['#submit'] = array('merci_rro_new_submit');

  return $form;
}
// merci_rro_add_new_override
function merci_rro_new_submit($form_id, $form_values) {

  $values = $form_values['values'];

  $sql = "INSERT INTO {merci_rro}
  (
    rid,	type, max_hours_per_reservation,
    allow_overnight, allow_weekends, late_fee_per_hour,
    rate_per_hour, fee_free_hours, min_cancel_hours,
    hours_mon, hours_tue, hours_wed,
    hours_thu, hours_fri, hours_sat,
    hours_sun
  )
  VALUES
  (
    %d, '%s', %d,
    %d, %d, %d,
    %d, %d, %d,
    '%s', '%s', '%s',
    '%s', '%s', '%s',
    '%s'
  )";

  db_query($sql,
    $values['merci_rro_roles'], $values['merci_rro_type'], $values['merci_rro_max_hours_per_reservation'],
    $values['merci_rro_allow_overnight'], $values['merci_rro_allow_weekends'], $values['merci_rro_rate_per_hour'],
    $values['merci_rro_late_fee_per_hour'], $values['merci_rro_fee_free_hours'], $values['merci_rro_min_cancel_hours'],
    $values['merci_hours_mon'], $values['merci_hours_tue'], $values['merci_hours_wed'],
    $values['merci_hours_thu'], $values['merci_hours_fri'], $values['merci_hours_sat'],
    $values['merci_hours_sun']
  );

  drupal_set_message(t('Override added for @role', array('@role' => merci_rro_role_name_from_rid($values['merci_rro_rid']))));
}
// merci_rro_new_submit

