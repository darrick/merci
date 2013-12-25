<?php

/**
 * Implements hook_field_widget_info().
 */
function merci_status_field_info() {
  $fields['merci_reservation_status'] = array(
    'label' => t('Merci reservation status'),
    'description' => t("Status of the reservation"),
    'settings' => array(
      'allowed_values' => array(),
      'allowed_values_function' => 'merci_status_allowed_values',
    ),
    'default_widget' => 'options_select',
    'default_formatter' => 'list_default',
  );

  return $fields;
}

function merci_status_field_validate($entity_type, $entity, $field, $instance, $langcode, $items, &$errors) {
  list_field_validate($entity_type, $entity, $field, $instance, $langcode, $items, &$errors);
}

function merci_status_field_is_empty($item, $field) {
  return list_field_is_empty($item, $field);
}

function merci_status_allowed_values($field, $instance, $entity_type, $entity) {
  $options = array(
    MERCI_STATUS_UNCONFIRMED => t('Unconfirmed'),
    MERCI_STATUS_PENDING  => t('Pending'),
    MERCI_STATUS_CHECKED_OUT      => t('Checked out'),
    MERCI_STATUS_CHECKED_IN      => t('Checked in'),
    MERCI_STATUS_CANCELLED      => t('Canceled'),
    MERCI_STATUS_DENIED      => t('Denied'),
    MERCI_STATUS_NO_SHOW      => t('No show'),
  );
  drupal_alter('merci_reservation_status_options', &$options);
  return $options;
}

function merci_status_options_list($field, $instance, $entity_type, $entity) {
  return merci_status_allowed_values($field, $instance, $entity_type, $entity);
}

function merci_status_field_formatter_info_alter(&$info) {
  // Let a new field type re-use an existing formatter.
  $info['list_default']['field types'][] = 'merci_reservation_status';
}
 
function merci_status_field_widget_info_alter(&$info) {
  // Let a new field type re-use an existing widget.
  $info['options_select']['field types'][] = 'merci_reservation_status';
}

