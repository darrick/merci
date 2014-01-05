<?php
/**
 * @file
 * Code for the Merci Status feature.
 */


function merci_workflow_validate_items($form, $form_state) {
  $node = (object) $form_state['values'];

  $entity_type  = 'node';

  $entity       = entity_metadata_wrapper($entity_type, $node);

  $date_field   = 'field_merci_date';
  $target_field = 'merci_reservation_items';

  $field_info = field_info_field($target_field);
  $target_type = $field_info['settings']['target_type'];

  $query = merci_workflow_overdue_query($entity_type, $entity, $date_field, $target_field);

  $ids = $query->execute()->fetchAllAssoc('target_id');

  if ($ids) {

    $items = entity_load($target_type, $ids);
    $targets = $entity->{$target_field}->raw();

    foreach ($targets as $delta => $target_id) {
      if (in_array($target_id, $overdue)) {
        form_set_error("$target_field][und][$delta][target_id", t('%name is past due.', array('%name' => $items[$target_id]->title)));
      }
    }
  } 
}

/*
 * Amend conflict query to only include conflicts if the reservation status is below MERCI_STATUS_CHECKED_IN
 */
function merci_query_merci_resource_alter(QueryAlterableInterface $query) {
  // Storage location for date field.
  $offset = variable_get('merci_workflow_state_offset', 0);
  $status_field = MERCI_STATUS_FIELD;
  $status_field_info = field_info_field($status_field);
  $status_storage      = $status_field_info['storage']['details']['sql']['FIELD_LOAD_CURRENT'];
  $keys = array_keys($status_storage);
  $status_table        = reset($keys);
  $status_column  = $status_storage[$status_table]['value'];

  $query->join($status_table, 'status_table', 'item_table.entity_id = status_table.entity_id');
  $query->addField('status_table', $status_column, 'field_status_value');
  $query->condition($status_column, MERCI_STATUS_CHECKED_OUT + $offset, '<=');
  $conditions = $query->conditions();
}

/*
 * Check if $target_id and date field are unique.
 * i.e. Given a $entity of $entity_type with date field $field and entity reference $target_id ...
 * check to ensure no other entities reference the $target_id at the same time.
 */
function merci_workflow_overdue_query($entity_type, $entity, $date_field, $target_field) {
  $target_ids = array();
  $targets = $entity->{$target_field}->raw();
  $dates   = $entity->{$date_field}->value();
  $exclude_id = $entity->getIdentifier();
  $offset = variable_get('merci_workflow_state_offset', 0);

  $status_field = MERCI_STATUS_FIELD;
  $gracetime    = 0;//$context['overdue_gracetime'];

  $date_start   = $dates['value'];

  $gracetime = "+1day";

  if ($gracetime) {
    $date_start = date('Y-m-d H:i:s', strtotime($dates['value'] . ' ' . $gracetime));
  }

  // Storage location for date field.
  $date_field_info = field_info_field($date_field);
  $date_storage      = $date_field_info['storage']['details']['sql']['FIELD_LOAD_CURRENT'];
  $keys = array_keys($date_storage);
  $date_table        = reset($keys);
  $date_column  = $date_storage[$date_table]['value'];
  $date_column2 = $date_storage[$date_table]['value2'];

  // Storage location for target field.
  $item_field_info = field_info_field($target_field);
  $item_storage      = $item_field_info['storage']['details']['sql']['FIELD_LOAD_CURRENT'];
  $keys = array_keys($item_storage);
  $item_table        = reset($keys);
  $item_column  = $item_storage[$item_table]['target_id'];


  // Build the query.
  $query = db_select($item_table, 'item_table');
  $query->addField('item_table', $item_column, 'target_id');

  if (count($targets) > 1) {
    $query->condition($item_column, $targets, 'IN');
  } else {
    $query->condition($item_column, reset($targets));
  }

  // Ignore myself.
  if ($exclude_id) {
    $query->condition('item_table.entity_id', $exclude_id, '!=');
  }

  $query->join($date_table, 'date_table', 'item_table.entity_id = date_table.entity_id');
  $query->addField('date_table', $date_column, 'field_date_value');
  $query->addField('date_table', $date_column2, 'field_date_value2');

  /* reservations in the past ... */
  $query->condition($date_column2, $date_start, '<=');

  // Storage location for date field.
  $status_field_info = field_info_field($status_field);
  $status_storage      = $status_field_info['storage']['details']['sql']['FIELD_LOAD_CURRENT'];
  $keys = array_keys($status_storage);
  $status_table        = reset($keys);
  $status_column  = $status_storage[$status_table]['value'];

  $query->join($status_table, 'status_table', 'item_table.entity_id = status_table.entity_id');
  $query->addField('status_table', $status_column, 'field_status_value');
  $query->condition($status_column, MERCI_STATUS_CHECKED_OUT + $offset, '=');
  $query->orderBy($date_column, 'ASC');

  // Return any ids of conflicting reservations if any.
  return $query;
}

