<?php
// $Id$

/**
 * @file
 * merci ui default views 
 */


/**
 * Implementation of hook_views_default_views().
 */
function merci_ui_views_default_views() {

  $view = new view;
  $view->name = 'merci_user_reservations';
  $view->description = 'A multi-dimensional calendar view for Merci Reservations with back/next navigation.';
  $view->tag = 'Merci User Reservations';
  $view->base_table = 'node';
  $view->api_version = '3.0-alpha1';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Default */
  $handler = $view->new_display('default', 'Default', 'default');
  $handler->display->display_options['title'] = 'Merci Reservation Calendar';
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['items_per_page'] = 0;
  $handler->display->display_options['access']['type'] = 'php';
  $handler->display->display_options['access']['code'] = '<?php
global $user;

if (
  (arg(1) == $user->uid && user_access(\'create reservations\')) or
  (user_access(\'administer MERCI\') || user_access(\'manage MERCI\'))
) {
  return TRUE;
}
else {
  return FALSE;
}

?>';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['query']['options']['distinct'] = TRUE;
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['style_plugin'] = 'calendar_nav';
/* Field: Node: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['label'] = '';
$handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['title']['alter']['trim'] = 0;
$handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['title']['alter']['html'] = 0;
$handler->display->display_options['fields']['title']['hide_empty'] = 0;
$handler->display->display_options['fields']['title']['empty_zero'] = 0;
$handler->display->display_options['fields']['title']['link_to_node'] = 1;
/* Field: Fields: field_merci_date */
$handler->display->display_options['fields']['entity_id']['id'] = 'entity_id';
$handler->display->display_options['fields']['entity_id']['table'] = 'field_data_field_merci_date';
$handler->display->display_options['fields']['entity_id']['field'] = 'entity_id';
$handler->display->display_options['fields']['entity_id']['label'] = 'Date';
$handler->display->display_options['fields']['entity_id']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['entity_id']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['entity_id']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['entity_id']['alter']['trim'] = 0;
$handler->display->display_options['fields']['entity_id']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['entity_id']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['entity_id']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['entity_id']['alter']['html'] = 0;
$handler->display->display_options['fields']['entity_id']['element_label_colon'] = 1;
$handler->display->display_options['fields']['entity_id']['element_default_classes'] = 1;
$handler->display->display_options['fields']['entity_id']['hide_empty'] = 0;
$handler->display->display_options['fields']['entity_id']['empty_zero'] = 0;
$handler->display->display_options['fields']['entity_id']['click_sort_column'] = 'value';
$handler->display->display_options['fields']['entity_id']['settings'] = array(
  'format_type' => 'long',
  'fromto' => 'both',
  'multiple_number' => '',
  'multiple_from' => '',
  'multiple_to' => '',
  'show_repeat_rule' => 'show',
);
/* Field: MERCI: Reserved items */
$handler->display->display_options['fields']['item_nid2']['id'] = 'item_nid2';
$handler->display->display_options['fields']['item_nid2']['table'] = 'merci_reservation_detail';
$handler->display->display_options['fields']['item_nid2']['field'] = 'item_nid2';
$handler->display->display_options['fields']['item_nid2']['label'] = 'Items';
$handler->display->display_options['fields']['item_nid2']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['item_nid2']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['item_nid2']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['item_nid2']['alter']['trim'] = 0;
$handler->display->display_options['fields']['item_nid2']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['item_nid2']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['item_nid2']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['item_nid2']['alter']['html'] = 0;
$handler->display->display_options['fields']['item_nid2']['element_label_colon'] = 1;
$handler->display->display_options['fields']['item_nid2']['element_default_classes'] = 1;
$handler->display->display_options['fields']['item_nid2']['hide_empty'] = 0;
$handler->display->display_options['fields']['item_nid2']['empty_zero'] = 0;
/* Field: MERCI: Reservation status */
$handler->display->display_options['fields']['status']['id'] = 'status';
$handler->display->display_options['fields']['status']['table'] = 'merci_reservation';
$handler->display->display_options['fields']['status']['field'] = 'status';
$handler->display->display_options['fields']['status']['label'] = 'Status';
$handler->display->display_options['fields']['status']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['status']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['status']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['status']['alter']['trim'] = 0;
$handler->display->display_options['fields']['status']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['status']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['status']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['status']['alter']['html'] = 0;
$handler->display->display_options['fields']['status']['element_label_colon'] = 1;
$handler->display->display_options['fields']['status']['element_default_classes'] = 1;
$handler->display->display_options['fields']['status']['hide_empty'] = 0;
$handler->display->display_options['fields']['status']['empty_zero'] = 0;
/* Sort criterion: Fields: field_merci_date (field_merci_date) - value */
$handler->display->display_options['sorts']['field_merci_date_value_1']['id'] = 'field_merci_date_value_1';
$handler->display->display_options['sorts']['field_merci_date_value_1']['table'] = 'field_data_field_merci_date';
$handler->display->display_options['sorts']['field_merci_date_value_1']['field'] = 'field_merci_date_value';
/* Argument: User: Uid */
$handler->display->display_options['arguments']['uid']['id'] = 'uid';
$handler->display->display_options['arguments']['uid']['table'] = 'users';
$handler->display->display_options['arguments']['uid']['field'] = 'uid';
$handler->display->display_options['arguments']['uid']['default_action'] = 'not found';
$handler->display->display_options['arguments']['uid']['style_plugin'] = 'default_summary';
$handler->display->display_options['arguments']['uid']['title'] = 'Reservations for %1';
$handler->display->display_options['arguments']['uid']['default_argument_type'] = 'fixed';
$handler->display->display_options['arguments']['uid']['validate_type'] = 'user';
$handler->display->display_options['arguments']['uid']['validate_options']['restrict_roles'] = 0;
$handler->display->display_options['arguments']['uid']['break_phrase'] = 0;
$handler->display->display_options['arguments']['uid']['not'] = 0;
/* Argument: Date: Date (node) */
$handler->display->display_options['arguments']['date_argument']['id'] = 'date_argument';
$handler->display->display_options['arguments']['date_argument']['table'] = 'node';
$handler->display->display_options['arguments']['date_argument']['field'] = 'date_argument';
$handler->display->display_options['arguments']['date_argument']['default_action'] = 'default';
$handler->display->display_options['arguments']['date_argument']['style_plugin'] = 'default_summary';
$handler->display->display_options['arguments']['date_argument']['validate_fail'] = 'ignore';
$handler->display->display_options['arguments']['date_argument']['date_fields'] = array(
  'field_data_field_merci_date.field_merci_date_value' => 'field_data_field_merci_date.field_merci_date_value',
);
$handler->display->display_options['arguments']['date_argument']['granularity'] = 'week';
/* Filter: Node: Published */
$handler->display->display_options['filters']['status']['id'] = 'status';
$handler->display->display_options['filters']['status']['table'] = 'node';
$handler->display->display_options['filters']['status']['field'] = 'status';
$handler->display->display_options['filters']['status']['value'] = '1';
$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
/* Filter: Node: Type */
$handler->display->display_options['filters']['type']['id'] = 'type';
$handler->display->display_options['filters']['type']['table'] = 'node';
$handler->display->display_options['filters']['type']['field'] = 'type';
$handler->display->display_options['filters']['type']['value'] = array(
  'merci_reservation' => 'merci_reservation',
);
$handler->display->display_options['filters']['type']['expose']['operator'] = 'type_op';
$handler->display->display_options['filters']['type']['expose']['label'] = 'Node: Type';
$handler->display->display_options['filters']['type']['expose']['identifier'] = 'type';
$handler->display->display_options['filters']['type']['expose']['reduce'] = 1;
/* Filter: MERCI: Reservation status */
$handler->display->display_options['filters']['status_1']['id'] = 'status_1';
$handler->display->display_options['filters']['status_1']['table'] = 'merci_reservation';
$handler->display->display_options['filters']['status_1']['field'] = 'status';
$handler->display->display_options['filters']['status_1']['exposed'] = TRUE;
$handler->display->display_options['filters']['status_1']['expose']['operator'] = 'status_1_op';
$handler->display->display_options['filters']['status_1']['expose']['label'] = 'Status';
$handler->display->display_options['filters']['status_1']['expose']['identifier'] = 'status_1';
$handler->display->display_options['filters']['status_1']['expose']['reduce'] = 0;
/* Filter: User: Current */
$handler->display->display_options['filters']['uid_current']['id'] = 'uid_current';
$handler->display->display_options['filters']['uid_current']['table'] = 'users';
$handler->display->display_options['filters']['uid_current']['field'] = 'uid_current';
$handler->display->display_options['filters']['uid_current']['value'] = '1';

/* Display: Calendar page */
$handler = $view->new_display('calendar', 'Calendar page', 'calendar_1');
$handler->display->display_options['items_per_page'] = 0;
$handler->display->display_options['pager']['type'] = 'some';
$handler->display->display_options['style_plugin'] = 'calendar_nav';
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['path'] = 'user/%/reservations';
$handler->display->display_options['menu']['type'] = 'tab';
$handler->display->display_options['menu']['title'] = 'Reservations';
$handler->display->display_options['menu']['description'] = 'Calendar to browse reservations';
$handler->display->display_options['menu']['weight'] = '0';
$handler->display->display_options['calendar_colors'] = array(
  'page' => '#ffff66',
  'story' => '#668cff',
);
$handler->display->display_options['calendar_popup'] = '1';
$handler->display->display_options['calendar_date_link'] = 'merci_reservation';

/* Display: Year view */
$handler = $view->new_display('calendar_period', 'Year view', 'calendar_period_1');
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'title' => 'title',
  'name' => 'name',
  'field_merci_date_value' => 'field_merci_date_value',
  'item_nid2' => 'item_nid2',
  'status' => 'status',
);
$handler->display->display_options['style_options']['default'] = '-1';
$handler->display->display_options['style_options']['info'] = array(
  'title' => array(
    'sortable' => 0,
    'separator' => '',
  ),
  'name' => array(
    'sortable' => 0,
    'separator' => '',
  ),
  'field_merci_date_value' => array(
    'sortable' => 0,
    'separator' => '',
  ),
  'item_nid2' => array(
    'separator' => '',
  ),
  'status' => array(
    'sortable' => 0,
    'separator' => '',
  ),
);
$handler->display->display_options['style_options']['override'] = 1;
$handler->display->display_options['style_options']['sticky'] = 0;
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['defaults']['row_plugin'] = FALSE;
$handler->display->display_options['defaults']['row_options'] = FALSE;
$handler->display->display_options['attachment_position'] = 'after';
$handler->display->display_options['inherit_exposed_filters'] = TRUE;
$handler->display->display_options['render_pager'] = TRUE;
$handler->display->display_options['displays'] = array(
  'calendar_1' => 'calendar_1',
  'default' => 0,
  'calendar_block_1' => 0,
);

/* Display: Month view */
$handler = $view->new_display('calendar_period', 'Month view', 'calendar_period_2');
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'calendar_style';
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['defaults']['row_plugin'] = FALSE;
$handler->display->display_options['defaults']['row_options'] = FALSE;
$handler->display->display_options['attachment_position'] = 'after';
$handler->display->display_options['inherit_exposed_filters'] = TRUE;
$handler->display->display_options['render_pager'] = TRUE;
$handler->display->display_options['displays'] = array(
  'calendar_1' => 'calendar_1',
  'default' => 0,
  'calendar_block_1' => 0,
);

/* Display: Day view */
$handler = $view->new_display('calendar_period', 'Day view', 'calendar_period_3');
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'title' => 'title',
  'name' => 'name',
  'field_merci_date_value' => 'field_merci_date_value',
  'item_nid2' => 'item_nid2',
  'status' => 'status',
  'title_1' => 'title_1',
);
$handler->display->display_options['style_options']['default'] = '-1';
$handler->display->display_options['style_options']['info'] = array(
  'title' => array(
    'sortable' => 0,
    'separator' => '',
  ),
  'name' => array(
    'sortable' => 0,
    'separator' => '',
  ),
  'field_merci_date_value' => array(
    'sortable' => 0,
    'separator' => '',
  ),
  'item_nid2' => array(
    'separator' => '',
  ),
  'status' => array(
    'sortable' => 0,
    'separator' => '',
  ),
  'title_1' => array(
    'sortable' => 0,
    'separator' => '',
  ),
);
$handler->display->display_options['style_options']['override'] = 1;
$handler->display->display_options['style_options']['sticky'] = 0;
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['defaults']['row_plugin'] = FALSE;
$handler->display->display_options['defaults']['row_options'] = FALSE;
$handler->display->display_options['attachment_position'] = 'after';
$handler->display->display_options['inherit_exposed_filters'] = TRUE;
$handler->display->display_options['render_pager'] = TRUE;
$handler->display->display_options['displays'] = array(
  'calendar_1' => 'calendar_1',
  'default' => 0,
  'block_1' => 0,
);

/* Display: Week view */
$handler = $view->new_display('calendar_period', 'Week view', 'calendar_period_4');
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'title' => 'title',
  'field_merci_date_value' => 'field_merci_date_value',
  'status' => 'status',
  'item_nid2' => 'item_nid2',
);
$handler->display->display_options['style_options']['default'] = '-1';
$handler->display->display_options['style_options']['info'] = array(
  'title' => array(
    'sortable' => 0,
    'separator' => '',
  ),
  'field_merci_date_value' => array(
    'sortable' => 0,
    'separator' => '',
  ),
  'status' => array(
    'sortable' => 0,
    'separator' => '',
  ),
  'item_nid2' => array(
    'separator' => '',
  ),
);
$handler->display->display_options['style_options']['override'] = 1;
$handler->display->display_options['style_options']['sticky'] = 0;
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['defaults']['row_plugin'] = FALSE;
$handler->display->display_options['defaults']['row_options'] = FALSE;
$handler->display->display_options['attachment_position'] = 'after';
$handler->display->display_options['inherit_exposed_filters'] = TRUE;
$handler->display->display_options['render_pager'] = TRUE;
$handler->display->display_options['displays'] = array(
  'calendar_1' => 'calendar_1',
  'default' => 0,
  'block_1' => 0,
  'block_2' => 0,
  'block_3' => 0,
);

/* Display: User Reservations */
$handler = $view->new_display('block', 'User Reservations', 'block_2');
$handler->display->display_options['defaults']['title'] = FALSE;
$handler->display->display_options['title'] = 'My Reservations';
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'default';
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['defaults']['row_plugin'] = FALSE;
$handler->display->display_options['row_plugin'] = 'fields';
$handler->display->display_options['defaults']['row_options'] = FALSE;
$handler->display->display_options['defaults']['filters'] = FALSE;
/* Filter: Node: Published */
$handler->display->display_options['filters']['status']['id'] = 'status';
$handler->display->display_options['filters']['status']['table'] = 'node';
$handler->display->display_options['filters']['status']['field'] = 'status';
$handler->display->display_options['filters']['status']['value'] = '1';
$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
/* Filter: Node: Type */
$handler->display->display_options['filters']['type']['id'] = 'type';
$handler->display->display_options['filters']['type']['table'] = 'node';
$handler->display->display_options['filters']['type']['field'] = 'type';
$handler->display->display_options['filters']['type']['value'] = array(
  'merci_reservation' => 'merci_reservation',
);
$handler->display->display_options['filters']['type']['expose']['operator'] = 'type_op';
$handler->display->display_options['filters']['type']['expose']['label'] = 'Node: Type';
$handler->display->display_options['filters']['type']['expose']['identifier'] = 'type';
$handler->display->display_options['filters']['type']['expose']['reduce'] = 1;
/* Filter: Date: Date (node) */
$handler->display->display_options['filters']['date_filter']['id'] = 'date_filter';
$handler->display->display_options['filters']['date_filter']['table'] = 'node';
$handler->display->display_options['filters']['date_filter']['field'] = 'date_filter';
$handler->display->display_options['filters']['date_filter']['operator'] = '>=';
$handler->display->display_options['filters']['date_filter']['expose']['operator'] = FALSE;
$handler->display->display_options['filters']['date_filter']['date_fields'] = array(
  'field_data_field_merci_date.field_merci_date_value' => 'field_data_field_merci_date.field_merci_date_value',
);
$handler->display->display_options['filters']['date_filter']['granularity'] = 'second';
$handler->display->display_options['filters']['date_filter']['default_date'] = 'now';
/* Filter: User: Current */
$handler->display->display_options['filters']['uid_current']['id'] = 'uid_current';
$handler->display->display_options['filters']['uid_current']['table'] = 'users';
$handler->display->display_options['filters']['uid_current']['field'] = 'uid_current';
$handler->display->display_options['filters']['uid_current']['value'] = '1';
$handler->display->display_options['filters']['uid_current']['expose']['operator'] = FALSE;
$handler->display->display_options['block_description'] = 'MERCI: User Upcoming Reservations';
$translatables['merci_user_reservations'] = array(
  t('Default'),
  t('Merci Reservation Calendar'),
  t('more'),
  t('Apply'),
  t('Reset'),
  t('Sort By'),
  t('Asc'),
  t('Desc'),
  t('Items per page'),
  t('- All -'),
  t('Offset'),
  t('Date'),
  t('Items'),
  t('Status'),
  t('All'),
  t('Reservations for %1'),
  t('Node: Type'),
  t('Calendar page'),
  t('Year view'),
  t('Month view'),
  t('Day view'),
  t('Week view'),
  t('User Reservations'),
  t('My Reservations'),
  t('MERCI: User Upcoming Reservations'),
);


$views[$view->name] = $view;

$view = new view;
$view->name = 'merci_reservation_calendar';
$view->description = 'A multi-dimensional calendar view for Merci Reservations with back/next navigation.';
$view->tag = 'Merci Reservation Calendar';
$view->base_table = 'node';
$view->api_version = '3.0-alpha1';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Default */
$handler = $view->new_display('default', 'Default', 'default');
$handler->display->display_options['title'] = 'Merci Reservation Calendar';
$handler->display->display_options['use_ajax'] = TRUE;
$handler->display->display_options['items_per_page'] = 0;
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['access']['perm'] = 'manage MERCI';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['query']['options']['distinct'] = TRUE;
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['style_plugin'] = 'calendar_nav';
/* Field: Node: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['label'] = '';
$handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['title']['alter']['trim'] = 0;
$handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['title']['alter']['html'] = 0;
$handler->display->display_options['fields']['title']['hide_empty'] = 0;
$handler->display->display_options['fields']['title']['empty_zero'] = 0;
$handler->display->display_options['fields']['title']['link_to_node'] = 1;
/* Field: User: Name */
$handler->display->display_options['fields']['name']['id'] = 'name';
$handler->display->display_options['fields']['name']['table'] = 'users';
$handler->display->display_options['fields']['name']['field'] = 'name';
$handler->display->display_options['fields']['name']['label'] = '';
$handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['name']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['name']['alter']['trim'] = 0;
$handler->display->display_options['fields']['name']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['name']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['name']['alter']['html'] = 0;
$handler->display->display_options['fields']['name']['element_label_colon'] = 1;
$handler->display->display_options['fields']['name']['element_default_classes'] = 1;
$handler->display->display_options['fields']['name']['hide_empty'] = 0;
$handler->display->display_options['fields']['name']['empty_zero'] = 0;
$handler->display->display_options['fields']['name']['link_to_user'] = 1;
$handler->display->display_options['fields']['name']['overwrite_anonymous'] = 0;
/* Field: Fields: field_merci_date */
$handler->display->display_options['fields']['entity_id']['id'] = 'entity_id';
$handler->display->display_options['fields']['entity_id']['table'] = 'field_data_field_merci_date';
$handler->display->display_options['fields']['entity_id']['field'] = 'entity_id';
$handler->display->display_options['fields']['entity_id']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['entity_id']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['entity_id']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['entity_id']['alter']['trim'] = 0;
$handler->display->display_options['fields']['entity_id']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['entity_id']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['entity_id']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['entity_id']['alter']['html'] = 0;
$handler->display->display_options['fields']['entity_id']['element_label_colon'] = 1;
$handler->display->display_options['fields']['entity_id']['element_default_classes'] = 1;
$handler->display->display_options['fields']['entity_id']['hide_empty'] = 0;
$handler->display->display_options['fields']['entity_id']['empty_zero'] = 0;
$handler->display->display_options['fields']['entity_id']['click_sort_column'] = 'value';
$handler->display->display_options['fields']['entity_id']['settings'] = array(
  'format_type' => 'long',
  'fromto' => 'both',
  'multiple_number' => '',
  'multiple_from' => '',
  'multiple_to' => '',
  'show_repeat_rule' => 'show',
);
/* Field: MERCI: Reserved items */
$handler->display->display_options['fields']['item_nid2']['id'] = 'item_nid2';
$handler->display->display_options['fields']['item_nid2']['table'] = 'merci_reservation_detail';
$handler->display->display_options['fields']['item_nid2']['field'] = 'item_nid2';
$handler->display->display_options['fields']['item_nid2']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['item_nid2']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['item_nid2']['alter']['trim'] = 0;
$handler->display->display_options['fields']['item_nid2']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['item_nid2']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['item_nid2']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['item_nid2']['alter']['html'] = 0;
$handler->display->display_options['fields']['item_nid2']['hide_empty'] = 0;
$handler->display->display_options['fields']['item_nid2']['empty_zero'] = 0;
/* Field: MERCI: Reservation status */
$handler->display->display_options['fields']['status']['id'] = 'status';
$handler->display->display_options['fields']['status']['table'] = 'merci_reservation';
$handler->display->display_options['fields']['status']['field'] = 'status';
$handler->display->display_options['fields']['status']['label'] = '';
$handler->display->display_options['fields']['status']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['status']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['status']['alter']['trim'] = 0;
$handler->display->display_options['fields']['status']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['status']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['status']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['status']['alter']['html'] = 0;
$handler->display->display_options['fields']['status']['hide_empty'] = 0;
$handler->display->display_options['fields']['status']['empty_zero'] = 0;
/* Sort criterion: Fields: field_merci_date (field_merci_date) - value */
$handler->display->display_options['sorts']['field_merci_date_value_1']['id'] = 'field_merci_date_value_1';
$handler->display->display_options['sorts']['field_merci_date_value_1']['table'] = 'field_data_field_merci_date';
$handler->display->display_options['sorts']['field_merci_date_value_1']['field'] = 'field_merci_date_value';
/* Argument: Date: Date (node) */
$handler->display->display_options['arguments']['date_argument']['id'] = 'date_argument';
$handler->display->display_options['arguments']['date_argument']['table'] = 'node';
$handler->display->display_options['arguments']['date_argument']['field'] = 'date_argument';
$handler->display->display_options['arguments']['date_argument']['default_action'] = 'default';
$handler->display->display_options['arguments']['date_argument']['style_plugin'] = 'default_summary';
$handler->display->display_options['arguments']['date_argument']['date_fields'] = array(
  'field_data_field_merci_date.field_merci_date_value' => 'field_data_field_merci_date.field_merci_date_value',
);
$handler->display->display_options['arguments']['date_argument']['granularity'] = 'week';
/* Filter: Node: Published */
$handler->display->display_options['filters']['status']['id'] = 'status';
$handler->display->display_options['filters']['status']['table'] = 'node';
$handler->display->display_options['filters']['status']['field'] = 'status';
$handler->display->display_options['filters']['status']['value'] = '1';
$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
/* Filter: Node: Type */
$handler->display->display_options['filters']['type']['id'] = 'type';
$handler->display->display_options['filters']['type']['table'] = 'node';
$handler->display->display_options['filters']['type']['field'] = 'type';
$handler->display->display_options['filters']['type']['value'] = array(
  'merci_reservation' => 'merci_reservation',
);
$handler->display->display_options['filters']['type']['expose']['operator'] = 'type_op';
$handler->display->display_options['filters']['type']['expose']['label'] = 'Node: Type';
$handler->display->display_options['filters']['type']['expose']['identifier'] = 'type';
$handler->display->display_options['filters']['type']['expose']['reduce'] = 1;
/* Filter: MERCI: Reservation status */
$handler->display->display_options['filters']['status_1']['id'] = 'status_1';
$handler->display->display_options['filters']['status_1']['table'] = 'merci_reservation';
$handler->display->display_options['filters']['status_1']['field'] = 'status';
$handler->display->display_options['filters']['status_1']['exposed'] = TRUE;
$handler->display->display_options['filters']['status_1']['expose']['operator'] = 'status_1_op';
$handler->display->display_options['filters']['status_1']['expose']['label'] = 'Status';
$handler->display->display_options['filters']['status_1']['expose']['identifier'] = 'status_1';
$handler->display->display_options['filters']['status_1']['expose']['reduce'] = 0;

/* Display: Calendar page */
$handler = $view->new_display('calendar', 'Calendar page', 'calendar_1');
$handler->display->display_options['items_per_page'] = 0;
$handler->display->display_options['pager']['type'] = 'some';
$handler->display->display_options['style_plugin'] = 'calendar_nav';
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['path'] = 'admin/merci/manage/reservations';
$handler->display->display_options['menu']['type'] = 'tab';
$handler->display->display_options['menu']['title'] = 'Manage Reservations';
$handler->display->display_options['menu']['description'] = 'Calendar to browse reservations';
$handler->display->display_options['menu']['weight'] = '0';
$handler->display->display_options['calendar_colors'] = array(
  'page' => '#ffff66',
  'story' => '#668cff',
);
$handler->display->display_options['calendar_popup'] = '1';
$handler->display->display_options['calendar_date_link'] = 'merci_reservation';

/* Display: Year view */
$handler = $view->new_display('calendar_period', 'Year view', 'calendar_period_1');
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'title' => 'title',
  'name' => 'name',
  'field_merci_date_value' => 'field_merci_date_value',
  'item_nid2' => 'item_nid2',
  'status' => 'status',
);
$handler->display->display_options['style_options']['default'] = '-1';
$handler->display->display_options['style_options']['info'] = array(
  'title' => array(
    'sortable' => 0,
    'separator' => '',
  ),
  'name' => array(
    'sortable' => 0,
    'separator' => '',
  ),
  'field_merci_date_value' => array(
    'sortable' => 0,
    'separator' => '',
  ),
  'item_nid2' => array(
    'separator' => '',
  ),
  'status' => array(
    'sortable' => 0,
    'separator' => '',
  ),
);
$handler->display->display_options['style_options']['override'] = 1;
$handler->display->display_options['style_options']['sticky'] = 0;
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['defaults']['row_plugin'] = FALSE;
$handler->display->display_options['defaults']['row_options'] = FALSE;
$handler->display->display_options['attachment_position'] = 'after';
$handler->display->display_options['inherit_exposed_filters'] = TRUE;
$handler->display->display_options['render_pager'] = TRUE;
$handler->display->display_options['displays'] = array(
  'calendar_1' => 'calendar_1',
  'default' => 0,
  'calendar_block_1' => 0,
);

/* Display: Month view */
$handler = $view->new_display('calendar_period', 'Month view', 'calendar_period_2');
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'calendar_style';
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['defaults']['row_plugin'] = FALSE;
$handler->display->display_options['defaults']['row_options'] = FALSE;
$handler->display->display_options['attachment_position'] = 'after';
$handler->display->display_options['inherit_exposed_filters'] = TRUE;
$handler->display->display_options['render_pager'] = TRUE;
$handler->display->display_options['displays'] = array(
  'calendar_1' => 'calendar_1',
  'default' => 0,
  'calendar_block_1' => 0,
);

/* Display: Day view */
$handler = $view->new_display('calendar_period', 'Day view', 'calendar_period_3');
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'title' => 'title',
  'name' => 'name',
  'field_merci_date_value' => 'field_merci_date_value',
  'item_nid2' => 'item_nid2',
  'status' => 'status',
  'title_1' => 'title_1',
);
$handler->display->display_options['style_options']['default'] = '-1';
$handler->display->display_options['style_options']['info'] = array(
  'title' => array(
    'sortable' => 0,
    'separator' => '',
  ),
  'name' => array(
    'sortable' => 0,
    'separator' => '',
  ),
  'field_merci_date_value' => array(
    'sortable' => 0,
    'separator' => '',
  ),
  'item_nid2' => array(
    'separator' => '',
  ),
  'status' => array(
    'sortable' => 0,
    'separator' => '',
  ),
  'title_1' => array(
    'sortable' => 0,
    'separator' => '',
  ),
);
$handler->display->display_options['style_options']['override'] = 1;
$handler->display->display_options['style_options']['sticky'] = 0;
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['defaults']['row_plugin'] = FALSE;
$handler->display->display_options['defaults']['row_options'] = FALSE;
$handler->display->display_options['attachment_position'] = 'after';
$handler->display->display_options['inherit_exposed_filters'] = TRUE;
$handler->display->display_options['render_pager'] = TRUE;
$handler->display->display_options['displays'] = array(
  'calendar_1' => 'calendar_1',
  'default' => 0,
  'block_1' => 0,
);

/* Display: Week view */
$handler = $view->new_display('calendar_period', 'Week view', 'calendar_period_4');
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'title' => 'title',
  'field_merci_date_value' => 'field_merci_date_value',
  'status' => 'status',
  'item_nid2' => 'item_nid2',
);
$handler->display->display_options['style_options']['default'] = '-1';
$handler->display->display_options['style_options']['info'] = array(
  'title' => array(
    'sortable' => 0,
    'separator' => '',
  ),
  'field_merci_date_value' => array(
    'sortable' => 0,
    'separator' => '',
  ),
  'status' => array(
    'sortable' => 0,
    'separator' => '',
  ),
  'item_nid2' => array(
    'separator' => '',
  ),
);
$handler->display->display_options['style_options']['override'] = 1;
$handler->display->display_options['style_options']['sticky'] = 0;
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['defaults']['row_plugin'] = FALSE;
$handler->display->display_options['defaults']['row_options'] = FALSE;
$handler->display->display_options['attachment_position'] = 'after';
$handler->display->display_options['inherit_exposed_filters'] = TRUE;
$handler->display->display_options['render_pager'] = TRUE;
$handler->display->display_options['displays'] = array(
  'calendar_1' => 'calendar_1',
  'default' => 0,
  'block_1' => 0,
  'block_2' => 0,
  'block_3' => 0,
);

/* Display: Upcoming Reservations */
$handler = $view->new_display('block', 'Upcoming Reservations', 'block_1');
$handler->display->display_options['defaults']['title'] = FALSE;
$handler->display->display_options['title'] = 'Upcoming Reservations';
$handler->display->display_options['defaults']['items_per_page'] = FALSE;
$handler->display->display_options['defaults']['use_more'] = FALSE;
$handler->display->display_options['use_more'] = TRUE;
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'default';
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['defaults']['row_plugin'] = FALSE;
$handler->display->display_options['row_plugin'] = 'fields';
$handler->display->display_options['defaults']['row_options'] = FALSE;
$handler->display->display_options['defaults']['filters'] = FALSE;
/* Filter: Node: Published */
$handler->display->display_options['filters']['status']['id'] = 'status';
$handler->display->display_options['filters']['status']['table'] = 'node';
$handler->display->display_options['filters']['status']['field'] = 'status';
$handler->display->display_options['filters']['status']['value'] = '1';
$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
/* Filter: Node: Type */
$handler->display->display_options['filters']['type']['id'] = 'type';
$handler->display->display_options['filters']['type']['table'] = 'node';
$handler->display->display_options['filters']['type']['field'] = 'type';
$handler->display->display_options['filters']['type']['value'] = array(
  'merci_reservation' => 'merci_reservation',
);
$handler->display->display_options['filters']['type']['expose']['operator'] = 'type_op';
$handler->display->display_options['filters']['type']['expose']['label'] = 'Node: Type';
$handler->display->display_options['filters']['type']['expose']['identifier'] = 'type';
$handler->display->display_options['filters']['type']['expose']['reduce'] = 1;
/* Filter: Date: Date (node) */
$handler->display->display_options['filters']['date_filter']['id'] = 'date_filter';
$handler->display->display_options['filters']['date_filter']['table'] = 'node';
$handler->display->display_options['filters']['date_filter']['field'] = 'date_filter';
$handler->display->display_options['filters']['date_filter']['operator'] = '>=';
$handler->display->display_options['filters']['date_filter']['expose']['operator'] = FALSE;
$handler->display->display_options['filters']['date_filter']['date_fields'] = array(
  'field_data_field_merci_date.field_merci_date_value' => 'field_data_field_merci_date.field_merci_date_value',
);
$handler->display->display_options['filters']['date_filter']['granularity'] = 'second';
$handler->display->display_options['filters']['date_filter']['default_date'] = 'now';
$handler->display->display_options['block_description'] = 'MERCI: Upcoming Reservations';

/* Display: User Reservations */
$handler = $view->new_display('block', 'User Reservations', 'block_2');
$handler->display->display_options['defaults']['title'] = FALSE;
$handler->display->display_options['title'] = 'My Reservations';
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'default';
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['defaults']['row_plugin'] = FALSE;
$handler->display->display_options['row_plugin'] = 'fields';
$handler->display->display_options['defaults']['row_options'] = FALSE;
$handler->display->display_options['defaults']['filters'] = FALSE;
/* Filter: Node: Published */
$handler->display->display_options['filters']['status']['id'] = 'status';
$handler->display->display_options['filters']['status']['table'] = 'node';
$handler->display->display_options['filters']['status']['field'] = 'status';
$handler->display->display_options['filters']['status']['value'] = '1';
$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
/* Filter: Node: Type */
$handler->display->display_options['filters']['type']['id'] = 'type';
$handler->display->display_options['filters']['type']['table'] = 'node';
$handler->display->display_options['filters']['type']['field'] = 'type';
$handler->display->display_options['filters']['type']['value'] = array(
  'merci_reservation' => 'merci_reservation',
);
$handler->display->display_options['filters']['type']['expose']['operator'] = 'type_op';
$handler->display->display_options['filters']['type']['expose']['label'] = 'Node: Type';
$handler->display->display_options['filters']['type']['expose']['identifier'] = 'type';
$handler->display->display_options['filters']['type']['expose']['reduce'] = 1;
/* Filter: Date: Date (node) */
$handler->display->display_options['filters']['date_filter']['id'] = 'date_filter';
$handler->display->display_options['filters']['date_filter']['table'] = 'node';
$handler->display->display_options['filters']['date_filter']['field'] = 'date_filter';
$handler->display->display_options['filters']['date_filter']['operator'] = '>=';
$handler->display->display_options['filters']['date_filter']['expose']['operator'] = FALSE;
$handler->display->display_options['filters']['date_filter']['date_fields'] = array(
  'field_data_field_merci_date.field_merci_date_value' => 'field_data_field_merci_date.field_merci_date_value',
);
$handler->display->display_options['filters']['date_filter']['granularity'] = 'second';
$handler->display->display_options['filters']['date_filter']['default_date'] = 'now';
/* Filter: User: Current */
$handler->display->display_options['filters']['uid_current']['id'] = 'uid_current';
$handler->display->display_options['filters']['uid_current']['table'] = 'users';
$handler->display->display_options['filters']['uid_current']['field'] = 'uid_current';
$handler->display->display_options['filters']['uid_current']['value'] = '1';
$handler->display->display_options['filters']['uid_current']['expose']['operator'] = FALSE;
$handler->display->display_options['block_description'] = 'MERCI: User Upcoming Reservations';

/* Display: Past Due Reservations */
$handler = $view->new_display('block', 'Past Due Reservations', 'block_3');
$handler->display->display_options['defaults']['title'] = FALSE;
$handler->display->display_options['title'] = 'Past Due Reservations';
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'default';
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['defaults']['row_plugin'] = FALSE;
$handler->display->display_options['row_plugin'] = 'fields';
$handler->display->display_options['defaults']['row_options'] = FALSE;
$handler->display->display_options['defaults']['arguments'] = FALSE;
$handler->display->display_options['defaults']['filters'] = FALSE;
/* Filter: Node: Published */
$handler->display->display_options['filters']['status']['id'] = 'status';
$handler->display->display_options['filters']['status']['table'] = 'node';
$handler->display->display_options['filters']['status']['field'] = 'status';
$handler->display->display_options['filters']['status']['value'] = '1';
$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
/* Filter: Node: Type */
$handler->display->display_options['filters']['type']['id'] = 'type';
$handler->display->display_options['filters']['type']['table'] = 'node';
$handler->display->display_options['filters']['type']['field'] = 'type';
$handler->display->display_options['filters']['type']['value'] = array(
  'merci_reservation' => 'merci_reservation',
);
$handler->display->display_options['filters']['type']['expose']['operator'] = 'type_op';
$handler->display->display_options['filters']['type']['expose']['label'] = 'Node: Type';
$handler->display->display_options['filters']['type']['expose']['identifier'] = 'type';
$handler->display->display_options['filters']['type']['expose']['reduce'] = 1;
/* Filter: MERCI: Reservation status */
$handler->display->display_options['filters']['status_1']['id'] = 'status_1';
$handler->display->display_options['filters']['status_1']['table'] = 'merci_reservation';
$handler->display->display_options['filters']['status_1']['field'] = 'status';
$handler->display->display_options['filters']['status_1']['value'] = array(
  3 => '3',
);
$handler->display->display_options['filters']['status_1']['expose']['operator'] = 'status_1_op';
$handler->display->display_options['filters']['status_1']['expose']['label'] = 'Status';
$handler->display->display_options['filters']['status_1']['expose']['identifier'] = 'status_1';
$handler->display->display_options['filters']['status_1']['expose']['reduce'] = 0;
/* Filter: Date: Date (node) */
$handler->display->display_options['filters']['date_filter']['id'] = 'date_filter';
$handler->display->display_options['filters']['date_filter']['table'] = 'node';
$handler->display->display_options['filters']['date_filter']['field'] = 'date_filter';
$handler->display->display_options['filters']['date_filter']['operator'] = '<=';
$handler->display->display_options['filters']['date_filter']['expose']['operator'] = FALSE;
$handler->display->display_options['filters']['date_filter']['date_fields'] = array(
  'field_data_field_merci_date.field_merci_date_value' => 'field_data_field_merci_date.field_merci_date_value',
);
$handler->display->display_options['filters']['date_filter']['granularity'] = 'second';
$handler->display->display_options['filters']['date_filter']['default_date'] = 'now';
$handler->display->display_options['block_description'] = 'MERCI: Past Due Reservations';
$translatables['merci_reservation_calendar'] = array(
  t('Default'),
  t('Merci Reservation Calendar'),
  t('more'),
  t('Apply'),
  t('Reset'),
  t('Sort By'),
  t('Asc'),
  t('Desc'),
  t('Items per page'),
  t('- All -'),
  t('Offset'),
  t('field_merci_date'),
  t('Reserved items'),
  t('All'),
  t('Node: Type'),
  t('Status'),
  t('Calendar page'),
  t('Year view'),
  t('Month view'),
  t('Day view'),
  t('Week view'),
  t('Upcoming Reservations'),
  t('MERCI: Upcoming Reservations'),
  t('User Reservations'),
  t('My Reservations'),
  t('MERCI: User Upcoming Reservations'),
  t('Past Due Reservations'),
  t('MERCI: Past Due Reservations'),
);

$views[$view->name] = $view;

$view = new view;
$view->name = 'merci_node_reservations';
$view->description = 'A multi-dimensional calendar view for Merci Reservations with back/next navigation.';
$view->tag = 'Merci Reservation Calendar';
$view->base_table = 'node';
$view->api_version = '3.0-alpha1';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Default */
$handler = $view->new_display('default', 'Default', 'default');
$handler->display->display_options['title'] = 'Merci Reservation Calendar';
$handler->display->display_options['use_ajax'] = TRUE;
$handler->display->display_options['items_per_page'] = 0;
$handler->display->display_options['access']['type'] = 'php';
$handler->display->display_options['access']['code'] = '<?php
$node = node_load(arg(1));
$merci_settings = merci_load_content_type_settings($node->type);

if ($merci_settings->type_setting == \'bucket\' || $merci_settings->type_setting == \'resource\') {
  if (user_access(\'manage MERCI\') || user_access(\'administer MERCI\')) {

    return TRUE;
}
}
else {

  return FALSE;
}

?>';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['query']['options']['distinct'] = TRUE;
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['style_plugin'] = 'calendar_nav';
/* Relationship: MERCI: Reserved item nid */
$handler->display->display_options['relationships']['item_nid']['id'] = 'item_nid';
$handler->display->display_options['relationships']['item_nid']['table'] = 'merci_reservation_detail';
$handler->display->display_options['relationships']['item_nid']['field'] = 'item_nid';
$handler->display->display_options['relationships']['item_nid']['required'] = 0;
/* Field: Node: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['label'] = '';
$handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['title']['alter']['trim'] = 0;
$handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['title']['alter']['html'] = 0;
$handler->display->display_options['fields']['title']['hide_empty'] = 0;
$handler->display->display_options['fields']['title']['empty_zero'] = 0;
$handler->display->display_options['fields']['title']['link_to_node'] = 1;
/* Field: User: Name */
$handler->display->display_options['fields']['name']['id'] = 'name';
$handler->display->display_options['fields']['name']['table'] = 'users';
$handler->display->display_options['fields']['name']['field'] = 'name';
$handler->display->display_options['fields']['name']['label'] = '';
$handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['name']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['name']['alter']['trim'] = 0;
$handler->display->display_options['fields']['name']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['name']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['name']['alter']['html'] = 0;
$handler->display->display_options['fields']['name']['element_label_colon'] = 1;
$handler->display->display_options['fields']['name']['element_default_classes'] = 1;
$handler->display->display_options['fields']['name']['hide_empty'] = 0;
$handler->display->display_options['fields']['name']['empty_zero'] = 0;
$handler->display->display_options['fields']['name']['link_to_user'] = 1;
$handler->display->display_options['fields']['name']['overwrite_anonymous'] = 0;
/* Field: Fields: field_merci_date */
$handler->display->display_options['fields']['entity_id']['id'] = 'entity_id';
$handler->display->display_options['fields']['entity_id']['table'] = 'field_data_field_merci_date';
$handler->display->display_options['fields']['entity_id']['field'] = 'entity_id';
$handler->display->display_options['fields']['entity_id']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['entity_id']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['entity_id']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['entity_id']['alter']['trim'] = 0;
$handler->display->display_options['fields']['entity_id']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['entity_id']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['entity_id']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['entity_id']['alter']['html'] = 0;
$handler->display->display_options['fields']['entity_id']['element_label_colon'] = 1;
$handler->display->display_options['fields']['entity_id']['element_default_classes'] = 1;
$handler->display->display_options['fields']['entity_id']['hide_empty'] = 0;
$handler->display->display_options['fields']['entity_id']['empty_zero'] = 0;
$handler->display->display_options['fields']['entity_id']['click_sort_column'] = 'value';
$handler->display->display_options['fields']['entity_id']['settings'] = array(
  'format_type' => 'long',
  'fromto' => 'both',
  'multiple_number' => '',
  'multiple_from' => '',
  'multiple_to' => '',
  'show_repeat_rule' => 'show',
);
/* Field: MERCI: Reserved items */
$handler->display->display_options['fields']['item_nid2']['id'] = 'item_nid2';
$handler->display->display_options['fields']['item_nid2']['table'] = 'merci_reservation_detail';
$handler->display->display_options['fields']['item_nid2']['field'] = 'item_nid2';
$handler->display->display_options['fields']['item_nid2']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['item_nid2']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['item_nid2']['alter']['trim'] = 0;
$handler->display->display_options['fields']['item_nid2']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['item_nid2']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['item_nid2']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['item_nid2']['alter']['html'] = 0;
$handler->display->display_options['fields']['item_nid2']['hide_empty'] = 0;
$handler->display->display_options['fields']['item_nid2']['empty_zero'] = 0;
/* Field: MERCI: Reservation status */
$handler->display->display_options['fields']['status']['id'] = 'status';
$handler->display->display_options['fields']['status']['table'] = 'merci_reservation';
$handler->display->display_options['fields']['status']['field'] = 'status';
$handler->display->display_options['fields']['status']['label'] = '';
$handler->display->display_options['fields']['status']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['status']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['status']['alter']['trim'] = 0;
$handler->display->display_options['fields']['status']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['status']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['status']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['status']['alter']['html'] = 0;
$handler->display->display_options['fields']['status']['hide_empty'] = 0;
$handler->display->display_options['fields']['status']['empty_zero'] = 0;
/* Sort criterion: Fields: field_merci_date (field_merci_date) - value */
$handler->display->display_options['sorts']['field_merci_date_value_1']['id'] = 'field_merci_date_value_1';
$handler->display->display_options['sorts']['field_merci_date_value_1']['table'] = 'field_data_field_merci_date';
$handler->display->display_options['sorts']['field_merci_date_value_1']['field'] = 'field_merci_date_value';
/* Argument: Node: Nid */
$handler->display->display_options['arguments']['nid']['id'] = 'nid';
$handler->display->display_options['arguments']['nid']['table'] = 'node';
$handler->display->display_options['arguments']['nid']['field'] = 'nid';
$handler->display->display_options['arguments']['nid']['relationship'] = 'item_nid';
$handler->display->display_options['arguments']['nid']['default_action'] = 'not found';
$handler->display->display_options['arguments']['nid']['style_plugin'] = 'default_summary';
$handler->display->display_options['arguments']['nid']['title'] = 'Reservations for %1';
$handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
$handler->display->display_options['arguments']['nid']['validate_type'] = 'node';
$handler->display->display_options['arguments']['nid']['validate_options']['access'] = '1';
$handler->display->display_options['arguments']['nid']['break_phrase'] = 0;
$handler->display->display_options['arguments']['nid']['not'] = 0;
/* Argument: Date: Date (node) */
$handler->display->display_options['arguments']['date_argument']['id'] = 'date_argument';
$handler->display->display_options['arguments']['date_argument']['table'] = 'node';
$handler->display->display_options['arguments']['date_argument']['field'] = 'date_argument';
$handler->display->display_options['arguments']['date_argument']['default_action'] = 'default';
$handler->display->display_options['arguments']['date_argument']['style_plugin'] = 'default_summary';
$handler->display->display_options['arguments']['date_argument']['date_fields'] = array(
  'field_data_field_merci_date.field_merci_date_value' => 'field_data_field_merci_date.field_merci_date_value',
);
$handler->display->display_options['arguments']['date_argument']['granularity'] = 'week';
/* Filter: Node: Published */
$handler->display->display_options['filters']['status']['id'] = 'status';
$handler->display->display_options['filters']['status']['table'] = 'node';
$handler->display->display_options['filters']['status']['field'] = 'status';
$handler->display->display_options['filters']['status']['value'] = '1';
$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
/* Filter: Node: Type */
$handler->display->display_options['filters']['type']['id'] = 'type';
$handler->display->display_options['filters']['type']['table'] = 'node';
$handler->display->display_options['filters']['type']['field'] = 'type';
$handler->display->display_options['filters']['type']['value'] = array(
  'merci_reservation' => 'merci_reservation',
);
$handler->display->display_options['filters']['type']['expose']['operator'] = 'type_op';
$handler->display->display_options['filters']['type']['expose']['label'] = 'Node: Type';
$handler->display->display_options['filters']['type']['expose']['identifier'] = 'type';
$handler->display->display_options['filters']['type']['expose']['reduce'] = 1;
/* Filter: MERCI: Reservation status */
$handler->display->display_options['filters']['status_1']['id'] = 'status_1';
$handler->display->display_options['filters']['status_1']['table'] = 'merci_reservation';
$handler->display->display_options['filters']['status_1']['field'] = 'status';
$handler->display->display_options['filters']['status_1']['exposed'] = TRUE;
$handler->display->display_options['filters']['status_1']['expose']['operator'] = 'status_1_op';
$handler->display->display_options['filters']['status_1']['expose']['label'] = 'Status';
$handler->display->display_options['filters']['status_1']['expose']['identifier'] = 'status_1';
$handler->display->display_options['filters']['status_1']['expose']['reduce'] = 0;

/* Display: Calendar page */
$handler = $view->new_display('calendar', 'Calendar page', 'calendar_1');
$handler->display->display_options['items_per_page'] = 0;
$handler->display->display_options['pager']['type'] = 'some';
$handler->display->display_options['style_plugin'] = 'calendar_nav';
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['path'] = 'node/%/reservations';
$handler->display->display_options['menu']['type'] = 'tab';
$handler->display->display_options['menu']['title'] = 'Reservations';
$handler->display->display_options['menu']['description'] = 'Calendar to browse reservations';
$handler->display->display_options['menu']['weight'] = '0';
$handler->display->display_options['calendar_colors'] = array(
  'page' => '#ffff66',
  'story' => '#668cff',
);
$handler->display->display_options['calendar_popup'] = '1';
$handler->display->display_options['calendar_date_link'] = 'merci_reservation';

/* Display: Year view */
$handler = $view->new_display('calendar_period', 'Year view', 'calendar_period_1');
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'title' => 'title',
  'name' => 'name',
  'field_merci_date_value' => 'field_merci_date_value',
  'item_nid2' => 'item_nid2',
  'status' => 'status',
);
$handler->display->display_options['style_options']['default'] = '-1';
$handler->display->display_options['style_options']['info'] = array(
  'title' => array(
    'sortable' => 0,
    'separator' => '',
  ),
  'name' => array(
    'sortable' => 0,
    'separator' => '',
  ),
  'field_merci_date_value' => array(
    'sortable' => 0,
    'separator' => '',
  ),
  'item_nid2' => array(
    'separator' => '',
  ),
  'status' => array(
    'sortable' => 0,
    'separator' => '',
  ),
);
$handler->display->display_options['style_options']['override'] = 1;
$handler->display->display_options['style_options']['sticky'] = 0;
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['defaults']['row_plugin'] = FALSE;
$handler->display->display_options['defaults']['row_options'] = FALSE;
$handler->display->display_options['attachment_position'] = 'after';
$handler->display->display_options['inherit_exposed_filters'] = TRUE;
$handler->display->display_options['render_pager'] = TRUE;
$handler->display->display_options['displays'] = array(
  'calendar_1' => 'calendar_1',
  'default' => 0,
  'calendar_block_1' => 0,
);

/* Display: Month view */
$handler = $view->new_display('calendar_period', 'Month view', 'calendar_period_2');
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'calendar_style';
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['defaults']['row_plugin'] = FALSE;
$handler->display->display_options['defaults']['row_options'] = FALSE;
$handler->display->display_options['attachment_position'] = 'after';
$handler->display->display_options['inherit_exposed_filters'] = TRUE;
$handler->display->display_options['render_pager'] = TRUE;
$handler->display->display_options['displays'] = array(
  'calendar_1' => 'calendar_1',
  'default' => 0,
  'calendar_block_1' => 0,
);

/* Display: Day view */
$handler = $view->new_display('calendar_period', 'Day view', 'calendar_period_3');
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'title' => 'title',
  'name' => 'name',
  'field_merci_date_value' => 'field_merci_date_value',
  'item_nid2' => 'item_nid2',
  'status' => 'status',
  'title_1' => 'title_1',
);
$handler->display->display_options['style_options']['default'] = '-1';
$handler->display->display_options['style_options']['info'] = array(
  'title' => array(
    'sortable' => 0,
    'separator' => '',
  ),
  'name' => array(
    'sortable' => 0,
    'separator' => '',
  ),
  'field_merci_date_value' => array(
    'sortable' => 0,
    'separator' => '',
  ),
  'item_nid2' => array(
    'separator' => '',
  ),
  'status' => array(
    'sortable' => 0,
    'separator' => '',
  ),
  'title_1' => array(
    'sortable' => 0,
    'separator' => '',
  ),
);
$handler->display->display_options['style_options']['override'] = 1;
$handler->display->display_options['style_options']['sticky'] = 0;
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['defaults']['row_plugin'] = FALSE;
$handler->display->display_options['defaults']['row_options'] = FALSE;
$handler->display->display_options['attachment_position'] = 'after';
$handler->display->display_options['inherit_exposed_filters'] = TRUE;
$handler->display->display_options['render_pager'] = TRUE;
$handler->display->display_options['displays'] = array(
  'calendar_1' => 'calendar_1',
  'default' => 0,
  'block_1' => 0,
);

/* Display: Week view */
$handler = $view->new_display('calendar_period', 'Week view', 'calendar_period_4');
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'title' => 'title',
  'field_merci_date_value' => 'field_merci_date_value',
  'status' => 'status',
  'item_nid2' => 'item_nid2',
);
$handler->display->display_options['style_options']['default'] = '-1';
$handler->display->display_options['style_options']['info'] = array(
  'title' => array(
    'sortable' => 0,
    'separator' => '',
  ),
  'field_merci_date_value' => array(
    'sortable' => 0,
    'separator' => '',
  ),
  'status' => array(
    'sortable' => 0,
    'separator' => '',
  ),
  'item_nid2' => array(
    'separator' => '',
  ),
);
$handler->display->display_options['style_options']['override'] = 1;
$handler->display->display_options['style_options']['sticky'] = 0;
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['defaults']['row_plugin'] = FALSE;
$handler->display->display_options['defaults']['row_options'] = FALSE;
$handler->display->display_options['attachment_position'] = 'after';
$handler->display->display_options['inherit_exposed_filters'] = TRUE;
$handler->display->display_options['render_pager'] = TRUE;
$handler->display->display_options['displays'] = array(
  'calendar_1' => 'calendar_1',
  'default' => 0,
  'block_1' => 0,
  'block_2' => 0,
  'block_3' => 0,
);
$translatables['merci_node_reservations'] = array(
  t('Default'),
  t('Merci Reservation Calendar'),
  t('more'),
  t('Apply'),
  t('Reset'),
  t('Sort By'),
  t('Asc'),
  t('Desc'),
  t('Items per page'),
  t('- All -'),
  t('Offset'),
  t('Reserved item node id'),
  t('field_merci_date'),
  t('Reserved items'),
  t('All'),
  t('Reservations for %1'),
  t('Node: Type'),
  t('Status'),
  t('Calendar page'),
  t('Year view'),
  t('Month view'),
  t('Day view'),
  t('Week view'),
);


$views[$view->name] = $view;

return $views;
}

