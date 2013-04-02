<?php

/**
 * This field handler aggregates operations that can be done on a merci
 * under a single field providing a more flexible way to present them in a view
 */
class merci_handler_merci_operations_field extends views_handler_field {
  function construct() {
    parent::construct();

    $this->additional_fields['merci_id'] = 'merci_id';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {

    $links = menu_contextual_links('merci', 'admin/content/merci/merci', array($this->get_value($values, 'merci_id')));
    
    if (!empty($links)) {
      return theme('links', array('links' => $links, 'attributes' => array('class' => array('links', 'inline', 'operations'))));
    }
  }
}
