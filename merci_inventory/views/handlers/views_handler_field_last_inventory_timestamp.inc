<?php

/**
 * @file
 * Definition of views_handler_field_last_comment_timestamp.
 */

/**
 * Field handler to display the timestamp of a comment with the count of comments.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_last_inventory_timestamp extends views_handler_field_date {
  function construct() {
    parent::construct();
    $this->additional_fields['inventory_count'] = 'inventory_count';
  }

  function render($values) {
    $inventory_count = $this->get_value($values, 'inventory_count');
    if (empty($this->options['empty_zero']) || $inventory_count) {
      return parent::render($values);
    }
    else {
      return NULL;
    }
  }
}
