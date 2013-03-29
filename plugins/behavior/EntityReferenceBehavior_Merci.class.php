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

/**
 * get all date fields on the site organized by entity and bundle
 */
    $fields_info = field_info_instances($instance['entity_type'], $instance['bundle']);
    $date_fields = array();
    foreach ($fields_info as $field_name => $info) {
      $more_info = field_info_field($field_name);
      if ( $more_info['type'] == 'datetime' || $more_info['type'] == 'date' || $more_info['type'] == 'datestamp') {
        $date_fields[$field_name] = $info['label'];
      }
    }
    $form = array();

    if (empty($date_fields)) {
      $form['merci'] = array(
        '#markup' => t('Please add a date field to this entity to use to filter reservations.')
      );
    } else {

      $settings = $field['settings']['handler_settings']['behaviors']['merci'];

      $form['date_field'] = array(
        '#type' => 'select',
        '#title' => t('Date field'),
        '#options' => $date_fields,
        '#default_value' => $settings['date_field'],
        '#description' => t('Select the date field to use to limit reservations of this entity field.'),
      );
    }
    return $form;
  }

  public function validate($entity_type, $entity, $field, $instance, $langcode, $items, &$errors){
    merci_api_validate_reference($entity_type, $entity, $field, $instance, $langcode, $items, &$errors);
  }
}
