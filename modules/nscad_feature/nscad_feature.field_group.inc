<?php
/**
 * @file
 * nscad_feature.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function nscad_feature_field_group_info() {
  $field_groups = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_filter_location|node|merci_reservation|form';
  $field_group->group_name = 'group_filter_location';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'merci_reservation';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Reservable Items',
    'weight' => '3',
    'children' => array(
      0 => 'field_kit',
      1 => 'choice_wrapper',
    ),
    'format_type' => 'div',
    'format_settings' => array(
      'label' => 'Reservable Items',
      'instance_settings' => array(
        'required_fields' => 1,
        'id' => '',
        'classes' => 'group-filter-location field-group-div',
        'description' => '',
        'show_label' => '0',
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'fast',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_filter_location|node|merci_reservation|form'] = $field_group;

  // Translatables
  // Included for use with string extractors like potx.
  t('Reservable Items');

  return $field_groups;
}
