<?php
// $Id$


/**
 * Implementation of hook_views_default_views().
 */
function merci_calendar_view_views_default_views() {

  /*
 * Merci Reservation Calendar View.
 */


  $view               = new view;
  $view->name         = 'merci_reservation_calendar';
  $view->description  = 'A multi-dimensional calendar view for Merci Reservations with back/next navigation.';
  $view->tag          = 'Merci Reservation Calendar';
  $view->view_php     = '';
  $view->base_table   = 'node';
  $view->is_cacheable = FALSE;
  $view->api_version  = 2;
  /* Edit this to true to make a default view disabled initially */
  $view->disabled = FALSE;
  $handler = $view->new_display('default', 'Default', 'default');
  $handler->override_option('fields', array(
      'title' => array(
        'label' => '',
        'link_to_node' => 1,
        'exclude' => 0,
        'id' => 'title',
        'table' => 'node',
        'field' => 'title',
        'relationship' => 'none',
      ),
      'field_merci_date_value' => array(
        'label' => '',
        'alter' => array(
          'alter_text' => 0,
          'text' => '',
          'make_link' => 0,
          'path' => '',
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
        'link_to_node' => 0,
        'label_type' => 'none',
        'format' => 'default',
        'multiple' => array(
          'multiple_number' => '',
          'multiple_from' => '',
          'multiple_to' => '',
          'group' => TRUE,
        ),
        'repeat' => array(
          'show_repeat_rule' => '',
        ),
        'fromto' => array(
          'fromto' => 'both',
        ),
        'exclude' => 0,
        'id' => 'field_merci_date_value',
        'table' => 'node_data_field_merci_date',
        'field' => 'field_merci_date_value',
        'override' => array(
          'button' => 'Override',
        ),
        'relationship' => 'none',
      ),
      'status' => array(
        'label' => 'Status',
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
        'table' => 'merci_reservation',
        'field' => 'status',
        'relationship' => 'none',
      ),
    ));
  $handler->override_option('sorts', array(
      'field_merci_date_value' => array(
        'exposed' => FALSE,
        'order' => 'ASC',
        'identifier' => 'unsorted',
        'delta' => -1,
        'id' => 'field_merci_date_value',
        'table' => 'node_data_field_merci_date',
        'field' => 'field_merci_date_value',
        'override' => array(
          'button' => 'Override',
        ),
        'relationship' => 'none',
      ),
    ));
  $handler->override_option('arguments', array(
      'date_argument' => array(
        'default_action' => 'default',
        'style_plugin' => 'default_summary',
        'style_options' => array(),
        'wildcard' => 'all',
        'wildcard_substitution' => 'All',
        'title' => '',
        'default_argument_type' => 'date',
        'default_argument' => '',
        'validate_type' => 'none',
        'validate_fail' => 'not found',
        'date_fields' => array(
          'node_data_field_merci_date.field_merci_date_value' => 'node_data_field_merci_date.field_merci_date_value',
        ),
        'date_method' => 'OR',
        'granularity' => 'day',
        'id' => 'date_argument',
        'table' => 'node',
        'field' => 'date_argument',
        'relationship' => 'none',
        'default_argument_user' => 0,
        'default_argument_fixed' => '',
        'default_argument_php' => '',
        'validate_argument_node_access' => 0,
        'validate_argument_nid_type' => 'nid',
        'validate_argument_type' => 'tid',
        'validate_argument_php' => '',
        'override' => array(
          'button' => 'Override',
        ),
        'default_options_div_prefix' => '',
        'validate_argument_is_member' => 0,
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
      'type' => array(
        'operator' => 'in',
        'value' => array(
          'merci_reservation' => 'merci_reservation',
        ),
        'group' => '0',
        'exposed' => FALSE,
        'expose' => array(
          'operator' => 'type_op',
          'label' => 'Node: Type',
          'use_operator' => 0,
          'identifier' => 'type',
          'optional' => 1,
          'single' => 1,
          'remember' => 0,
          'reduce' => 1,
        ),
        'id' => 'type',
        'table' => 'node',
        'field' => 'type',
        'relationship' => 'none',
        'override' => array(
          'button' => 'Override',
        ),
      ),
      'status_1' => array(
        'operator' => 'in',
        'value' => array(),
        'group' => '0',
        'exposed' => TRUE,
        'expose' => array(
          'use_operator' => 0,
          'operator' => 'status_1_op',
          'identifier' => 'status_1',
          'label' => 'Status',
          'bef_filter_description' => '',
          'bef_format' => 'default',
          'optional' => 1,
          'single' => 1,
          'remember' => 0,
          'reduce' => 0,
          'bef_select_all_none' => 0,
        ),
        'id' => 'status_1',
        'table' => 'merci_reservation',
        'field' => 'status',
        'relationship' => 'none',
      ),
    ));
  $handler->override_option('access', array(
      'type' => 'perm',
      'perm' => 'manage MERCI',
    ));
  $handler->override_option('cache', array(
      'type' => 'none',
    ));
  $handler->override_option('title', 'Merci Reservation Calendar');
  $handler->override_option('header_format', '2');
  $handler->override_option('header_empty', 0);
  $handler->override_option('use_ajax', TRUE);
  $handler->override_option('items_per_page', 0);
  $handler->override_option('style_plugin', 'calendar_nav');
  $handler->override_option('style_options', array(
      'name_size' => '3',
      'with_weekno' => '0',
    ));
  $handler = $view->new_display('calendar', 'Calendar page', 'calendar_1');
  $handler->override_option('style_options', array());
  $handler->override_option('path', 'admin/merci/manage/reservations');
  $handler->override_option('menu', array(
      'type' => 'tab',
      'title' => 'Manage Reservations',
      'description' => 'Calendar to browse reservations',
      'weight' => '0',
      'name' => 'navigation',
    ));
  $handler->override_option('tab_options', array(
      'type' => 'none',
      'title' => '',
      'description' => '',
      'weight' => 0,
      'name' => 'navigation',
    ));
  $handler->override_option('calendar_colors', array(
      'page' => '#ffff66',
      'story' => '#668cff',
    ));
  $handler->override_option('calendar_colors_vocabulary', array());
  $handler->override_option('calendar_colors_taxonomy', array());
  $handler->override_option('calendar_popup', '1');
  $handler->override_option('calendar_date_link', 'merci_reservation');
  $handler = $view->new_display('calendar_period', 'Year view', 'calendar_period_1');
  $handler->override_option('style_plugin', 'calendar_style');
  $handler->override_option('style_options', array(
      'display_type' => 'year',
      'name_size' => '1',
    ));
  $handler->override_option('attachment_position', 'after');
  $handler->override_option('inherit_arguments', TRUE);
  $handler->override_option('inherit_exposed_filters', TRUE);
  $handler->override_option('inherit_pager', FALSE);
  $handler->override_option('render_pager', TRUE);
  $handler->override_option('displays', array(
      'calendar_1' => 'calendar_1',
      'default' => 0,
      'calendar_block_1' => 0,
    ));
  $handler->override_option('calendar_type', 'year');
  $handler = $view->new_display('calendar_period', 'Month view', 'calendar_period_2');
  $handler->override_option('style_plugin', 'table');
  $handler->override_option('style_options', array(
      'grouping' => '',
      'override' => 1,
      'sticky' => 0,
      'order' => 'asc',
      'columns' => array(
        'title' => 'title',
        'field_merci_date_value' => 'field_merci_date_value',
        'status' => 'status',
      ),
      'info' => array(
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
      ),
      'default' => '-1',
    ));
  $handler->override_option('attachment_position', 'after');
  $handler->override_option('inherit_arguments', TRUE);
  $handler->override_option('inherit_exposed_filters', TRUE);
  $handler->override_option('inherit_pager', FALSE);
  $handler->override_option('render_pager', TRUE);
  $handler->override_option('displays', array(
      'calendar_1' => 'calendar_1',
      'default' => 0,
      'calendar_block_1' => 0,
    ));
  $handler->override_option('calendar_type', 'month');
  $handler = $view->new_display('calendar_period', 'Day view', 'calendar_period_3');
  $handler->override_option('style_plugin', 'table');
  $handler->override_option('style_options', array(
      'grouping' => '',
      'override' => 1,
      'sticky' => 0,
      'order' => 'asc',
      'columns' => array(
        'title' => 'title',
        'field_merci_date_value' => 'field_merci_date_value',
        'status' => 'status',
      ),
      'info' => array(
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
      ),
      'default' => '-1',
    ));
  $handler->override_option('attachment_position', 'after');
  $handler->override_option('inherit_arguments', TRUE);
  $handler->override_option('inherit_exposed_filters', TRUE);
  $handler->override_option('inherit_pager', FALSE);
  $handler->override_option('render_pager', TRUE);
  $handler->override_option('displays', array(
      'calendar_1' => 'calendar_1',
      'default' => 0,
      'block_1' => 0,
    ));
  $handler->override_option('calendar_type', 'day');
  $handler = $view->new_display('calendar_period', 'Week view', 'calendar_period_4');
  $handler->override_option('fields', array(
      'title' => array(
        'label' => '',
        'link_to_node' => 1,
        'exclude' => 0,
        'id' => 'title',
        'table' => 'node',
        'field' => 'title',
        'relationship' => 'none',
      ),
      'field_merci_date_value' => array(
        'label' => '',
        'alter' => array(
          'alter_text' => 0,
          'text' => '',
          'make_link' => 0,
          'path' => '',
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
        'link_to_node' => 0,
        'label_type' => 'none',
        'format' => 'default',
        'multiple' => array(
          'multiple_number' => '',
          'multiple_from' => '',
          'multiple_to' => '',
          'group' => TRUE,
        ),
        'repeat' => array(
          'show_repeat_rule' => '',
        ),
        'fromto' => array(
          'fromto' => 'both',
        ),
        'exclude' => 0,
        'id' => 'field_merci_date_value',
        'table' => 'node_data_field_merci_date',
        'field' => 'field_merci_date_value',
        'override' => array(
          'button' => 'Override',
        ),
        'relationship' => 'none',
      ),
      'status' => array(
        'label' => 'Status',
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
        'table' => 'merci_reservation',
        'field' => 'status',
        'relationship' => 'none',
      ),
      'field_merci_date_value_1' => array(
        'id' => 'field_merci_date_value_1',
        'table' => 'node_data_field_merci_date',
        'field' => 'field_merci_date_value',
      ),
    ));
  $handler->override_option('style_plugin', 'table');
  $handler->override_option('style_options', array(
      'grouping' => '',
      'override' => 1,
      'sticky' => 0,
      'order' => 'asc',
      'columns' => array(
        'title' => 'title',
        'field_merci_date_value' => 'field_merci_date_value',
        'status' => 'status',
        'field_merci_date_value_1' => 'field_merci_date_value_1',
      ),
      'info' => array(
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
        'field_merci_date_value_1' => array(
          'sortable' => 0,
          'separator' => '',
        ),
      ),
      'default' => '-1',
    ));
  $handler->override_option('attachment_position', 'after');
  $handler->override_option('inherit_arguments', TRUE);
  $handler->override_option('inherit_exposed_filters', TRUE);
  $handler->override_option('inherit_pager', FALSE);
  $handler->override_option('render_pager', TRUE);
  $handler->override_option('displays', array(
      'calendar_1' => 'calendar_1',
      'page_1' => 'page_1',
      'default' => 0,
      'block_1' => 0,
    ));
  $handler->override_option('calendar_type', 'week');
  $handler = $view->new_display('block', 'Upcoming Reservations', 'block_1');
  $handler->override_option('sorts', array(
      'field_merci_date_value' => array(
        'exposed' => FALSE,
        'order' => 'ASC',
        'identifier' => 'unsorted',
        'delta' => -1,
        'id' => 'field_merci_date_value',
        'table' => 'node_data_field_merci_date',
        'field' => 'field_merci_date_value',
        'override' => array(
          'button' => 'Use default',
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
      'type' => array(
        'operator' => 'in',
        'value' => array(
          'merci_reservation' => 'merci_reservation',
        ),
        'group' => '0',
        'exposed' => FALSE,
        'expose' => array(
          'operator' => 'type_op',
          'label' => 'Node: Type',
          'use_operator' => 0,
          'identifier' => 'type',
          'optional' => 1,
          'single' => 1,
          'remember' => 0,
          'reduce' => 1,
        ),
        'id' => 'type',
        'table' => 'node',
        'field' => 'type',
        'relationship' => 'none',
        'override' => array(
          'button' => 'Override',
        ),
      ),
      'date_filter' => array(
        'operator' => '>=',
        'value' => array(
          'value' => NULL,
          'min' => NULL,
          'max' => NULL,
          'default_date' => 'now',
          'default_to_date' => '',
        ),
        'group' => '0',
        'exposed' => FALSE,
        'expose' => array(
          'operator' => FALSE,
          'label' => '',
        ),
        'date_fields' => array(
          'node_data_field_merci_date.field_merci_date_value' => 'node_data_field_merci_date.field_merci_date_value',
        ),
        'date_method' => 'OR',
        'granularity' => 'second',
        'form_type' => 'date_select',
        'default_date' => 'now',
        'default_to_date' => '',
        'year_range' => '-3:+3',
        'id' => 'date_filter',
        'table' => 'node',
        'field' => 'date_filter',
        'override' => array(
          'button' => 'Use default',
        ),
        'relationship' => 'none',
      ),
    ));
  $handler->override_option('title', 'Upcoming Reservations');
  $handler->override_option('items_per_page', 5);
  $handler->override_option('use_more', 1);
  $handler->override_option('use_more_always', 0);
  $handler->override_option('style_plugin', 'default');
  $handler->override_option('style_options', array(
      'grouping' => '',
    ));
  $handler->override_option('block_description', 'MERCI: Upcoming Reservations');
  $handler->override_option('block_caching', -1);
  $handler = $view->new_display('block', 'User Reservations', 'block_2');
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
      'type' => array(
        'operator' => 'in',
        'value' => array(
          'merci_reservation' => 'merci_reservation',
        ),
        'group' => '0',
        'exposed' => FALSE,
        'expose' => array(
          'operator' => 'type_op',
          'label' => 'Node: Type',
          'use_operator' => 0,
          'identifier' => 'type',
          'optional' => 1,
          'single' => 1,
          'remember' => 0,
          'reduce' => 1,
        ),
        'id' => 'type',
        'table' => 'node',
        'field' => 'type',
        'relationship' => 'none',
        'override' => array(
          'button' => 'Override',
        ),
      ),
      'date_filter' => array(
        'operator' => '>=',
        'value' => array(
          'value' => NULL,
          'min' => NULL,
          'max' => NULL,
          'default_date' => 'now',
          'default_to_date' => '',
        ),
        'group' => '0',
        'exposed' => FALSE,
        'expose' => array(
          'operator' => FALSE,
          'label' => '',
        ),
        'date_fields' => array(
          'node_data_field_merci_date.field_merci_date_value' => 'node_data_field_merci_date.field_merci_date_value',
        ),
        'date_method' => 'OR',
        'granularity' => 'second',
        'form_type' => 'date_select',
        'default_date' => 'now',
        'default_to_date' => '',
        'year_range' => '-3:+3',
        'id' => 'date_filter',
        'table' => 'node',
        'field' => 'date_filter',
        'override' => array(
          'button' => 'Use default',
        ),
        'relationship' => 'none',
      ),
      'uid_current' => array(
        'operator' => '=',
        'value' => '1',
        'group' => '0',
        'exposed' => FALSE,
        'expose' => array(
          'operator' => FALSE,
          'label' => '',
        ),
        'id' => 'uid_current',
        'table' => 'users',
        'field' => 'uid_current',
        'override' => array(
          'button' => 'Use default',
        ),
        'relationship' => 'none',
      ),
    ));
  $handler->override_option('title', 'My Reservations');
  $handler->override_option('style_plugin', 'default');
  $handler->override_option('style_options', array(
      'grouping' => '',
    ));
  $handler->override_option('block_description', 'MERCI: User Upcoming Reservations');
  $handler->override_option('block_caching', -1);
  $views[$view->name] = $view;

  /*
   * View 'merci_inventory'
   */

  $view               = new view;
  $view->name         = 'merci_inventory';
  $view->description  = '';
  $view->tag          = '';
  $view->view_php     = '';
  $view->base_table   = 'node';
  $view->is_cacheable = FALSE;
  $view->api_version  = 2;
  /* Edit this to true to make a default view disabled initially */
  $view->disabled = FALSE;
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
    ));
  $handler->override_option('access', array(
      'type' => 'perm',
      'perm' => 'manage MERCI',
    ));
  $handler->override_option('cache', array(
      'type' => 'none',
    ));
  $handler->override_option('title', 'MERCI Inventory');
  $handler->override_option('use_ajax', TRUE);
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
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->override_option('path', 'admin/merci/manage/inventory');
  $handler->override_option('menu', array(
      'type' => 'tab',
      'title' => 'Manage Inventory',
      'description' => 'Manage reservable equipment',
      'weight' => '8',
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

