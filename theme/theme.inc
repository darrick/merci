<?php
// $Id$

/**
 * @file
 * MERCI - Managed Equipment Reservation Checkout and Inventory
 */

function template_preprocess_merci_conflict_grid(&$variables) {

  $type = $variables['type'];
  $title = $variables['title'];
  $start = $variables['start'];
  $end = $variables['end'];
  $nid = $variables['nid'];

  $hours = array();
  if (user_access('administer MERCI') || user_access('create reservations outside hours of operation')) {
    $adminhours = explode('-', variable_get('merci_hours_admin', '07:00-23:00'));

    for($day=0; $day < 7; $day++) {
      $hours[$day]['open'] = $adminhours[0];
      $hours[$day]['close'] = $adminhours[1];
    }
  }
  else {
    $hours = merci_load_hours_of_operation();
  }

  // Convert the start and end dates to the site timezone.
  $start = date_make_date($start, "UTC");
  $end = date_make_date($end,"UTC");
  $timezone = variable_get('date_default_timezone_name', 'UTC');
  date_timezone_set($start, timezone_open($timezone));
  date_timezone_set($end, timezone_open($timezone));

  // Expand our search to the entire day in the users timezone.
  $start_day = date_format($start,'w');
  $end_day = date_format($end,'w');
  $start_date = $dates['value']['date'] = date_format($start,'Y-m-d');
  $dates['value']['time'] = $hours[$start_day]['open'];
  $end_date = $dates['value2']['date'] = date_format($end,'Y-m-d');
  $dates['value2']['time'] = $hours[$end_day]['close'];

  $dates = merci_convert_date_popup($dates,'Y-m-d H:i');

  $start_mysql = $dates['value'];
  $end_mysql = $dates['value2'];

  $reservations = merci_load_reservations_for_node_in_timespan($nid, $type, $start_mysql, $end_mysql);

  $reservations_by_date = array();

  $timezone_offset = variable_get('date_default_timezone', 0);

  //for each day we are looking at.
  while($start_date <= $end_date){

    $reservations_by_date[$start_date] = array();

    $day = date('w',strtotime($start_date));

    // loop through all the items.
    foreach($reservations as $item => $values) {

      $reservations_by_date[$start_date][$item] = array();

      $time = strtotime($start_date.' '.$hours[$day]['open']);
      $close = strtotime($start_date.' '.$hours[$day]['close']);

      // all the reservations for that item.
      foreach($values as $reservation) {

        // Convert to site timezone.
        $tz_start_date = strtotime($reservation->field_merci_date_value) + $timezone_offset;
        $tz_end_date   = strtotime($reservation->field_merci_date_value2)   + $timezone_offset;
        $tz_start_date = $tz_start_date < $close ? $tz_start_date : $close;
        $tz_end_date   = $tz_end_date < $close ? $tz_end_date : $close; 
        while($time < $tz_start_date) {
          $reservations_by_date[$start_date][$item][date('H:i',$time)] = 'available';
          $time += 15*60;
        }
        while($time >= $tz_start_date && $time < $tz_end_date) { 
            $reservations_by_date[$start_date][$item][date('H:i',$time)] = 'unavailable';
            $time += 15*60;
          }
        $time = $tz_end_date;
      }
      while($time < $close) {
        $reservations_by_date[$start_date][$item][date('H:i',$time)] = 'available';
        $time += 15*60;
      }
    }
    $start_date = date('Y-m-d', strtotime($start_date . ' +1 day'));
  }

  $variables['reservations_by_date'] = $reservations_by_date;

  $pretty_hours = array();

  foreach($reservations_by_date as $date => $values) {
    $day = date('w',strtotime($date));
    $time = $hours[$day]['open'];
    while($time < $hours[$day]['close']) {
      $pretty_hours[$date][] = date('ga',strtotime($time));
      $time = date('H:i', strtotime($time . ' +1 hour'));
    }
  }


  $variables['pretty_hours'] = $pretty_hours;
}

/**
 * Builds the reserved items table.
 */
function theme_merci_build_reservation_table_form($form) {

  $output = '';
  $header = $form['#header'];
  $node   = $form['#node'];
  $rows   = array();
  foreach ($form['#table'] as $did => $columns) {
    
    $item = drupal_render($form['items'][$did]);
        
    // The content type name.
    if (isset($columns['display_item'])) {
      $item .= $columns['display_item'];
    }
   
    $rows[$did] = array(
      $item,
      $form['#table'][$did]['type'],
    );

    foreach($form['#header'] as $index => $value) {
      if ($value == t('Accessories')) {
        $rows[$did][$index] = drupal_render($form['accessories'][$did]);
      }
      else if ($value == t('Operations')) {
        $rows[$did][$index] = $form['#table'][$did]['ops'];
      }
    }

  }

  if (!empty($rows)) {
    $table_caption = in_array((int) $node->merci['status'], array(MERCI_STATUS_CHECKED_OUT, MERCI_STATUS_CHECKED_IN)) ? t('Checked out items') : t('Currently reserved items');
    $output .= '<div class="existing-items-table-header">' . $table_caption . '</div>';
    $output .= theme('table', $header, $rows);
  }

  return $output;
}

