<?php

// Bucket/resource sub-types.
define('MERCI_SUB_TYPE_ITEM', 1);
define('MERCI_SUB_TYPE_RESERVATION', 2);

class MerciItemMigration extends Migration {
  public function __construct($arguments) {
    parent::__construct($arguments);

    $this->source = new MerciItemMigrateSource();

    // Entity type, and bundle.
    $this->destination = new MigrateDestinationCommerceProduct('commerce_product', 'merci_resource');

    $this->map = new MigrateSQLMap(
      $this->machineName,
      array(
        'nid' => array( 
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
        )
      ),
      MigrateDestinationEntityAPI::getKeySchema('commerce_product') 
      /*
      array(
        'product_id' => array( 
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
        )
      )
       */
    );

    $this->addFieldMapping('sku', 'title');
    $this->addFieldMapping('title', 'title');
    $this->addFieldMapping('field_part_description', 'body');
    $this->addFieldMapping('field_quantity')->defaultValue(1);
    $this->addFieldMapping('commerce_price')->defaultValue(0);
    $this->addFieldMapping('status', 'merci_default_availability'); 

  }

  public function prepareRow($row) {
    $row->merci_default_availability = $row->merci_default_availability == 1 ? 1 : 0;
    return TRUE;
  }
  public function prepare($node, stdClass $row) {
  }
}


class MerciItemMigrateSource extends MigrateSourceSQL {

  // Your constructor will initialize any parameters to your migration. It's
  // important to pass through the options, so general options such as
  // cache_counts will work.
  public function __construct() {

    /*
    $merci_settings = db_query("SELECT * FROM {merci_node_type} mt INNER JOIN {node_type} nt ON mt.type = nt.type WHERE merci_type_setting <> :merci_type_setting", array(':merci_type_setting' => 'disabled'));

    $types = array();

    foreach ($merci_settings as $merci_setting) {
      $types[] = $merci_setting->type;
    }
     */
    $query = db_select('node', 'n')
      ->fields('n')
      ->condition('n.status',1,'=');

    $query->join('merci_node_type', 'mnt', 'n.type = mnt.type');

    $query->condition('mnt.merci_type_setting','disabled','<>');

    $query->fields('mnt');

    parent::__construct($query);
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
        merci_migrate_legacy_node_load($node);
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


/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function merci_migrate_legacy_node_load(&$node) {
  // Process active MERCI node types and reservation nodes.
  $settings = merci_load_item_settings($node);
  foreach ($settings as $key => $value) {
    $node->{$key} = $value;
  }
}

// Loads the current settings for reservable item nodes.
/* If you just want the content type settings just pass only node->type.
 */

function merci_load_item_settings($object) {

  // Allow either a node object or type name to be passed in.
  if (is_string($object)) {
    $type = $object;
  }
  else {
    $node = (array) $object;
    $type = $node['type'];
  }

  // Initialize here in case there is no nid.
  $item_settings = array();

  // Settings from the content type edit page.
  $content_settings = merci_content_types($type);

  if (isset($node['nid'])) {
    // Settings common to all merci item nodes.
    // resource or bucket.
    $merci_type = isset($content_settings['merci_type_setting']) ? $content_settings['merci_type_setting'] : '';
    
    $vid = $node['vid'];
    
    $item_settings = merci_reservation_item_node_settings($vid);
    switch ($merci_type) {
      case 'bucket':
        // TODO: move to seperate module.
        if ($item_settings['merci_sub_type'] == MERCI_SUB_TYPE_RESERVATION) {
          unset($item_settings['merci_default_availability']);
          unset($item_settings['merci_item_status']);
          $item_settings += merci_bucket_node_settings($vid);
        }
        break;

      case 'resource':
        // TODO: move to seperate module.
        $item_settings += merci_resource_node_settings($vid);
        break;
    }
  }
  return (object) ($item_settings + $content_settings);
}

function merci_bucket_node_settings($vid) {
  if (!$vid) {
    return;
  }
  return db_query("SELECT merci_late_fee_per_hour, merci_rate_per_hour, merci_fee_free_hours FROM {merci_bucket_node} WHERE vid = :vid", array(':vid' => $vid))->fetch(PDO::FETCH_ASSOC);
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function merci_resource_node_settings($vid) {
  if (!$vid) {
    return;
  }
  return db_query("SELECT merci_late_fee_per_hour, merci_rate_per_hour, merci_fee_free_hours, merci_min_cancel_hours, merci_autocheckout, merci_autocheckin, merci_selfcheckout FROM {merci_resource_node} WHERE vid = :vid", array(':vid' => $vid))->fetch(PDO::FETCH_ASSOC);
}

function merci_reservation_item_node_settings($vid) {
  if (!$vid) {
    return ;
  }
  return db_query("SELECT merci_default_availability, merci_sub_type, merci_item_status FROM {merci_reservation_item_node} WHERE vid = :vid", array(':vid' => $vid))->fetch(PDO::FETCH_ASSOC);
}

function merci_content_types($type_name = NULL) {
  // handle type name with either an underscore or a dash
   $type_name = !empty($type_name) ? str_replace('-', '_', $type_name) : NULL;

  $info = _merci_content_type_info();
  if (!isset($type_name)) {
    return $info;
  }
  else {
    return isset($info[$type_name]) ? $info[$type_name] : array();
  }
}

function _merci_content_type_info($reset = FALSE) {
  static $info;
  if ($reset || !isset($info)) {
    if (!$reset && $cached = cache_get('merci_content_type_info')) {
      $info = $cached->data;
    }
    else {
      $info = array();
      // Load MERCI node type settings.
      $merci_settings = db_query("SELECT * FROM {merci_node_type} mt INNER JOIN {node_type} nt ON mt.type = nt.type WHERE merci_type_setting <> :merci_type_setting", array(':merci_type_setting' => 'disabled'));
      // with the correct stuff..
      foreach ($merci_settings as $merci_setting) {
        $merci_setting = (array) $merci_setting;
        $merci_setting['type_name'] = $merci_setting['name'];
        $grouping = $merci_setting['merci_type_setting'] == 'resource' ? $merci_setting['type_name'] : t('Buckets');
        $tid = variable_get('merci_grouping_' . $merci_setting['type'], 0);
        if ($tid) {
          $term = taxonomy_term_load($tid);
          if ($term) {
            $grouping = $term->name;
          }
        }
        $merci_setting['merci_item_grouping'] = $grouping;
        unset($merci_setting['name']);
        $info[$merci_setting['type']] = $merci_setting;
      }
      cache_set('merci_content_type_info', $info);
    }
  }
  return $info;
}

