<?php

/**
 * Form callback: create or edit a entity_type.
 *
 * @param $entity_type
 *   The entity_type object to edit or for a create form an empty entity_type object
 *     with only a entity_type type defined.
 */
function merci_reservation_form($form, &$form_state, $entity) {
  $entity_type = $form_state['entity_type'];
  $entity_info = $entity->entityInfo();
  $path = $entity_info['admin ui']['path'];
  $form['#entity'] = $entity;

  // Ensure the owner name is accessible if the uid is set.
  if (!empty($entity->uid) && $owner = user_load($entity->uid)) {
    $entity->owner = $owner->name;
  }


  $form['owner'] = array(
    '#type' => 'textfield', 
    '#access' => user_access('edit any merci_reservation entities'),
    '#title' => t('Reservation for ...'), 
    '#maxlength' => 60, 
    '#autocomplete_path' => 'user/autocomplete', 
    '#default_value' => !empty($entity->owner) ? $entity->owner : '', 
    '#weight' => -1, 
    '#description' => t('Leave blank for %anonymous.', array('%anonymous' => variable_get('anonymous', t('Anonymous')))),
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
    '#submit' => $submit + array('merci_reservation_form_submit'),
  );

  if (!empty($entity->id)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#limit_validation_errors' => array(),
      '#submit' => $submit + array('merci_reservation_form_submit_delete'),
      '#weight' => 45,
    );
  }

  // We append the validate handler to #validate in case a form callback_wrapper
  // is used to add validate handlers earlier.
  $form['#validate'][] = 'merci_reservation_form_validate';


  return $form;
}

/**
 * Form API validate callback for the entity_type form
 */
function merci_reservation_form_validate(&$form, &$form_state) {
  $entity_type = $form_state['entity_type'];
  $entity = $form_state[$entity_type];
  
  // Validate the "authored by" field.
  if (!empty($form_state['values']['owner']) && !($account = user_load_by_name($form_state['values']['owner']))) {
    // The use of empty() is mandatory in the context of usernames
    // as the empty string denotes the anonymous user. In case we
    // are dealing with an anonymous user we set the user ID to 0.
    form_set_error('name', t('The username %name does not exist.', array('%name' => $form_state['values']['owner'])));
  }
  // Notify field widgets to validate their data.
  field_attach_form_validate($entity_type, $entity, $form, $form_state);

  /*
   * Todo this is evergreen dependent.
   */
  /*
  if (empty($entity->merci_customer_billing) and empty($entity->merci_customer_homeroom) and empty($entity->merci_customer_organization)) {

    $parents_path = implode('][', $form['customer_profile_type_ui']['customer_profile_ui_checkboxes']['merci_customer_billing']['#array_parents']) ;
    form_set_error($parents_path, t('You need to add a patron before checking out equipment'));
  }
   */

  if ($form_state['triggering_element']['#value'] == 'Save') {
    if (count($entity->merci_line_item_reference['und']) == 1) {
      form_set_error('', t('You need to add resources to the checkout.'));
    }
  }

  // Do no validation if their errors from the main validation function.
  if (form_get_errors()) {
    return;
  }

  $errors = array();
  $overrides = array();

  $error_wrapper = new EntityMetadataArrayObject($errors);

  $entity_wrapper = entity_metadata_wrapper($entity_type, $entity);
  rules_invoke_event('merci_reservation_validate', $entity_wrapper, $error_wrapper);
  $errors = $error_wrapper->getArray();
  if (!empty($errors)) {
    $parents_path = implode('][', $form['#parents']);
    form_set_error($parents_path, implode(', ', $errors));
  }
}


/**
 * Form API submit callback for the entity_type form.
 * 
 * @todo remove hard-coded link
 */
function merci_reservation_form_submit(&$form, &$form_state) {
  
  $entity_type = $form_state['entity_type'];

  $entity = entity_ui_controller($entity_type)->entityFormSubmitBuildEntity($form, $form_state);
  $entity_info = $entity->entityInfo();
  $path = $entity_info['admin ui']['path'];
  
  // A user might assign the node author by entering a user name in the node
  // form, which we then need to translate to a user ID.
  if (isset($entity->owner)) {
    if ($account = user_load_by_name($entity->owner)) {
      $entity->uid = $account->uid;
    }
    else {
      $entity->uid = 0;
    }
  }
  // Add in created and changed times.
  if ($entity->is_new = isset($entity->is_new) ? $entity->is_new : 0){
    $entity->created = time();
  }

  $entity->changed = time();
  
  // Save the entity_type and go back to the list of entity_types
  $entity->save();
  $type_url_str = str_replace('_', '-', $entity_type);
  //$form_state['redirect'] = $path . '/' . $entity->id . '/edit';//'admin/content/' . $type_url_str;

  drupal_set_message(t('Checkout saved.'));
}

/**
 * Form API submit callback for the delete button.
 * 
 * @todo Remove hard-coded path
 */
function merci_reservation_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'merci_reservation/' . $form_state['merci_reservation']->id . '/delete';
}


/**
 * Form callback: confirmation form for deleting a merci_reservation_detail.
 *
 * @param $merci_reservation_detail
 *   The merci_reservation_detail to delete
 *
 * @see confirm_form()
 */
function merci_reservation_operation_form($form, &$form_state, $entity_type, $entity) {
  $form_state['merci_reservation'] = $entity;

  $form['#submit'][] = 'merci_reservation_delete_form_submit';

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
 * Submit callback for merci_reservation_detail_delete_form
 */
function merci_reservation_delete_form_submit($form, &$form_state) {
  $entity_type = 'merci_reservation';
  $entity = $form_state['merci_reservation'];

  entity_delete($entity_type, $entity->id);

  drupal_set_message(t('The merci_reservation %name has been deleted.', array('%name' => $entity->name)));
  watchdog('merci_reservation', 'Deleted %type %name.', array('%type' => $entity->type, '%name' => $entity->id));

  //$form_state['redirect'] = 'admin/merci/reservations/list';
}

