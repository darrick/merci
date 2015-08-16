<?php
/**
 * A null implementation of EntityReference_SelectionHandler.
 */
class MerciDefaultTargetController_bucket extends MerciDefaultTargetController {

  public bucket_field = 'merci_bucket_field';

  public function conflicts() {
    $query = $this->buildConflictQuery();

    $result = $query->execute();
    $conflicts = array();
    foreach ($result as $record){
      if (!array_key_exists($record->target_id, $conflicts)) {
        $conflicts[$record->target_id] = array();
      } 
      $conflicts[$record->target_id] = $record->reservation_id;
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

  public function bestFit() {
    $target_field = $this->target_field;
    $date_field   = $this->date_field;
    $bucket_field = $this->bucket_field;
    $dates        = $this->dates;
    $targets      = $this->targets;

    if (!$bucket or !property_exists($bucket, $bucket_field)) {
      return;
    }

    $langcode = $bucket->language ? $bucket->language : LANGUAGE_NONE;

    // Get all the items in the bucket.
    $items = $bucket->{$bucket_field}[$langcode];
    $max = count($items);

    // Temp array to store all the bucket items tied to their reservations during the given time period.
    $item_reservations = array();

    // Reservations reserving the item directly.
    foreach ($items as $target) {
      $entities = entity_load($target_type, array($target['target_id']));
      $node = reset($entities);
      $handler = Merci_ReservableHandler_Generic::getInstance($target_type, $node, $this->preset); 

      // Check availability
      $reservations = $handler->reservations($dates);
      // Is it available?
      $item_reservations[$target['target_id']] = $reservations;
    }

    $handler = Merci_ReservableHandler_Generic_resource::getInstance($target_type, $node, $this->preset); 

    // Other bucket reservations.
    $bucket_reservations = $handler->reservations($dates);

    $value = $date_field . '_value';
    $value2 = $date_field . '2_value';

    // Add ourselves to the list of reservations.
    // Storage location for date field.
    $bucket_reservations[0] = new StdClass;
    $bucket_reservations[0]->{$value} = $dates['value']; 
    $bucket_reservations[0]->{$value2} = $dates['value2']; 
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
    uasort($bucket_reservations, "self::merci_bucket_cmp_length");

    // First-fit algorythm.
    foreach ($bucket_reservations as $reservation_id => $bucket_reservation) {

      // Go through each bucket item to look for a available slot for this reservation.
      foreach ($item_reservations as $target_id => $reservations) {
        foreach ($reservations as $reservation) {
          if (self::merci_bucket_intersects($reservation, $bucket_reservation)) {
            //Conflict so skip saving the reservation to this slot and try to use the next bucket item.
            goto end;
          }
        }
        // We've found a slot so test the next reservation.
        $item_reservations[$target_id][$bucket_reservation->entity_id] = $bucket_reservation;
        $bucket_reservations[$reservation_id]->item_id = $target_id;

        // Look for a slot for the next reservation.
        break;
        end:
      }
      // No slots for this reservation.
      if (!property_exists($bucket_reservations[$reservation_id], 'item_id')){
        return ($bucket_reservations);
      }

    }
    return ($bucket_reservations);
  }

/*
 * |----------------------|        range 1
 * |--->                           range 2 overlap
 *  |--->                          range 2 overlap
 *                        |--->    range 2 overlap
 *                         |--->   range 2 no overlap
 */
  private static function merci_bucket_intersects($r1, $r2, $date_field_name) {
    $value = $date_field_name . '_value';
    $value2 = $date_field_name . '2_value';
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

  private static function merci_bucket_cmp_length($a, $b, $date_field_name) {
    $value = $date_field_name . '_value';
    $value2 = $date_field_name . '2_value';
    $len_a = date_format(date_create($a->{$value2}),'U') - date_format(date_create($a->{$value}), 'U');  
    $len_b = date_format(date_create($b->{$value2}),'U') - date_format(date_create($b->{$value}), 'U');  
    if ($len_a == $len_b) {
      return 0;
    }
    return ($len_a < $len_b) ? 1 : -1;
  }

}
