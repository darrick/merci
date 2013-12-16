<?php

/**
 * @file
 * Field widget related code for Organic groups.
 */


/**
 * Implements hook_field_widget_info().
 */
function merci_field_info() {
  $fields['merci_reservable_type'] = array(
    'label' => t('Merci reservable type'),
    'description' => t("Stores the reservable item type (i.e. resource, bucket or kit.)"),
    'settings' => array(
      'allowed_values' => array(),
      'allowed_values_function' => '',
    ),
    'default_widget' => 'options_select',
    'default_formatter' => 'list_default',
  );

  return $fields;
}

function merci_field_validate($entity_type, $entity, $field, $instance, $langcode, $items, &$errors) {
  list_field_validate($entity_type, $entity, $field, $instance, $langcode, $items, &$errors);
}

function merci_field_is_empty($item, $field) {
  return list_field_is_empty($item, $field);
}

function merci_options_list($field, $instance, $entity_type, $entity) {
  $options = array(
    'resource' => t('Resoruce'),
    'bucket'   => t('Bucket'),
    'kit'      => t('Kit'),
  );
  drupal_alter('merci_reservable_type_options', &$options);
  return $options;
}

function merci_field_formatter_info_alter(&$info) {
  // Let a new field type re-use an existing formatter.
  $info['list_default']['field types'][] = 'merci_reservable_type';
}
 
function merci_field_widget_info_alter(&$info) {
  // Let a new field type re-use an existing widget.
  $info['options_select']['field types'][] = 'merci_reservable_type';
}

