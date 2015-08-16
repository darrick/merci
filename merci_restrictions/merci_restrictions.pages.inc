<?php

/**
 * Form callback: create or edit a entity_type.
 *
 * @param $entity_type
 *   The entity_type object to edit or for a create form an empty entity_type object
 *     with only a entity_type type defined.
 */
function merci_restrictions_form($form, &$form_state, $entity) {
  $entity_type = $form_state['entity_type'];
  $entity_info = $entity->entityInfo();
  $path = $entity_info['admin ui']['path'];
  $form['#entity'] = $entity;

  $form['title'] = array(
    '#type' => 'textfield', 
    '#title' => 'Title', 
    '#default_value' => !empty($entity->title) ? $entity->title : '', 
    '#required' => TRUE, 
    '#weight' => -5,
  );

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
    '#submit' => $submit + array('merci_restrictions_form_submit'),
  );

  if (!empty($entity->id)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#limit_validation_errors' => array(),
      '#submit' => $submit + array('merci_restrictions_form_submit_delete'),
      '#weight' => 45,
    );
  }

  // We append the validate handler to #validate in case a form callback_wrapper
  // is used to add validate handlers earlier.
  $form['#validate'][] = 'merci_restrictions_form_validate';

  if (empty($form['field_operator']['und'][0]['target_id']['#default_value'])) {
    global $user;
    $form['field_operator']['und'][0]['target_id']['#default_value'] = $user->name . ' (' . $user->uid . ')';
  }

  return $form;
}

/**
 * Form API validate callback for the entity_type form
 */
function merci_restrictions_form_validate(&$form, &$form_state) {
  $entity_type = $form_state['entity_type'];
  $entity = $form_state[$entity_type];
  
  // Notify field widgets to validate their data.
  field_attach_form_validate($entity_type, $entity, $form, $form_state);

  /*
  $behaviors = merci_get_accessible_behavior_plugins();

  foreach($behaviors as $name => $plugin) {
    $class = _merci_get_behavior_handler($name, $entity_type, $entity);
    $class->validate($form, $form_state);
    // Do no validation if their errors from the main validation function.
    if (form_get_errors()) {
      return;
    }
  }
   */
}


/**
 * Form API submit callback for the entity_type form.
 * 
 * @todo remove hard-coded link
 */
function merci_restrictions_form_submit(&$form, &$form_state) {
  
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
  
  entity_save($entity_type, $entity);
  //$entity->save();
  $type_url_str = str_replace('_', '-', $entity_type);
  $form_state['redirect'] = $path . '/' . $entity->id . '/edit';//'admin/content/' . $type_url_str;

  drupal_set_message(t('Checkout saved.'));
}

/**
 * Form API submit callback for the delete button.
 * 
 * @todo Remove hard-coded path
 */
function merci_restrictions_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'merci_restrictions/' . $form_state['merci_restrictions']->id . '/delete';
}


/**
 * Form callback: confirmation form for deleting a merci_restrictions_detail.
 *
 * @param $merci_restrictions_detail
 *   The merci_restrictions_detail to delete
 *
 * @see confirm_form()
 */
function merci_restrictions_operation_form($form, &$form_state, $entity_type, $entity) {
  $form_state['merci_restrictions'] = $entity;

  $form['#submit'][] = 'merci_restrictions_delete_form_submit';

  $form = confirm_form($form,
    t('Are you sure you want to delete %type %name?', array('%type' => $entity->type, '%name' => $entity->id)),
    'admin/merci/reservations',
    '<p>' . t('This action cannot be undone.') . '</p>',
    t('Delete'),
    t('Cancel'),
    'confirm'
  );
  
  return $form;
}

/**
 * Submit callback for merci_restrictions_detail_delete_form
 */
function merci_restrictions_delete_form_submit($form, &$form_state) {
  $entity_type = 'merci_restrictions';
  $entity = $form_state['merci_restrictions'];

  entity_delete($entity_type, $entity->id);

  drupal_set_message(t('The merci_restrictions %name has been deleted.', array('%name' => $entity->name)));
  watchdog('merci_restrictions', 'Deleted %type %name.', array('%type' => $entity->type, '%name' => $entity->id));

  //$form_state['redirect'] = 'admin/merci/reservations/list';
}

