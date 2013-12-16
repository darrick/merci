<?php

/**
 * @file
 * Abstraction of the selection logic of an entity reference field.
 *
 * Implementations that wish to provide an implementation of this should
 * register it using CTools' plugin system.
 */
interface Merci_ReservableHandler {
  /**
   * Factory function: create a new instance of this handler for a given field.
   *
   * @param $field
   *   A field datastructure.
   * @return EntityReferenceHandler
   */
  public static function getInstance($entity_type = NULL, $entity = NULL, $preset = NULL, $target_type);
  public function reservations($dates, $exclude_id);
  public function available($dates, $exclude_id);
  public function dateRestrictions($dates);
  public function restrictions();

  /**
   * Generate a settings form for this handler.
   */
  public static function settingsForm();
}

/**
 * A null implementation of EntityReference_SelectionHandler.
 */
class Merci_ReservableHandler_Generic implements Merci_ReservableHandler {
  public static function getInstance($entity_type = NULL, $entity = NULL, $preset = NULL, $target_type = NULL) {
    $merci_reservable_type = property_exists($entity, MERCI_RESERVABLE_TYPE_FIELD) ? $entity->{MERCI_RESERVABLE_TYPE_FIELD}[LANGUAGE_NONE][0]['value'] : NULL;
    if (class_exists($class_name = 'Merci_ReservableHandler_Generic_' . $merci_reservable_type)) {
      return new $class_name($entity_type, $entity, $preset);
    }
    else {
      return new Merci_ReservableHandler_Generic($entity_type, $entity, $preset);
    }
  }

  public function __construct($entity_type, $entity, $preset) {
    $this->entity_type = $entity_type;
    $this->entity      = $entity;
    $this->settings    = merci_preset_load_settings($preset);
    // Get entity ids and language.
    list($entity_id, $vid, $bundle) = entity_extract_ids($entity_type, $entity);
    $this->entity_id = $entity_id;
  }

  public function itemCount() {
    return 1;
  }

  public function access($account) {
    $rights = &drupal_static(__FUNCTION__, array());

    // If no user object is supplied, the access check is for the current user.
    if (empty($account)) {
      $account = $GLOBALS['user'];
    }

    $entity_id = $this->entity_id;
    // If we've already checked access for this node, user and op, return from
    // cache.
    if (isset($rights[$account->uid][$entity_id])) {
      return $rights[$account->uid][$entity_id];
    }

    $access = module_invoke_all('merci_item_access', $this->entity_type, $this->entity, $account);
    if (in_array(MERCI_ITEM_ACCESS_DENY, $access, TRUE)) {
      $rights[$account->uid][$entity_id] = FALSE;
      return FALSE;
    }
    elseif (in_array(MERCI_ITEM_ACCESS_ALLOW, $access, TRUE)) {
      $rights[$account->uid][$entity_id] = TRUE;
      return TRUE;
    }
  }

  public function reservations($dates, $exclude_id = NULL) {
    return array();
  }

  public function available($dates, $exclude_id = NULL) {
    if (!empty($dates)) {
      $reservations = $this->reservations($dates, $exclude_id);
      if (count($reservations) >= $this->itemCount()) {
        return FALSE;
      }
    }

    if (!empty($dates) and $this->dateRestrictions($dates)) {
      return FALSE;
    }

    if ($this->restrictions()) {
      return FALSE;
    }
    return TRUE;
  }

  public function dateRestrictions($dates) {
    $result = module_invoke_all('merci_date_restrictions', $this, $dates);

    if (in_array(TRUE, $result, TRUE)) {
      return TRUE;
    }
    return FALSE;
  }

  public function restrictions() {
    $result = module_invoke_all('merci_restrictions', $this);

    if (in_array(TRUE, $result, TRUE)) {
      return TRUE;
    }
    return FALSE;
  }

  public static function settingsForm() {
  }
}

class Merci_ReservableHandler_Generic_resource extends Merci_ReservableHandler_Generic {

  public static function getInstance($entity_type = NULL, $entity = NULL, $preset = NULL, $target_type = NULL) {
    return new Merci_ReservableHandler_Generic_resource($entity_type, $entity, $preset);
  }


  public function reservations($dates, $exclude_id = NULL) {
    $query = $this->buildItemReservationQuery($dates, $exclude_id);
    // Return ids of reservations if any.
    return ($query->execute()->fetchAllAssoc('entity_id'));
  }

  protected function buildItemReservationQuery($dates, $exclude_id = NULL) {

    $target_field = $this->settings['target_field'];
    $date_field   = $this->settings['date_field'];
    $target_id    = $this->entity_id;

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
    $query->addField('item_table', 'entity_id', 'entity_id');

    $query->condition($item_column, $target_id);

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
