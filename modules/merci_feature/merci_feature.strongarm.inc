<?php
/**
 * @file
 * merci_feature.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function merci_feature_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_reservable_item';
  $strongarm->value = 0;
  $export['comment_anonymous_reservable_item'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_reservable_kit';
  $strongarm->value = 0;
  $export['comment_anonymous_reservable_kit'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_reservation';
  $strongarm->value = 0;
  $export['comment_anonymous_reservation'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_reservable_item';
  $strongarm->value = 1;
  $export['comment_default_mode_reservable_item'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_reservable_kit';
  $strongarm->value = 1;
  $export['comment_default_mode_reservable_kit'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_reservation';
  $strongarm->value = 1;
  $export['comment_default_mode_reservation'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_reservable_item';
  $strongarm->value = '50';
  $export['comment_default_per_page_reservable_item'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_reservable_kit';
  $strongarm->value = '50';
  $export['comment_default_per_page_reservable_kit'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_reservation';
  $strongarm->value = '50';
  $export['comment_default_per_page_reservation'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_reservable_item';
  $strongarm->value = 1;
  $export['comment_form_location_reservable_item'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_reservable_kit';
  $strongarm->value = 1;
  $export['comment_form_location_reservable_kit'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_reservation';
  $strongarm->value = 1;
  $export['comment_form_location_reservation'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_reservable_item';
  $strongarm->value = '1';
  $export['comment_preview_reservable_item'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_reservable_kit';
  $strongarm->value = '1';
  $export['comment_preview_reservable_kit'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_reservation';
  $strongarm->value = '1';
  $export['comment_preview_reservation'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_reservable_item';
  $strongarm->value = '2';
  $export['comment_reservable_item'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_reservable_kit';
  $strongarm->value = '2';
  $export['comment_reservable_kit'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_reservation';
  $strongarm->value = '2';
  $export['comment_reservation'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_reservable_item';
  $strongarm->value = 1;
  $export['comment_subject_field_reservable_item'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_reservable_kit';
  $strongarm->value = 1;
  $export['comment_subject_field_reservable_kit'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_reservation';
  $strongarm->value = 1;
  $export['comment_subject_field_reservation'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__reservable_item';
  $strongarm->value = array(
    'view_modes' => array(),
    'extra_fields' => array(
      'form' => array(
        'title' => array(
          'weight' => '-5',
        ),
      ),
      'display' => array(),
    ),
  );
  $export['field_bundle_settings_node__reservable_item'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__reservable_kit';
  $strongarm->value = array(
    'view_modes' => array(),
    'extra_fields' => array(
      'form' => array(
        'title' => array(
          'weight' => '-5',
        ),
      ),
      'display' => array(),
    ),
  );
  $export['field_bundle_settings_node__reservable_kit'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__reservation';
  $strongarm->value = array(
    'view_modes' => array(),
    'extra_fields' => array(
      'form' => array(
        'title' => array(
          'weight' => '-5',
        ),
      ),
      'display' => array(),
    ),
  );
  $export['field_bundle_settings_node__reservation'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_reservable_item';
  $strongarm->value = array(
    0 => 'main-menu',
  );
  $export['menu_options_reservable_item'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_reservable_kit';
  $strongarm->value = array(
    0 => 'main-menu',
  );
  $export['menu_options_reservable_kit'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_reservation';
  $strongarm->value = array(
    0 => 'main-menu',
  );
  $export['menu_options_reservation'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_reservable_item';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_reservable_item'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_reservable_kit';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_reservable_kit'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_reservation';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_reservation'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_reservable_item';
  $strongarm->value = array(
    0 => 'status',
    1 => 'promote',
  );
  $export['node_options_reservable_item'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_reservable_kit';
  $strongarm->value = array(
    0 => 'status',
    1 => 'promote',
  );
  $export['node_options_reservable_kit'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_reservation';
  $strongarm->value = array(
    0 => 'status',
    1 => 'promote',
  );
  $export['node_options_reservation'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_reservable_item';
  $strongarm->value = '1';
  $export['node_preview_reservable_item'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_reservable_kit';
  $strongarm->value = '1';
  $export['node_preview_reservable_kit'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_reservation';
  $strongarm->value = '1';
  $export['node_preview_reservation'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_reservable_item';
  $strongarm->value = 1;
  $export['node_submitted_reservable_item'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_reservable_kit';
  $strongarm->value = 1;
  $export['node_submitted_reservable_kit'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_reservation';
  $strongarm->value = 1;
  $export['node_submitted_reservation'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'rules_forms_event_info';
  $strongarm->value = array(
    'reservation_node_form' => array(
      'label' => 'Reservation node form',
      'elements' => array(
        'textfield:title' => array(
          'type' => 'textfield',
          'label' => 'Title',
        ),
        'fieldset:revision_information' => array(
          'type' => 'fieldset',
          'label' => 'Revision information',
        ),
        'checkbox:revision_information:revision' => array(
          'type' => 'checkbox',
          'label' => 'Create new revision',
        ),
        'textarea:revision_information:log' => array(
          'type' => 'textarea',
          'label' => 'Revision log message',
        ),
        'fieldset:author' => array(
          'type' => 'fieldset',
          'label' => 'Authoring information',
        ),
        'textfield:author:name' => array(
          'type' => 'textfield',
          'label' => 'Authored by',
        ),
        'textfield:author:date' => array(
          'type' => 'textfield',
          'label' => 'Authored on',
        ),
        'fieldset:options' => array(
          'type' => 'fieldset',
          'label' => 'Publishing options',
        ),
        'checkbox:options:status' => array(
          'type' => 'checkbox',
          'label' => 'Published',
        ),
        'checkbox:options:promote' => array(
          'type' => 'checkbox',
          'label' => 'Promoted to front page',
        ),
        'checkbox:options:sticky' => array(
          'type' => 'checkbox',
          'label' => 'Sticky at top of lists',
        ),
        'submit:actions:submit' => array(
          'type' => 'submit',
          'label' => 'Save',
        ),
        'submit:actions:preview' => array(
          'type' => 'submit',
          'label' => 'Preview',
        ),
        'date_combo:field_reservation_date:und:0' => array(
          'type' => 'date_combo',
          'label' => 'Reservation Date',
        ),
        'checkbox:field_reservation_date:und:0:show_todate' => array(
          'type' => 'checkbox',
          'label' => 'Show End Date',
        ),
        'date_popup:field_reservation_date:und:0:value' => array(
          'type' => 'date_popup',
          'label' => '',
        ),
        'textfield:field_reservation_date:und:0:value:date' => array(
          'type' => 'textfield',
          'label' => 'Date',
        ),
        'textfield:field_reservation_date:und:0:value:time' => array(
          'type' => 'textfield',
          'label' => 'Time',
        ),
        'date_popup:field_reservation_date:und:0:value2' => array(
          'type' => 'date_popup',
          'label' => 'To:',
        ),
        'textfield:field_reservation_date:und:0:value2:date' => array(
          'type' => 'textfield',
          'label' => 'Date',
        ),
        'textfield:field_reservation_date:und:0:value2:time' => array(
          'type' => 'textfield',
          'label' => 'Time',
        ),
        'select:field_reservation_items:und:0:target_id' => array(
          'type' => 'select',
          'label' => '',
        ),
        'select:field_reservation_items:und:0:_weight' => array(
          'type' => 'select',
          'label' => 'Weight for row 1',
        ),
        'submit:field_reservation_items:und:add_more' => array(
          'type' => 'submit',
          'label' => 'Add another item',
        ),
        'fieldset:comment_settings' => array(
          'type' => 'fieldset',
          'label' => 'Comment settings',
        ),
        'radios:comment_settings:comment' => array(
          'type' => 'radios',
          'label' => 'Comments',
        ),
        'radio:comment_settings:comment:2' => array(
          'type' => 'radio',
          'label' => 'Open',
        ),
        'radio:comment_settings:comment:1' => array(
          'type' => 'radio',
          'label' => 'Closed',
        ),
        'radio:comment_settings:comment:0' => array(
          'type' => 'radio',
          'label' => 'Hidden',
        ),
        'fieldset:menu' => array(
          'type' => 'fieldset',
          'label' => 'Menu settings',
        ),
        'checkbox:menu:enabled' => array(
          'type' => 'checkbox',
          'label' => 'Provide a menu link',
        ),
        'textfield:menu:link:link_title' => array(
          'type' => 'textfield',
          'label' => 'Menu link title',
        ),
        'textarea:menu:link:description' => array(
          'type' => 'textarea',
          'label' => 'Description',
        ),
        'select:menu:link:parent' => array(
          'type' => 'select',
          'label' => 'Parent item',
        ),
        'select:menu:link:weight' => array(
          'type' => 'select',
          'label' => 'Weight',
        ),
        'fieldset:path' => array(
          'type' => 'fieldset',
          'label' => 'URL path settings',
        ),
        'textfield:path:alias' => array(
          'type' => 'textfield',
          'label' => 'URL alias',
        ),
      ),
      'path' => 'node/add/reservation',
      'buttons' => FALSE,
      'validate' => array(),
      'submit' => array(),
    ),
  );
  $export['rules_forms_event_info'] = $strongarm;

  return $export;
}
