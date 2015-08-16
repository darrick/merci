<?php

/**
 * @file
 * Provide views data and handlers for merci_inventory.module.
 *
 * @ingroup views_module_handlers
 */

/**
 * Implements hook_views_data().
 */

function merci_inventory_views_data_alter(&$data) {

  // ----------------------------------------------------------------------
  // merci_inventory_statistics table

  // define the group
  $data['merci_inventory_statistics']['table']['group']  = t('Merci Inventory Statistics');

  // joins
  $data['merci_inventory_statistics']['table']['join'] = array(
    //...to the commerce_product table
    'commerce_product' => array(
      'type' => 'LEFT',
      'left_field' => 'product_id',
      'field' => 'entity_id',
     ),
  );

  // last_merci_inventory_timestamp
  $data['merci_inventory_statistics']['last_inventory_timestamp'] = array(
    'title' => t('Last inventory time'),
    'help' => t('Date and time of when the last inventory was done.'),
    'field' => array(
      'handler' => 'views_handler_field_last_inventory_timestamp',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  // merci_inventory_count
  $data['merci_inventory_statistics']['inventory_count'] = array(
    'title' => t('Inventory count'),
    'help' => t('The number of inventories a resource has.'),
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
      'handler' => 'views_handler_argument',
    ),
  );

  // last_merci_inventory_timestamp
  $data['merci_inventory_statistics']['last_updated'] = array(
    'title' => t('Updated/merci_inventoryed date'),
    'help' => t('The most recent of last merci_inventory posted or node updated time.'),
    'field' => array(
      'handler' => 'views_handler_field_mis_last_updated',
      'click sortable' => TRUE,
      'no group by' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_mis_last_updated',
      'no group by' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_mis_last_updated',
    ),
  );

  $data['merci_inventory_statistics']['inventory_id'] = array(
    'title' => t('Last inventory ID'),
    'help' => t('Display the last inventory of a resource'),
    'relationship' => array(
      'title' => t('Last Inventory'),
      'help' => t('The last inventory of a resource.'),
      'group' => t('Merci Inventory'),
      'base' => 'merci_inventory',
      'base field' => 'inventory_id',
      'handler' => 'views_handler_relationship',
      'label' => t('Last Inventory'),
    ),
  );

  // last_merci_inventory_uid
  $data['merci_inventory_statistics']['last_inventory_uid'] = array(
    'title' => t('Last inventory uid'),
    'help' => t('The User ID of the author of the last inventory of a resource.'),
    'relationship' => array(
      'title' => t('Last inventory author'),
      'base' => 'users',
      'base field' => 'uid',
      'handler' => 'views_handler_relationship',
      'label' => t('Last inventory author'),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
    ),
  );

}


