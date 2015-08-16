<?php

class MerciTaxonomyRestrictionsMigration extends Migration {
  public function __construct($arguments) {
    parent::__construct($arguments);

    $this->source = new MerciContentTypesMigrateSource();

    $this->destination = new MigrateDestinationRestrictions('merci_restrictions');

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'merci_item_grouping' => array(
          'type' => 'varchar',
          'length' => 32,
          'not null' => TRUE,
        )
      ),
      MigrateDestinationRestrictions::getKeySchema()
    );

    $this->addFieldMapping('field_max_length_of_checkout', 'merci_max_hours_per_reservation');
    $this->addFieldMapping('field_allow_overnight', 'merci_allow_overnight');
    $this->addFieldMapping('field_allow_weekends', 'merci_allow_weekends');
    $this->addFieldMapping('title', 'merci_item_grouping');
  }

}

class MerciItemRestrictionsMigration extends Migration {
  public function __construct($arguments) {
    parent::__construct($arguments);

    $this->source = new MerciContentTypesMigrateSource();

    $this->destination = new MigrateDestinationRestrictions('merci_restrictions');

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'type' => array(
          'type' => 'varchar',
          'length' => 32,
          'not null' => TRUE,
          'alias' => 'mt',
        )
      ),
      MigrateDestinationRestrictions::getKeySchema()
    );

    $this->addFieldMapping('field_max_length_of_checkout', 'merci_max_hours_per_reservation')->callbacks(array($this, 'modifyMaxHours'));
    $this->addFieldMapping('field_allow_overnight', 'merci_allow_overnight');
    $this->addFieldMapping('field_allow_weekends', 'merci_allow_weekends');
    $this->addFieldMapping('field_auto_checkout', 'merci_autocheckout');
    $this->addFieldMapping('field_auto_checkin', 'merci_autocheckin');
    $this->addFieldMapping('field_self_checkout', 'merci_selfcheckout');
    $this->addFieldMapping('field_minimum_cancelation_time', 'merci_min_cancel_hours');
    $this->addFieldMapping('title', 'type_name');
    $this->addFieldMapping('field_allowed_roles', 'rids');
  }

  protected function modifyMaxHours($value) {
    if (!empty($value)) {
      return '+' . $value . ' hours';
    }

    return $value;
  }

  public function prepareRow($row) {
    $result = db_query("SELECT rid FROM {role_permission} WHERE permission = :permission", array(':permission' => 'edit own ' . $row->type . ' content'));
    $row->rids = array();
    foreach ($result as $role) {
      $enabled = db_query("SELECT count(rid) FROM {role_permission} WHERE permission = 'merci permissions allowed role' AND rid = :rid", array(':rid' => $role->rid))->fetchField();  

      if (!$enabled) {
        user_role_grant_permissions($role->rid, array('merci permissions allowed role'));
      }

      $row->rids[] = $role->rid;
    }
    return TRUE;
  }

}

/**
 * @file
 * Support for node destinations.
 */

// TODO:
// Make sure this works with updates, explicit destination keys

/**
 * Destination class implementing migration into nodes.
 */
class MigrateDestinationRestrictions extends MigrateDestinationEntity {
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
      parent::__construct('merci_restrictions', $bundle, $options);
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
    $fields['id'] = t('Entity ID');
    $fields['type'] = t('Type');
    $fields['title'] = t('Title');

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
    MigrateDestinationEntity::fieldAttachValidate('merci_restrictions', $object);

    migrate_instrument_start('merci_restrictions__save');
    entity_save('merci_restrictions', $object);
    migrate_instrument_stop('merci_restrictions_save');

    if (isset($object->id)) {
      $this->numCreated++;


      $return = array($object->id);
    }
    else {
      $return = FALSE;
    }

    $this->complete($object, $row);
    return $return;
  }

  public function rollback(array $id) {
    static $count = 0;
    migrate_instrument_start('merci_restrictions_delete');
    $this->prepareRollback($id);
    entity_delete('merci_restrictions', reset($id));
    $this->completeRollback($id);
    migrate_instrument_stop('merci_restrictions_delete');
  }
}

