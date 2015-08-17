<?php

/**
 * @file
 * Contains EntityValidatorException.
 */

class MerciException extends Exception {


  protected $parents;
  protected $delta; 
  protected $data;

  function __construct($msg, $parents = '', $delta = 0, $data = array()) {
    parent::__construct($msg);
    $this->parents    = $parents;
    $this->delta = $delta;
    $this->data    = $data;
  }

  public function getParents () {
    return $this->parents;
  }

  public function getDelta () {
    return $this->delta;
  }

  public function getData () {
    return $this->data;
  }

}


