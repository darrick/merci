<?php

/**
 * @file
 * Work calendar editing UI.
 */

/**
 * Generates the merci hours editing form.
 *
 * @see merci_hours_form_submit().
 * @see merci_hours_form_submit_delete().
 */
function merci_hours_form($form, &$form_state, $merci_hours, $op = 'edit') {
  if ($op == 'clone') {
    $merci_hours->label .= ' (cloned)';
    $merci_hours->name .= '_clone';
  }

  $form['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Label'),
    '#default_value' => $merci_hours->label,
    '#maxlength' => 255,
    '#required' => TRUE,
  );
  $form['name'] = array(
    '#type' => 'machine_name',
    '#default_value' => $merci_hours->name,
    '#disabled' => $merci_hours->isLocked() && $op != 'clone',
    '#maxlength' => 32,
    '#machine_name' => array(
      'exists' => 'merci_hours_load',
      'source' => array('label'),
     ),
  );
  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#default_value' => $merci_hours->description,
  );

  field_attach_form('merci_hours', $merci_hours, $form, $form_state);

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save work calendar'),
    '#weight' => 40,
  );
  if (!$merci_hours->isLocked() && $op != 'add' && $op != 'clone') {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete work calendar'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('merci_hours_form_submit_delete'),
    );
  }
  return $form;
}

/**
 * Form API submit callback for the save button.
 *
 * @see merci_hours_form().
 */
function merci_hours_form_submit($form, &$form_state) {
  $merci_hours = entity_ui_form_submit_build_entity($form, $form_state);
  // Update availability before save.
  //$merci_hours->updateWeek($form_state['values']['week_days']);
  // Save and go back.
  $merci_hours->save();
  $form_state['redirect'] = 'admin/structure/merci-hours';
}

/**
 * Form API submit callback for the delete button.
 *
 * @see merci_hours_form().
function merci_hours_form_submit_delete($form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/work-calendars/manage/' . $form_state['merci_hours']->name . '/delete';
}
