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
    return merci_api_settings_form($field, $instance);
  }

  public function validate($entity_type, $entity, $field, $instance, $langcode, $items, &$errors){
    merci_api_validate_reference($entity_type, $entity, $field, $instance, $langcode, $items, &$errors);
  }
}
