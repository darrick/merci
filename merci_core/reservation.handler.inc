<?php


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
class MerciDefaultController {

  protected $item_type, $entity, $context;

  public static function getInstance($item_type, $context) {

    if (class_exists($class_name = 'MerciDefaultController_' . $item_type)) {
      return new $class_name($item_type, $context);
    }
    else {
      return new MerciDefaultController($item_type, $context);
    }
  }

  public function __construct($item_type, $context) {
    $this->context = $context;
    $this->item_type  = $item_type;
  }

  /*
   * Determine if merci_line_item $entity conflicts with any other existing line_items.
   *
   * Returns array of conflicting line items.
   */

  public function conflicts($entity) {
    $this->entity       = $entity;
    $query = $this->buildConflictQuery();

    $result = $query->execute();
    $conflicts = array();
    foreach ($result as $record){
      $conflicts[] = $record;
    }
    return $conflicts;
  }

  protected function buildConflictQuery() {
    $context = $this->context;

    $dates        = $this->entity->{$context['date_field']}->value();
    $item_id      = $this->entity->{$context['item_field']}->raw();
    $exclude_id   = $this->entity->getIdentifier();
    $entity_type  = $this->entity->type();

    // Storage location for date field.
    $date_field_info    = field_info_field($context['date_field']);
    $date_storage       = $date_field_info['storage']['details']['sql']['FIELD_LOAD_CURRENT'];
    $keys               = array_keys($date_storage);
    $date_table         = reset($keys);
    $date_column        = $date_storage[$date_table]['value'];
    $date_column2       = $date_storage[$date_table]['value2'];

    // Storage location for item field.
    $item_field_info    = field_info_field($context['item_field']);
    $item_storage       = $item_field_info['storage']['details']['sql']['FIELD_LOAD_CURRENT'];
    $keys               = array_keys($item_storage);
    $item_table         = reset($keys);
    $keys               = array_keys($item_field_info['indexes']);
    $item_key           = reset($keys);
    $item_column        = $item_storage[$item_table][$item_key];

    // Build the query.
    $query = db_select($item_table, 'item_table');
    $query->addField('item_table', $item_column, 'item_id');
    $query->addField('item_table', 'entity_id', 'parent_id');

    $query->condition($item_column, $item_id);
    // Ignore myself.
    if ($exclude_id) {
      $query->condition('item_table.entity_id', $exclude_id, '!=');
    }

    $query->join('merci_line_item', 'merci_line_item', 'item_table.entity_id = merci_line_item.line_item_id');
    $query->addField('merci_line_item', 'quantity', 'quantity');
    $query->condition('merci_line_item.status', 1, '=');
    /*
    $query->join($quantity_table, 'quantity_table', 'quantity_table.entity_id = quantity_table.entity_id');
    $query->addField('quantity_table', $quantity_column);//, MERCI_DATE_FIELD_ALIAS);
    */

    $query->join($date_table, 'date_table', 'item_table.entity_id = date_table.entity_id');
    $query->addField('date_table', $date_column);//, MERCI_DATE_FIELD_ALIAS);
    $query->addField('date_table', $date_column2);//, MERCI_DATE_FIELD_ALIAS2);
    $query->condition('date_table.entity_type',$entity_type,'=');
    $query->condition('date_table.deleted' , 0, '=');

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

/**
 * A null implementation of EntityReference_SelectionHandler.
 */
class MerciDefaultController_non_inventory extends MerciDefaultController {

  protected $date_column, $date_column2;

  public function conflicts($entity) {
    return $this->bestFit($entity);
  }

  public function quantity_available($entity) {
    $context = $this->context;

    $resource  = $entity->{$context['item_field']};
    $quantity = $resource->{$context['quantity_field']}->value();

    return $quantity - count($this->bestFit($entity));
  }

  public function reservations($dates, $exclude_id) {
    $bestfit = $this->bestFit($dates);
    $reservations = array();
    foreach ($bestfit as $enity_id => $reservation) {
      $reservations[] = $entity_id;
    }
    return $reservations;
  }

  public function bestFit($entity) {

    $context = $this->context;

    $resource  = $entity->{$context['item_field']};

    // Determine if the quantity field exists.  If so use it.
    try {
      $quantity = $resource->{$context['quantity_field']}->value();
    } catch (EntityMetadataWrapperException $e) {
      $quantity = 1;
    }

    $dates        = $entity->{$context['date_field']}->value();
    
    // Storage location for date field.
    $date_field_info    = field_info_field($context['date_field']);
    $date_storage       = $date_field_info['storage']['details']['sql']['FIELD_LOAD_CURRENT'];
    $keys               = array_keys($date_storage);
    $date_table         = reset($keys);
    $this->date_column  = $date_column        = $date_storage[$date_table]['value'];
    $this->date_column2 = $date_column2       = $date_storage[$date_table]['value2'];

    // Split reservations based on quantity. 
    $reservations = array();
    foreach(parent::conflicts($entity) as $reservation) {
      for ($i = 0; $i < $reservation->quantity; $i++) {
        $reservations[] = $reservation;
      }
    }

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
    uasort($reservations, array($this, "merci_bucket_cmp_length"));

    $buckets = array();
    // First-fit algorythm.
    foreach ($reservations as $test_reservation) {

      // Go through each bucket item to look for a available slot for this reservation.
      //
      // Find a bucket to use for this reservation.
      for ($i = 0; $i < $quantity; $i++) {

        $fits = TRUE;
        // Bucket already has other reservations we need to check against for a fit.
        if (array_key_exists($i, $buckets)) {
          foreach ($buckets[$i] as $reservation) {
            if ($this->merci_bucket_intersects($reservation, $test_reservation)) {
              //Conflict so skip saving the reservation to this slot and try to use the next bucket item.
              $fits = FALSE;
              break;
            }
          }
        }

        // We've found a slot so test the next reservation.
        if ($fits) {
          if (array_key_exists($i, $buckets)) {
            $buckets[$i] = array();
          }
          $buckets[$i][] = $test_reservation;
          break;
        }

      }
    }
    return $buckets;
  }

/*
 * |----------------------|        range 1
 * |--->                           range 2 overlap
 *  |--->                          range 2 overlap
 *                        |--->    range 2 overlap
 *                         |--->   range 2 no overlap
 */
  private function merci_bucket_intersects($r1, $r2) {
    $value = $this->date_column;
    $value2 = $this->date_column2;
    /*
     * Make sure r1 start date is before r2 start date.
     */
    if (date_create($r1->{$value}) > date_create($r2->{$value})) {
      $temp = $r1;
      $r1 = $r2;
      $r2 = $temp;
    }

    if (date_create($r2->{$value}) <= date_create($r1->{$value2})) {
      return true;
    }
    return false;

  }

  private function merci_bucket_cmp_length($a, $b) {
    $value = $this->date_column;
    $value2 = $this->date_column2;
    $len_a = date_format(date_create($a->{$value2}),'U') - date_format(date_create($a->{$value}), 'U');  
    $len_b = date_format(date_create($b->{$value2}),'U') - date_format(date_create($b->{$value}), 'U');  
    if ($len_a == $len_b) {
      return 0;
    }
    return ($len_a < $len_b) ? 1 : -1;
  }

}
