<?php

class MerciLineItemType extends Entity {

  protected function defaultLabel() {
    if (property_exists($this, 'label')) {
      return $this->label;
    }

    return "MERCI Line Item Type";
  }

  protected function defaultUri() {
    $entity_info = $this->entityInfo();
    return array('path' => $entity_info['admin ui']['path'] . '/' . $this->id);
  }
}
