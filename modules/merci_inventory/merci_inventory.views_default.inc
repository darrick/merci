<?php

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
	'status' => array(
	  'label' => 'Inventory Status',
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
	  'exclude' => 0,
	  'id' => 'status',
	  'table' => 'merci_node_type',
	  'field' => 'status',
	  'override' => array(
		'button' => 'Use default',
	  ),
	  'relationship' => 'none',
	),
	'item_status' => array(
	  'label' => 'Reservation Status',
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
	  'exclude' => 0,
	  'id' => 'item_status',
	  'table' => 'merci_reservation_item_node',
	  'field' => 'item_status',
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

  return $views;
}

