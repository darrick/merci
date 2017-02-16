<?php
/**
 * @file
 * The primary PHP file for this theme.
 */

function nscad_preprocess_merci_printable_contract(&$variables) {

  $wrapper = entity_metadata_wrapper('node', $variables['node']);

  // Create kit variable.
  $kits = array();
  $kit_items = array();
  foreach ($wrapper->merci_field_kit->getIterator() as $delta => $kit_wrapper) {
    $kit = array(
      '#title' => $kit_wrapper->label(),
      'items' => array(),
    );
    foreach ($kit_wrapper->field_items as $delta => $item_wrapper) {
      // Get accessories.
      $accessories = array();
      foreach ($variables['items'] as $delta => $item) {
        if ($item['merci_item_nid'] == $item_wrapper->getIdentifier()) {
          $item_node = node_load($variables['items'][$delta]['merci_placeholder_nid']);
          $placeholder_wrapper = entity_metadata_wrapper('node', $item_node);
          try {
            foreach ($placeholder_wrapper->field_merci_accessories->getIterator() as $delta => $entity_wrapper) {
              $accessories[] = $entity_wrapper->label();
            }
          }
          catch (EntityMetadataWrapperException $exc) {
            // Do nothing.
          }
        }
      }

      $kit['items'][] = array(
        'item_title' => $item_wrapper->label(),
        'accessories' => $accessories,
      );
      $kit_items[] = $item_wrapper->getIdentifier();
    }
    $kits[] = $kit;
  }
  $variables['kits'] = $kits;

  $selected_items = array();
  foreach ($variables['items'] as $delta => $item) {
    $selected_items[$delta] = $item['merci_item_nid'];
  }

  $non_kit_items = array_diff($selected_items, $kit_items);

  $items = array();
  foreach ($non_kit_items as $delta => $item_id) {
    $item_node = node_load($variables['items'][$delta]['merci_placeholder_nid']);
    $item_wrapper = entity_metadata_wrapper('node', $item_node);
    $item = array(
      'item_title' => $variables['items'][$delta]['item_title'],
      'accessories' => array(),
    );
    foreach ($item_wrapper->field_merci_accessories->getIterator() as $delta => $entity_wrapper) {
      $item['accessories'][] = $entity_wrapper->label();
    }
    $items[] = $item;

  }
  $variables['items'] = $items;
} // merci_printable_contract
