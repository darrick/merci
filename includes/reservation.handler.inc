<?php

define('MERCI_DATE_FIELD_ALIAS', 'merci_date_field_value');
define('MERCI_DATE_FIELD_ALIAS2', 'merci_date_field_value');
/**
 * @file
 * Abstraction of the selection logic of an entity reference field.
 *
 * Implementations that wish to provide an implementation of this should
 * register it using CTools' plugin system.
 */
interface Merci_ReservationHandler {
  /**
   * Factory function: create a new instance of this handler for a given field.
   *
   * @param $field
   *   A field datastructure.
   * @return EntityReferenceHandler
   */
  public static function getInstance($target_type, $entity, $date_field, $target_field);

  public function validateItems(&$errors);

}

/**
 * A null implementation of EntityReference_SelectionHandler.
 */
class Merci_ReservationHandler_Generic implements Merci_ReservationHandler {
  public static function getInstance($target_type = NULL, $entity, $date_field, $target_field) {

    if (class_exists($class_name = 'Merci_ReservationHandler_Generic_' . $target_type)) {
      return new $class_name($target_type, $entity, $date_field, $target_field);
    }
    else {
      return new Merci_ReservationHandler_Generic($target_type, $entity, $date_field, $target_field);
    }
  }

  public function __construct($target_type, $entity, $date_field, $target_field) {
    $this->target_type  = $target_type;
    $this->entity       = $entity;
    $this->date_field   = $date_field;
    $this->target_field = $target_field;
    $this->dates        = $entity->{$date_field}->value(); 
    $targets            = $entity->{$target_field}->raw(); 
    foreach ($targets as $delta => $target) {
      if (!empty($target) and intval($target)) {
        $this->targets[$delta] = $target;
      }
    }
  }


  public function validateItems(&$errors) {

    $this->validateConflicts($errors);
    $this->validateTooMany($errors);
    // Let other modules validate the entity.
    // Avoid module_invoke_all() to let $errors be taken by reference.
    foreach (module_implements('merci_generic_items_validate') as $module) {
      $function = $module . '_merci_generic_items_validate';
    //  $function($this, $errors);
    }


    // TODO too many selected.
  }

  public function validateTooMany(&$errors) {
  }

  public function conflictingReservations() {
    $query = $this->buildItemReservationQuery();
    $query->addField('item_table', 'entity_id', 'reservation_id');

    $reservations = $query->execute()->fetchAllAssoc('target_id');
    return $reservations;
  }

  public function validateConflicts(&$errors) {
    $query = $this->buildItemReservationQuery();

    $reservations = $query->execute()->fetchAllAssoc('target_id');

    foreach ($this->targets as $delta => $target_id) {
      if (array_key_exists($target_id, $reservations)) {
        $errors[$this->target_field][$delta][] = array(
          'error' => 'merci',
          'message' => t("Item is not available to reserve"),
        );
      }
    }
  }

  protected function buildItemReservationQuery() {
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
    //$query->addTag('merci_resource');
    //$query->addMetaData('merci_reservable_handler', $this);

    return $query;
  }
}

