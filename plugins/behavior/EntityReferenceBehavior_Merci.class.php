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

    $default = isset($instance['settings']['behaviors']['merci']['preset']) ? $instance['settings']['behaviors']['merci']['preset'] : '';

    $presets = merci_preset_load();

    // If there are presets prompt the user to select one or create another.
    // If no, prompt to create a preset.
    if (!empty($presets)) {
      foreach (merci_preset_load() as $preset => $item) {
        $options[$preset] = $item['preset_name'];
      }

      $form['preset'] = array(
        '#title' => t('Select preset'),
        '#type' => 'select',
        '#default_value' => $default,
        '#options' => $options,
      );
      $form['links'] = array(
        '#theme' => 'links',
        '#links' => array(
          array(
            'title' => t('Create new preset'),
            'href' => 'admin/structure/merci/merci/add',
          ),
          array(
            'title' => t('Manage presets'),
            'href' => 'admin/structure/merci/merci',
          ),
        ),
      );
    }
    else {
      $form['no_preset_message'] = array(
        '#markup' => '<div class="messages warning">' . t('No presets are available. You must to <a href="@create">create a preset</a> to proceed.', array('@create' => url('admin/structure/merci/merci/add'))) . '</div>',
      );
    }
    $data = '(function ($) { $(document).ready(function() { $("input").click(function() { ($(this).val() == "reservation") ? $("#merci-preset-div").show() : $("#merci-preset-div").hide(); });}); })(jQuery);';
    //drupal_add_js($data, array('type' => 'inline'));
    return $form;
  }

  public function validate($entity_type, $entity, $field, $instance, $langcode, $items, &$errors){

    $targets = array();
    $target_type = $instance['entity_type'];
    list($entity_id,,) = entity_extract_ids($entity_type, $entity);

    $preset = $instance['settings']['behaviors']['merci']['preset'];
    $target_type = $instance['entity_type'];
    $settings = merci_preset_load_settings($preset);

    $date_field = $settings['date_field'];
    $dates = property_exists($entity, $date_field) ? $entity->{$date_field}[LANGUAGE_NONE][0] : NULL;

    foreach ($items as $delta => $item) {
      if (!empty($item['target_id'])) {
        $targets[] = $item['target_id'];
      }
    }

    $targets = entity_load($target_type, $targets);

    foreach ($items as $delta => $item) {
      $target = $targets[$item['target_id']];
      $handler = Merci_ReservableHandler_Generic::getInstance($target_type, $target, $preset); 

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
