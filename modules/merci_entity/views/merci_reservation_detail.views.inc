<?php

/**
 * @file
 * Providing extra functionality for the MerciReservationDetail UI via views.
 */


/**
 * Implements hook_views_data()
 */
function merci_reservation_detail_views_data_alter(&$data) { 
  $data['merci_reservation_detail']['link_merci_reservation_detail'] = array(
    'field' => array(
      'title' => t('Link'),
      'help' => t('Provide a link to the merci_reservation_detail.'),
      'handler' => 'merci_reservation_detail_handler_link_field',
    ),
  );
  $data['merci_reservation_detail']['edit_merci_reservation_detail'] = array(
    'field' => array(
      'title' => t('Edit Link'),
      'help' => t('Provide a link to the edit form for the merci_reservation_detail.'),
      'handler' => 'merci_reservation_detail_handler_edit_link_field',
    ),
  );
  $data['merci_reservation_detail']['delete_merci_reservation_detail'] = array(
    'field' => array(
      'title' => t('Delete Link'),
      'help' => t('Provide a link to delete the merci_reservation_detail.'),
      'handler' => 'merci_reservation_detail_handler_delete_link_field',
    ),
  );
  // This content of this field are decided based on the menu structure that
  // follows merci_reservation_details/merci_reservation_detail/%merci_reservation_detail_id/op
  $data['merci_reservation_detail']['operations'] = array(
    'field' => array(
      'title' => t('Operations links'),
      'help' => t('Display all operations available for this merci_reservation_detail.'),
      'handler' => 'merci_reservation_detail_handler_merci_reservation_detail_operations_field',
    ),
  );
}


/**
 * Implements hook_views_default_views().
 */
function merci_reservation_detail_views_default_views() {
  $views = array();

  $view = new view;
  $view->name = 'merci_reservation_details';
  $view->description = 'A list of all merci_reservation_details';
  $view->tag = 'merci_reservation_details';
  $view->base_table = 'merci_reservation_detail';
  $view->human_name = 'MerciReservationDetails';
  $view->core = 7;
  $view->api_version = '3.0-alpha1';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'MerciReservationDetails';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'create any merci_reservation_detail type';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'name' => 'name',
    'merci_reservation_detail_id' => 'merci_reservation_detail_id',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'merci_reservation_detail_id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 0;
  $handler->display->display_options['style_options']['empty_table'] = 0;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['label'] = 'Empty ';
  $handler->display->display_options['empty']['area']['empty'] = FALSE;
  $handler->display->display_options['empty']['area']['content'] = 'No merci_reservation_details have been created yet';
  /* Field: MerciReservationDetail: MerciReservationDetail ID */
  $handler->display->display_options['fields']['merci_reservation_detail_id']['id'] = 'merci_reservation_detail_id';
  $handler->display->display_options['fields']['merci_reservation_detail_id']['table'] = 'merci_reservation_detail';
  $handler->display->display_options['fields']['merci_reservation_detail_id']['field'] = 'merci_reservation_detail_id';
  $handler->display->display_options['fields']['merci_reservation_detail_id']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['merci_reservation_detail_id']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['merci_reservation_detail_id']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['merci_reservation_detail_id']['alter']['external'] = 0;
  $handler->display->display_options['fields']['merci_reservation_detail_id']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['merci_reservation_detail_id']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['merci_reservation_detail_id']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['merci_reservation_detail_id']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['merci_reservation_detail_id']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['merci_reservation_detail_id']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['merci_reservation_detail_id']['alter']['html'] = 0;
  $handler->display->display_options['fields']['merci_reservation_detail_id']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['merci_reservation_detail_id']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['merci_reservation_detail_id']['hide_empty'] = 0;
  $handler->display->display_options['fields']['merci_reservation_detail_id']['empty_zero'] = 0;
  /* Field: MerciReservationDetail: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'merci_reservation_detail';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['name']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['name']['alter']['external'] = 0;
  $handler->display->display_options['fields']['name']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['name']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['name']['alter']['html'] = 0;
  $handler->display->display_options['fields']['name']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['name']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['name']['empty_zero'] = 0;
  /* Field: MerciReservationDetail: Link */
  $handler->display->display_options['fields']['link_merci_reservation_detail']['id'] = 'link_merci_reservation_detail';
  $handler->display->display_options['fields']['link_merci_reservation_detail']['table'] = 'merci_reservation_detail';
  $handler->display->display_options['fields']['link_merci_reservation_detail']['field'] = 'link_merci_reservation_detail';
  $handler->display->display_options['fields']['link_merci_reservation_detail']['label'] = 'View';
  $handler->display->display_options['fields']['link_merci_reservation_detail']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['link_merci_reservation_detail']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['link_merci_reservation_detail']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['link_merci_reservation_detail']['alter']['external'] = 0;
  $handler->display->display_options['fields']['link_merci_reservation_detail']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['link_merci_reservation_detail']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['link_merci_reservation_detail']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['link_merci_reservation_detail']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['link_merci_reservation_detail']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['link_merci_reservation_detail']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['link_merci_reservation_detail']['alter']['html'] = 0;
  $handler->display->display_options['fields']['link_merci_reservation_detail']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['link_merci_reservation_detail']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['link_merci_reservation_detail']['hide_empty'] = 0;
  $handler->display->display_options['fields']['link_merci_reservation_detail']['empty_zero'] = 0;
  /* Field: MerciReservationDetail: Operations links */
  $handler->display->display_options['fields']['operations']['id'] = 'operations';
  $handler->display->display_options['fields']['operations']['table'] = 'merci_reservation_detail';
  $handler->display->display_options['fields']['operations']['field'] = 'operations';
  $handler->display->display_options['fields']['operations']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['external'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['operations']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['operations']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['html'] = 0;
  $handler->display->display_options['fields']['operations']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['operations']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['operations']['hide_empty'] = 0;
  $handler->display->display_options['fields']['operations']['empty_zero'] = 0;
  
  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'merci_reservation_details_admin_page');
  $handler->display->display_options['path'] = 'admin/content/merci_reservation_details/list';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'List';
  $handler->display->display_options['menu']['weight'] = '-10';
  $handler->display->display_options['tab_options']['type'] = 'tab';
  $handler->display->display_options['tab_options']['title'] = 'MerciReservationDetails';
  $handler->display->display_options['tab_options']['description'] = 'Manage merci_reservation_details';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $handler->display->display_options['tab_options']['name'] = 'management';
  $translatables['merci_reservation_details'] = array(
    t('Master'),
    t('MerciReservationDetails'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('Empty '),
    t('No merci_reservation_details have been created yet'),
    t('MerciReservationDetail ID'),
    t('.'),
    t(','),
    t('Name'),
    t('View'),
    t('Operations links'),
    t('Page'),
  );
  $views[] = $view;
  return $views;

}
