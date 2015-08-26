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

  protected $dates, $date_table, $date_column, $date_column2, $item_table, $item_column, $items, $item_type, $entity, $context;

  protected $conflicting_entities;

  protected $quantity_reserved;

  protected $validated = FALSE;

  protected $items_is_list = FALSE;

  protected $errors = NULL;

  public static function getInstance($entity, $item_type, $context) {

    if (class_exists($class_name = 'MerciDefaultController_' . $item_type)) {
      return new $class_name($entity, $item_type, $context);
    }
    else {
      return new MerciDefaultController($entity, $item_type, $context);
    }
  }

  public function __construct($entity, $item_type, $context) {
    $this->context = $context;
    $this->item_type  = $item_type;
    $this->entity       = $entity;

    $this->items = array();
    // Determine if dealing with multiple values.
    if (substr($entity->{$context['item_field']}->type(), 0, 5) == 'list<') {
      foreach ($entity->{$context['item_field']}->getIterator() as $delta => $item) {
        if ($item->getIdentifier()) {
          $this->items[$delta] = $item;
        }
      }
      $this->items_is_list = TRUE;
    } else {
      $this->items[0] = $entity->{$context['item_field']};
    }

    if (substr($entity->{$context['date_field']}->type(), 0, 5) == 'list<') {
      foreach ($entity->{$context['date_field']}->getIterator() as $delta => $item) {
        $this->dates[$delta] = $item->value();
      }
    } else {
      $this->dates[0] = $entity->{$context['date_field']}->value();
    }

    // Storage location for date field.
    $date_field_info    = field_info_field($context['date_field']);
    $date_storage       = $date_field_info['storage']['details']['sql']['FIELD_LOAD_CURRENT'];
    $keys               = array_keys($date_storage);
    $this->date_table         = reset($keys);
    $this->date_column        = $date_storage[$this->date_table]['value'];
    $this->date_column2       = $date_storage[$this->date_table]['value2'];

    // Storage location for item field.
    $item_field_info    = field_info_field($context['item_field']);
    $item_storage       = $item_field_info['storage']['details']['sql']['FIELD_LOAD_CURRENT'];
    $keys               = array_keys($item_storage);
    $this->item_table         = reset($keys);
    $keys               = array_keys($item_field_info['indexes']);
    $item_key           = reset($keys);
    $this->item_column        = $item_storage[$this->item_table][$item_key];
  }

  public function getErrors($delta = NULL, $dates = array()) {
    // Determine if reserving too many of the same item.
    if ($this->errors === NULL) {
      $this->validate();
      $entity = $this->entity;
      $entity_type = $entity->type();
      $context = $this->context;
      $errors = array();
      if ($this->items_is_list) {
        foreach ($entity->{$context['item_field']}->getIterator() as $delta => $resource) {
          $item_id = $resource->getIdentifier();
          if (empty($item_id)) {
            continue;
          }
          if (empty($item_count[$item_id])) {
            $item_count[$item_id] = 0;
          }
          $item_count[$item_id]++;
          try {
            $quantity_reservable = $resource->field_quantity->value();
          } catch (EntityMetadataWrapperException $e) {
            $quantity_reservable = 1;
          }
          if ($item_count[$item_id] > $quantity_reservable) {
            // Selected to many.
            if (!array_key_exists($delta, $errors)) {
              $errors[$delta] = array();
            }
            $parents_path = implode('][', array($context['item_field'], 'und', $delta, 'target_id'));
            $errors[$delta][MERCI_ERROR_TOO_MANY] =  t('%name: You have selected too many of the same item.  We only have %quantity available but you reserved %reserved.',
              array(
                '%name' => $resource->label(),
                '%quantity' => $quantity_reservable,
                '%reserved' => $item_count[$item_id],
              ));
          }
        }
      } else {

        $resource = $entity->{$context['item_field']};
        $item_count[$resource->getIdentifier()] = $entity->quantity->value();
        try {
          $quantity_reservable = $resource->field_quantity->value();
        } catch (EntityMetadataWrapperException $e) {
          $quantity_reservable = 1;
        }
        if ($entity->quantity->value() > $quantity_reservable) {
          if (!array_key_exists($delta, $errors)) {
            $errors[$delta] = array();
          }
          $errors[$delta][MERCI_ERROR_TOO_MANY] =  t('%name: You have selected too many of the same item.  We only have %quantity available but you reserved %reserved.',
            array(
              '%name' => $resource->label(),
              '%quantity' => $quantity_reservable,
              '%reserved' => $entity->quantity->value(),
            ));
          // Selected to many.
        }
      }

      $reserved = $this->getQuantityReserved();

      $reserved = $reserved ? $reserved : array();

      $reserved_so_far_by_me = array();

      foreach ($reserved as $delta => $start_dates) {

        $conflict_errors = array();

        // Load the resource being reserved.
        if ($this->items_is_list) {
          $resource  = $entity->{$context['item_field']}[$delta];
        } else {
          $resource  = $entity->{$context['item_field']};
        }

        // Determine if the quantity field exists.  If so use it.
        try {
          $quantity_reservable = $resource->field_quantity->value();
        } catch (EntityMetadataWrapperException $e) {
          $quantity_reservable = 1;
        }

        $item_id = $resource->getIdentifier();
        if (empty($reserved_so_far_by_me[$item_id])) {
          $reserved_so_far_by_me[$item_id] = 0;
        }
        $reserved_so_far_by_me[$item_id]++;

        foreach ($this->dates as $dates) {

          $quantity_reserved = $this->getQuantityReserved($delta, $dates);


          // Determine if there are conflicts for this date and item.
          if ($quantity_reservable >= $quantity_reserved + $reserved_so_far_by_me[$item_id]) {
            continue;
          }
          // Load each conflicting entity so we can show information about it to
          // the user.
          $ids = array();
          foreach ($this->getConflicts($delta, $dates) as $conflict) {
            $ids[] = $conflict->parent_id;
          }

          // Load the entities which hold the conflicting item.
          $entities = entity_load($entity_type, $ids);

          $line_items = array();

          foreach ($entities as $id => $line_item) {
            $entity_uri = entity_uri($entity_type, $line_item);
            $entity_label = entity_label($entity_type, $line_item);
            $line_items[] = l(t("@label", array('@label' => $entity_label)), $entity_uri['path']);
          }

          $date_start = $dates['value'];
          // Don't show the date repeat rule in the error message.
          unset ($dates['rrule']);

          $render_dates = field_view_value($entity_type, $entity->value(), $context['date_field'], $dates);
          $conflict_errors[$date_start] = t('%name is already reserved by: !items for selected dates !dates',
            array(
              '%name' => $resource->label(),
              '!items' => implode(', ', $line_items),
              '!dates' => render($render_dates),
            ));
        }
        if ($conflict_errors) {
          if (!array_key_exists($delta, $errors)) {
            $errors[$delta] = array();
          }
          $errors[$delta][MERCI_ERROR_CONFLICT] = $conflict_errors;
        }
      }
      $this->errors = $errors;
    }
    return $this->errors;
  }

  protected function validate() {
    if (!$this->validated) {
      $this->conflicting_entities = $this->conflicts();
      $this->validated = TRUE;
      foreach ($this->conflicting_entities as $delta => $dates) {
        foreach ($dates as $date_start => $conflicts) {
          if (!array_key_exists($delta, $this->quantity_reserved)) {
            $this->quantity_reserved[$delta] = array();
          }
          $this->quantity_reserved[$delta][$date_start] = 1;
        }
      }
    }
  }

  public function getConflicts($delta = NULL, $dates = array()) {

    $this->validate();
    $conflicts = $this->conflicting_entities;

    if ($delta === NULL) {
      return $conflicts;
    }

    if (empty($dates)) {
      return array_key_exists($delta, $conflicts) ?
        $conflicts[$delta] : FALSE;
    }

    $date_value = $dates['value'];
    return (array_key_exists($delta, $conflicts) and array_key_exists($date_value, $conflicts[$delta])) ?
      $conflicts[$delta][$date_value] : FALSE;
  }

  public function getQuantityReserved($delta = NULL, $dates = array()) {

    $this->validate();
    $quantity_reserved = $this->quantity_reserved;

    if ($delta === NULL) {
      return $quantity_reserved;
    }

    if (empty($dates)) {
      return array_key_exists($delta, $quantity_reserved) ?
        $quantity_reserved[$delta] : 0;
    }

    $date_value = $dates['value'];
    return (array_key_exists($delta, $quantity_reserved) and array_key_exists($date_value, $quantity_reserved[$delta])) ?
      $quantity_reserved[$delta][$date_value] : 0;
  }

  /*
   * Determine if merci_line_item $entity conflicts with any other existing line_items.
   *
   * Returns array of conflicting line items.
   */

  protected function conflicts() {
    $conflicts = array();
    foreach ($this->dates as $dates) {
      $date_value = $dates['value'];

      $query = $this->buildConflictQuery($dates);

      $result = $query->execute();
      foreach ($result as $record){
        if (!isset($conflicts[$record->item_id])) {
          $conflicts[$record->item_id] = array();
        }
        if (!isset($conflicts[$record->item_id][$date_value])) {
          $conflicts[$record->item_id][$date_value] = array();
        }
        $conflicts[$record->item_id][$date_value][] = $record;
      }

    }

    $return = array();
    foreach ($this->items as $delta => $item) {
      if (isset($conflicts[$item->getIdentifier()])) {
        $return[$delta] = $conflicts[$item->getIdentifier()];
      }
    }
    return $return;
  }

  protected function buildConflictQuery($dates) {
    $context = $this->context;

    $exclude_id   = $this->entity->getIdentifier();
    $entity_type  = $this->entity->type();
    $item_table   = $this->item_table;
    $item_column  = $this->item_column;
    $date_table   = $this->date_table;
    $date_column  = $this->date_column;
    $date_column2 = $this->date_column2;

    $items = array();

    foreach ($this->items as $delta => $item) {
      $items[] = $item->getIdentifier();
    }


    // Build the query.
    $query = db_select($item_table, 'item_table');
    $query->addField('item_table', $item_column, 'item_id');
    $query->addField('item_table', 'entity_id', 'parent_id');

    if (count($this->items) == 1) {
      $query->condition($item_column, reset($items));
    } else {
      $query->condition($item_column, $items, 'IN');
    }
    // Ignore myself.
    if ($exclude_id) {
      $query->condition('item_table.entity_id', $exclude_id, '!=');
    }

    $query->join('merci_line_item', 'merci_line_item', 'item_table.entity_id = merci_line_item.line_item_id');
    $query->addField('merci_line_item', 'quantity', 'quantity');
    $query->condition('merci_line_item.status', 1, '=');

    $query->join($date_table, 'date_table', 'item_table.entity_id = date_table.entity_id');
    $query->addField('date_table', $date_column);//, MERCI_DATE_FIELD_ALIAS);
    $query->addField('date_table', $date_column2);//, MERCI_DATE_FIELD_ALIAS2);
    $query->condition('date_table.entity_type',$entity_type,'=');
    $query->condition('date_table.deleted' , 0, '=');

    // TODO handled multiple dates.
    $query->condition(
      db_or()
      //  start falls within another reservation.
      //                     |-------------this-------------|
      //            |-------------conflict-------------------------|
      //            OR
      //                     |-------------this-------------------------------|
      //            |-------------conflict-------------------------|
      ->condition(
        db_and()->condition($date_column, $dates['value'], '<=')->condition($date_column2, $dates['value'], '>=')
      )
      //  end falls within another reservation.
      //                     |-------------this-------------------------------|
      //                                   |-------------conflict-------------------------|
      ->condition(
        db_and()->condition($date_column, $dates['value2'], '<=')->condition($date_column2, $dates['value2'], '>=')
      )
      //  start before another reservation.
      //  end after another reservation.
      //                     |-------------------------this-------------------------------|
      //                            |----------------conflict------------------|
      ->condition(
        db_and()->condition($date_column, $dates['value'], '>')->condition($date_column2, $dates['value2'], '<')
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
  protected $buckets;

  protected function validate() {
    if (!$this->validated) {
      $this->buckets = $this->fillBuckets();
      $this->validated = TRUE;
      $conflicts = array();
      foreach ($this->buckets as $delta => $dates) {
        foreach ($dates as $date_value => $buckets){
          if (!isset($this->quantity_reserved[$delta])) {
            $this->quantity_reserved[$delta] = array();
          }
          $this->quantity_reserved[$delta][$date_value] = count($buckets);
          if (!isset($conflicts[$delta])) {
            $conflicts[$delta] = array();
          }
          $conflicts[$delta][$date_value] = array();
          foreach ($buckets as $bucket) {
            $conflicts[$delta][$date_value] = array_merge($conflicts[$delta][$date_value], $bucket);
          }
        }
      }
      $this->conflicting_entities = $conflicts;
    }
  }

  public function fillBuckets() {
    $conflicts = array();
    foreach ($this->dates as $dates) {
      $date_value = $dates['value'];
      $result = $this->bestFit($dates);
      // Result is array indexed by $delta of filled buckets.
      foreach ($result as $delta => $buckets) {
        if (!isset($conflicts[$delta])) {
          $conflicts[$delta] = array();
        }
        $conflicts[$delta][$date_value] = $buckets;

      }
    }
    return $conflicts;
  }

  public function reservations($dates, $exclude_id) {
    $bestfit = $this->bestFit($dates);
    $reservations = array();
    foreach ($bestfit as $enity_id => $reservation) {
      $reservations[] = $entity_id;
    }
    return $reservations;
  }

  /*
   * Perform first-fit algorhtym on reservations into buckets.
   *
   * Return array indexed by item delta of array of filled buckets.
   */
  public function bestFit($dates) {

    $entity = $this->entity;
    $context = $this->context;
    $best_fit = array();


    $parent_conflicts = parent::conflicts($dates);

    $date_value = $dates['value'];

    foreach ($this->items as $delta => $item) {

      // No need to sort into buckets if there is nothing to sort into buckets.
      if (!array_key_exists($delta, $parent_conflicts) or !array_key_exists($date_value, $parent_conflicts[$delta])) {
        continue;
      }

      // Determine if the quantity field exists.  If so use it.
      try {
        $quantity = $item->{$context['quantity_field']}->value();
      } catch (EntityMetadataWrapperException $e) {
        $quantity = 1;
      }

      // Split reservations based on quantity. 
      $reservations = array();

      foreach($parent_conflicts[$delta][$date_value] as $reservation) {
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
      if (count($buckets)) {
        $best_fit[$delta] = $buckets;
      }
    }
    return $best_fit;
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