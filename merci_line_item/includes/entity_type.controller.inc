<?php

/**
 * The controller class for line items contains methods for the line item CRUD
 * operations. The load method is inherited from the default controller.
 */
class MerciLineItemTypeController extends EntityAPIControllerExportable {
   public function create(array $values = array()) {
    $values += array(
      'label' => '',
    );
    return parent::create($values);
  }

  public function load($ids = array(), $conditions = array()) {
    $entity = parent::load($ids, $conditions);
    return $entity;
  }
}
