<?php

/**
 * Views for line item reference displays.
 */

/**
 * Implements hook_views_default_views().
 */
function merci_line_item_views_default_views() {
  $view = new view();
  $view->name = 'merci_line_item_table';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'merci_line_item';
  $view->human_name = 'Merci Line Item Table';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['grouping'] = array(
    0 => array(
      'field' => 'merci_checkout_status',
      'rendered' => 1,
      'rendered_strip' => 0,
    ),
    1 => array(
      'field' => 'merci_checkout_dates',
      'rendered' => 1,
      'rendered_strip' => 0,
    ),
  );
  $handler->display->display_options['style_options']['columns'] = array(
    'line_item_id' => 'line_item_id',
    'merci_checkout_dates' => 'merci_checkout_dates',
    'merci_checkout_status' => 'merci_checkout_status',
    'merci_resource' => 'merci_resource',
    'quantity' => 'quantity',
    'sku' => 'sku',
    'merci_resource_display' => 'merci_resource_display',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'line_item_id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'merci_checkout_dates' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'merci_checkout_status' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'merci_resource' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'quantity' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'sku' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'merci_resource_display' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['merci_resource_target_id']['id'] = 'merci_resource_target_id';
  $handler->display->display_options['relationships']['merci_resource_target_id']['table'] = 'field_data_merci_resource';
  $handler->display->display_options['relationships']['merci_resource_target_id']['field'] = 'merci_resource_target_id';
  /* Field: MERCI Line Item: Line item ID */
  $handler->display->display_options['fields']['line_item_id']['id'] = 'line_item_id';
  $handler->display->display_options['fields']['line_item_id']['table'] = 'merci_line_item';
  $handler->display->display_options['fields']['line_item_id']['field'] = 'line_item_id';
  /* Field: MERCI Line item: Checkout Status */
  $handler->display->display_options['fields']['merci_checkout_status']['id'] = 'merci_checkout_status';
  $handler->display->display_options['fields']['merci_checkout_status']['table'] = 'field_data_merci_checkout_status';
  $handler->display->display_options['fields']['merci_checkout_status']['field'] = 'merci_checkout_status';
  /* Field: MERCI Line item: Merci Resource */
  $handler->display->display_options['fields']['merci_resource']['id'] = 'merci_resource';
  $handler->display->display_options['fields']['merci_resource']['table'] = 'field_data_merci_resource';
  $handler->display->display_options['fields']['merci_resource']['field'] = 'merci_resource';
  $handler->display->display_options['fields']['merci_resource']['settings'] = array(
    'link' => 0,
  );
  /* Field: MERCI Line Item: Quantity */
  $handler->display->display_options['fields']['quantity']['id'] = 'quantity';
  $handler->display->display_options['fields']['quantity']['table'] = 'merci_line_item';
  $handler->display->display_options['fields']['quantity']['field'] = 'quantity';
  $handler->display->display_options['fields']['quantity']['precision'] = '0';
  /* Field: MERCI Line item: Checkout Dates */
  $handler->display->display_options['fields']['merci_checkout_dates']['id'] = 'merci_checkout_dates';
  $handler->display->display_options['fields']['merci_checkout_dates']['table'] = 'field_data_merci_checkout_dates';
  $handler->display->display_options['fields']['merci_checkout_dates']['field'] = 'merci_checkout_dates';
  $handler->display->display_options['fields']['merci_checkout_dates']['settings'] = array(
    'format_type' => 'long',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Field: Commerce Product: SKU */
  $handler->display->display_options['fields']['sku']['id'] = 'sku';
  $handler->display->display_options['fields']['sku']['table'] = 'commerce_product';
  $handler->display->display_options['fields']['sku']['field'] = 'sku';
  $handler->display->display_options['fields']['sku']['relationship'] = 'merci_resource_target_id';
  $handler->display->display_options['fields']['sku']['link_to_product'] = 0;
  /* Field: MERCI Line item: Resource Display */
  $handler->display->display_options['fields']['merci_resource_display']['id'] = 'merci_resource_display';
  $handler->display->display_options['fields']['merci_resource_display']['table'] = 'field_data_merci_resource_display';
  $handler->display->display_options['fields']['merci_resource_display']['field'] = 'merci_resource_display';
  $handler->display->display_options['fields']['merci_resource_display']['settings'] = array(
    'link' => 0,
  );
  /* Sort criterion: MERCI Line item: Checkout Status (merci_checkout_status) */
  $handler->display->display_options['sorts']['merci_checkout_status_value']['id'] = 'merci_checkout_status_value';
  $handler->display->display_options['sorts']['merci_checkout_status_value']['table'] = 'field_data_merci_checkout_status';
  $handler->display->display_options['sorts']['merci_checkout_status_value']['field'] = 'merci_checkout_status_value';
  /* Sort criterion: Commerce Product: SKU */
  $handler->display->display_options['sorts']['sku']['id'] = 'sku';
  $handler->display->display_options['sorts']['sku']['table'] = 'commerce_product';
  $handler->display->display_options['sorts']['sku']['field'] = 'sku';
  $handler->display->display_options['sorts']['sku']['relationship'] = 'merci_resource_target_id';
  /* Contextual filter: MERCI Line Item: Line item ID */
  $handler->display->display_options['arguments']['line_item_id']['id'] = 'line_item_id';
  $handler->display->display_options['arguments']['line_item_id']['table'] = 'merci_line_item';
  $handler->display->display_options['arguments']['line_item_id']['field'] = 'line_item_id';
  $handler->display->display_options['arguments']['line_item_id']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['line_item_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['line_item_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['line_item_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['line_item_id']['summary_options']['items_per_page'] = '25';
  /* Filter criterion: MERCI Line Item: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'merci_line_item';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'merci_line_item' => 'merci_line_item',
  );

  $views[$view->name] = $view;

  return $views;
}
