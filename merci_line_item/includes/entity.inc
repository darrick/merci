<?php

class MerciLineItem extends Entity {

  protected function defaultLabel() {
    if (property_exists($this, 'line_item_id')) {
      return "MERCI Line Item " . $this->line_item_id;
    }

    return "MERCI Line Item";
  }

  protected function defaultUri() {
    $entity_info = $this->entityInfo();
    return array('path' => $entity_info['admin ui']['path'] . '/' . $this->line_item_id);
  }
}
