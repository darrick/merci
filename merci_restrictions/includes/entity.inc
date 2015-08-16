<?php
/**
* Task class.
*/
class MerciRestrictions extends Entity {
  protected function defaultLabel() {
    if (property_exists($this, 'id')) {
      return "Checkout " . $this->id;
    }

    return "Checkout";
  }
  protected function defaultUri() {
    $entity_info = $this->entityInfo();
    return array('path' => $entity_info['admin ui']['path'] . '/' . $this->id);
  }
}
