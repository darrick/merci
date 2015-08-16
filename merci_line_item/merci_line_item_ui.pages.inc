<?php

/**
 * Form callback: create or edit a entity_type.
 *
 * @param $entity_type
 *   The entity_type object to edit or for a create form an empty entity_type object
 *     with only a entity_type type defined.
 */
function merci_line_item_form($form, &$form_state, $entity) {
  $entity_type = $form_state['entity_type'];
  $entity_info = $entity->entityInfo();
  $path = $entity_info['admin ui']['path'];
  $form['#entity'] = $entity;

  // Add the field related form elements.
  $form_state[$entity_type] = $entity;
  field_attach_form($entity_type, $entity, $form, $form_state);

  $form['actions'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('form-actions')),
    '#weight' => 400,
  );

  // We add the form's #submit array to this button along with the actual submit
  // handler to preserve any submit handlers added by a form callback_wrapper.
  $submit = array();

  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#submit' => $submit + array('merci_line_item_ui_form_submit'),
  );

  if (!empty($entity->id)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#limit_validation_errors' => array(),
      '#submit' => $submit + array('merci_line_item_ui_form_submit_delete'),
      '#weight' => 45,
    );
  }

  // We append the validate handler to #validate in case a form callback_wrapper
  // is used to add validate handlers earlier.
  $form['#validate'][] = 'merci_line_item_ui_form_validate';

  return $form;
}

/**
 * Form API validate callback for the entity_type form
 */
function merci_line_item_ui_form_validate(&$form, &$form_state) {
  $entity_type = $form_state['entity_type'];
  $entity = $form_state[$entity_type];
  
  // Notify field widgets to validate their data.
  field_attach_form_validate($entity_type, $entity, $form, $form_state);

  // Do no validation if their errors from the main validation function.
  if (form_get_errors()) {
    return;
  }

}


/**
 * Form API submit callback for the entity_type form.
 * 
 * @todo remove hard-coded link
 */
function merci_line_item_ui_form_submit(&$form, &$form_state) {
  
  $entity_type = $form_state['entity_type'];

  $entity = entity_ui_controller($entity_type)->entityFormSubmitBuildEntity($form, $form_state);
  $entity_info = $entity->entityInfo();
  $path = $entity_info['admin ui']['path'];
  // Save the entity_type and go back to the list of entity_types
  
  // Add in created and changed times.
  if ($entity->is_new = isset($entity->is_new) ? $entity->is_new : 0){
    $entity->created = time();
  }

  $entity->changed = time();
  
  $entity->save();
  $type_url_str = str_replace('_', '-', $entity_type);
  $form_state['redirect'] = $path . '/' . $entity->line_item_id . '/edit';//'admin/content/' . $type_url_str;

  drupal_set_message(t('Line item saved.'));
}

/**
 * Form API submit callback for the delete button.
 * 
 * @todo Remove hard-coded path
 */
function merci_line_item_ui_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'merci_line_item/' . $form_state['merci_line_item']->id . '/delete';
}


/**
 * Form callback: confirmation form for deleting a merci_reservation_detail.
 *
 * @param $merci_reservation_detail
 *   The merci_reservation_detail to delete
 *
 * @see confirm_form()
 */
function merci_line_item_ui_operation_form($form, &$form_state, $entity_type, $entity) {
  $form_state['merci_line_item'] = $entity;

  $form['#submit'][] = 'merci_line_item_ui_delete_form_submit';

  $form = confirm_form($form,
    t('Are you sure you want to delete %type %name?', array('%type' => $entity->type, '%name' => $entity->id)),
    'admin/merci/reservation',
    '<p>' . t('This action cannot be undone.') . '</p>',
    t('Delete'),
    t('Cancel'),
    'confirm'
  );
  
  return $form;
}

/**
 * Submit callback for merci_reservation_detail_delete_form
 */
function merci_line_item_ui_delete_form_submit($form, &$form_state) {
  $entity_type = 'merci_line_item';
  $entity = $form_state['merci_line_item'];

  entity_delete($entity_type, $entity->id);

  drupal_set_message(t('The merci_line_item %name has been deleted.', array('%name' => $entity->name)));
  watchdog('merci_line_item', 'Deleted %type %name.', array('%type' => $entity->type, '%name' => $entity->id));

  //$form_state['redirect'] = 'admin/merci/reservations/list';
}

