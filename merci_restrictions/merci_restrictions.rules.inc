<?php

/**
 * Implements hook_rules_action_info() on behalf of the user module.
 */
function merci_restrictions_rules_action_info() {
  $defaults = array(
   'parameter' => array(
      'line_item' => array(
        'type' => 'entity',
        'label' => t('Line item'),
        'save' => FALSE,
      ),
      'reservation' => array(
        'type' => 'entity',
        'label' => t('Reservation'),
        'save' => FALSE,
      ),
      'role_restrictions' => array(
        'type' => 'list<merci_restrictions>',
        'label' => t('Role restrictions'),
        'save' => FALSE,
      ),
      'roles_user' => array(
        'type' => 'list<text>',
        'label' => t('User roles'),
        'save' => FALSE,
      ),
      'errors' => array(
        'type' => 'struct',
        'label' => t('Errors'),
        'save' => FALSE,
      ),
    ),
    'group' => t('MERCI'),
  );
  $actions['merci_restrictions_conflicts'] = $defaults + array(
    'label' => t('Validate MERCI restrictions'),
    'base' => 'merci_restrictions_rules_line_item_validate',
  );
  return $actions;
}

function merci_restrictions_rules_line_item_validate($line_item, $reservation, $role_restrictions, $roles_user, $errors)
{
  $max_length = variable_get('merci_default_max_length', MERCI_DEFAULT_MAX_LENGTH);
  $allow_overnight = variable_get('merci_default_allow_overnight', MERCI_DEFAULT_ALLOW_OVERNIGHT);

  foreach ($role_restrictions as $restriction) {

    $restriction_wrapper = entity_metadata_wrapper('merci_restrictions', $restriction);
    $roles_required = $restriction_wrapper->{MERCI_ALLOWED_ROLES}->value();

    if (array_intersect($roles_user, $roles_required)) {
      $allow_overnight = $allow_overnight > $restriction_wrapper->{MERCI_CHECKOUT_OFFSITE}->value() ? $allow_overnight : $restriction_wrapper->{MERCI_CHECKOUT_OFFSITE}->value();

      $max_length_test = $restriction_wrapper->{MERCI_CHECKOUT_MAX_LENGTH}->value();
      if (!empty($max_length_test)) {
        $test_date = new DateObject('now ' . $max_length_test);
        if ($test_date > new DateObject('now ' . $max_length)) {
          $max_length = $max_length_test;
        }
      }
    }

  }
  $start_date = new DateObject($line_item->{MERCI_CHECKOUT_DATES}->value->value());//, 'UTC');
  $end_date   = new DateObject($line_item->{MERCI_CHECKOUT_DATES}->value2->value());//, 'UTC');
  // Test allow overnight.
  if (!$allow_overnight) {
    $start_day = $start_date->format('m-d-Y');
    $end_day = $end_date->format('m-d-Y');
    if ($start_day != $end_day) {
      $error = $errors->offsetExists(MERCI_CHECKOUT_DATES) ? $errors->offsetGet(MERCI_CHECKOUT_DATES) : array();
      $error[] = t('User cannot checkout this resource overnight');
      $errors->offsetSet(MERCI_CHECKOUT_DATES, $error);
    }
  }

  if (!empty($max_length)) {
    $test_date = $start_date->modify($max_length);
    if ($test_date < $end_date) {
      $error = $errors->offsetExists(MERCI_CHECKOUT_DATES) ? $errors->offsetGet(MERCI_CHECKOUT_DATES) : array();
      $error[] = t('User cannot checkout this resource for longer then %interval', array('%interval' => $max_length));
      $errors->offsetSet(MERCI_CHECKOUT_DATES, $error);
    }
  }
}

