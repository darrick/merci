<?php

/**
 * UI controller providing UI for content entities.
 *
 * For a controller providing UI for bundleable content entities, see
 * EntityBundleableUIController.
 * For a controller providing admin UI for configuration entities, see
 * EntityDefaultUIController.
 */
class MerciRestrictionsUIController extends EntityBundleableUIController {

  /**
   * Provides definitions for implementing hook_menu().
   */
  public function hook_menu() {
    $items = parent::hook_menu();


    //$items[$this->path . '/add']['page callback'] = 'merci_restrictions_add_page';
    //$items[$this->path . '/add']['title callback'] = 'merci_restrictions_get_action_title';


    return $items;
  }

}

