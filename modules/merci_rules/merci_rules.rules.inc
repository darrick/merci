<?php
/**
 * @file
 * Provides Rules integration for reservations.
 *
 */

function merci_rules_rules_event_info_alter(&$events) {
  $form_events = variable_get('rules_forms_event_info', array());
  foreach ($form_events as $form_id => $info) {
    $events['rules_forms_' . $form_id . '_form_validate']['variables']['entity'] = array(
      'type' => 'entity',
      'label' => 'Entity',
      'handler' => 'merci_rules_events_variable_entity_load',
    ); 
  }
}

/** 
 * Implementation of hook_rules_condition_info()
 * Provides a Rules condition for "contains a conflict"
 */
function merci_rules_rules_condition_info() {
  return array(
    'merci_rules_condition_conflict' => array(
      'group' => t('MERCI'),
      'label' => t('Contains a reservation conflict'),
      'module' => 'merci_rules',
      'parameter' => array(
        'entity' => array(
          'type' => 'entity',
          'label' => t('Entity'),
          'description' => t('Specifies the entity for which to evaluate the condition.'),
          'restriction' => 'selector',
        ),
        'datetime_field' => array(
          'type' => 'text',
          'label' => t('Date Field'),
          'description' => t('The name of the date field for the reservation.'),
          'options list' => 'merci_rules_condition_entity_has_field_options',
          'restriction' => 'input',
        ),
        'entityreference_field' => array(
          'type' => 'text',
          'label' => t('Field'),
          'description' => t('The name of the field containing the reservable items.'),
          'options list' => 'merci_rules_condition_entity_has_field_options',
          'restriction' => 'input',
        ),
      ),
    ),
  );
}

/** 
 * Implementation of hook_rules_action_info()
 * Provides a Rules action for "load list of conflicting nodes"
 */
function merci_rules_rules_action_info() {
  return array(
    'merci_rules_action_unavailable_entity_list' => array(
      'group' => t('MERCI'),
      'label' => t('Load a list selected items which are unavailable'),
      'module' => 'merci_rules',
      'parameter' => array(
        'entity' => array(
          'type' => 'entity',
          'label' => t('Entity'),
          'description' => t('Specifies the entity for which to evaluate the condition.'),
          'restriction' => 'selector',
        ),
        'datetime_field' => array(
          'type' => 'text',
          'label' => t('Date Field'),
          'description' => t('The name of the date field for the reservation.'),
          'options list' => 'merci_rules_condition_entity_has_field_options',
          'restriction' => 'input',
        ),
        'entityreference_field' => array(
          'type' => 'text',
          'label' => t('Field'),
          'description' => t('The name of the field containing the reservable items.'),
          'options list' => 'merci_rules_condition_entity_has_field_options',
          'restriction' => 'input',
        ),
      ),
      'provides' => array(
        'entity_object_list' => array(
          'type' => 'list<integer>',
          'label' => t('List of conflicting reservation entity ids'),
        ),
      ),
    ),
  );
}

/**
 *  * Returns options for choosing a field for the selected entity.
 *   */
function merci_rules_condition_entity_has_field_options(RulesAbstractPlugin $element, $param_name) {
  $options = array();
  list($type) = explode('_', $param_name);

  foreach (field_info_fields() as $field_name => $field) {
    if ($field['type'] == $type) {
      foreach ($field['bundles'] as $entity_type => $bundles) {
        if (!isset($options[$entity_type])) {
          $options[$entity_type] = array();
        }
        foreach ($bundles as $bundle) {
          $options[$entity_type][$field_name] = $bundle . ':' . $field_name;
        }
      }
    }
  }
  return $options;
}
