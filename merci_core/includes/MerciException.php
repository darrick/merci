<?php

/**
 * @file
 * Contains EntityValidatorException.
 */

class MerciException extends Exception {


  protected $field;
  protected $delta; 
  protected $data;

  function __construct($msg, $field = '', $data = array()) {
    parent::__construct($msg);
    $this->field    = $field;
    $this->data    = $data;
  }

  public function getField () {
    return $this->field;
  }

  public function getData () {
    return $this->data;
  }

}


