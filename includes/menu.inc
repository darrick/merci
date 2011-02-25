<?php

/**
 * Menu callback for AHAH additions.
 */
function merci_choice_js() {

  list($form, $form_state) = merci_ahah_get_form();

  $selected_portion = $form['choice_wrapper'];

  // To avoid doubling-up the wrapper, we have to remove it here.
  unset($selected_portion['#prefix'], $selected_portion['#suffix']);

  //$output = theme('status_messages') . drupal_render($selected_portion);
  // BAD HACK to not show validation error messages for the date combo.  Not sure how to turn validation off for that.
  theme('status_messages');
  $output = drupal_render($selected_portion);

  // Final rendering callback.
  drupal_json(array('status' => TRUE, 'data' => $output));

}

function merci_taxonomy_json($node,$vid,$did) {

  db_query("DELETE FROM {term_node} WHERE vid = %d and tid IN (SELECT tid FROM {term_data} td WHERE td.vid = %d)",$node->vid,$vid); 

  $choices = $_POST['merci_reservation_items'] 
    ? $_POST['merci_reservation_items'][$did]['accessories']['choices'][$vid]  : $_POST[$did]['accessories']['choices'][$vid] ;

  foreach($choices as $tid) {
    if($tid) {
      db_query('INSERT INTO {term_node} (nid, vid, tid) VALUES (%d, %d, %d)', $node->nid, $node->vid, $tid);
    }
  }

  $output = theme('status_messages') ;
  drupal_json(array('status' => TRUE, 'data' => $output));
}



/**
 * returns the number of items currently checked out for each content type
 * TODO: change this to a views view.
 */
function merci_current_inventory() {

  $header = array (
    t('Item'),
    t('On Hand'),
    t('Unavailable'),
    t('Checked Out'),
    t('Total'),
    t('Operations')
  );

  $merci_types = merci_content_types();


  $rows   = array();

  foreach ($merci_types as $merci_type) { 

    $info = content_types($merci_type['type']);

    $operations = l(t('Edit'),'admin/content/node-type/'.$info['url_str'],array('query' => array('destination' => 'admin/merci/manage/current_inventory')));
    $operations .= " | ";
    $operations .= l(t('Add Item'),'node/add/'.$info['url_str'],array('query' => array('destination' => 'admin/merci/manage/current_inventory')));

    $checked_out = merci_get_reservation_count($merci_type['type']);

    $available = merci_get_count($merci_type,MERCI_AVA_F);
    $onhand = $available - $checked_out;

    $unavailable = merci_get_count($merci_type,MERCI_UNA_F);
    $total = $onhand + $unavailable + $checked_out;
    $available = l($onhand,'admin/merci/manage/current_inventory/'.$info['type'].'/Available');
    $checked_out = l($checked_out,'admin/merci/manage/current_inventory/'.$info['type'].'/checked-out');
    $unavailable = l($unavailable,'admin/merci/manage/current_inventory/'.$info['type'].'/Unavailable');

    $title = views_get_view('merci_inventory_view')
      ? l($info['name'], 'admin/merci/manage/inventory', array('html' => TRUE, 'query' => 'type[0]=' . $merci_type['type'])) 
      : $info['name'];

    $rows[] = array(
      $title,
      $available,
      $unavailable,
      $checked_out,
      $total,
      $operations,
    );
  }
  $output = theme('table', $header, $rows);
  $output .= '&raquo; ' . l(t('Add a new content type'), 'admin/content/types/add', array('query' => array('destination' => 'admin/merci/manage/current_inventory')));
  return $output;
}
