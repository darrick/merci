<?php
/**
 * @file
 * evergreen_resources.features.inc
 */

/**
 * Implements hook_commerce_product_default_types().
 */
function evergreen_resources_commerce_product_default_types() {
  $items = array(
    'merci_resource' => array(
      'type' => 'merci_resource',
      'name' => 'Resource',
      'description' => 'This resource is reservable, and works with the commerce reservation system.',
      'help' => '',
      'revision' => 1,
    ),
  );
  return $items;
}

/**
 * Implements hook_ctools_plugin_api().
 */
function evergreen_resources_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function evergreen_resources_node_info() {
  $items = array(
    'merci_resource_display' => array(
      'name' => t('Resource Display'),
      'base' => 'node_content',
      'description' => t('A resource display for a reservable item.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
