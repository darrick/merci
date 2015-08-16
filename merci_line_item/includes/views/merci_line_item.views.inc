<?php

/**
 * Export Drupal Commerce line items to Views.
 */

/**
 * Implements hook_views_data()
 */
function merci_line_item_views_data() {
  $data = array();

  $data['merci_line_item']['table']['group']  = t('MERCI Line Item');

  $data['merci_line_item']['table']['base'] = array(
    'field' => 'line_item_id',
    'title' => t('MERCI Line Item'),
    'help' => t('A line item referenced by another entity.'),
    'access query tag' => 'merci_line_item_access',
  );
  $data['merci_line_item']['table']['entity type'] = 'merci_line_item';

  // Expose the line item ID.
  $data['merci_line_item']['line_item_id'] = array(
    'title' => t('Line item ID'),
    'help' => t('The unique internal identifier of the line item.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'merci_line_item_handler_argument_line_item_line_item_id',
      'name field' => 'line_item_label',
      'numeric' => TRUE,
      'validate type' => 'line_item_id',
    ),
  );

  // Expose the product type.
  $data['merci_line_item']['type'] = array(
    'title' => t('Type'),
    'help' => t('The human-readable name of the type of the line item.'),
    'field' => array(
      'handler' => 'merci_line_item_handler_field_line_item_type',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'merci_line_item_handler_filter_line_item_type',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // TODO: Expose the display view build mode.

  // Expose the type-dependent line item title.
  $data['merci_line_item']['line_item_title'] = array(
    'field' => array(
      'title' => t('Title'),
      'help' => t('The title of the line item determined by its type.'),
      'handler' => 'merci_line_item_handler_field_line_item_title',
    ),
  );

  // Expose the line item label.
  $data['merci_line_item']['line_item_label'] = array(
    'title' => t('Label'),
    'help' => t('The label of the line item.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the line item quantity.
  $data['merci_line_item']['quantity'] = array(
    'title' => t('Quantity'),
    'help' => t('The quantity of the line item.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
      'float' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  // Adds a textfield to edit line item quantity on the view.
  $data['merci_line_item']['edit_quantity'] = array(
    'field' => array(
      'title' => t('Quantity text field'),
      'help' => t('Adds a text field to edit the line item quantity in the View.'),
      'handler' => 'merci_line_item_handler_field_edit_quantity',
    ),
  );

  // Adds a button to delete a line item.
  $data['merci_line_item']['edit_delete'] = array(
    'field' => array(
      'title' => t('Delete button'),
      'help' => t('Adds a button to delete a line item.'),
      'handler' => 'merci_line_item_handler_field_edit_delete',
    ),
  );

  // Expose the order ID.
  /*
  $data['merci_line_item']['entity_id'] = array(
    'title' => t('Entity ID', array(), array('context' => 'a MERCI reservation')),
    'help' => t('The unique internal identifier of the associated reservation.'),
    'field' => array(
      'handler' => 'commerce_order_handler_field_order',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'commerce_order_handler_argument_order_order_id',
      'name field' => 'order_number',
      'numeric' => TRUE,
      'validate type' => 'order_id',
    ),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'commerce_order',
      'field' => 'order_id',
      'label' => t('Order', array(), array('context' => 'a drupal commerce order')),
    ),
  );
   */

  // Expose the created and changed timestamps.
  $data['merci_line_item']['created'] = array(
    'title' => t('Created date'),
    'help' => t('The date the line item was created.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  $data['merci_line_item']['created_fulldate'] = array(
    'title' => t('Created date'),
    'help' => t('In the form of CCYYMMDD.'),
    'argument' => array(
      'field' => 'created',
      'handler' => 'views_handler_argument_node_created_fulldate',
    ),
  );

  $data['merci_line_item']['created_year_month'] = array(
    'title' => t('Created year + month'),
    'help' => t('In the form of YYYYMM.'),
    'argument' => array(
      'field' => 'created',
      'handler' => 'views_handler_argument_node_created_year_month',
    ),
  );

  $data['merci_line_item']['created_timestamp_year'] = array(
    'title' => t('Created year'),
    'help' => t('In the form of YYYY.'),
    'argument' => array(
      'field' => 'created',
      'handler' => 'views_handler_argument_node_created_year',
    ),
  );

  $data['merci_line_item']['created_month'] = array(
    'title' => t('Created month'),
    'help' => t('In the form of MM (01 - 12).'),
    'argument' => array(
      'field' => 'created',
      'handler' => 'views_handler_argument_node_created_month',
    ),
  );

  $data['merci_line_item']['created_day'] = array(
    'title' => t('Created day'),
    'help' => t('In the form of DD (01 - 31).'),
    'argument' => array(
      'field' => 'created',
      'handler' => 'views_handler_argument_node_created_day',
    ),
  );

  $data['merci_line_item']['created_week'] = array(
    'title' => t('Created week'),
    'help' => t('In the form of WW (01 - 53).'),
    'argument' => array(
      'field' => 'created',
      'handler' => 'views_handler_argument_node_created_week',
    ),
  );

  $data['merci_line_item']['changed'] = array(
    'title' => t('Updated date'),
    'help' => t('The date the line item was last updated.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  $data['merci_line_item']['changed_fulldate'] = array(
    'title' => t('Updated date'),
    'help' => t('In the form of CCYYMMDD.'),
    'argument' => array(
      'field' => 'changed',
      'handler' => 'views_handler_argument_node_created_fulldate',
    ),
  );

  $data['merci_line_item']['changed_year_month'] = array(
    'title' => t('Updated year + month'),
    'help' => t('In the form of YYYYMM.'),
    'argument' => array(
      'field' => 'changed',
      'handler' => 'views_handler_argument_node_created_year_month',
    ),
  );

  $data['merci_line_item']['changed_timestamp_year'] = array(
    'title' => t('Updated year'),
    'help' => t('In the form of YYYY.'),
    'argument' => array(
      'field' => 'changed',
      'handler' => 'views_handler_argument_node_created_year',
    ),
  );

  $data['merci_line_item']['changed_month'] = array(
    'title' => t('Updated month'),
    'help' => t('In the form of MM (01 - 12).'),
    'argument' => array(
      'field' => 'changed',
      'handler' => 'views_handler_argument_node_created_month',
    ),
  );

  $data['merci_line_item']['changed_day'] = array(
    'title' => t('Updated day'),
    'help' => t('In the form of DD (01 - 31).'),
    'argument' => array(
      'field' => 'changed',
      'handler' => 'views_handler_argument_node_created_day',
    ),
  );

  $data['merci_line_item']['changed_week'] = array(
    'title' => t('Updated week'),
    'help' => t('In the form of WW (01 - 53).'),
    'argument' => array(
      'field' => 'changed',
      'handler' => 'views_handler_argument_node_created_week',
    ),
  );

  // Define a handler for an area used to summarize a set of line items.
  $data['merci_line_item']['line_item_summary'] = array(
    'title' => t('Line item summary'),
    'help' => t('Summarize the line items in a View with an optional link to checkout.'),
    'area' => array(
      'handler' => 'merci_line_item_handler_area_line_item_summary',
    ),
  );

  return $data;
}

function merci_line_item_views_data_alter(&$data) {
  $data['node']['number_reserved'] = array(
    'title' => t('Number reserved'),
    'help' => t('The number of reservations during the date span.'),
    'real field' => 'nid',
    'field' => array(
      'handler' => 'merci_line_item_handler_field_number_reserved',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'merci_line_item_handler_filter_number_reserved',
    ),
  );
}
