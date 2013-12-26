<?php
/**
 * @file
 * merci_feature.default_field_validation_rules.inc
 */

/**
 * Implements hook_default_field_validation_rule().
 */
function merci_feature_default_field_validation_rule() {
  $export = array();

  $rule = new stdClass();
  $rule->disabled = FALSE; /* Edit this to true to make a default rule disabled initially */
  $rule->api_version = 2;
  $rule->rulename = 'Unique Kit Item';
  $rule->name = 'unique_kit_item';
  $rule->field_name = 'field_kit_item';
  $rule->col = 'target_id';
  $rule->entity_type = 'node';
  $rule->bundle = 'reservable_kit';
  $rule->validator = 'field_validation_unique_validator';
  $rule->settings = array(
    'data' => 'global',
    'bypass' => 0,
    'roles' => array(
      2 => 0,
      3 => 0,
      4 => 0,
    ),
    'errors' => 0,
  );
  $rule->error_message = 'This item must be unique to this kit.';
  $export['unique_kit_item'] = $rule;

  return $export;
}
