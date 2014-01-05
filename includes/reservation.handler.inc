<?php

define('MERCI_DATE_FIELD_ALIAS', 'merci_date_field_value');
define('MERCI_DATE_FIELD_ALIAS2', 'merci_date_field_value');

class MerciDefaultReservationController {

  protected $conflict_cache = array();

  protected $entity_type, $entity, $date_field, $target_field;

  public static function getInstance($entity_type, $entity, $date_field, $target_field) {

    if (class_exists($class_name = 'MerciDefaultReservationController_' . $entity_type)) {
      return new $class_name($entity_type, $entity, $date_field, $target_field);
    }
    else {
      return new MerciDefaultReservationController($entity_type, $entity, $date_field, $target_field);
    }
  }

  public function __construct($entity_type, $entity, $date_field, $target_field) {
    $this->entity_type  = $entity_type;
    $this->entity       = $entity;
    $this->date_field   = $date_field;
    $this->target_field = $target_field;
  }

  public function entity() { return $this->entity; }
  public function date_field() { return $this->date_field; }
  public function target_field() { return $this->target_field; }

  public function conflicts() {
    $field_info = field_info_field($this->target_field);
    $target_type = $field_info['settings']['target_type'];
    if (!array_key_exists($target_type, $this->conflict_cache)) {
      $handler = MerciDefaultTargetController::getInstance($target_type, $this); 
      $conflict_cache[$target_type] = $handler->conflicts();
    }
    return $conflict_cache[$target_type];
  }

  public function conflictingReservations() {
    $conflicts = $this->conflicts(); 
    $ids = array();
    foreach ($conflicts as $reservations) {
      foreach ($reservations as $reservation_id){
        if (!in_array($reservation_id, $ids)) {
          $ids[] = $reservation_id;
        }
      }
    }
    return $ids;
  }

  public function conflictingTargets() {
    return array_keys($this->conflicts());
  }
}
/**
 * @file
 * Abstraction of the selection logic of an entity reference field.
 *
 * Implementations that wish to provide an implementation of this should
 * register it using CTools' plugin system.
 */
/**
 * A null implementation of EntityReference_SelectionHandler.
 */
class MerciDefaultTargetController {

  protected $target_type, $entity, $date_field, $target_field, $dates, $targets;

  public static function getInstance($target_type, $controller) {

    if (class_exists($class_name = 'MerciDefaultTargetController_' . $target_type)) {
      return new $class_name($target_type, $controller);
    }
    else {
      return new MerciDefaultTargetController($target_type, $controller);
    }
  }

  public function __construct($target_type, $controller) {
    $this->target_type  = $target_type;
    $this->entity       = $controller->entity();
    $this->date_field   = $controller->date_field();
    $this->target_field = $controller->target_field();
    $this->dates        = $this->entity->{$this->date_field}->value();
    $targets            = $this->entity->{$this->target_field}->raw();
    foreach ($targets as $delta => $target) {
      if (!empty($target) and intval($target)) {
        $this->targets[$delta] = $target;
      }
    }
  }


  public function conflicts() {
    $query = $this->buildConflictQuery();

    $result = $query->execute();
    $conflicts = array();
    foreach ($result as $record){
      dpm($record);
      if (!array_key_exists($record->target_id, $conflicts)) {
        $conflicts[$record->target_id] = array();
      } 
      $conflicts[$record->target_id] = $record->reservation_id;
    }
    return $conflicts;
  }

  protected function buildConflictQuery() {
    $dates = $this->dates;

    $exclude_id = $this->entity->getIdentifier();
    $target_ids = $this->targets;

    $target_field = $this->target_field;
    $date_field   = $this->date_field;
    //$status_field = $this->status_field;

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
    $query->addField('item_table', 'entity_id', 'reservation_id');

    if (count($target_ids) > 1) {
      $query->condition($item_column, $target_ids, 'IN');
    } else {
      $query->condition($item_column, reset($target_ids));
    }

    // Ignore myself.
    if ($exclude_id) {
      $query->condition('item_table.entity_id', $exclude_id, '!=');
    }

    $query->join($date_table, 'date_table', 'item_table.entity_id = date_table.entity_id');
    $query->addField('date_table', $date_column, MERCI_DATE_FIELD_ALIAS);
    $query->addField('date_table', $date_column2, MERCI_DATE_FIELD_ALIAS2);

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

    // Add a generic entity access tag to the query.
    $query->addTag('merci_resource');
    $query->addMetaData('merci_reservable_handler', $this);

    return $query;
  }
}

