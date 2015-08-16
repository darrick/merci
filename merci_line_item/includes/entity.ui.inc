<?php

/**
 * UI controller providing UI for content entities.
 *
 * For a controller providing UI for bundleable content entities, see
 * EntityBundleableUIController.
 * For a controller providing admin UI for configuration entities, see
 * EntityDefaultUIController.
 */
class MerciLineItemUIController extends EntityBundleableUIController {

  /**
   * Provides definitions for implementing hook_menu().
   */
  public function hook_menu() {
    $items = parent::hook_menu();

    foreach (merci_line_item_types() as $type => $line_item_type) {
      // Convert underscores to hyphens for the menu item argument.
      //    $type_arg = strtr($type, '_', '-');
      //

      /*
      $items['admin/merci/merci_line_item_types/' . $type] = array(
        'title' => $line_item_type->name,
        'page callback' => 'merci_line_item_type_redirect',
        'page arguments' => array($type),
        'access arguments' => array('administer merci line item types'),
      );
       */
    }

    return $items;
  }

}

