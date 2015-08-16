<?php

/**
 * Export Drupal Commerce orders to Views.
 */

/**
 * Implements hook_views_data()
 */
function merci_reservation_views_data_alter(&$data) {

  $data['merci_reservation']['operations'] = array(
    'field' => array(
      'title' => t('Operations links'),
      'help' => t('Display all the available operations links for the order.'),
      'handler' => 'merci_reservation_handler_field_operations',
    ),
  );


  // Expose links to operate on the order.
  $data['merci_reservation']['view_checkout'] = array(
    'field' => array(
      'title' => t('Link'),
      'help' => t('Provide a simple link to the administrator view of the order.'),
      'handler' => 'merci_reservation_handler_field_order_link',
    ),
  );
 // title
  // This definition has more items in it than it needs to as an example.
  $data['merci_reservation']['title'] = array(
    'title' => t('Title'), // The item it appears as on the UI,
    'help' => t('The content title.'), // The help that appears on the UI,
     // Information for displaying a title as a field
    'field' => array(
      'handler' => 'merci_reservation_handler_field_title',
      'click sortable' => TRUE,
      'link_to_entity default' => TRUE,
     ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    // Information for accepting a title as a filter
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose links to operate on the order.
  $data['commerce_product']['current_checkout'] = array(
    'field' => array(
      'title' => t('Checkout'),
      'help' => t('Provide a simple link to a current checkout for the item if it exists.'),
      'handler' => 'merci_reservation_handler_field_current_checkout',
    ),
  );

}

