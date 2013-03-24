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
    $form = array();
    $form['merci'] = array(
      '#markup' => t('This behavior has no options')
    );
    return $form;
  }
  public function validate($entity_type, $entity, $field, $instance, $langcode, $items, &$errors){
    dpm ($entity);
    dpm($field);
    dpm($instance);
    dpm($items);
    dpm($entity_type);

    $langcode = $entity->language ? $entity->language : LANGUAGE_NONE;
    $target_id  = $items[0]['target_id'];        

    // Is this content type active?

    // Does the user have access to manage reservations or this content type?

    // Did the user select too many of the same bucket item?

    // Did the user select too many of the same item?

    // Is it available?
    if (merci_is_item_reserved($target_id, $entity)) {
      $errors[$field['field_name']][$langcode][0][] = array(
        'error' => 'merci_item_conflict',
        'message' => t('%name: the item cannot be reserved at this time.', array('%name' => $instance['label'])),
      );
    }

    // Are we checking an existing item?

    // Is the item available at this time?

    // How many items are overdue and thus unavailable at this time?

    // Show the error if conflict due to overdue.

    // Check item restrictions.  max hours, etc.
  }
}
