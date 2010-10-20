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

  $view               = new view;
  $view->name         = 'merci_my_reservations';
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
          'strip_tags' => 0,
          'html' => 0,
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
      'field_merci_date_value' => array(
        'label' => 'Reservation',
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
          'strip_tags' => 0,
          'html' => 0,
        ),
        'empty' => '',
        'hide_empty' => 0,
        'empty_zero' => 0,
        'link_to_node' => 0,
        'label_type' => 'widget',
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
        'relationship' => 'none',
      ),
    ));
  $handler->override_option('sorts', array(
      'field_merci_date_value' => array(
        'order' => 'ASC',
        'delta' => -1,
        'id' => 'field_merci_date_value',
        'table' => 'node_data_field_merci_date',
        'field' => 'field_merci_date_value',
        'relationship' => 'none',
      ),
    ));
  $handler->override_option('arguments', array(
      'uid' => array(
        'default_action' => 'ignore',
        'style_plugin' => 'default_summary',
        'style_options' => array(),
        'wildcard' => 'all',
        'wildcard_substitution' => 'All',
        'title' => '%2',
        'breadcrumb' => '',
        'default_argument_type' => 'fixed',
        'default_argument' => '',
        'validate_type' => 'none',
        'validate_fail' => 'not found',
        'break_phrase' => 0,
        'not' => 0,
        'id' => 'uid',
        'table' => 'users',
        'field' => 'uid',
        'validate_user_argument_type' => 'uid',
        'validate_user_roles' => array(
          '2' => 0,
          '14' => 0,
          '3' => 0,
          '6' => 0,
          '9' => 0,
          '10' => 0,
          '11' => 0,
          '8' => 0,
          '7' => 0,
          '13' => 0,
          '4' => 0,
          '12' => 0,
          '5' => 0,
        ),
        'relationship' => 'none',
        'default_options_div_prefix' => '',
        'default_argument_user' => 0,
        'default_argument_fixed' => '',
        'default_argument_php' => '',
        'validate_argument_node_type' => array(
          'webform' => 0,
          'merci_reservation' => 0,
          'forum' => 0,
          'album' => 0,
          'article' => 0,
          'blog' => 0,
          'curriculum' => 0,
          'embedded_show' => 0,
          'feed' => 0,
          'front_page_image' => 0,
          'instructor' => 0,
          'merci_audio_kit' => 0,
          'merci_audio_mixer' => 0,
          'merci_edit_suite' => 0,
          'merci_facility' => 0,
          'merci_field_gear_accessory' => 0,
          'merci_inventory_master' => 0,
          'merci_light_kit' => 0,
          'merci_sony_pd150' => 0,
          'merci_sony_pd170' => 0,
          'merci_staff_facility' => 0,
          'merci_studio' => 0,
          'merci_tripod' => 0,
          'om_airing' => 0,
          'om_feed' => 0,
          'om_project' => 0,
          'om_project_blog' => 0,
          'om_project_event' => 0,
          'om_project_type' => 0,
          'om_show' => 0,
          'om_timeslot_event' => 0,
          'om_timeslot_rule' => 0,
          'om_timeslot_server' => 0,
          'om_timeslot_theme' => 0,
          'page' => 0,
          'person' => 0,
          'resource_link' => 0,
          'song' => 0,
          'story' => 0,
          'success_story' => 0,
          'unit' => 0,
        ),
        'validate_argument_node_access' => 0,
        'validate_argument_nid_type' => 'nid',
        'validate_argument_vocabulary' => array(
          '12' => 0,
          '1' => 0,
          '8' => 0,
          '10' => 0,
          '7' => 0,
          '3' => 0,
          '4' => 0,
          '2' => 0,
          '6' => 0,
          '9' => 0,
          '5' => 0,
        ),
        'validate_argument_type' => 'tid',
        'validate_argument_transform' => 0,
        'validate_user_restrict_roles' => 0,
        'validate_argument_node_flag_name' => '*relationship*',
        'validate_argument_node_flag_test' => 'flaggable',
        'validate_argument_node_flag_id_type' => 'id',
        'validate_argument_user_flag_name' => '*relationship*',
        'validate_argument_user_flag_test' => 'flaggable',
        'validate_argument_user_flag_id_type' => 'id',
        'validate_argument_is_member' => 0,
        'validate_argument_php' => '',
      ),
    ));
  $handler->override_option('filters', array(
      'type' => array(
        'operator' => 'in',
        'value' => array(
          'merci_reservation' => 'merci_reservation',
        ),
        'group' => '0',
        'exposed' => FALSE,
        'expose' => array(
          'operator' => FALSE,
          'label' => '',
        ),
        'id' => 'type',
        'table' => 'node',
        'field' => 'type',
        'relationship' => 'none',
      ),
      'date_filter' => array(
        'operator' => '>',
        'value' => array(
          'value' => NULL,
          'min' => NULL,
          'max' => NULL,
          'default_date' => 'now -1 day',
          'default_to_date' => '',
        ),
        'group' => '0',
        'exposed' => TRUE,
        'expose' => array(
          'use_operator' => 0,
          'operator' => 'date_filter_op',
          'identifier' => 'date_filter',
          'label' => 'Show Reservations Due After',
          'optional' => 1,
          'remember' => 0,
        ),
        'date_fields' => array(
          'node_data_field_merci_date.field_merci_date_value2' => 'node_data_field_merci_date.field_merci_date_value2',
        ),
        'date_method' => 'OR',
        'granularity' => 'day',
        'form_type' => 'date_select',
        'default_date' => 'now -1 day',
        'default_to_date' => '',
        'year_range' => '-3:+3',
        'id' => 'date_filter',
        'table' => 'node',
        'field' => 'date_filter',
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
        'relationship' => 'none',
      ),
    ));
  $handler->override_option('access', array(
      'type' => 'none',
    ));
  $handler->override_option('cache', array(
      'type' => 'none',
    ));
  $handler->override_option('footer', '<?php
print \'<div id="make-merci-reservation">\';
print l(t(\'Make a Reservation\'), \'node/add/merci-reservation\'); 
print \'</div>\';');
  $handler->override_option('footer_format', '3');
  $handler->override_option('footer_empty', 1);
  $handler->override_option('empty', 'You have no upcoming reservations.');
  $handler->override_option('empty_format', '1');
  $handler->override_option('items_per_page', 25);
  $handler->override_option('use_pager', 'mini');
  $handler->override_option('style_plugin', 'table');
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
          'sortable' => 1,
          'separator' => '',
        ),
      ),
      'default' => '-1',
    ));

  $views[$view->name] = $view;
  
  $view = new view;
$view->name = 'merci_incoming_reservations';
$view->description = '';
$view->tag = '';
$view->view_php = '';
$view->base_table = 'node';
$view->is_cacheable = FALSE;
$view->api_version = 2;
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->override_option('fields', array(
  'name' => array(
    'label' => 'Name',
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
    'link_to_user' => 1,
    'overwrite_anonymous' => 0,
    'anonymous_text' => '',
    'exclude' => 0,
    'id' => 'name',
    'table' => 'users',
    'field' => 'name',
    'override' => array(
      'button' => 'Override',
    ),
    'relationship' => 'none',
  ),
  'field_merci_date_value2' => array(
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
    'label_type' => 'none',
    'format' => 'default',
    'multiple' => array(
      'multiple_number' => '',
      'multiple_from' => '',
      'multiple_to' => '',
      'group' => '',
    ),
    'repeat' => array(
      'show_repeat_rule' => '',
    ),
    'fromto' => array(
      'fromto' => 'value2',
    ),
    'exclude' => 1,
    'id' => 'field_merci_date_value2',
    'table' => 'node_data_field_merci_date',
    'field' => 'field_merci_date_value2',
    'override' => array(
      'button' => 'Override',
    ),
    'relationship' => 'none',
  ),
  'field_merci_date_value' => array(
    'label' => 'Reservation',
    'alter' => array(
      'alter_text' => 1,
      'text' => '[field_merci_date_value]<br />
[field_merci_date_value2]',
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
    'label_type' => 'widget',
    'format' => 'default',
    'multiple' => array(
      'multiple_number' => '',
      'multiple_from' => '',
      'multiple_to' => '',
      'group' => '',
    ),
    'repeat' => array(
      'show_repeat_rule' => '',
    ),
    'fromto' => array(
      'fromto' => 'value',
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
  'item_nid2' => array(
    'label' => 'Reserved items',
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
    'type' => 'separator',
    'separator' => ', ',
    'exclude' => 0,
    'id' => 'item_nid2',
    'table' => 'merci_reservation_detail',
    'field' => 'item_nid2',
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
    'override' => array(
      'button' => 'Override',
    ),
    'relationship' => 'none',
  ),
  'edit_node' => array(
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
    'text' => '',
    'exclude' => 0,
    'id' => 'edit_node',
    'table' => 'node',
    'field' => 'edit_node',
    'override' => array(
      'button' => 'Override',
    ),
    'relationship' => 'none',
  ),
));
$handler->override_option('sorts', array(
  'field_merci_date_value' => array(
    'order' => 'ASC',
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
$handler->override_option('filters', array(
  'type' => array(
    'operator' => 'in',
    'value' => array(
      'merci_reservation' => 'merci_reservation',
    ),
    'group' => '0',
    'exposed' => FALSE,
    'expose' => array(
      'operator' => FALSE,
      'label' => '',
    ),
    'id' => 'type',
    'table' => 'node',
    'field' => 'type',
    'override' => array(
      'button' => 'Override',
    ),
    'relationship' => 'none',
  ),
  'date_filter' => array(
    'operator' => '>',
    'value' => array(
      'value' => NULL,
      'min' => NULL,
      'max' => NULL,
      'default_date' => '12AM today',
      'default_to_date' => '',
    ),
    'group' => '0',
    'exposed' => FALSE,
    'expose' => array(
      'operator' => 'date_filter_op',
      'label' => 'Reservation Starts After:',
      'use_operator' => 0,
      'identifier' => 'date_filter',
      'optional' => 1,
      'remember' => 0,
    ),
    'date_fields' => array(
      'node_data_field_merci_date.field_merci_date_value2' => 'node_data_field_merci_date.field_merci_date_value2',
    ),
    'date_method' => 'OR',
    'granularity' => 'day',
    'form_type' => 'date_popup',
    'default_date' => '12AM today',
    'default_to_date' => '',
    'year_range' => '-3:+3',
    'id' => 'date_filter',
    'table' => 'node',
    'field' => 'date_filter',
    'override' => array(
      'button' => 'Override',
    ),
    'relationship' => 'none',
  ),
  'status' => array(
    'operator' => 'in',
    'value' => array(
      '3' => '3',
    ),
    'group' => '0',
    'exposed' => FALSE,
    'expose' => array(
      'operator' => FALSE,
      'label' => '',
    ),
    'id' => 'status',
    'table' => 'merci_reservation',
    'field' => 'status',
    'override' => array(
      'button' => 'Override',
    ),
    'relationship' => 'none',
  ),
));
$handler->override_option('access', array(
  'type' => 'none',
));
$handler->override_option('cache', array(
  'type' => 'none',
));
$handler->override_option('empty', '<div class="messages status">Anticipating no returns.</div>');
$handler->override_option('empty_format', '2');
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
    'name' => 'name',
    'group_nid' => 'group_nid',
    'field_merci_date_value' => 'field_merci_date_value',
    'item_nid2' => 'item_nid2',
    'status' => 'status',
    'edit_node' => 'edit_node',
  ),
  'info' => array(
    'title' => array(
      'sortable' => 0,
      'separator' => '',
    ),
    'name' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'group_nid' => array(
      'separator' => '',
    ),
    'field_merci_date_value' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'item_nid2' => array(
      'separator' => '',
    ),
    'status' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'edit_node' => array(
      'separator' => '',
    ),
  ),
  'default' => 'field_merci_date_value',
));
$handler = $view->new_display('page', 'Incoming', 'page_1');
$handler->override_option('use_ajax', TRUE);
$handler->override_option('path', 'admin/merci/manage/reservations/incoming');
$handler->override_option('menu', array(
  'type' => 'tab',
  'title' => 'Incoming',
  'description' => '',
  'weight' => '1',
  'name' => 'navigation',
));
$handler->override_option('tab_options', array(
  'type' => 'normal',
  'title' => 'Reservations',
  'description' => '',
  'weight' => '0',
  'name' => 'navigation',
));

  
  $views[$view->name] = $view;

$view = new view;
$view->name = 'merci_overdue_reservations';
$view->description = '';
$view->tag = '';
$view->view_php = '';
$view->base_table = 'node';
$view->is_cacheable = FALSE;
$view->api_version = 2;
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->override_option('fields', array(
  'name' => array(
    'label' => 'Name',
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
    'link_to_user' => 1,
    'overwrite_anonymous' => 0,
    'anonymous_text' => '',
    'exclude' => 0,
    'id' => 'name',
    'table' => 'users',
    'field' => 'name',
    'override' => array(
      'button' => 'Override',
    ),
    'relationship' => 'none',
  ),
  'field_merci_date_value2' => array(
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
    'label_type' => 'none',
    'format' => 'default',
    'multiple' => array(
      'multiple_number' => '',
      'multiple_from' => '',
      'multiple_to' => '',
      'group' => '',
    ),
    'repeat' => array(
      'show_repeat_rule' => '',
    ),
    'fromto' => array(
      'fromto' => 'value2',
    ),
    'exclude' => 1,
    'id' => 'field_merci_date_value2',
    'table' => 'node_data_field_merci_date',
    'field' => 'field_merci_date_value2',
    'override' => array(
      'button' => 'Override',
    ),
    'relationship' => 'none',
  ),
  'field_merci_date_value' => array(
    'label' => 'Reservation',
    'alter' => array(
      'alter_text' => 1,
      'text' => '[field_merci_date_value]<br />
[field_merci_date_value2]',
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
    'label_type' => 'widget',
    'format' => 'default',
    'multiple' => array(
      'multiple_number' => '',
      'multiple_from' => '',
      'multiple_to' => '',
      'group' => '',
    ),
    'repeat' => array(
      'show_repeat_rule' => '',
    ),
    'fromto' => array(
      'fromto' => 'value',
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
  'item_nid2' => array(
    'label' => 'Reserved items',
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
    'type' => 'separator',
    'separator' => ', ',
    'exclude' => 0,
    'id' => 'item_nid2',
    'table' => 'merci_reservation_detail',
    'field' => 'item_nid2',
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
    'override' => array(
      'button' => 'Override',
    ),
    'relationship' => 'none',
  ),
  'edit_node' => array(
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
    'text' => '',
    'exclude' => 0,
    'id' => 'edit_node',
    'table' => 'node',
    'field' => 'edit_node',
    'override' => array(
      'button' => 'Override',
    ),
    'relationship' => 'none',
  ),
));
$handler->override_option('filters', array(
  'type' => array(
    'operator' => 'in',
    'value' => array(
      'merci_reservation' => 'merci_reservation',
    ),
    'group' => '0',
    'exposed' => FALSE,
    'expose' => array(
      'operator' => FALSE,
      'label' => '',
    ),
    'id' => 'type',
    'table' => 'node',
    'field' => 'type',
    'override' => array(
      'button' => 'Override',
    ),
    'relationship' => 'none',
  ),
  'date_filter' => array(
    'operator' => '<',
    'value' => array(
      'min' => NULL,
      'max' => NULL,
      'value' => NULL,
      'default_date' => 'now',
      'default_to_date' => '',
    ),
    'group' => '0',
    'exposed' => FALSE,
    'expose' => array(
      'operator' => 'date_filter_op',
      'label' => 'Reservation Ended:',
      'use_operator' => 0,
      'identifier' => 'date_filter',
      'optional' => 1,
      'remember' => 0,
    ),
    'date_fields' => array(
      'node_data_field_merci_date.field_merci_date_value2' => 'node_data_field_merci_date.field_merci_date_value2',
    ),
    'date_method' => 'OR',
    'granularity' => 'minute',
    'form_type' => 'date_popup',
    'default_date' => 'now',
    'default_to_date' => '',
    'year_range' => '-3:+3',
    'id' => 'date_filter',
    'table' => 'node',
    'field' => 'date_filter',
    'override' => array(
      'button' => 'Override',
    ),
    'relationship' => 'none',
  ),
  'status' => array(
    'operator' => 'in',
    'value' => array(
      '3' => '3',
    ),
    'group' => '0',
    'exposed' => FALSE,
    'expose' => array(
      'operator' => FALSE,
      'label' => '',
    ),
    'id' => 'status',
    'table' => 'merci_reservation',
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
$handler->override_option('header_format', '2');
$handler->override_option('header_empty', 0);
$handler->override_option('footer_format', '2');
$handler->override_option('footer_empty', 0);
$handler->override_option('empty', '<div class="messages status">There are currently no overdue reservations.</div>');
$handler->override_option('empty_format', '2');
$handler->override_option('items_per_page', 25);
$handler->override_option('use_pager', '1');
$handler->override_option('style_plugin', 'list');
$handler->override_option('style_options', array(
  'grouping' => '',
  'type' => 'ul',
));
$handler = $view->new_display('page', 'Overdue', 'page_1');
$handler->override_option('use_ajax', TRUE);
$handler->override_option('style_plugin', 'table');
$handler->override_option('style_options', array(
  'grouping' => '',
  'override' => 1,
  'sticky' => 0,
  'order' => 'asc',
  'columns' => array(
    'name' => 'name',
    'group_nid' => 'group_nid',
    'field_merci_date_value2' => 'field_merci_date_value2',
    'field_merci_date_value' => 'field_merci_date_value',
    'item_nid2' => 'item_nid2',
    'status' => 'status',
    'edit_node' => 'edit_node',
  ),
  'info' => array(
    'name' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'group_nid' => array(
      'separator' => '',
    ),
    'field_merci_date_value2' => array(
      'sortable' => 0,
      'separator' => '',
    ),
    'field_merci_date_value' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'item_nid2' => array(
      'separator' => '',
    ),
    'status' => array(
      'sortable' => 0,
      'separator' => '',
    ),
    'edit_node' => array(
      'separator' => '',
    ),
  ),
  'default' => 'field_merci_date_value',
));
$handler->override_option('path', 'admin/merci/manage/reservations/overdue');
$handler->override_option('menu', array(
  'type' => 'tab',
  'title' => 'Overdue',
  'description' => '',
  'weight' => '4',
  'name' => 'navigation',
));
$handler->override_option('tab_options', array(
  'type' => 'normal',
  'title' => 'Upcoming Reservations',
  'description' => '',
  'weight' => '0',
  'name' => 'navigation',
));
$handler = $view->new_display('block', 'Overdue Warning', 'block_1');
$handler->override_option('fields', array(
  'name' => array(
    'label' => 'Name',
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
    'link_to_user' => 1,
    'overwrite_anonymous' => 0,
    'anonymous_text' => '',
    'exclude' => 1,
    'id' => 'name',
    'table' => 'users',
    'field' => 'name',
    'override' => array(
      'button' => 'Use default',
    ),
    'relationship' => 'none',
  ),
  'item_nid2' => array(
    'label' => 'Reserved items',
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
    'type' => 'separator',
    'separator' => ', ',
    'exclude' => 1,
    'id' => 'item_nid2',
    'table' => 'merci_reservation_detail',
    'field' => 'item_nid2',
    'override' => array(
      'button' => 'Use default',
    ),
    'relationship' => 'none',
  ),
  'edit_node' => array(
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
    'text' => 'Edit',
    'exclude' => 1,
    'id' => 'edit_node',
    'table' => 'node',
    'field' => 'edit_node',
    'override' => array(
      'button' => 'Use default',
    ),
    'relationship' => 'none',
  ),
  'field_merci_date_value2' => array(
    'label' => '',
    'alter' => array(
      'alter_text' => 1,
      'text' => '[name] didn\'t return [item_nid2] that were due [field_merci_date_value2] - [edit_node]',
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
    'label_type' => 'none',
    'format' => 'short',
    'multiple' => array(
      'multiple_number' => '',
      'multiple_from' => '',
      'multiple_to' => '',
      'group' => '',
    ),
    'repeat' => array(
      'show_repeat_rule' => '',
    ),
    'fromto' => array(
      'fromto' => 'value2',
    ),
    'exclude' => 0,
    'id' => 'field_merci_date_value2',
    'table' => 'node_data_field_merci_date',
    'field' => 'field_merci_date_value2',
    'override' => array(
      'button' => 'Use default',
    ),
    'relationship' => 'none',
  ),
));
$handler->override_option('css_class', 'message warning');
$handler->override_option('header', '<b>OVERDUE ITEMS:</b><br />');
$handler->override_option('empty', '');
$handler->override_option('items_per_page', 5);
$handler->override_option('use_pager', '0');
$handler->override_option('block_description', '');
$handler->override_option('block_caching', -1);

  
  $views[$view->name] = $view;
  
  $view = new view;
$view->name = 'merci_unconfirmed_reservations';
$view->description = '';
$view->tag = '';
$view->view_php = '';
$view->base_table = 'node';
$view->is_cacheable = FALSE;
$view->api_version = 2;
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->override_option('fields', array(
  'name' => array(
    'label' => 'Name',
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
    'link_to_user' => 1,
    'overwrite_anonymous' => 0,
    'anonymous_text' => '',
    'exclude' => 0,
    'id' => 'name',
    'table' => 'users',
    'field' => 'name',
    'override' => array(
      'button' => 'Override',
    ),
    'relationship' => 'none',
  ),
  'field_merci_date_value2' => array(
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
    'label_type' => 'none',
    'format' => 'default',
    'multiple' => array(
      'multiple_number' => '',
      'multiple_from' => '',
      'multiple_to' => '',
      'group' => '',
    ),
    'repeat' => array(
      'show_repeat_rule' => '',
    ),
    'fromto' => array(
      'fromto' => 'value2',
    ),
    'exclude' => 1,
    'id' => 'field_merci_date_value2',
    'table' => 'node_data_field_merci_date',
    'field' => 'field_merci_date_value2',
    'override' => array(
      'button' => 'Override',
    ),
    'relationship' => 'none',
  ),
  'field_merci_date_value' => array(
    'label' => 'Reservation',
    'alter' => array(
      'alter_text' => 1,
      'text' => '[field_merci_date_value]<br />
[field_merci_date_value2]',
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
    'label_type' => 'widget',
    'format' => 'default',
    'multiple' => array(
      'multiple_number' => '',
      'multiple_from' => '',
      'multiple_to' => '',
      'group' => '',
    ),
    'repeat' => array(
      'show_repeat_rule' => '',
    ),
    'fromto' => array(
      'fromto' => 'value',
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
  'item_nid2' => array(
    'label' => 'Reserved items',
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
    'type' => 'separator',
    'separator' => ', ',
    'exclude' => 0,
    'id' => 'item_nid2',
    'table' => 'merci_reservation_detail',
    'field' => 'item_nid2',
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
    'override' => array(
      'button' => 'Override',
    ),
    'relationship' => 'none',
  ),
  'edit_node' => array(
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
    'text' => '',
    'exclude' => 0,
    'id' => 'edit_node',
    'table' => 'node',
    'field' => 'edit_node',
    'override' => array(
      'button' => 'Override',
    ),
    'relationship' => 'none',
  ),
));
$handler->override_option('filters', array(
  'type' => array(
    'operator' => 'in',
    'value' => array(
      'merci_reservation' => 'merci_reservation',
    ),
    'group' => '0',
    'exposed' => FALSE,
    'expose' => array(
      'operator' => FALSE,
      'label' => '',
    ),
    'id' => 'type',
    'table' => 'node',
    'field' => 'type',
    'override' => array(
      'button' => 'Override',
    ),
    'relationship' => 'none',
  ),
  'date_filter' => array(
    'operator' => '>',
    'value' => array(
      'value' => NULL,
      'min' => NULL,
      'max' => NULL,
      'default_date' => '-1 day',
      'default_to_date' => '',
    ),
    'group' => '0',
    'exposed' => TRUE,
    'expose' => array(
      'use_operator' => 0,
      'operator' => 'date_filter_op',
      'identifier' => 'date_filter',
      'label' => 'Reservation Starts After:',
      'optional' => 1,
      'remember' => 0,
    ),
    'date_fields' => array(
      'node_data_field_merci_date.field_merci_date_value' => 'node_data_field_merci_date.field_merci_date_value',
    ),
    'date_method' => 'OR',
    'granularity' => 'day',
    'form_type' => 'date_popup',
    'default_date' => '-1 day',
    'default_to_date' => '',
    'year_range' => '-3:+3',
    'id' => 'date_filter',
    'table' => 'node',
    'field' => 'date_filter',
    'override' => array(
      'button' => 'Override',
    ),
    'relationship' => 'none',
  ),
  'status' => array(
    'operator' => 'in',
    'value' => array(
      '1' => '1',
    ),
    'group' => '0',
    'exposed' => FALSE,
    'expose' => array(
      'operator' => FALSE,
      'label' => '',
    ),
    'id' => 'status',
    'table' => 'merci_reservation',
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
$handler->override_option('empty', '<div class="messages status">There are currently no unconfirmed reservations.</div>');
$handler->override_option('empty_format', '2');
$handler->override_option('items_per_page', 25);
$handler->override_option('use_pager', '1');
$handler->override_option('style_plugin', 'bulk');
$handler->override_option('style_options', array(
  'grouping' => '',
  'override' => 1,
  'sticky' => 0,
  'order' => 'asc',
  'columns' => array(
    'name' => 'name',
    'field_merci_date_value2' => 'field_merci_date_value2',
    'field_merci_date_value' => 'field_merci_date_value',
    'item_nid2' => 'item_nid2',
    'status' => 'status',
    'edit_node' => 'edit_node',
  ),
  'info' => array(
    'name' => array(
      'sortable' => 0,
      'separator' => '',
    ),
    'field_merci_date_value2' => array(
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
    'edit_node' => array(
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
    'merci_operations_update' => 'merci_operations_update',
    'node_assign_owner_action' => 0,
    'views_bulk_operations_delete_node_action' => 0,
    'node_mass_update:a27b9efabcd054685a549378b174ad11' => 0,
    'system_message_action' => 0,
    'views_bulk_operations_action' => 0,
    'views_bulk_operations_script_action' => 0,
    'node_make_sticky_action' => 0,
    'node_make_unsticky_action' => 0,
    'node_mass_update:c4d3b28efb86fd703619a50b74d43794' => 0,
    'views_bulk_operations_fields_action' => 0,
    'views_bulk_operations_taxonomy_action' => 0,
    'views_bulk_operations_argument_selector_action' => 0,
    'node_promote_action' => 0,
    'node_mass_update:14de7d028b4bffdf2b4a266562ca18ac' => 0,
    'node_mass_update:9c585624b9b3af0b4687d5f97f35e047' => 0,
    'node_publish_action' => 0,
    'system_goto_action' => 0,
    'node_unpromote_action' => 0,
    'node_mass_update:8ce21b08bb8e773d10018b484fe4815e' => 0,
    'node_save_action' => 0,
    'system_send_email_action' => 0,
    'creativecommons_set_license_action' => 0,
    'node_mass_update:0ccad85c1ebe4c9ceada1aa64293b080' => 0,
    'node_unpublish_action' => 0,
    'node_unpublish_by_keyword_action' => 0,
    'auto_nodetitle_operations_update' => 0,
    'pathauto_node_update_alias_multiple:620e193b20ba9caa374fea9ca0ad38f0' => 0,
  ),
  'views_bulk_operations_fields_action' => array(
    'php_code' => 0,
    'display_fields' => array(
      '_all_' => '_all_',
    ),
  ),
));
$handler = $view->new_display('page', 'Unconfirmed', 'page_1');
$handler->override_option('use_ajax', TRUE);
$handler->override_option('path', 'admin/merci/manage/reservations/unconfirmed');
$handler->override_option('menu', array(
  'type' => 'tab',
  'title' => 'Unconfirmed',
  'description' => '',
  'weight' => '2',
  'name' => 'navigation',
));
$handler->override_option('tab_options', array(
  'type' => 'normal',
  'title' => 'Upcoming Reservations',
  'description' => '',
  'weight' => '0',
  'name' => 'navigation',
));

  
  $views[$view->name] = $view;
  $view = new view;
$view->name = 'merci_upcoming_reservations';
$view->description = '';
$view->tag = '';
$view->view_php = '';
$view->base_table = 'node';
$view->is_cacheable = FALSE;
$view->api_version = 2;
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->override_option('fields', array(
  'name' => array(
    'label' => 'Name',
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
    'link_to_user' => 1,
    'overwrite_anonymous' => 0,
    'anonymous_text' => '',
    'exclude' => 0,
    'id' => 'name',
    'table' => 'users',
    'field' => 'name',
    'override' => array(
      'button' => 'Override',
    ),
    'relationship' => 'none',
  ),
  'field_merci_date_value2' => array(
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
    'label_type' => 'none',
    'format' => 'default',
    'multiple' => array(
      'multiple_number' => '',
      'multiple_from' => '',
      'multiple_to' => '',
      'group' => '',
    ),
    'repeat' => array(
      'show_repeat_rule' => '',
    ),
    'fromto' => array(
      'fromto' => 'value2',
    ),
    'exclude' => 1,
    'id' => 'field_merci_date_value2',
    'table' => 'node_data_field_merci_date',
    'field' => 'field_merci_date_value2',
    'override' => array(
      'button' => 'Override',
    ),
    'relationship' => 'none',
  ),
  'field_merci_date_value' => array(
    'label' => 'Reservation',
    'alter' => array(
      'alter_text' => 1,
      'text' => '[field_merci_date_value]<br />
[field_merci_date_value2]',
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
    'label_type' => 'widget',
    'format' => 'default',
    'multiple' => array(
      'multiple_number' => '',
      'multiple_from' => '',
      'multiple_to' => '',
      'group' => '',
    ),
    'repeat' => array(
      'show_repeat_rule' => '',
    ),
    'fromto' => array(
      'fromto' => 'value',
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
  'item_nid2' => array(
    'label' => 'Reserved items',
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
    'type' => 'separator',
    'separator' => ', ',
    'exclude' => 0,
    'id' => 'item_nid2',
    'table' => 'merci_reservation_detail',
    'field' => 'item_nid2',
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
    'override' => array(
      'button' => 'Override',
    ),
    'relationship' => 'none',
  ),
  'edit_node' => array(
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
    'text' => '',
    'exclude' => 0,
    'id' => 'edit_node',
    'table' => 'node',
    'field' => 'edit_node',
    'override' => array(
      'button' => 'Override',
    ),
    'relationship' => 'none',
  ),
));
$handler->override_option('sorts', array(
  'field_merci_date_value' => array(
    'order' => 'ASC',
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
$handler->override_option('filters', array(
  'type' => array(
    'operator' => 'in',
    'value' => array(
      'merci_reservation' => 'merci_reservation',
    ),
    'group' => '0',
    'exposed' => FALSE,
    'expose' => array(
      'operator' => FALSE,
      'label' => '',
    ),
    'id' => 'type',
    'table' => 'node',
    'field' => 'type',
    'override' => array(
      'button' => 'Override',
    ),
    'relationship' => 'none',
  ),
  'date_filter' => array(
    'operator' => '>',
    'value' => array(
      'value' => NULL,
      'min' => NULL,
      'max' => NULL,
      'default_date' => '-1 day',
      'default_to_date' => '',
    ),
    'group' => '0',
    'exposed' => TRUE,
    'expose' => array(
      'use_operator' => 0,
      'operator' => 'date_filter_op',
      'identifier' => 'date_filter',
      'label' => 'Reservation Starts After:',
      'optional' => 1,
      'remember' => 0,
    ),
    'date_fields' => array(
      'node_data_field_merci_date.field_merci_date_value' => 'node_data_field_merci_date.field_merci_date_value',
    ),
    'date_method' => 'OR',
    'granularity' => 'day',
    'form_type' => 'date_popup',
    'default_date' => '-1 day',
    'default_to_date' => '',
    'year_range' => '-3:+3',
    'id' => 'date_filter',
    'table' => 'node',
    'field' => 'date_filter',
    'override' => array(
      'button' => 'Override',
    ),
    'relationship' => 'none',
  ),
));
$handler->override_option('access', array(
  'type' => 'none',
));
$handler->override_option('cache', array(
  'type' => 'none',
));
$handler->override_option('header_format', '2');
$handler->override_option('header_empty', 0);
$handler->override_option('footer_format', '3');
$handler->override_option('footer_empty', 0);
$handler->override_option('empty', '<div class="messages status">There are no Upcoming Reservations</div>');
$handler->override_option('empty_format', '2');
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
    'name' => 'name',
    'field_merci_date_value' => 'field_merci_date_value',
    'item_nid2' => 'item_nid2',
    'status' => 'status',
    'edit_node' => 'edit_node',
  ),
  'info' => array(
    'title' => array(
      'sortable' => 0,
      'separator' => '',
    ),
    'name' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'field_merci_date_value' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'item_nid2' => array(
      'separator' => '',
    ),
    'status' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'edit_node' => array(
      'separator' => '',
    ),
  ),
  'default' => 'field_merci_date_value',
));
$handler = $view->new_display('page', 'Upcoming', 'page_1');
$handler->override_option('use_ajax', TRUE);
$handler->override_option('path', 'admin/merci/manage/reservations/upcoming');
$handler->override_option('menu', array(
  'type' => 'default tab',
  'title' => 'Upcoming',
  'description' => '',
  'weight' => '0',
  'name' => 'navigation',
));
$handler->override_option('tab_options', array(
  'type' => 'normal',
  'title' => 'Reservations',
  'description' => '',
  'weight' => '0',
  'name' => 'navigation',
));
$handler = $view->new_display('block', 'Upcoming Block', 'block_1');
$handler->override_option('fields', array(
  'name' => array(
    'label' => 'Name',
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
    'link_to_user' => 1,
    'overwrite_anonymous' => 0,
    'anonymous_text' => '',
    'exclude' => 1,
    'id' => 'name',
    'table' => 'users',
    'field' => 'name',
    'override' => array(
      'button' => 'Use default',
    ),
    'relationship' => 'none',
  ),
  'item_nid2' => array(
    'label' => 'Reserved items',
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
    'type' => 'separator',
    'separator' => ', ',
    'exclude' => 1,
    'id' => 'item_nid2',
    'table' => 'merci_reservation_detail',
    'field' => 'item_nid2',
    'override' => array(
      'button' => 'Use default',
    ),
    'relationship' => 'none',
  ),
  'field_merci_date_value2' => array(
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
    'label_type' => 'none',
    'format' => 'default',
    'multiple' => array(
      'multiple_number' => '',
      'multiple_from' => '',
      'multiple_to' => '',
      'group' => '',
    ),
    'repeat' => array(
      'show_repeat_rule' => '',
    ),
    'fromto' => array(
      'fromto' => 'value2',
    ),
    'exclude' => 1,
    'id' => 'field_merci_date_value2',
    'table' => 'node_data_field_merci_date',
    'field' => 'field_merci_date_value2',
    'override' => array(
      'button' => 'Override',
    ),
    'relationship' => 'none',
  ),
  'field_merci_date_value' => array(
    'label' => '',
    'alter' => array(
      'alter_text' => 1,
      'text' => '[name] reserved [item_nid2] starting at [field_merci_date_value]
',
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
    'label_type' => 'none',
    'format' => 'short',
    'multiple' => array(
      'multiple_number' => '',
      'multiple_from' => '',
      'multiple_to' => '',
      'group' => '',
    ),
    'repeat' => array(
      'show_repeat_rule' => '',
    ),
    'fromto' => array(
      'fromto' => 'value',
    ),
    'exclude' => 0,
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
  'type' => array(
    'operator' => 'in',
    'value' => array(
      'merci_reservation' => 'merci_reservation',
    ),
    'group' => '0',
    'exposed' => FALSE,
    'expose' => array(
      'operator' => FALSE,
      'label' => '',
    ),
    'id' => 'type',
    'table' => 'node',
    'field' => 'type',
    'override' => array(
      'button' => 'Override',
    ),
    'relationship' => 'none',
  ),
  'date_filter' => array(
    'operator' => '>',
    'value' => array(
      'value' => NULL,
      'min' => NULL,
      'max' => NULL,
      'default_date' => '-1 day',
      'default_to_date' => '',
    ),
    'group' => '0',
    'exposed' => FALSE,
    'expose' => array(
      'operator' => 'date_filter_op',
      'label' => 'Reservation Starts After:',
      'use_operator' => 0,
      'identifier' => 'date_filter',
      'optional' => 1,
      'remember' => 0,
    ),
    'date_fields' => array(
      'node_data_field_merci_date.field_merci_date_value' => 'node_data_field_merci_date.field_merci_date_value',
    ),
    'date_method' => 'OR',
    'granularity' => 'day',
    'form_type' => 'date_popup',
    'default_date' => '-1 day',
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
  'status' => array(
    'operator' => 'in',
    'value' => array(
      '1' => '1',
      '2' => '2',
    ),
    'group' => '0',
    'exposed' => FALSE,
    'expose' => array(
      'operator' => FALSE,
      'label' => '',
    ),
    'id' => 'status',
    'table' => 'merci_reservation',
    'field' => 'status',
    'override' => array(
      'button' => 'Use default',
    ),
    'relationship' => 'none',
  ),
));
$handler->override_option('items_per_page', 5);
$handler->override_option('use_pager', '0');
$handler->override_option('style_plugin', 'list');
$handler->override_option('style_options', array());
$handler->override_option('block_description', '');
$handler->override_option('block_caching', -1);
$handler = $view->new_display('block', 'No Show Warning', 'block_2');
$handler->override_option('fields', array(
  'name' => array(
    'label' => 'Name',
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
    'link_to_user' => 1,
    'overwrite_anonymous' => 0,
    'anonymous_text' => '',
    'exclude' => 1,
    'id' => 'name',
    'table' => 'users',
    'field' => 'name',
    'override' => array(
      'button' => 'Use default',
    ),
    'relationship' => 'none',
  ),
  'item_nid2' => array(
    'label' => 'Reserved items',
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
    'type' => 'separator',
    'separator' => ', ',
    'exclude' => 1,
    'id' => 'item_nid2',
    'table' => 'merci_reservation_detail',
    'field' => 'item_nid2',
    'override' => array(
      'button' => 'Use default',
    ),
    'relationship' => 'none',
  ),
  'edit_node' => array(
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
    'text' => 'Edit',
    'exclude' => 1,
    'id' => 'edit_node',
    'table' => 'node',
    'field' => 'edit_node',
    'override' => array(
      'button' => 'Use default',
    ),
    'relationship' => 'none',
  ),
  'field_merci_date_value2' => array(
    'label' => '',
    'alter' => array(
      'alter_text' => 1,
      'text' => '[name] never picked up the [item_nid2] reserved on [field_merci_date_value2] - [edit_node] ',
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
    'label_type' => 'none',
    'format' => 'default',
    'multiple' => array(
      'multiple_number' => '',
      'multiple_from' => '',
      'multiple_to' => '',
      'group' => '',
    ),
    'repeat' => array(
      'show_repeat_rule' => '',
    ),
    'fromto' => array(
      'fromto' => 'value2',
    ),
    'exclude' => 0,
    'id' => 'field_merci_date_value2',
    'table' => 'node_data_field_merci_date',
    'field' => 'field_merci_date_value2',
    'override' => array(
      'button' => 'Use default',
    ),
    'relationship' => 'none',
  ),
  'field_merci_date_value' => array(
    'label' => 'Reservation',
    'alter' => array(
      'alter_text' => 0,
      'text' => '[field_merci_date_value]<br />
[field_merci_date_value2]',
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
    'label_type' => 'widget',
    'format' => 'default',
    'multiple' => array(
      'multiple_number' => '',
      'multiple_from' => '',
      'multiple_to' => '',
      'group' => '',
    ),
    'repeat' => array(
      'show_repeat_rule' => '',
    ),
    'fromto' => array(
      'fromto' => 'value',
    ),
    'exclude' => 1,
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
  'type' => array(
    'operator' => 'in',
    'value' => array(
      'merci_reservation' => 'merci_reservation',
    ),
    'group' => '0',
    'exposed' => FALSE,
    'expose' => array(
      'operator' => FALSE,
      'label' => '',
    ),
    'id' => 'type',
    'table' => 'node',
    'field' => 'type',
    'override' => array(
      'button' => 'Override',
    ),
    'relationship' => 'none',
  ),
  'date_filter' => array(
    'operator' => '<',
    'value' => array(
      'min' => NULL,
      'max' => NULL,
      'value' => NULL,
      'default_date' => 'now',
      'default_to_date' => '',
    ),
    'group' => '0',
    'exposed' => FALSE,
    'expose' => array(
      'operator' => 'date_filter_op',
      'label' => 'Reservation Starts After:',
      'use_operator' => 0,
      'identifier' => 'date_filter',
      'optional' => 1,
      'remember' => 0,
    ),
    'date_fields' => array(
      'node_data_field_merci_date.field_merci_date_value' => 'node_data_field_merci_date.field_merci_date_value',
    ),
    'date_method' => 'OR',
    'granularity' => 'minute',
    'form_type' => 'date_popup',
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
  'status' => array(
    'operator' => 'in',
    'value' => array(
      '1' => '1',
      '2' => '2',
    ),
    'group' => '0',
    'exposed' => FALSE,
    'expose' => array(
      'operator' => FALSE,
      'label' => '',
    ),
    'id' => 'status',
    'table' => 'merci_reservation',
    'field' => 'status',
    'override' => array(
      'button' => 'Use default',
    ),
    'relationship' => 'none',
  ),
));
$handler->override_option('css_class', 'message warning');
$handler->override_option('header', '<b>NO SHOWS:</b><br />');
$handler->override_option('empty', '');
$handler->override_option('items_per_page', 5);
$handler->override_option('use_pager', '0');
$handler->override_option('style_plugin', 'list');
$handler->override_option('style_options', array(
  'grouping' => '',
  'type' => 'ul',
));
$handler->override_option('block_description', '');
$handler->override_option('block_caching', -1);

  $views[$view->name] = $view;

  return $views;
}

