<?php

/**
 * @file
 * MERCI - Managed Equipment Reservation Checkout and Inventory
 */



function merci_node_type_update($info) {
  if (isset($info->old_type) && $info->type != $info->old_type) {
    db_query("UPDATE {merci_node_type} SET type = '%s' WHERE type = '%s'", $info->type, $info->old_type);
    cache_clear_all('merci_' . $info->type . '_data','cache');
    cache_clear_all('merci_content_type_info','cache');
  }
}

function merci_node_type_existing($type) {
  return db_result(db_query("SELECT type FROM {merci_node_type} WHERE type = '%s'",$type));
}

function merci_node_type_delete($type) {
  db_query("DELETE FROM {merci_node_type} WHERE type = '%s'", $type);
  variable_del('merci_type_setting_' . $info->type);
  cache_clear_all('merci_' . $info->type . '_data','cache');
  cache_clear_all('merci_content_type_info','cache');
}

function merci_reservation_status($node) {
  return db_result(db_query("SELECT merci_reservation_status FROM {merci_reservation} WHERE vid = %d", $node->vid));
}

function merci_reservation_status_nid($node) {
  $current_status = db_result(db_query("SELECT m.merci_reservation_status FROM {node} n INNER JOIN {merci_reservation} m ON n.vid = m.vid WHERE n.nid = %d", $node->nid));
}

function merci_reservation_items($node) {
  $items = db_query("SELECT m.did, m.merci_item_status, merci_placeholder_nid, pn.title AS placeholder_title, merci_item_nid, tn.title AS item_title, nt.type, nt.name FROM {merci_reservation_detail} m INNER JOIN {node} pn ON m.merci_placeholder_nid = pn.nid INNER JOIN {node_type} nt ON pn.type = nt.type LEFT JOIN {node} tn ON m.merci_item_nid = tn.nid WHERE m.vid = %d", $node->vid);

  $reservation_items = array();

  while ($item = db_fetch_array($items)) {
    $reservation_items[$item['did']] = $item;
  }
  return $reservation_items;
}

function merci_get_reservation_count($content_type) {
  return db_result(db_query("SELECT COUNT(n.nid) as total FROM {node} n 
    JOIN {merci_reservation_detail} mrin ON mrin.vid = n.vid 
    JOIN {node} ctn ON mrin.merci_item_nid = ctn.nid 
    WHERE ctn.type = '%s' and mrin.merci_item_status = %d", $content_type, MERCI_ITEM_STATUS_CHECKED_OUT));
}

function merci_delete_record($table, &$object, $update = array()) {
  // Standardize $update to an array.
  if (is_string($update)) {
    $update = array($update);
  }

  $schema = drupal_get_schema($table);
  if (empty($schema)) {
    return FALSE;
  }

  // Convert to an object if needed.
  if (is_array($object)) {
    $object = (object) $object;
    $array = TRUE;
  }
  else {
    $array = FALSE;
  }

  $fields = $values = array();


  // Build the SQL.
  $query = '';

  foreach ($update as $key) {
    $conditions[] = "$key = " . db_type_placeholder($schema['fields'][$key]['type']);
    $values[] = $object->$key;
  }

  $query = "DELETE FROM {" . $table . "} WHERE " . implode(' AND ', $conditions);

  // Execute the SQL.
  if (db_query($query, $values)) {

    $return = true;
  }
  else {
    $return = FALSE;
  }

  // If we began with an array, convert back so we don't surprise the caller.
  if ($array) {
    $object = (array) $object;
  }

  return $return;
}
/**
 * Returns totals for reporting.
 *
 * @param $type
 *   The bucket or resrouce node type.
 * @param $startdate
 *   TRUE to restrict to published items, FALSE otherwise.
 *
 * @return
 *   Total reservation number for that type betweent the start and end dates
TODO NOT USER.
 */
function merci_reservation_totals($type, $startdate, $enddate) {
  $result = db_query("SELECT COUNT(nid) as total  FROM {node} WHERE type LIKE '%s' and status = 0 AND created > %d AND created < %d", $type, $startdate, $enddate);
  $reservationnode = db_fetch_object($result);
  return $reservationnode->total;
}
function merci_has_accessories($content_type) {
  if(empty($content_type)) return;
  return db_fetch_object(db_query("SELECT * FROM {vocabulary_node_types} WHERE type = '%s'", $content_type));
}

/**
 * Pass type as user/project and uid/nid
 * returns total hours
 */
function merci_total_usage($id, $type = 'user', $date = NULL) {

  if ($type == 'project') {
    $result = db_query("SELECT * FROM {merci_reservation} mr JOIN node n ON n.vid = mr.vid JOIN og_ancestry og ON og.nid = n.nid JOIN content_type_merci_reservation ct ON n.vid = ct.vid WHERE group_nid = %d", $id);
  }
  else {
    if ($date) {
      $result = db_query("SELECT * FROM {merci_reservation} JOIN node ON node.vid = merci_reservation.vid JOIN content_type_merci_reservation ON node.vid = content_type_merci_reservation.vid WHERE uid = %d AND field_merci_date_value > '%s'", $id, $date);
    }
    else {
      $result = db_query("SELECT * FROM {merci_reservation} JOIN node ON node.vid = merci_reservation.vid JOIN content_type_merci_reservation ON node.vid = content_type_merci_reservation.vid WHERE uid = %d", $id);
    }
  }

  //add the reservation total minutes for each item in the reservation
  while ($reservationnode = db_fetch_object($result)) {

    $minutes = $minutes + ((strtotime($reservationnode->field_merci_date_value2) - strtotime($reservationnode->field_merci_date_value)));
  }

  return $minutes;
}
