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

/**
 * Get taxonomy data as JSON for a node
 *
 * @param $node_id
 *   The node ID.
 *
 * @return
 *   JSON string of taxonomy data
 */
function merci_taxonomy_json($node_id) {

  $node = node_load($node_id);
  $output = array();

  if (user_access('manage MERCI')) {

    // Current user has access to update this node

    $vocabularies = taxonomy_get_vocabularies($node->type);

    if (isset($_POST['taxonomy'])) {

      // Changes to taxonomy sent

      $required_sent = TRUE;

      // Make sure all required vocabularies were sent

      foreach ($vocabularies as $vocabulary) {

        if (
          ($vocabulary->required) &&
          (!isset($_POST['taxonomy'][$vocabulary->vid]))
        ) {

          $required_sent = FALSE;
        }
        // if
      }
      // foreach

      if ($required_sent) {

        // Save new node revision, clear taxonomy, and re-add sent term

        node_save($node);
        taxonomy_node_delete_revision($node);

        foreach ($_POST['taxonomy'] as $vocabulary => $terms) {
          foreach ($terms as $id => $term_id) {
            db_query('INSERT INTO {term_node} (nid, vid, tid) VALUES (%d, %d, %d)', $node->nid, $node->vid, $term_id);
          }
          // foreach
        }
        // foreach

        $output['status'] = 'success';
        $output['node'] = $node_id;
      }
      // if
      else {

        // Missing required vocabulary

        $output['status'] = 'failed';
        $output['error'] = 'Required fields not selected.';

      }
      // else
    }
    // if
    else {

      // Get last author admin status and node ID

      $user = user_load($node->revision_uid);

      $output['node'] = $node_id;
      $output['vocabularies'] = array();
      $output['admin'] = user_access('administer MERCI', $user);

      usort($vocabularies, 'merci_by_vid');

      // Get the select HTML and previously selected options for each vocabulary

      foreach ($vocabularies as $vocabulary) {

        $select             = taxonomy_form($vocabulary->vid);
        $select['#name']    = 'taxonomy[' . $vocabulary->vid . ']';
        $select['#parents'] = array();
        $select_html        = theme_select($select);

        $vocabulary_output = array(
          'select' => $select_html,
          'selected' => array(),
        );

        foreach ($node->taxonomy as $tid => $term) $vocabulary_output['selected'][] = $tid;

        $output['vocabularies'][] = $vocabulary_output;
      }
      // foreach
    }
    // else
  }
  // if
  else {

    // No permission
    $output['error'] = 'Permission denied.';
  }
  // else

  drupal_json($output);
}
// merci_taxonomy_json


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
    $operations .= " ";
    $operations .= l(t('Add Item'),'node/add/'.$info['url_str'],array('query' => array('destination' => 'admin/merci/manage/current_inventory')));

    $checked_out = merci_get_reservation_count($merci_type['type']);

    $available = merci_get_count($merci_type,MERCI_AVA_F);
    $onhand = $available - $checked_out;

    $unavailable = merci_get_count($merci_type,MERCI_UNA_F);
    $total = $onhand + $unavailable + $checked_out;
    $available = l($onhand,'admin/merci/manage/current_inventory/'.$info['type'].'/Available');
    $checked_out = l($checked_out,'admin/merci/manage/current_inventory/'.$info['type'].'/checked-out');
    $unavailable = l($unavailable,'admin/merci/manage/current_inventory/'.$info['type'].'/Unavailable');

    // TODO: Check if the View exists... not the module that normally creates it.
    $title = module_exists('merci_inventory') 
      ? l($info['name'], 'admin/merci/manage/inventory', array('html' => TRUE, 'query' => 'type[0]=' . $merci_type['type'])) 
      : l($info['name'],'admin/merci/manage/current_inventory/'. $info['type'] .'/all');

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
  $output .= l(t('Add Merci Type'),'admin/content/types/add',array('query' => array('destination' => 'admin/merci/manage/current_inventory')));
  return $output;
}

/**
 * Callback function for updating pricing in Reservation form.
 */
function merci_pricing($start, $end, $node_type_array) {
  
  $discount = variable_get('merci_membership_discount', 1);
  $debug = FALSE;
      
  if ($debug) {
   
    $dates[0] = 1292607900;
    $dates[1] = 1291747500;
    $items[0] = 1;
    
  } 
  else {
  
    $items = explode(',', $_POST['items']);
    $dates = array(
      strtotime($_POST['date'][0] . ' ' . $_POST['time'][0]),
      strtotime($_POST['date'][1] . ' ' . $_POST['time'][1]),
    );
  
  }
  
  $hours = round(($dates[1] - $dates[0]) / 3600, 2);
  
  foreach ($items as $item) {
    
    if (is_numeric($item)) {
      //this is a Resource to load the pricing based on the node
      $item_node = node_load($item);
      $fee_hours       = $hours - ($item_node->merci['fee_free_hours']);
      $rate_per_hour   = $item_node->merci['rate_per_hour']; 
      
    } 
    else {
      //for buckets, load settings based on type
      //$type = merci_load_content_type_settings($item);
      $type = merci_load_item_settings(NULL,$item);
      $fee_hours       = $hours - ($type['fee_free_hours']);
      $rate_per_hour   = $type['rate_per_hour'];
      
    }
    
    $commercial_cost = $rate_per_hour * $hours;
    $commercial_cost_total += $commercial_cost; 
     
    $member_cost     = ($fee_hours > 0) ? ($rate_per_hour * $discount) * $fee_hours : 0;
    $member_cost_total += $member_cost;
    
  } //foreach
  
  // create return object
  $return = new StdClass;
  $return->member = $member_cost_total;
  $return->commercial = $commercial_cost_total;
  return drupal_json($return);
}
// merci_daypart_pricing
