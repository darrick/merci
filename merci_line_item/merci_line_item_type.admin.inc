<?php

/**
 * Generates the merci_reservation_detail type editing form.
 */
function merci_line_item_type_form($form, &$form_state, $merci_line_item_type, $op = 'edit') {

  if ($op == 'clone') {
    $merci_line_item_type->label .= ' (cloned)';
    $merci_line_item_type->type = '';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $merci_line_item_type->label,
    '#description' => t('The human-readable name of this merci_reservation_detail type.'),
    '#required' => TRUE,
    '#size' => 30,
  );
  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($merci_line_item_type->type) ? $merci_line_item_type->type : '',
    '#maxlength' => 32,
//    '#disabled' => $merci_line_item_type->isLocked() && $op != 'clone',
    '#machine_name' => array(
      'exists' => 'merci_line_item_type_load',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this merci_reservation_detail type. It must only contain lowercase letters, numbers, and underscores.'),
  );


  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save line item type'),
    '#weight' => 40,
  );

  return $form;
}

/**
 *  * Form API submit callback for the type form.
 *   */
function merci_line_item_type_form_submit(&$form, &$form_state) {
  $merci_line_item_type = entity_ui_form_submit_build_entity($form, $form_state);
  $merci_line_item_type->save();
  $form_state['redirect'] = 'admin/merci/merci-line-item-types';
}
