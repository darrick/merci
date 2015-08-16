<?php 

class MerciItemDisplayMigration extends Migration {
  public function __construct($arguments) {
    parent::__construct($arguments);

    $this->source = new MerciContentTypesMigrateSource();

    $this->dependencies = array('MerciItem', 'MerciTaxonomy', 'MerciItemRestrictions');
    $this->destination = new MigrateDestinationNode('merci_resource_display');

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'type' => array(
          'type' => 'varchar',
          'length' => 32,
          'not null' => TRUE,
          'alias' => 'mt',
        )
      ),
      MigrateDestinationNode::getKeySchema()
    );

    $this->addFieldMapping('title', 'type_name');
    $this->addFieldMapping('body', 'description');
    $this->addFieldMapping('field_resource_type', 'merci_item_grouping');
    //$this->addFieldMapping('field_crp_product_reference', 'skus')->sourceMigration('MerciItem');
    $this->addFieldMapping('field_crp_product_reference', 'skus');
    $this->addFieldMapping('uid')->defaultValue(1);
    $this->addFieldMapping('field_replacement_cost')->defaultValue(0); 
    $this->addFieldMapping('field_late_fine', 'merci_late_fee_per_hour')->defaultValue(0); 
    $this->addFieldMapping('field_restrictions', 'type')->sourceMigration('MerciItemRestrictions');
  }

  public function prepare($node, stdClass $row) {
  //  drush_print_r($node);
   // drush_print_r($row);
  }
}


class MerciContentTypesMigrateSource extends MigrateSourceSQL {

  protected $merci_types;

  protected $current_type;

  // Your constructor will initialize any parameters to your migration. It's
  // important to pass through the options, so general options such as
  // cache_counts will work.
  public function __construct(array $options = array()) {

    $fields = array(
      'type' => t('Type'), 
      'merci_type_setting' => t('Type Setting'), 
      'merci_max_hours_per_reservation' => t('Max hours'),
      'merci_allow_overnight' => t('Allow overnight'),
      'merci_allow_weekends' => t('Allow weekends'),
      'merci_late_fee_per_hour' => t('late fee/hour'),
      'merci_rate_per_hour' => t('rate/hour'),
      'merci_fee_free_hours' => t('fee free hours'),
      'merci_active_status' => t('Active status'),
      'merci_spare_items' => t('Spare items'),
      'merci_min_cancel_hours' => t('Minimum cancelation hours'),
      'merci_autocheckout' => t('Auto checkout'),
      'merci_autocheckin' => t('Auto checkin'),
      'merci_selfcheckout' => t('Self checkout'),
      'merci_grouping' => t('Item grouping'),
      'merci_auto_assign_bucket_item' => t('Auto assign bucket'),
    );

    $query = db_select('merci_node_type', 'mt')
      ->condition('merci_type_setting', 'disabled', '<>')
      ->fields('mt', array_keys($fields));

    $table_alias = $query->join('node_type', 'nt', 'nt.type = mt.type');

    $query
      ->fields('nt',array('name','description'));

    parent::__construct($query, array(), NULL, $options);

  }

  /**
   * Downloads the next set of data from the source database.
   */
  protected function getSkus($merci_type) {

    $query = new EntityFieldQuery();
    $entities = $query->entityCondition('entity_type', 'node')
      ->propertyCondition('type', $merci_type->type)
      ->propertyCondition('status', 1)
      ->execute();
    if ($entities) {
      $skus = array();
      $ids = array_keys($entities['node']);
      $products = node_load_multiple($ids);

      foreach ($products as $item) {
        //$skus[] = $product->title;
        $product = commerce_product_load_by_sku($item->title);
        $skus[] = $product->product_id;
      }
      return $skus;
    } else {
      return NULL;
    }

  }
  
  /**
   * Implementation of MigrateSource::getNextRow().
   *
   * @return object
   */
  public function getNextRow() {

    $row = parent::getNextRow();

    if($row) {

      $row->skus = $this->getSkus($row);

      $row->type_name = $row->name;

      $grouping = $row->merci_type_setting == 'resource' ? $row->type_name : t('Buckets');
      $tid = variable_get('merci_grouping_' . $row->type, 0);
      if ($tid) {
        $term = taxonomy_term_load($tid);
        if ($term) {
          $grouping = $term->name;
        }
      }
      $row->merci_item_grouping = $grouping;
    }

    return $row;
  }


}

