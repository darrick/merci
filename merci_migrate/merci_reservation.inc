<?php

class MerciReservationMigration extends Migration {
  public function __construct($arguments) {
    parent::__construct($arguments);


    $this->source = new MigrateSourceReservation();

    //$this->dependencies = array('MerciLineItem');
    $this->destination = new MigrateDestinationEntityAPI('merci_reservation', 'checkout');

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'nid' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
        )
      ),
      MigrateDestinationEntityAPI::getKeySchema('merci_reservation')
    );


    $this->addFieldMapping('merci_line_item_reference', 'reservation_items')->sourceMigration('MerciLineItem');
    $this->addFieldMapping('field_owner', 'uid');
    $this->addFieldMapping('field_operator', 'uid');
    $this->addFieldMapping('created', 'created');

    /*
    $this->addFieldMapping('merci_checkout_dates', 'field_merci_date_value');
    $this->addFieldMapping('merci_checkout_dates:to', 'field_merci_date_value2');
    $this->addFieldMapping('merci_reservation_status', 'merci_checkout_status');
    $this->addFieldMapping('quantity')->defaultValue(1);
    $this->addFieldMapping('merci_resource_reference', 'merci_item_nid')->sourceMigration('MerciItem');
    $this->addFieldMapping('line_item_label', 'title');
    $this->addFieldMapping('merci_checkout_status', 'merci_reservation_status');
    //$this->addFieldMapping('merci_resource_display', 'resource_display_id');
    $this->addFieldMapping('entity_id','nid'); 
     */
  }


}


class MigrateSourceReservation extends MigrateSourceSQL {

  // Your constructor will initialize any parameters to your migration. It's
  // important to pass through the options, so general options such as
  // cache_counts will work.
  public function __construct() {

    $query = db_select('node', 'n')
      ->condition('n.type', 'merci_reservation')
      ->condition('n.status',1,'=')
      ->fields('n');

    parent::__construct($query);
  }

  public function fields($migration = NULL) {
    $properties = entity_get_property_info('node');
    $fields = array();

    foreach ($properties['properties'] as $name => $property_info) {
      if (isset($property_info['setter callback'])) {
        $fields[$name] = $property_info['description'];
      }
    }

    // Then add in anything provided by handlers
    $fields += migrate_handler_invoke_all('Entity', 'fields', 'node', 'merci_reservation');

    return $fields;
  }
  /**
   * Implementation of MigrateSource::getNextRow().
   *
   * @return object
   */
  public function getNextRow() {

    $row = parent::getNextRow();

    if ($row) {
      $node = node_load($row->nid);

      if (is_object($node)) {
        $node->merci_reservations = merci_reservation_items($node);
        $node->reservation_items = array();
        foreach ($node->merci_reservations as $reservation) {
          $node->reservation_items[] = array(
            $reservation['merci_item_nid'],
            $row->nid,
          );
        }
        return $node;
      }
      else {
        return NULL;
      }
    }
    else {
      return NULL;
    }
  }

}



class MigrateDestinationReservation extends MigrateDestinationEntityAPI {
  protected $bypassDestIdCheck = FALSE;

  static public function getKeySchema() {
    return array(
      'id' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'description' => 'Primary Key: Unique ID.',
      ),
    );
  }

  public function __construct($bundle, array $options = array()) {
      parent::__construct('merci_line_item', $bundle, $options);
  }

  /**
   * Import a single node.
   *
   * @param $node
   *  Node object to build. Prefilled with any fields mapped in the Migration.
   * @param $row
   *  Raw source data object - passed through to prepare/complete handlers.
   * @return array
   *  Array of key fields (nid only in this case) of the node that was saved if
   *  successful. FALSE on failure.
   */
  public function import(stdClass $object, stdClass $row) {
    // Updating previously-migrated content?
    $migration = Migration::currentMigration();

    if (!isset($object->type)) {
      // Default the type to our designated destination bundle (by doing this
      // conditionally, we permit some flexibility in terms of implementing
      // migrations which can affect more than one type).
      $object->type = $this->bundle;
    }

    // Invoke migration prepare handlers
    $this->prepare($object, $row);


    // Validate field data prior to saving.
    MigrateDestinationEntity::fieldAttachValidate('merci_line_item', $object);

    migrate_instrument_start('merci_line_item_save');
    entity_save('merci_line_item', $object);
    migrate_instrument_stop('merci_line_item_save');

    if (isset($object->line_item_id)) {
      $this->numCreated++;


      $return = array($object->line_item_id);
    }
    else {
      $return = FALSE;
    }

    $this->complete($object, $row);
    return $return;
  }

  public function rollback(array $id) {
    static $count = 0;
    migrate_instrument_start('merci_line_item_delete');
    $this->prepareRollback($id);
    entity_delete('merci_line_item', reset($id));
    $this->completeRollback($id);
    migrate_instrument_stop('merci_line_item_delete');
  }
}

