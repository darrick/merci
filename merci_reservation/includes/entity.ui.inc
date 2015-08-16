<?php

/**
 * UI controller providing UI for content entities.
 *
 * For a controller providing UI for bundleable content entities, see
 * EntityBundleableUIController.
 * For a controller providing admin UI for configuration entities, see
 * EntityDefaultUIController.
 */
class ReservationUIController extends EntityBundleableUIController {

  /**
   * Provides definitions for implementing hook_menu().
   */
  public function hook_menu() {
    $items = parent::hook_menu();


    $items[$this->path . '/add']['page callback'] = 'merci_reservation_add_page';
    $items[$this->path . '/add']['title callback'] = 'merci_reservation_get_action_title';


    return $items;
  }

}

