<?php
/**
* Implementation of hook_views_default_views().
*/
function merci_views_default_views() {
  /*
  * View 'merci_inventory'
  */
  $view = new view;
  $view->name = 'merci_inventory';
  $view->description = '';
  $view->tag = '';
  $view->view_php = '';
  $view->base_table = 'node';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
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
      'type' => array(
        'operator' => 'in',
        'value' => array(
          'boom_pole' => 'boom_pole',
          'canon_video_light' => 'canon_video_light',
          'canon_xl1_dv_camera' => 'canon_xl1_dv_camera',
          'dv_palmcorder' => 'dv_palmcorder',
          'editing_computer_equipment' => 'editing_computer_equipment',
          'external_hard_drives' => 'external_hard_drives',
          'facilities' => 'facilities',
          'field_monitor' => 'field_monitor',
          'handheld_microphone' => 'handheld_microphone',
          'hdv_deck_portable' => 'hdv_deck_portable',
          'hdv_palmcorder' => 'hdv_palmcorder',
          'headphone' => 'headphone',
          'large_tripod' => 'large_tripod',
          'lavalier_microphone' => 'lavalier_microphone',
          'light_kit' => 'light_kit',
          'monopod' => 'monopod',
          'outdoor_reflector' => 'outdoor_reflector',
          'panasonic_dvx100a_dv_camera' => 'panasonic_dvx100a_dv_camera',
          'projector_large_venue' => 'projector_large_venue',
          'projector_medium_venue' => 'projector_medium_venue',
          'shotgun_microphone' => 'shotgun_microphone',
          'small_tripod' => 'small_tripod',
          'sony_lseries_video_light' => 'sony_lseries_video_light',
          'sony_pd170_dvcam' => 'sony_pd170_dvcam',
          'sony_z1u_hdv_camera' => 'sony_z1u_hdv_camera',
          'studios' => 'studios',
          'wide_angle_adapter_sony_z1u' => 'wide_angle_adapter_sony_z1u',
          'wireless_lavalier' => 'wireless_lavalier',
          'xlr_adapter' => 'xlr_adapter',
        ),
        'group' => '0',
        'exposed' => TRUE,
        'expose' => array(
          'use_operator' => 0,
          'operator' => 'type_op',
          'identifier' => 'type',
          'label' => 'Type',
          'optional' => 1,
          'single' => 0,
          'remember' => 0,
          'reduce' => 1,
        ),
        'id' => 'type',
        'table' => 'node',
        'field' => 'type',
        'override' => array(
          'button' => 'Override',
        ),
        'relationship' => 'none',
      ),
  ));
  $handler->override_option('access', array(
      'type' => 'role',
      'role' => array(
        '13' => 13,
        '3' => 3,
      ),
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
  $handler->override_option('path', 'admin/merci/inventory');
  $handler->override_option('menu', array(
      'type' => 'normal',
      'title' => 'Inventory',
      'description' => '',
      'weight' => '8',
      'name' => 'navigation',
  ));
  $handler->override_option('tab_options', array(
      'type' => 'none',
      'title' => '',
      'description' => '',
      'weight' => 0,
  ));
  $views[$view->name] = $view;

  /*
  * View 'merci_reservations'
  */
  $view = new view;
  $view->name = 'merci_reservations';
  $view->description = '';
  $view->tag = '';
  $view->view_php = '';
  $view->base_table = 'node';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
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
        'link_to_node' => 0,
        'label_type' => 'none',
        'format' => 'default',
        'multiple' => array(
          'group' => TRUE,
          'multiple_number' => '',
          'multiple_from' => '',
          'multiple_reversed' => FALSE,
        ),
        'exclude' => 0,
        'id' => 'field_merci_date_value',
        'table' => 'node_data_field_merci_date',
        'field' => 'field_merci_date_value',
        'relationship' => 'none',
      ),
      'status' => array(
        'label' => 'Status',
        'exclude' => 0,
        'id' => 'status',
        'table' => 'merci_reservation',
        'field' => 'status',
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
          'button' => 'Override',
        ),
        'relationship' => 'none',
      ),
      'date_filter' => array(
        'operator' => '=',
        'value' => array(
          'value' => NULL,
          'min' => NULL,
          'max' => NULL,
          'default_date' => 'today',
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
        'granularity' => 'day',
        'form_type' => 'date_select',
        'default_date' => 'today',
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
      'type' => 'role',
      'role' => array(
        '13' => 13,
        '15' => 15,
        '3' => 3,
      ),
  ));
  $handler->override_option('cache', array(
      'type' => 'none',
  ));
  $handler->override_option('title', 'Search Reservation by User');
  $handler->override_option('footer_format', '2');
  $handler->override_option('footer_empty', 0);
  $handler->override_option('use_ajax', TRUE);
  $handler->override_option('items_per_page', 0);
  $handler->override_option('use_pager', '1');
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
          'sortable' => 1,
          'separator' => '',
        ),
        'status' => array(
          'sortable' => 1,
          'separator' => '',
        ),
      ),
      'default' => 'field_merci_date_value',
  ));
  $handler = $view->new_display('page', 'Upcoming Reservations', 'page_1');
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
        'operator' => '>=',
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
          'label' => 'Date: Date',
          'optional' => 1,
          'remember' => 0,
        ),
        'date_fields' => array(
          'node_data_field_merci_date.field_merci_date_value' => 'node_data_field_merci_date.field_merci_date_value',
        ),
        'granularity' => 'day',
        'form_type' => 'date_select',
        'default_date' => 'now -1 day',
        'default_to_date' => '',
        'id' => 'date_filter',
        'table' => 'node',
        'field' => 'date_filter',
        'override' => array(
          'button' => 'Use default',
        ),
        'relationship' => 'none',
      ),
  ));
  $handler->override_option('access', array(
      'type' => 'role',
      'role' => array(
        '6' => 6,
        '13' => 13,
        '3' => 3,
      ),
  ));
  $handler->override_option('title', 'Upcoming Reservations');
  $handler->override_option('header', '<a href="/admin/merci/reservation_calendar/2009-03">Month View</a><br />
    <a href="/admin/merci/reservation_calendar/2009-W7">Week View</a><br />
    <a href="/admin/merci/reservations/byuser">By User</a>
    ');
  $handler->override_option('header_format', '3');
  $handler->override_option('header_empty', 1);
  $handler->override_option('path', 'admin/merci/reservations/upcoming');
  $handler->override_option('menu', array(
      'type' => 'none',
      'title' => 'MERCI Reservations',
      'description' => '',
      'weight' => '40',
      'name' => 'navigation',
  ));
  $handler->override_option('tab_options', array(
      'type' => 'none',
      'title' => '',
      'description' => '',
      'weight' => 0,
  ));
  $handler = $view->new_display('page', 'My Reservations', 'page_2');
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
        'operator' => '>=',
        'value' => array(
          'value' => NULL,
          'min' => NULL,
          'max' => NULL,
          'default_date' => 'now -3 day',
          'default_to_date' => '',
        ),
        'group' => '0',
        'exposed' => TRUE,
        'expose' => array(
          'use_operator' => 0,
          'operator' => 'date_filter_op',
          'identifier' => 'date_filter',
          'label' => 'Adjust date to view reservations created after that date',
          'optional' => 1,
          'remember' => 0,
        ),
        'date_fields' => array(
          'node_data_field_merci_date.field_merci_date_value' => 'node_data_field_merci_date.field_merci_date_value',
        ),
        'granularity' => 'day',
        'form_type' => 'date_select',
        'default_date' => 'now -3 day',
        'default_to_date' => '',
        'id' => 'date_filter',
        'table' => 'node',
        'field' => 'date_filter',
        'relationship' => 'none',
        'override' => array(
          'button' => 'Use default',
        ),
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
    $handler->override_option('path', 'merci/reservations');
    $handler->override_option('menu', array(
        'type' => 'normal',
        'title' => 'My Reservations',
        'description' => '',
        'weight' => '30',
        'name' => 'menu-member-tools',
    ));
    $handler->override_option('tab_options', array(
        'type' => 'none',
        'title' => '',
        'description' => '',
        'weight' => 0,
    ));
    $handler = $view->new_display('page', 'Reservations by User', 'page_3');
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
        'uid' => array(
          'operator' => 'in',
          'value' => '',
          'group' => '0',
          'exposed' => TRUE,
          'expose' => array(
            'use_operator' => 0,
            'operator' => 'uid_op',
            'identifier' => 'uid',
            'label' => 'Username',
            'optional' => 1,
            'remember' => 0,
            'reduce' => 0,
          ),
          'id' => 'uid',
          'table' => 'users',
          'field' => 'uid',
          'override' => array(
            'button' => 'Use default',
          ),
          'relationship' => 'none',
        ),
    ));
    $handler->override_option('title', 'Search Reservations by User');
    $handler->override_option('path', 'admin/merci/reservations/byuser');
    $handler->override_option('menu', array(
        'type' => 'tab',
        'title' => 'By User',
        'description' => '',
        'weight' => '0',
        'name' => 'navigation',
    ));
    $handler->override_option('tab_options', array(
        'type' => 'none',
        'title' => '',
        'description' => '',
        'weight' => 0,
    ));
    $handler = $view->new_display('page', 'out', 'page_4');
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
            'button' => 'Use default',
          ),
          'relationship' => 'none',
        ),
        'date_filter' => array(
          'operator' => '<=',
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
            'operator' => FALSE,
            'label' => '',
          ),
          'date_fields' => array(
            'node_data_field_merci_date.field_merci_date_value' => 'node_data_field_merci_date.field_merci_date_value',
          ),
          'date_method' => 'OR',
          'granularity' => 'day',
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
    $handler->override_option('title', 'Checked Out Reservations');
    $handler->override_option('empty', 'There are no items out at this time.');
    $handler->override_option('empty_format', '1');
    $handler->override_option('path', 'admin/merci/reservations/out');
    $handler->override_option('menu', array(
        'type' => 'none',
        'title' => '',
        'description' => '',
        'weight' => 0,
        'name' => 'navigation',
    ));
    $handler->override_option('tab_options', array(
        'type' => 'none',
        'title' => '',
        'description' => '',
        'weight' => 0,
    ));
    $handler = $view->new_display('page', 'late', 'page_5');
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
            'button' => 'Use default',
          ),
          'relationship' => 'none',
        ),
        'date_filter' => array(
          'operator' => '<',
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
          'granularity' => 'day',
          'form_type' => 'date_select',
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
    ));
    $handler->override_option('title', 'Overdue Reservations');
    $handler->override_option('empty', 'There are currently no overdue reservations.');
    $handler->override_option('empty_format', '1');
    $handler->override_option('path', 'admin/merci/reservations/late');
    $handler->override_option('menu', array(
        'type' => 'none',
        'title' => '',
        'description' => '',
        'weight' => 0,
        'name' => 'navigation',
    ));
    $handler->override_option('tab_options', array(
        'type' => 'none',
        'title' => '',
        'description' => '',
        'weight' => 0,
    ));
    $handler = $view->new_display('page', 'noshow', 'page_6');
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
        'date_filter' => array(
          'operator' => '<=',
          'value' => array(
            'value' => NULL,
            'min' => NULL,
            'max' => NULL,
            'default_date' => 'today',
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
          'granularity' => 'day',
          'form_type' => 'date_select',
          'default_date' => 'today',
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
    $handler->override_option('title', 'Unclaimed Reservations');
    $handler->override_option('path', 'admin/merci/reservations/noshow');
    $handler->override_option('menu', array(
        'type' => 'none',
        'title' => '',
        'description' => '',
        'weight' => 0,
        'name' => 'navigation',
    ));
    $handler->override_option('tab_options', array(
        'type' => 'none',
        'title' => '',
        'description' => '',
        'weight' => 0,
    ));
    $handler = $view->new_display('page', 'today', 'page_7');
    $handler->override_option('title', 'Today\'s Reservations');
    $handler->override_option('header', '<a href="admin/merci/reservations/upcoming">Upcoming Reservations</a> <a href="admin/merci/reservations/byuser">By User</a>');
    $handler->override_option('header_format', '1');
    $handler->override_option('header_empty', 0);
    $handler->override_option('footer', '<br>
      <h1>Currently Out</h1>
      [view:merci_reservations=page_4]
      <br>
      <h1>No call, No show</h1>
      [view:merci_reservations=page_6]');
    $handler->override_option('path', 'admin/merci/reservations/today');
    $handler->override_option('menu', array(
        'type' => 'normal',
        'title' => 'MERCI Reservations',
        'description' => '',
        'weight' => '0',
        'name' => 'navigation',
    ));
    $handler->override_option('tab_options', array(
        'type' => 'none',
        'title' => '',
        'description' => '',
        'weight' => 0,
    ));
  $views[$view->name] = $view;

  return $views;
}
