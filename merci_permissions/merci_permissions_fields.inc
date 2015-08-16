<?php

function merci_permissions_fields() {
  $field_bases = array();

  // Exported field_base: MERCI_ALLOWED_ROLES
  $field_bases[MERCI_ALLOWED_ROLES] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => MERCI_ALLOWED_ROLES,
    'foreign keys' => array(),
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        'faculty' => 'Faculty',
        'student' => 'Student',
        'administrator' => 'Administrator',
        'homeroom' => 'Homeroom',
        'manager' => 'Manager',
        'operator' => 'Operator',
        'operator_lead' => 'Operator Lead',
        'organization' => 'Organization',
        'suspended' => 'Suspended',
      ),
      'allowed_values_function' => 'merci_permissions_allowed_values',
    ),
    'translatable' => 0,
    'type' => 'list_text',
  );

  // Exported field_base: 'field_override_permissions'
  $field_bases[MERCI_OVERRIDE_PERMISSIONS] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => MERCI_OVERRIDE_PERMISSIONS,
    'foreign keys' => array(
      'users' => array(
        'columns' => array(
          'target_id' => 'uid',
        ),
        'table' => 'users',
      ),
    ),
    'indexes' => array(
      'target_id' => array(
        0 => 'target_id',
      ),
    ),
    'locked' => 0,
    'module' => 'entityreference',
    'settings' => array(
      'handler' => 'base',
      'handler_settings' => array(
        'behaviors' => array(
          'views-select-list' => array(
            'status' => 0,
          ),
        ),
        'sort' => array(
          'type' => 'none',
        ),
        'target_bundles' => array(),
      ),
      'target_type' => 'user',
    ),
    'translatable' => 0,
    'type' => 'entityreference',
  );

  return $field_bases;
}

function merci_permissions_instances() {
  $field_instances = array();

  // Exported field_instance: MERCI_ALLOWED_ROLES
  $field_instances[MERCI_ALLOWED_ROLES] = array(
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'administrator' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 2,
      ),
      'default' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 2,
      ),
    ),
    'label' => 'Allowed Roles',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_buttons',
      'weight' => 6,
    ),
  );

  // Exported field_instance: 'merci_line_item-merci_product-field_override_permissions'
  $field_instances[MERCI_OVERRIDE_PERMISSIONS] = array(
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'add_form' => array(
        'label' => 'above',
        'module' => 'entityreference',
        'settings' => array(
          'grid_field_formatter' => array(
            'columns' => 1,
            'grid_enable' => FALSE,
          ),
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 4,
      ),
      'default' => array(
        'label' => 'above',
        'module' => 'entityreference',
        'settings' => array(
          'grid_field_formatter' => array(
            'columns' => 1,
            'grid_enable' => FALSE,
          ),
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 10,
      ),
      'list' => array(
        'label' => 'above',
        'module' => 'entityreference',
        'settings' => array(
          'grid_field_formatter' => array(
            'columns' => 1,
            'grid_enable' => FALSE,
          ),
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 5,
      ),
    ),
    'label' => 'Override permissions',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'entityreference',
      'settings' => array(
        'match_operator' => 'CONTAINS',
        'path' => '',
        'size' => 60,
      ),
      'type' => 'entityreference_autocomplete',
      'weight' => 9,
    ),
  );

  return $field_instances;

}
