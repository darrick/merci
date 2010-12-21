<?php
// $Id$

/**
 * @file
 * merci inventory default views 
 */


/**
 * Implementation of hook_views_default_views().
 */
function merci_inventory_views_default_views() {

  $view = new view;
  $view->name = 'merci_inventory_view';
  $view->description = '';
  $view->tag = '';
  $view->view_php = '';
  $view->base_table = 'node';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('fields', array(
  'title' => array(
    'label' => 'Title',
    'link_to_node' => 1,
    'exclude' => 0,
    'id' => 'title',
    'table' => 'node',
    'field' => 'title',
    'relationship' => 'none',
    'override' => array(
    'button' => 'Override',
    ),
  ),
  'type' => array(
    'label' => 'Type',
    'link_to_node' => 0,
    'exclude' => 0,
    'id' => 'type',
    'table' => 'node',
    'field' => 'type',
    'relationship' => 'none',
    'override' => array(
    'button' => 'Override',
    ),
  ),
  'body' => array(
    'id' => 'body',
    'table' => 'node_revisions',
    'field' => 'body',
  ),
  ));
  $handler->override_option('sorts', array(
  'title' => array(
    'order' => 'ASC',
    'id' => 'title',
    'table' => 'node',
    'field' => 'title',
    'override' => array(
    'button' => 'Override',
    ),
    'relationship' => 'none',
  ),
  ));
  $handler->override_option('filters', array(
  'status' => array(
    'operator' => '=',
    'value' => '1',
    'group' => '0',
    'exposed' => FALSE,
    'expose' => array(
    'operator' => FALSE,
    'label' => '',
    ),
    'id' => 'status',
    'table' => 'node',
    'field' => 'status',
    'override' => array(
    'button' => 'Override',
    ),
    'relationship' => 'none',
  ),
  'type_setting' => array(
    'operator' => 'in',
    'value' => array(
    'bucket' => 'bucket',
    'resource' => 'resource',
    ),
    'group' => '0',
    'exposed' => FALSE,
    'expose' => array(
    'operator' => FALSE,
    'label' => '',
    ),
    'id' => 'type_setting',
    'table' => 'merci_node_type',
    'field' => 'type_setting',
    'override' => array(
    'button' => 'Override',
    ),
    'relationship' => 'none',
  ),
  'type' => array(
    'operator' => 'in',
    'value' => array(
    'merci_boom_pole' => 'merci_boom_pole',
    'merci_conference_room' => 'merci_conference_room',
    'merci_edit_suite' => 'merci_edit_suite',
    'merci_field_camera' => 'merci_field_camera',
    'merci_field_light_kit' => 'merci_field_light_kit',
    'merci_handheld_microphone' => 'merci_handheld_microphone',
    'merci_monopod' => 'merci_monopod',
    'merci_shotgun_microphone' => 'merci_shotgun_microphone',
    'merci_studio' => 'merci_studio',
    'merci_studio_light_kit' => 'merci_studio_light_kit',
    'merci_tripod' => 'merci_tripod',
    ),
    'group' => '0',
    'exposed' => TRUE,
    'expose' => array(
    'use_operator' => 0,
    'operator' => 'type_op',
    'identifier' => 'type',
    'label' => 'Type',
    'optional' => 1,
    'single' => 0,
    'remember' => 1,
    'reduce' => 1,
    ),
    'id' => 'type',
    'table' => 'node',
    'field' => 'type',
    'override' => array(
    'button' => 'Override',
    ),
    'relationship' => 'none',
  ),
  ));
  $handler->override_option('access', array(
  'type' => 'role',
  'role' => array(
    '13' => 13,
    '3' => 3,
  ),
  ));
  $handler->override_option('cache', array(
  'type' => 'none',
  ));
  $handler->override_option('title', 'MERCI Inventory');
  $handler->override_option('items_per_page', 25);
  $handler->override_option('use_pager', '1');
  $handler->override_option('style_plugin', 'table');
  $handler->override_option('style_options', array(
  'grouping' => '',
  'override' => 1,
  'sticky' => 0,
  'order' => 'asc',
  'columns' => array(
    'title' => 'title',
    'type' => 'type',
  ),
  'info' => array(
    'title' => array(
    'sortable' => 1,
    'separator' => '',
    ),
    'type' => array(
    'sortable' => 1,
    'separator' => '',
    ),
  ),
  'default' => '-1',
  ));
  $handler = $view->new_display('page', 'Inventory Listing', 'page_1');
  $handler->override_option('fields', array(
  'title' => array(
    'label' => 'Title',
    'link_to_node' => 1,
    'exclude' => 0,
    'id' => 'title',
    'table' => 'node',
    'field' => 'title',
    'relationship' => 'none',
    'override' => array(
    'button' => 'Override',
    ),
  ),
  'type' => array(
    'label' => 'Type',
    'link_to_node' => 0,
    'exclude' => 0,
    'id' => 'type',
    'table' => 'node',
    'field' => 'type',
    'relationship' => 'none',
    'override' => array(
    'button' => 'Override',
    ),
  ),
  'default_availability' => array(
    'label' => 'Bucket Status',
    'alter' => array(
    'alter_text' => 0,
    'text' => '',
    'make_link' => 0,
    'path' => '',
    'link_class' => '',
    'alt' => '',
    'prefix' => '',
    'suffix' => '',
    'help' => '',
    'trim' => 0,
    'max_length' => '',
    'word_boundary' => 1,
    'ellipsis' => 1,
    'strip_tags' => 0,
    'html' => 0,
    ),
    'exclude' => 0,
    'id' => 'default_availability',
    'table' => 'merci_bucket_node',
    'field' => 'default_availability',
    'override' => array(
    'button' => 'Use default',
    ),
    'relationship' => 'none',
  ),
  'default_availability_1' => array(
    'label' => 'Resource Status',
    'alter' => array(
    'alter_text' => 0,
    'text' => '',
    'make_link' => 0,
    'path' => '',
    'link_class' => '',
    'alt' => '',
    'prefix' => '',
    'suffix' => '',
    'help' => '',
    'trim' => 0,
    'max_length' => '',
    'word_boundary' => 1,
    'ellipsis' => 1,
    'strip_tags' => 0,
    'html' => 0,
    ),
    'exclude' => 0,
    'id' => 'default_availability_1',
    'table' => 'merci_resource_node',
    'field' => 'default_availability',
    'override' => array(
    'button' => 'Use default',
    ),
    'relationship' => 'none',
  ),
  ));
  $handler->override_option('path', 'admin/merci/manage/inventory/view');
  $handler->override_option('menu', array(
  'type' => 'default tab',
  'title' => 'Inventory',
  'description' => '',
  'weight' => '8',
  'name' => 'navigation',
  ));
  $handler->override_option('tab_options', array(
  'type' => 'normal',
  'title' => 'Inventory',
  'description' => '',
  'weight' => '0',
  'name' => 'navigation',
  ));

  $views[$view->name] = $view;
  
  $view = new view;
  $view->name = 'merci_inventory_managed_items';
  $view->description = '';
  $view->tag = '';
  $view->view_php = '';
  $view->base_table = 'node';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('fields', array(
  'title' => array(
    'label' => '',
    'alter' => array(
    'alter_text' => 0,
    'text' => '',
    'make_link' => 0,
    'path' => '',
    'link_class' => '',
    'alt' => '',
    'prefix' => '',
    'suffix' => '',
    'target' => '',
    'help' => '',
    'trim' => 0,
    'max_length' => '',
    'word_boundary' => 1,
    'ellipsis' => 1,
    'html' => 0,
    'strip_tags' => 0,
    ),
    'empty' => '',
    'hide_empty' => 0,
    'empty_zero' => 0,
    'link_to_node' => 0,
    'exclude' => 0,
    'id' => 'title',
    'table' => 'node',
    'field' => 'title',
    'relationship' => 'none',
  ),
  ));
  $handler->override_option('filters', array(
  'type_setting' => array(
    'operator' => 'in',
    'value' => array(
    'bucket' => 'bucket',
    'resource' => 'resource',
    ),
    'group' => '0',
    'exposed' => FALSE,
    'expose' => array(
    'operator' => FALSE,
    'label' => '',
    ),
    'id' => 'type_setting',
    'table' => 'merci_node_type',
    'field' => 'type_setting',
    'relationship' => 'none',
  ),
  'status' => array(
    'operator' => '=',
    'value' => '1',
    'group' => '0',
    'exposed' => FALSE,
    'expose' => array(
    'operator' => FALSE,
    'label' => '',
    ),
    'id' => 'status',
    'table' => 'node',
    'field' => 'status',
    'relationship' => 'none',
  ),
  ));
  $handler->override_option('access', array(
  'type' => 'none',
  ));
  $handler->override_option('cache', array(
  'type' => 'none',
  ));
  $handler->override_option('items_per_page', 0);

  $views[$view->name] = $view;
  
  $view = new view;
  $view->name = 'merci_inventory_take';
  $view->description = '';
  $view->tag = '';
  $view->view_php = '';
  $view->base_table = 'node';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = TRUE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('fields', array(
  'title' => array(
    'label' => 'Title',
    'alter' => array(
    'alter_text' => 0,
    'text' => '',
    'make_link' => 0,
    'path' => '',
    'link_class' => '',
    'alt' => '',
    'prefix' => '',
    'suffix' => '',
    'target' => '',
    'help' => '',
    'trim' => 0,
    'max_length' => '',
    'word_boundary' => 1,
    'ellipsis' => 1,
    'html' => 0,
    'strip_tags' => 0,
    ),
    'empty' => '',
    'hide_empty' => 0,
    'empty_zero' => 0,
    'link_to_node' => 1,
    'exclude' => 0,
    'id' => 'title',
    'table' => 'node',
    'field' => 'title',
    'relationship' => 'none',
  ),
  ));
  $handler->override_option('filters', array(
  'status_1' => array(
    'operator' => '=',
    'value' => '1',
    'group' => '0',
    'exposed' => FALSE,
    'expose' => array(
    'operator' => FALSE,
    'label' => '',
    ),
    'id' => 'status_1',
    'table' => 'node',
    'field' => 'status',
    'relationship' => 'none',
  ),
  'type_setting' => array(
    'operator' => 'in',
    'value' => array(
    'bucket' => 'bucket',
    'resource' => 'resource',
    ),
    'group' => '0',
    'exposed' => FALSE,
    'expose' => array(
    'operator' => FALSE,
    'label' => '',
    ),
    'id' => 'type_setting',
    'table' => 'merci_node_type',
    'field' => 'type_setting',
    'relationship' => 'none',
  ),
  ));
  $handler->override_option('access', array(
  'type' => 'none',
  ));
  $handler->override_option('cache', array(
  'type' => 'none',
  ));
  $handler->override_option('items_per_page', 50);
  $handler->override_option('use_pager', 'mini');
  $handler->override_option('style_plugin', 'bulk');
  $handler->override_option('style_options', array(
  'grouping' => '',
  'override' => 1,
  'sticky' => 0,
  'order' => 'asc',
  'columns' => array(
    'title' => 'title',
  ),
  'info' => array(
    'title' => array(
    'sortable' => 0,
    'separator' => '',
    ),
  ),
  'default' => '-1',
  'execution_type' => '1',
  'display_type' => '0',
  'hide_select_all' => 0,
  'skip_confirmation' => 0,
  'display_result' => 1,
  'merge_single_action' => 1,
  'selected_operations' => array(
    'merci_inventory_operations_update' => 'merci_inventory_operations_update',
    ),
  'views_bulk_operations_fields_action' => array(
    'php_code' => 0,
    'display_fields' => array(
    '_all_' => '_all_',
    ),
  ),
  ));
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->override_option('path', 'admin/merci/manage/inventory/take');
  $handler->override_option('menu', array(
  'type' => 'tab',
  'title' => 'Take Inventory',
  'description' => '',
  'weight' => '10',
  'name' => 'navigation',
  ));
  $handler->override_option('tab_options', array(
  'type' => 'none',
  'title' => '',
  'description' => '',
  'weight' => 0,
  'name' => 'navigation',
  ));

  $views[$view->name] = $view;

  return $views;
}

