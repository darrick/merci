<?php

class MerciTaxonomyMigration extends Migration {
  public function __construct($arguments) {
    parent::__construct($arguments);

    // Set source.
    // Multiple content types to new single item type.
    //
    // foreach merci_reservable items as content_type ...
    // .. create new content_type.
    // .. migrate items to new content_type.
    $this->source = new MerciContentTypesMigrateSource(array('map_joinable' => FALSE));

    $this->destination = new MigrateDestinationTerm('resource_type');

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'merci_item_grouping' => array(
          'type' => 'varchar',
          'length' => 32,
          'not null' => TRUE,
        )
      ),
      MigrateDestinationTerm::getKeySchema()
    );
    $this->addFieldMapping('name', 'merci_item_grouping');
    //$this->addFieldMapping('field_restrictions', 'merci_item_grouping')->sourceMigration('MerciTaxonomyRestrictions');

  }

}

