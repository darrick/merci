<?php

/**
 * @file
 * Contains a Views field handler to take care of displaying edit links
 * as fields
 */


class merci_reservation_detail_handler_edit_link_field extends merci_reservation_detail_handler_link_field {
  function construct() {
    parent::construct();
    $this->additional_fields['type'] = 'type';
  }


  function render($values) {
    $type = $values->{$this->aliases['type']};
    
    //Creating a dummy merci_reservation_detail to check access against
    $dummy_merci_reservation_detail = (object) array('type' => $type);
    if (!merci_reservation_detail_access('edit', $dummy_merci_reservation_detail)) {
      return;
    }
    
    $text = !empty($this->options['text']) ? $this->options['text'] : t('edit');
    $merci_reservation_detail_id = $values->{$this->aliases['merci_reservation_detail_id']};
    
    return l($text, 'admin/content/merci_reservation_details/merci_reservation_detail/' . $merci_reservation_detail_id . '/edit');
  }
}
