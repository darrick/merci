<?php
/**
 * @file
 * nscad_feature.features.inc
 */

/**
 * Implements hook_views_api().
 */
function nscad_feature_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function nscad_feature_node_info() {
  $items = array(
    'example_item' => array(
      'name' => t('Example Item'),
      'base' => 'node_content',
      'description' => t('Example reservable item.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'location' => array(
      'name' => t('Location'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'reservable_kit' => array(
      'name' => t('Reservable Kit'),
      'base' => 'node_content',
      'description' => t('A kit of reservable items.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
