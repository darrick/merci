<?php
// $Id$


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

  return $views;
}

