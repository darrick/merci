<?php

/**
 * Adds a widget form for setting generic day of week and open and close times.
 */

/**
 * Implements hook_field_widget_info().
 *
 * Field type modules willing to use those widgets should:
 * - Use hook_field_widget_info_alter() to append their field own types to the
 *   list of types supported by the widgets,
 */
function merci_hours_field_widget_info() {
  return array(
    'merci_hours' => array(
      'label' => t('Merci Hours'),
      'field types' => array('date', 'datetime', 'datestamp'),
    ),
  );
}

/**
 * Implements hook_theme().
 */
function merci_hours_theme() {
  return array(
    'merci_hours_element' => array(
      'render element' => 'form',
    ),
  );
}

function merci_hours_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  
  $field_name = $field['field_name'];
  $parents = $form['#parents'];

  $field_state = field_form_get_state($parents, $field_name, $langcode, $form_state);
  $max = $field_state['items_count'];

  $title = check_plain($instance['label']);
  $description = field_filter_xss($instance['description']);

  $id_prefix = implode('-', array_merge($parents, array($field_name)));
  $wrapper_id = drupal_html_id($id_prefix . '-add-more-wrapper');

  module_load_include('inc', 'date_api', 'date_api_elements');
  $timezone = date_get_timezone($field['settings']['tz_handling'], date_default_timezone());
  $days = date_week_days();

  $element['#element_validate'] = array('merci_hours_widget_validate');
  $element['#theme'] = 'merci_hours_element';
  if (!isset($element['#pre_render'])) {
      $element['#pre_render'] = array();
  }
  array_unshift($element['#pre_render'], 'merci_hours_element_pre_render');
  $element['#date_timezone'] = $timezone;
  $element['#date_format'] = 'Y-m-d H:i:s';

  if (array_key_exists($delta, $items)) {
    $item = $items[$delta];
    if (array_key_exists('value', $item) && !empty($item['value'])) {
      $open = new DateObject($item['value']);
      $day = $open->format('w');
    }
  }

  $element['value'] = array(
    '#type' => 'hidden',
    '#value' => !empty($item) ? $item['value'] : '',
  );
  $element['value2'] = array(
    '#type' => 'hidden',
    '#value' => !empty($item) ? $item['value2'] : '',
  );

  $element['container'] = array(
    '#type' => 'fieldset',
    //'#title' => 'Office hours',
    '#attributes' => array('class' => array('container-inline', 'merci-hours-field')),
  );

  $element['container']['day'] = array(
    '#type' => 'select',
    '#options' => $days,
    '#default_value' => isset($day) ? $day : '',
    '#weight' => 1,
    '#attributes' => array('class' => array('container-inline')),
  );

  $element['container']['value'] = array(
    '#type' => 'date_popup',
    '#theme_wrappers' => array('date_combo'),
    '#weight' => $delta,
    '#default_value' => isset($item['value']) ? $item['value'] : '',
    '#date_timezone' => $timezone,
    '#date_format' => 'g:ia',

    // Store the original values, for use with disabled and hidden fields.
    //'#date_items' => isset($items[$delta]) ? $items[$delta] : '',
    '#weight' => 2,
  );

  $element['container']['value2'] = array(
    '#type' => 'date_popup',
    '#theme_wrappers' => array('date_combo'),
    '#weight' => $delta,
    '#default_value' => isset($item['value2']) ? $item['value2'] : '',
    '#date_timezone' => $timezone,
    '#date_format' => 'g:ia',

    // Store the original values, for use with disabled and hidden fields.
    '#weight' => 3,
  );

  $element['container']['remove_hours'] = array(
    '#type' => 'checkbox',
    '#default_value' => FALSE,
    '#title' => t('Remove'),
    '#weight' => 4,
  );
  return $element;

}

function merci_hours_widget_element_is_empty($element) {
  
  if ($element['container']['day']['#value'] == '') { // and !$element['container']['value']['time']['#value'] and !$element['container']['value2']['time']['#value']) {
    return TRUE;
  }
  return FALSE;

}

/**
 * Form element validation handler for options element.
 */
function merci_hours_widget_validate($element, &$form_state) {
  if(merci_hours_widget_element_is_empty($element)) {
    return;
  }

  if ($element['container']['day']['#value'] == '') {
    form_error($element['container']['day'], t('!name field is required.', array('!name' => $element['container']['day']['#title'])));
  }
  if ($element['container']['value']['time']['#value'] == '') {
    form_error($element['container']['value'], t('!name field is required.', array('!name' => $element['container']['value']['#title'])));
  }
  if ($element['container']['value2']['time']['#value'] == '') {
    form_error($element['container']['value2'], t('!name field is required.', array('!name' => $element['container']['value2']['#title'])));
  }
  if (form_get_errors()) {
    return;
  }

  $days =  date_week_days_untranslated();

  $open = new DateObject($days[$element['container']['day']['#value']] . ' ' . $element['container']['value']['time']['#value']);
  $close = new DateObject($days[$element['container']['day']['#value']] . ' ' . $element['container']['value2']['time']['#value']);

  if ($open > $close) {
    form_error($element['container']['value'], t('!open must be less then |close time.', array('!open' => $element['container']['value']['#title'], '|close' => $element['container']['value2']['#title'])));
  }
  $field = field_widget_field($element, $form_state);
  $open = date_format($open, date_type_format($field['type']));
  $close = date_format($close, date_type_format($field['type']));

  $value = array(
    'value' => $open,
    'value2' => $close,
    'remove_hours' => $element['container']['remove_hours']['#value'],
  );

  form_set_value($element, $value, $form_state);
}

function merci_hours_element_pre_render($element) {
  $element['container']['value']['time']['#title'] = t('Open');
  $element['container']['value2']['time']['#title'] = t('Close');
  return $element;
}
/**
 * Themes the line item manager widget form element.
 */
function theme_merci_hours_element($variables) {
  drupal_add_css(drupal_get_path('module', 'merci_hours') . '/merci_hours.css');
}

