<?php
/**
 * A null implementation of EntityReference_SelectionHandler.
 */
class Merci_ReservableHandler_Generic_bucket implements Merci_ReservableHandler_Generic {

  protected function __construct($entity_type, $entity, $preset) {
    $this->entity_type = $entity_type;
    $this->entity      = $entity;
    $this->preset      = $preset;
    // Get entity ids and language.
    list($entity_id, $vid, $bundle) = entity_extract_ids($entity_type, $entity);
    $this->entity_id = $entity_id;
  }

  public function itemCount() {
    $items = $bucket->{$bucket_field}[$langcode];
    return count($items);
  }

  public function reservations($dates) {
    $bestfit = $this->bestFit($dates);
    $reservations = array();
    foreach ($bestfit as $enity_id => $reservation) {
      $reservations[] = $entity_id;
    }
    return $reservations;
  }

  public function availability($dates) {
    $reservations = $this->reservations($dates);
    if (count($reservations >= $this->count())) {
      return FALSE;
    }
    return TRUE;
  }

  public function bestFit($dates) {
    $target_field = $this->preset['settings']['target_field'];
    $date_field   = $this->preset['settings']['date_field'];
    $bucket_field = $this->preset['settings']['bucket_field'];
    $target_id    = $this->entity_id;
    $target_type  = $this->entity_type;
    $bucket       = $this->entity;

    $target_field_info = field_read_field($target_field);

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


    // Add ourselves to the list of reservations.
    // Storage location for date field.
    $bucket_reservations[0] = new StdClass;
    $bucket_reservations[0]->{MERCI_DATE_FIELD_ALIAS} = $dates['value']; 
    $bucket_reservations[0]->{MERCI_DATE_FIELD_ALIAS2} = $dates['value2']; 
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
      foreach ($item_reservations as $target_id => $reservations) {
        foreach ($reservations as $reservation) {
          if (merci_bucket_intersects($reservation, $bucket_reservation)) {
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
}
