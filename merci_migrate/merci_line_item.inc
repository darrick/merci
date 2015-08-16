<?php

class MerciLineItemMigration extends Migration {
  public function __construct($arguments) {
    parent::__construct($arguments);

    $query = db_select('merci_reservation_detail', 'mrd')
      ->fields('mrd');

    $query->join('node', 'n', 'n.nid = mrd.merci_item_nid');
    $query->join('merci_reservation_old', 'mrs', 'mrs.nid = mrd.nid');
    $query->join('field_data_field_merci_date', 'fdfmd', "mrd.nid = fdfmd.entity_id AND entity_type = 'node'");

    $query->fields('n')->fields('mrs')->fields('fdfmd');
    $this->source = new MigrateSourceSQL($query);

    $this->dependencies = array('MerciItem', 'MerciItemDisplay');
    $this->destination = new MigrateDestinationLineItem('merci_line_item');

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'merci_item_nid' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
        ),
        'nid' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
          'alias' => 'mrd',
        )
      ),
      MigrateDestinationLineItem::getKeySchema()
    );


    $this->addFieldMapping('merci_checkout_dates', 'field_merci_date_value');
    $this->addFieldMapping('merci_checkout_dates:to', 'field_merci_date_value2');
    $this->addFieldMapping('merci_reservation_status', 'merci_checkout_status');
    $this->addFieldMapping('quantity')->defaultValue(1);
    $this->addFieldMapping('merci_resource_reference', 'merci_item_nid')->sourceMigration('MerciItem');
    $this->addFieldMapping('line_item_label', 'title');
    $this->addFieldMapping('merci_checkout_status', 'merci_reservation_status');
    //$this->addFieldMapping('merci_resource_display', 'resource_display_id');
    $this->addFieldMapping('entity_id','nid'); 
  }

  public function prepareRow($row) {
    $status = array(
      1 => 'pending',
      2 => 'reserved',
      3 => 'checked out',
      4 => 'returned',
    );

    if ($row->merci_reservation_status <= 4) {

      $row->merci_reservation_status = $status[$row->merci_reservation_status];
    } else {
      $row->merci_reservation_status = 'returned';
    }
    $row->field_merci_date_value = $row->field_merci_date_value . ' UTC';
    $row->field_merci_date_value2 = $row->field_merci_date_value2 . ' UTC';
    return TRUE;
  }

}



class MigrateDestinationLineItem extends MigrateDestinationEntity {
  protected $bypassDestIdCheck = FALSE;

  static public function getKeySchema() {
    return array(
      'line_item_id' => array(
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
   * Returns a list of fields available to be mapped for the node type (bundle)
   *
   * @param Migration $migration
   *  Optionally, the migration containing this destination.
   * @return array
   *  Keys: machine names of the fields (to be passed to addFieldMapping)
   *  Values: Human-friendly descriptions of the fields.
   */
  public function fields($migration = NULL) {
    $fields = array();
    // First the core (node table) properties
    $fields['line_item_id'] = t('Entity ID');
    $fields['type'] = t('Type');
    $fields['line_item_label'] = t('Title');
    $fields['entity_id'] = t('Parent ID');
    $fields['quantity'] = t('Quantity');
    $fields['created'] = t('Created');
    $fields['changed'] = t('Changed');

    // Then add in anything provided by handlers
    $fields += migrate_handler_invoke_all('Entity', 'fields', $this->entityType, $this->bundle, $migration);

    return $fields;
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

/**
 *  * @todo Please document this function.
 *   * @see http://drupal.org/node/1354
 *    */
function merci_reservation_status($node) {
  return db_query("SELECT merci_reservation_status FROM {merci_reservation} WHERE vid = :vid", array(':vid' => $node->vid))->fetchField();
}

/**
 *  * @todo Please document this function.
 *   * @see http://drupal.org/node/1354
 *    */
function merci_reservation_items($node) {
  $items = db_query("SELECT m.did, m.merci_item_status, merci_placeholder_nid, pn.title AS placeholder_title, merci_item_nid, tn.title AS item_title, nt.type, nt.name FROM {merci_reservation_detail} m INNER JOIN {node} pn ON m.merci_placeholder_nid = pn.nid INNER JOIN {node_type} nt ON pn.type = nt.type LEFT JOIN {node} tn ON m.merci_item_nid = tn.nid WHERE m.vid = :vid", array(':vid' => $node->vid));

  $reservation_items = array();

  foreach ($items as $item) {
    $item = (array) $item;
    $reservation_items[$item['did']] = $item;
  }
  return $reservation_items;
}
