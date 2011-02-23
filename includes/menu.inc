<?php
// $Id$

/**
 * Menu callback for AHAH additions.
 */
function merci_choice_js() {
  $form_state = array('storage' => NULL, 'submitted' => TRUE);
  $form_build_id = $_POST['form_build_id'];

  // Get the form from the cache.
  $form = form_get_cache($form_build_id, $form_state);
  $args = $form['#parameters'];
  $form_id = array_shift($args);
  // We will run some of the submit handlers so we need to disable redirecting.
  $form['#redirect'] = FALSE;
  // We need to process the form, prepare for that by setting a few internals
  // variables.
  $form['#post'] = $_POST;
  $form['#programmed'] = FALSE;
  $form_state['post'] = $_POST;
  $delta = count($_POST['choice']);
  $nid = isset($_POST['nid']) ? $_POST['nid'] : 0;
  if ((int) $nid) {
    $node = node_load($nid);
  }
  else {
    $node = new stdClass();
  }
  
  $dates = $_POST['field_merci_date'][0];
  // If a start and end date exist, we have to massage them
  // into the proper format from user input.
  // TODO: is there a more elegant way to do this?
  if ($dates['value']['date'] && $dates['value']['time'] && $dates['value2']['date'] && $dates['value2']['time']) {
    $default_format = $form['#field_info']['field_merci_date']['widget']['input_format'];
    $form_state['values']['field_merci_date'][0] = merci_convert_date_popup($dates, $default_format);
  }
  else {
    $form_state = array();
  }

  // Build our new form element.
  $form_element = _merci_choice_form($node, $form_state, $delta, $default);
  drupal_alter('form', $form_element, array(), 'merci_choice_js');

  /*
  // Build the new form.
  $form_state = array('submitted' => FALSE);
  $form_build_id = $_POST['form_build_id'];
  // Add the new element to the stored form. Without adding the element to the
  // form, Drupal is not aware of this new elements existence and will not
  // process it. We retreive the cached form, add the element, and resave.
  if (!$form = form_get_cache($form_build_id, $form_state)) {
    exit();
  }
   */
  $form['choice_wrapper']['choice'][$delta] = $form_element;
  form_set_cache($form_build_id, $form, $form_state);
  $form += array(
    '#post' => $_POST,
    '#programmed' => FALSE,
  );

  // Rebuild the form.
  $form = form_builder('merci_reservation_node_form', $form, $form_state);

  // Render the new output.
  $choice_form = $form['choice_wrapper']['choice'];
  // Prevent duplicate wrappers.
  unset($choice_form['#prefix'], $choice_form['#suffix']);
  $choice_form[$delta]['#attributes']['class'] = empty($choice_form[$delta]['#attributes']['class']) ? 'ahah-new-content' : $choice_form[$delta]['#attributes']['class'] . ' ahah-new-content';
  $output = theme('status_messages') . drupal_render($choice_form);

  drupal_json(array('status' => TRUE, 'data' => $output));
}

function merci_date_filter_js() {
  $form_state = array('storage' => NULL, 'submitted' => TRUE);
  $form_build_id = $_POST['form_build_id'];

  // Get the form from the cache.
  $form = form_get_cache($form_build_id, $form_state);
  $args = $form['#parameters'];
  $form_id = array_shift($args);
  // We will run some of the submit handlers so we need to disable redirecting.
  $form['#redirect'] = FALSE;
  // We need to process the form, prepare for that by setting a few internals
  // variables.
  $form['#post'] = $_POST;
  $form['#programmed'] = FALSE;
  $form_state['post'] = $_POST;

  $choice_count = count($_POST['choice']);
  $nid = isset($_POST['nid']) ? $_POST['nid'] : 0;
  if ((int) $nid) {
    $node = node_load($nid);
  }
  else {
    $node = new stdClass();
  }
  
  $dates = $_POST['field_merci_date'][0];
  // If a start and end date exist, we have to massage them
  // into the proper format from user input.
  // TODO: is there a more elegant way to do this?
  if ($dates['value']['date'] && $dates['value']['time'] && $dates['value2']['date'] && $dates['value2']['time']) {
    $default_format = $form['#field_info']['field_merci_date']['widget']['input_format'];
    $form_state['values']['field_merci_date'][0] = merci_convert_date_popup($dates, $default_format);
  }
  else {
    $form_state = array();
  }

  // Add the current choices to the form.
  for ($delta = 0; $delta < $choice_count; $delta++) {
    $default = isset($node->choice[$delta]['item']) ? $node->choice[$delta]['item'] : '';

    $form['choice_wrapper']['choice'][$delta] = _merci_choice_form($node, $form_state, $delta, $default);
  }

  /*
  // Build the new form.
  $form_state = array('submitted' => FALSE);
  $form_build_id = $_POST['form_build_id'];
  // Add the new element to the stored form. Without adding the element to the
  // form, Drupal is not aware of this new elements existence and will not
  // process it. We retreive the cached form, add the element, and resave.
  if (!$form = form_get_cache($form_build_id, $form_state)) {
    exit();
  }
   */
  //$form['choice_wrapper']['choice'] = $form_element;
  form_set_cache($form_build_id, $form, $form_state);
  $form += array(
    '#post' => $_POST,
    '#programmed' => FALSE,
  );

  // Rebuild the form.
  $form = form_builder('merci_reservation_node_form', $form, $form_state);
  // Render the new output.
  $choice_form = $form['choice_wrapper']['choice'];
  //$choice_form = $form_element;
  // Prevent duplicate wrappers.
  unset($choice_form['#prefix'], $choice_form['#suffix']);
  //$choice_form[$delta]['#attributes']['class'] = empty($choice_form[$delta]['#attributes']['class']) ? 'ahah-new-content' : $choice_form[$delta]['#attributes']['class'] . ' ahah-new-content';
  $output = theme('status_messages') . drupal_render($choice_form);

  drupal_json(array('status' => TRUE, 'data' => $output));
}

function merci_taxonomy_json($node,$vid,$did) {


  db_query("DELETE FROM {term_node} WHERE vid = %d and tid IN (SELECT tid FROM {term_data} td WHERE td.vid = %d)",$node->vid,$vid); 

  $choices = $_POST['existing_items'] 
    ? $_POST['existing_items']['accessories'][$did]['choices'][$vid]  : $_POST['accessories'][$did]['choices'][$vid] ;

  foreach($choices as $tid) {
    if($tid) {
      db_query('INSERT INTO {term_node} (nid, vid, tid) VALUES (%d, %d, %d)', $node->nid, $node->vid, $tid);
    }
  }

  $output = theme('status_messages') ;
  drupal_json(array('status' => TRUE, 'data' => $output));
}



// Changes the reservation from UNCONFIRMED to CONFIRMED.
// Normally accessed from email sent to user
// TODO: Currently takes nid, but should be a more unique id
function merci_confirm_via_email($nid) {
  global $basepath;
  if (merci_confirm_reservation($nid)) {
    drupal_set_message(t('Reservation confirmed'));
  } 
  else {
    drupal_set_message(t('Reservation not confirmed.  The Reservation may have been confirmed by staff.')); 
  }
  
  $return = $_GET["destination"];
  if (!$return) {
    $return = $basepath;
  }
  drupal_goto($return);
  
}

// merci_confirm_via_email
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