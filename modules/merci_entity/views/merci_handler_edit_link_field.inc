<?php

/**
 * @file
 * Contains a Views field handler to take care of displaying edit links
 * as fields
 */


class merci_handler_edit_link_field extends merci_handler_link_field {
  function construct() {
    parent::construct();
    $this->additional_fields['type'] = 'type';
  }


  function render($values) {
    $type = $values->{$this->aliases['type']};
    
    //Creating a dummy merci to check access against
    $dummy_merci = (object) array('type' => $type);
    if (!merci_access('edit', $dummy_merci)) {
      return;
    }
    
    $text = !empty($this->options['text']) ? $this->options['text'] : t('edit');
    $merci_id = $values->{$this->aliases['merci_id']};
    
    return l($text, 'admin/content/merci/merci/' . $merci_id . '/edit');
  }
}
