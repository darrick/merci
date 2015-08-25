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

    $field_date   = MERCI_CHECKOUT_DATES;
    $field_resource = MERCI_RESOURCE_REFERENCE;

    $line_item_wrapper = entity_metadata_wrapper('merci_line_item', $entity);

    // No conflict checking or returned, missing or broken items.
    if (empty($entity->status)) {
      return;
    }

    // Check of conflicts.
    $context = array(
      'quantity_field' => 'quantity',
      'date_field' => $field_date,
      'item_field' => $field_resource,
    );

    $controller = merci_get_controller($line_item_wrapper, 'non_inventory', $context);
    /*
    $resource  = $line_item_wrapper->{$context['item_field']};

    // Determine if the quantity field exists.  If so use it.
    try {
      $quantity = $resource->{$context['quantity_field']}->value();
    } catch (EntityMetadataWrapperException $e) {
      $quantity = 1;
    }
     */

    $quantity = 1;

    // Returns array of $product_id => $line_item_id
    $reserved = $controller->getQuantityReserved();
    if ($reserved) {
      throw new MerciException('MERCI conflict errors', MERCI_RESOURCE_REFERENCE, $controller);
    }
      //throw new MerciException('MERCI conflict errors', MERCI_RESOURCE_REFERENCE, $conflicts);
  }
}
