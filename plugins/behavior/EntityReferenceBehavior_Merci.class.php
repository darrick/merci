<?php

/**
 * @file
 * CTools plugin class for the taxonomy-index behavior.
 */

/**
 * Extends an entityreference field to maintain its references to taxonomy terms
 * in the {taxonomy_index} table.
 *
 * Note, unlike entityPostInsert() and entityPostUpdate(), entityDelete()
 * is not needed as cleanup is performed by taxonomy module in
 * taxonomy_delete_node_index().
 */
class EntityReferenceBehavior_Merci extends EntityReference_BehaviorHandler_Abstract {

  /**
   * Overrides EntityReference_BehaviorHandler_Abstract::access().
   *
   * Ensure that it is only enabled for ER instances on nodes targeting
   * terms, and the core variable to maintain index is enabled.
   */
  public function access($field, $instance) {
    return true;
  }

  public function presave($entity_type, $entity, $field, $instance, $langcode, &$items) {
   // merci_api_presave_reference($entity_type, $entity, $field, $instance, $langcode, $items, &$errors);
  }
  public function insert($entity_type, $entity, $field, $instance, $langcode, &$items) {

    //merci_api_insert_reference($entity_type, $entity, $field, $instance, $langcode, $items, &$errors);
  }
  /**
   * Overrides EntityReference_BehaviorHandler_Abstract::entityPostInsert().
   *
   * Runs after hook_node_insert() used by taxonomy module.
   */
  public function entityPostInsert($entity_type, $entity, $field, $instance) {
  }

  /**
   * Overrides EntityReference_BehaviorHandler_Abstract::entityPostUpdate().
   *
   * Runs after hook_node_update() used by taxonomy module.
   */
  public function entityPostUpdate($entity_type, $entity, $field, $instance) {
  }

  /**
   * Overrides EntityReference_BehaviorHandler_Abstract::settingsForm().
   */
  public function settingsForm($field, $instance) {
    $form = array();

    $settings = isset($instance['settings']['behaviors']['merci']) ? $instance['settings']['behaviors']['merci'] : array();
    /**
     * get all date fields on the site organized by entity and bundle
     * Use field_info_field_map() instead.
     */
    $fields_info = field_info_instances($instance['entity_type'], $instance['bundle']);

    // Allow the uesr to select the name of the entityreference attached to a reservatble item.
    $date_fields = array('disabled' => t('Disabled'));

    foreach ($fields_info as $field_name => $field_info) {
      $more_info = field_info_field($field_name);
      if ( $more_info['type'] == 'datetime') {
        $date_fields[$field_name] = $field_name;
      }
    }

    $form['date_field'] = array(
      '#type' => 'select',
      '#title' => t('Date field'),
      '#options' => $date_fields,
      '#default_value' => array_key_exists('date_field', $settings) ? $settings['date_field'] : NULL,
      '#description' => t('Select the date field to use for reservations.'),
    );

    // Allow the uesr to select the name of the entityreference attached to a reservatble item.
    $status_fields = array('disabled' => t('Disabled'));

    foreach ($fields_info as $field_name => $field_info) {
      $more_info = field_info_field($field_name);
      if ( $more_info['type'] == 'merci_reservation_status') {
        $status_fields[$field_name] = $field_name;
      }
    }

    $form['status_field'] = array(
      '#type' => 'select',
      '#title' => t('Status field'),
      '#options' => $status_fields,
      '#default_value' => array_key_exists('status_field', $settings) ? $settings['status_field'] : NULL,
      '#description' => t('Select the status field to use for reservations.'),
    );

    $form['overdue_gracetime'] = array(
      '#type' => 'textfield',
      '#title' => t('Status grace time'),
      '#description' => t('Relative gracetime a reservation can be overdue to not conflict with the reservation; valid periods are "y" for years, "m" for months, "w" for weeks, and "d" for days. For example, "+1m +7d" represents one month and seven days from today. "-1d" represents one day before today.'),
      '#default_value' => $settings['overdue_gracetime'],
    ); 

    return $form;
  }

  public function validate($entity_type, $entity, $field, $instance, $langcode, $items, &$errors){
    $target_type = $field['settings']['target_type'];

    $targets = array($target_type => $items);

    $context = $instance['settings']['behaviors']['merci'];

    $context += array(
      'target_field' => $this->field['field_name'],
      'field' => $field,
      'instance' => $instance,
      'langcode' => $langcode,
    );

    merci_items_validate($targets, $entity_type, $entity, $context, $errors);
  }
}
