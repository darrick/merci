<?php
/**
* Task Type class.
*/
class MerciReservationType extends Entity {
  public $type;
  public $label;
  public $weight = 0;
  public function __construct($values = array()) {
    parent::__construct($values, 'merci_reservation_type');
  }

  function isLocked() {
    return isset($this->status) && empty($this->is_new) && (($this->status & ENTITY_IN_CODE) || ($this->status & ENTITY_FIXED));
  }
}

class MerciReservationTypeController extends EntityAPIControllerExportable {
   public function create(array $values = array()) {
    $values += array(
      'label' => '',
    );
    return parent::create($values);
  }

  /**
   * Save Task Type.
   */
  public function save($entity, DatabaseTransaction $transaction = NULL) {
    parent::save($entity, $transaction);
    // Rebuild menu registry. We do not call menu_rebuild directly, but set
    // variable that indicates rebuild in the end.
    // @see _http://drupal.org/node/1399618
    variable_set('menu_rebuild_needed', TRUE);
  }

  public function delete($ids, DatabaseTransaction $transaction = NULL) {
    $entities = $ids ? $this->load($ids) : FALSE;
    if ($entities) {

      foreach ($entities as $id => $entity) {
        field_attach_delete_bundle('merci_reservation', $entity->type);
        $children = entity_load('merci_reservation', FALSE, array('type' => $entity->type));
        if (count($children)) {
          entity_delete_multiple('merci_reservation', array_keys($children));
        }
      }
      parent::delete($ids, $transaction);
    }
  }
}
