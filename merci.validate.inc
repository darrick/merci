<?php

/* 
 * Main entry for validation.
 * Developers should either call via hook_validate, hook_node_validate or hook_field_validate.
 */
function merci_api_validate_items($entity_type, $entity, $items, &$errors){
  if (empty($entity_type)) {
    return;
  }
  $merci_settings = merci_settings_load($entity_type, $entity);
  $date_field = $merci_settings['date_field'];
  $target_field = $merci_settings['target_field'];

  foreach (module_implements('merci_validate_items') as $module) {
    $function = $module . '_merci_validate_items';
    $function($entity_type, $entity, $date_field, $target_field, $items, &$errors);
  }
}

function merci_api_validate_restrictions($entity_type, $entity, $field, $instance, $langcode, $items, &$errors){
  if (empty($entity_type)) {
    return;
  }

  foreach (module_implements('merci_validate_restrictions') as $module) {
    $function = $module . '_merci_validate_restrictions';
    $function($entity_type, $entity, $field, $instance, $langcode, $items, &$errors);
  }
}

function merci_settings_load($node) {
  $settings = array();
  foreach (module_implements('merci_settings_load') as $module) {
    $function = $module . '_merci_settings_load';
    $function($node, &$settings);
  }
  return $settings;
}

function merci_merci_settings_load($node, &$settings) {
  $settings['date_field'] = 'field_merci_date';
  $settings['target_field'] = 'field_merci_items'; 
}

function is_merci_reservation_node($node) {
  //dpm($node);
  return ($node->type == 'merci_reservation');
}

function merci_type_setting($node) {
  $type = $node->type;
  return variable_get('merci_type_setting_' . $type, 'disabled');
}



function merci_api_get_item_count($target_id) {
  return 1;
}
/*
 * Check if $target_id and date field are unique.
 * i.e. Given a $entity of $entity_type with date field $field and entity reference $target_id ...
 * check to ensure no other entities reference the $target_id at the same time.
 */
function merci_api_check_item_conflicts($entity_type, $entity, $date_field, $target_field, $target_id) {


  if (!property_exists($entity, $date_field) or !property_exists($entity, $target_field)) {
    return;
  }
  // Get entity ids and language.
  list($entity_id, $vid, $bundle) = entity_extract_ids($entity_type, $entity);
  $langcode = $entity->language ? $entity->language : LANGUAGE_NONE;


  // Storage location for date field.
  $date_field_info = field_info_field($date_field);
  $date_storage      = $date_field_info['storage']['details']['sql']['FIELD_LOAD_CURRENT'];
  $keys = array_keys($date_storage);
  $date_table        = reset($keys);
  $date_column  = $date_storage[$date_table]['value'];
  $date_column2 = $date_storage[$date_table]['value2'];

  $dates = $entity->{$date_field}[$langcode][0];
  // Storage location for target field.
  $item_field_info = field_info_field($target_field);
  $item_storage      = $item_field_info['storage']['details']['sql']['FIELD_LOAD_CURRENT'];
  $keys = array_keys($item_storage);
  $item_table        = reset($keys);
  $item_column  = $item_storage[$item_table]['target_id'];


  // Build the query.
  $query = db_select($item_table, 'item_table');
  $query->addField('item_table', 'entity_id', 'entity_id');

  $query->condition($item_column, $target_id);

  // Ignore myself.
  if ($entity_id) {
    $query->condition('item_table.entity_id', $entity_id, '!=');
  }

  $query->join($date_table, 'date_table', 'item_table.entity_id = date_table.entity_id');
  $query->addField('date_table', $date_column, 'field_date_value');
  $query->addField('date_table', $date_column2, 'field_date_value2');

  $query->condition( db_or()
    //  start falls within another reservation.
    //                     |-------------this-------------|
    //            |-------------conflict-------------------------|
    //            OR
    //                     |-------------this-------------------------------|
    //            |-------------conflict-------------------------|
      ->condition(db_and() 
      ->condition($date_column, $dates['value'], '<=')->condition($date_column2, $dates['value'], '>=')
    )
  //  end falls within another reservation.
  //                     |-------------this-------------------------------|
  //                                   |-------------conflict-------------------------|
      ->condition(db_and() 
      ->condition($date_column, $dates['value2'], '<=')->condition($date_column2, $dates['value2'], '>=')
    )
//  start before another reservation.
//  end after another reservation.
//                     |-------------------------this-------------------------------|
//                            |----------------conflict------------------|
      ->condition(db_and()
      ->condition($date_column, $dates['value'], '>')->condition($date_column2, $dates['value2'], '<')
            )
    );

  $query->orderBy($date_column, 'ASC');

  $conditions = $query->conditions();
  dpm( $conditions);
  dpm($conditions[2]['field']->conditions());
  foreach (module_implements('merci_alter_conflict_query') as $module) {
    $function = $module . '_merci_alter_conflict_query';
    $function($entity, &$query);
  }
  // Return any ids of conflicting reservations if any.
  return ($query->execute()->fetchAllAssoc('entity_id'));
}

function merci_api_check_bucket_conflicts($entity_type, $entity, $date_field, $target_field, $bucket_field, $target_type, $target_id) {

  $bucket_item = entity_load($target_type, array($target_id));
  $bucket_item = $bucket_item[$target_id];
  //dpm($bucket_item);
  $langcode = $bucket_item->language ? $bucket_item->language : LANGUAGE_NONE;

  // Get all the items in the bucket.
  $items = $bucket_item->{$bucket_field}[$langcode];
  $max = count($items);

  // Temp array to store all the bucket items tied to their reservations during the given time period.
  $bucket = array();

  // Reservations reserving the item directly.
  foreach ($items as $target) {
    // Is it available?
    $item_reservations = merci_api_check_item_conflicts($entity_type, $entity, $date_field, $target_field, $target['target_id']);
    $bucket[$target['target_id']] = $item_reservations;
  }

  // Other bucket reservations.
  $bucket_reservations = merci_api_check_item_conflicts($entity_type, $entity, $date_field, $target_field, $target_id);

  // Add ourselves to the list of reservations.
  // Storage location for date field.
  $date_field_info = field_info_field($date_field);
  $date_storage      = $date_field_info['storage']['details']['sql']['FIELD_LOAD_CURRENT'];
  $date_table        = reset(array_keys($date_storage));
  $date_column  = $date_storage[$date_table]['value'];
  $date_column2 = $date_storage[$date_table]['value2'];
  $dates = $entity->{$date_field}[$langcode][0];
  $bucket_reservations[0] = new StdClass;
  $bucket_reservations[0]->{$date_column} = $dates['value']; 
  $bucket_reservations[0]->{$date_column2} = $dates['value2']; 
  $bucket_reservations[0]->entity_id = 0; 

  // Determine how many bucket items are needed for this time period.
  // Need to sort like this:
  //            .... time ....
  // item1  x x a a a x x x x x f x e e e x x x x x
  // item2  x x x d d d d d d x x x x c c c x x x x
  // item3  x x b b b b b b b b b b b b b x x x x x
  // etc ......
  //
  //      // Order by lenght of reservation descending.
  //      // Do first-fit algorythm.



  // Sort by length of reservation.
  uasort($bucket_reservations, "merci_bucket_cmp_length");

  // First-fit algorythm.
  foreach ($bucket_reservations as $reservation_id => $bucket_reservation) {

    // Go through each bucket item to look for a available slot for this reservation.
    foreach ($bucket as $target_id => $reservations) {
      foreach ($reservations as $reservation) {
        if (merci_bucket_intersects($reservation, $bucket_reservation)) {
          //Conflict so skip saving the reservation to this slot and try to use the next bucket item.
          //goto end;
        }
      }
      // We've found a slot so test the next reservation.
      $bucket[$target_id][$bucket_reservation->entity_id] = $bucket_reservation;
      $bucket_reservations[$reservation_id]->item_id = $target_id;

      // Look for a slot for the next reservation.
      break;
      //end:
    }
    // No slots for this reservation.
    if (!property_exists($bucket_reservations[$reservation_id], 'item_id')){
      return ($bucket_reservations);
      break;
    }

  }
  return false;
}


/*
 * |----------------------|        range 1
 * |--->                           range 2 overlap
 *  |--->                          range 2 overlap
 *                        |--->    range 2 overlap
 *                         |--->   range 2 no overlap
 */
function merci_bucket_intersects($r1, $r2) {
  /*
   * Make sure r1 start date is before r2 start date.
   */
  if (date_create($r1->field_date_value) > date_create($r2->field_date_value)) {
    $temp = $r1;
    $r1 = $r2;
    $r2 = $temp;
  }

  if (date_create($r2->field_date_value) <= date_create($r2->field_date_value2)) {
    return true;
  }
  return false;

}

function merci_bucket_cmp_length($a, $b) {
  $len_a = date_format(date_create($a->field_date_value2),'U') - date_format(date_create($a->field_date_value), 'U');  
  $len_b = date_format(date_create($b->field_date_value2),'U') - date_format(date_create($b->field_date_value), 'U');  
  if ($len_a == $len_b) {
    return 0;
  }
  return ($len_a < $len_b) ? 1 : -1;
}

