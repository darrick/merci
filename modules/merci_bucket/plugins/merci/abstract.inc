<?php

/**
 * Additional behaviors for a Entity Reference field.
 *
 * Implementations that wish to provide an implementation of this should
 * register it using CTools' plugin system.
 */
interface Merci_Handler {

  /**
   * Constructor for the behavior.
   *
   * @param $behavior
   *   The name of the behavior plugin.
   */
  public function __construct($behavior);

  /**
   * Act on validating an entity reference field.
   *
   * @see hook_field_validate()
   */
  public function validate($entity_type, $entity, $field, $instance, $langcode, $items, &$errors);

}

/**
 * An abstract implementation of Merci_ConflictHandler.
 */
abstract class Merci_ConflictHandler_Abstract implements Merci_ConflictHandler {

  /**
   * The name of the behavior plugin.
   */
  protected $behavior;

  /**
   * The plugin definition.
   */
  protected $plugin;

  public function __construct($behavior) {
    $this->behavior = $behavior;

    ctools_include('plugins');
    $plugin = ctools_get_plugins('entityreference', 'behavior', $behavior);
    $this->plugin = $plugin;
  }

  public function schema_alter(&$schema, $field) {}

  public function property_info_alter(&$info, $entity_type, $field, $instance, $field_type) {}

  public function views_data_alter(&$data, $field) {}

  public function load($entity_type, $entities, $field, $instances, $langcode, &$items) {}

  public function is_empty_alter(&$empty, $item, $field) {}

  public function validate($entity_type, $entity, $field, $instance, $langcode, $items, &$errors) {}

  public function presave($entity_type, $entity, $field, $instance, $langcode, &$items) {}

  public function insert($entity_type, $entity, $field, $instance, $langcode, &$items) {}

  public function postInsert($entity_type, $entity, $field, $instance) {}

  public function update($entity_type, $entity, $field, $instance, $langcode, &$items) {}

  public function postUpdate($entity_type, $entity, $field, $instance) {}

  public function delete($entity_type, $entity, $field, $instance, $langcode, &$items) {}

  public function postDelete($entity_type, $entity, $field, $instance) {}

  public function entityPostInsert($entity_type, $entity, $field, $instance) {}

  public function entityPostUpdate($entity_type, $entity, $field, $instance) {}

  public function entityPostDelete($entity_type, $entity, $field, $instance) {}

  public function settingsForm($field, $instance) {}

  public function access($field, $instance) {
    return TRUE;
  }
}

/**
 * A broken implementation of Merci_ConflictHandler.
 */
class Merci_ConflictHandler_Broken extends Merci_ConflictHandler_Abstract {
  public function settingsForm($field, $instance) {
    $form['behavior_handler'] = array(
      '#markup' => t('The selected behavior handler is broken.'),
    );
    return $form;
  }
}
