<?php
/**
 * @file
 * evergreen_resources.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function evergreen_resources_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_merci_resource_display';
  $strongarm->value = 0;
  $export['comment_anonymous_merci_resource_display'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_merci_resource_display';
  $strongarm->value = 1;
  $export['comment_default_mode_merci_resource_display'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_merci_resource_display';
  $strongarm->value = '50';
  $export['comment_default_per_page_merci_resource_display'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_merci_resource_display';
  $strongarm->value = 1;
  $export['comment_form_location_merci_resource_display'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_merci_resource_display';
  $strongarm->value = '1';
  $export['comment_merci_resource_display'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_merci_resource_display';
  $strongarm->value = '1';
  $export['comment_preview_merci_resource_display'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_merci_resource_display';
  $strongarm->value = 1;
  $export['comment_subject_field_merci_resource_display'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_merci_resource_display';
  $strongarm->value = array(
    0 => 'main-menu',
  );
  $export['menu_options_merci_resource_display'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_merci_resource_display';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_merci_resource_display'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_merci_resource_display';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_merci_resource_display'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_merci_resource_display';
  $strongarm->value = '1';
  $export['node_preview_merci_resource_display'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_merci_resource_display';
  $strongarm->value = 1;
  $export['node_submitted_merci_resource_display'] = $strongarm;

  return $export;
}
