<?php

abstract class MerciBehavior {

  public $entity;

  function __construct($entity_type, $entity) {
    $this->entity = entity_metadata_wrapper($entity_type, $entity);
  }

  public function form_alter(&$form, $form_state, $form_id) {
  }

  public function validate($form, &$form_state) {
  }

  public function access() {
    return TRUE;
  }
}
