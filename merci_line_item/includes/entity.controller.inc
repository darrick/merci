<?php

/**
 * The controller class for line items contains methods for the line item CRUD
 * operations. The load method is inherited from the default controller.
 */
class MerciLineItemController extends EntityAPIController {
  /**
   * Create a new entity.
   *
   * @param array $values
   *   An array of values to set, keyed by property name.
   * @return
   *   A new instance of the entity type.
   */
  public function create(array $values = array()) {
    global $user;
    $values += array(
      'line_item_id' => NULL,
      'entity_id' => 0,
      'type' => MERCI_LINE_ITEM,
      'line_item_label' => '',
      'quantity' => 1,
      'created' => '',
      'changed' => '',
      'owner' => $user->name,
      'data' => array(),
    );
    return parent::create($values);
  }
}
