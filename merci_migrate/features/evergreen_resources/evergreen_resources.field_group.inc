<?php
/**
 * @file
 * evergreen_resources.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function evergreen_resources_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_costs|node|merci_resource_display|form';
  $field_group->group_name = 'group_costs';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'merci_resource_display';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Costs',
    'weight' => '8',
    'children' => array(
      0 => 'field_replacement_cost',
      1 => 'field_late_fine',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-costs field-group-tab',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_costs|node|merci_resource_display|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_details|node|merci_resource_display|form';
  $field_group->group_name = 'group_details';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'merci_resource_display';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Details',
    'weight' => '7',
    'children' => array(
      0 => 'field_manufacturer',
      1 => 'field_model',
      2 => 'field_url',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-details field-group-tab',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_details|node|merci_resource_display|form'] = $field_group;

  return $export;
}
