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

    return $form;
  }

  public function validate($entity_type, $entity, $field, $instance, $langcode, $items, &$errors){

    dpm($instance);
    dpm($field);
    if ($entity_type == NULL) {
      return;
    }
    $targets = array();
    $target_type = $field['settings']['target_type'];//$instance['entity_type'];
    list($entity_id,,) = entity_extract_ids($entity_type, $entity);

    $settings = $instance['settings']['behaviors']['merci'];

    $date_field = $settings['date_field'];
    $settings['target_field'] = $field['field_name'];
    $settings['instance'] = $instance;
    $dates = property_exists($entity, $date_field) ? $entity->{$date_field}[LANGUAGE_NONE][0] : NULL;

    foreach ($items as $delta => $item) {
      if (!empty($item['target_id'])) {
        $targets[] = $item['target_id'];
      }
    }

    $targets = entity_load($target_type, $targets);
    dpm($target_type);
    dpm($targets);

    foreach ($items as $delta => $item) {
      if (empty($item['target_id'])) {
        continue;
      }
      $target = $targets[$item['target_id']];
      $handler = Merci_ReservableHandler_Generic::getInstance($target_type, $target, $settings); 
      dpm($handler);

      // Check availability
      if (!$handler->available($dates, $entity_id)) {
        $errors[$field['field_name']][$langcode][$delta][] = array(
          'error' => 'merci',
          'message' => t("Item is not available to reserve"),
          'delta' => $delta,
        );
      }
    }

    // TODO: check too many.
    // Check too many.
    /*
    $selected_count[$target_id] = isset($selected_count[$target_id]) ? $selected_count[$target_id] + 1 : 1;
    if ( ! isset($inventory_count[$target_id])) { 
      $inventory_count[$target_id]       = $handler->itemCount(); 
    }
    if ($selected_count[$target_id] > $inventory_count[$target_id]) {
      $errors[$target_field][$langcode][$delta][] = array(
        'error' => 'merci_item_conflict',
        'message' => t("You've selected too many %name's.  We only have %amount in the current inventory.", 
        array('%name' => $node->title, '%amount' => $inventory_count[$target_id])),
      );
      continue;
    }
     */
  }
}
