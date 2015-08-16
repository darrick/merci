<?php

function merci_restrictions_fields() {
  $field_bases = array();

  // Exported field_base: MERCI_RESTRICTIONS
  $field_bases[MERCI_RESTRICTIONS] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => MERCI_RESTRICTIONS,
    'foreign keys' => array(
      'merci_restrictions' => array(
        'columns' => array(
          'target_id' => 'id',
        ),
        'table' => 'merci_restrictions',
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
      'target_type' => 'merci_restrictions',
    ),
    'translatable' => 0,
    'type' => 'entityreference',
  );

  // Exported field_base: MERCI_CHECKOUT_OFFSITE
  $field_bases[MERCI_CHECKOUT_OFFSITE] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => MERCI_CHECKOUT_OFFSITE,
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
        0 => '',
        1 => '',
      ),
      'allowed_values_function' => '',
    ),
    'translatable' => 0,
    'type' => 'list_boolean',
  );

  // Exported field_base: MERCI_CHECKOUT_MAX_LENGTH
  $field_bases[MERCI_CHECKOUT_MAX_LENGTH] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => MERCI_CHECKOUT_MAX_LENGTH,
    'foreign keys' => array(
      'format' => array(
        'columns' => array(
          'format' => 'format',
        ),
        'table' => 'filter_format',
      ),
    ),
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(
      'max_length' => 255,
    ),
    'translatable' => 0,
    'type' => 'text',
  );
  // Exported field_base: 'field_override_restrictions'
  $field_bases[MERCI_OVERRIDE_RESTRICTIONS] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => MERCI_OVERRIDE_RESTRICTIONS,
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

function merci_restrictions_instances() {
  $field_instances = array();

  // Exported field_instance: MERCI_CHECKOUT_OFFSITE
  $field_instances[MERCI_CHECKOUT_OFFSITE] = array(
    'default_value' => array(
      0 => array(
        'value' => 0,
      ),
    ),
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'boolean_formatter',
        'settings' => array(
          'custom_off' => '',
          'custom_on' => '',
          'format' => 'yes-no',
          'reverse' => 0,
        ),
        'type' => 'boolean_yes_no',
        'weight' => 2,
      ),
    ),
    'label' => 'Checkout Offsite',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'display_label' => 1,
      ),
      'type' => 'options_onoff',
      'weight' => 2,
    ),
  );

  // Exported field_instance: MERCI_CHECKOUT_MAX_LENGTH
  $field_instances[MERCI_CHECKOUT_MAX_LENGTH] = array(
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 1,
      ),
    ),
    'label' => 'Max Length of Checkout',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 1,
    ),
  );
  // Exported field_instance: MERCI_OVERRIDE_RESTRICTIONS
  $field_instances[MERCI_OVERRIDE_RESTRICTIONS] = array(
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
        'weight' => 6,
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
        'weight' => 12,
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
        'weight' => 6,
      ),
    ),
    'label' => 'Override Restrictions',
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
      'weight' => 10,
    ),
  );

  // Exported field_instance: MERCI_RESTRICTIONS
  $field_instances[MERCI_RESTRICTIONS] = array(
    'deleted' => 0,
    'description' => '',
    'display' => array(
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
        'weight' => 3,
      ),
    ),
    'label' => 'Restrictions',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'inline_entity_form',
      'settings' => array(
        'fields' => array(),
        'type_settings' => array(
          'allow_existing' => FALSE,
          'delete_references' => 1,
          'label_plural' => 'merci restrictions',
          'label_singular' => 'merci restriction',
          'match_operator' => 'CONTAINS',
          'override_labels' => 0,
        ),
      ),
      'type' => 'inline_entity_form',
      'weight' => 7,
    ),
  );

  return $field_instances;

}
