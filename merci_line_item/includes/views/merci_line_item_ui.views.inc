<?php

/**
 * Export Drupal Commerce orders to Views.
 */

/**
 * Implements hook_views_data()
 */
function merci_line_item_ui_views_data_alter(&$data) {

  $data['merci_line_item']['operations'] = array(
    'field' => array(
      'title' => t('Operations links'),
      'help' => t('Display all the available operations links for the line item.'),
      'handler' => 'merci_line_item_ui_handler_field_operations',
    ),
  );

}

