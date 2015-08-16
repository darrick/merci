<?php

/**
 * @file
 * Definition of views_handler_filter_string.
 */

/**
 * Basic textfield filter to handle string filtering commands
 * including equality, like, not like, etc.
 *
 * @ingroup views_filter_handlers
 */
class merci_commerce_handler_filter_string extends views_handler_filter_string {

  /**
   * Provide a simple textfield for equality
   */
  function value_form(&$form, &$form_state) {
    parent::value_form($form, $form_state);

    if ($this->operator == 'word') {
      $form['value']['#type'] = 'textarea';
      unset($form['value']['#size']);
    }
  }

  function op_word($field) {
    $this->value = preg_replace("/[\n\r]/"," ",$this->value);
    return parent::op_word($field);
  }

}
