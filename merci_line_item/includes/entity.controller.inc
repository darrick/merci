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
      'uid' => $user->uid,
      'status' => 1,
      'data' => array(),
    );
    return parent::create($values);
  }

  public function validate($entity) {

    // No conflict checking or returned, missing or broken items.
    if (empty($entity->status)) {
      return;
    }

    // Check of conflicts.
    $context = array(
      'quantity_field' => 'field_quantity',
      'date_field' => MERCI_CHECKOUT_DATES,
      'item_field' => MERCI_RESOURCE_REFERENCE,
    );

    $line_item_wrapper = entity_metadata_wrapper('merci_line_item', $entity);

    $controller = merci_get_controller($line_item_wrapper, 'non_inventory', $context);

    if ($controller->getErrors()) {
      throw new MerciException('MERCI conflict errors', MERCI_RESOURCE_REFERENCE, $controller);
    }
  }
}
