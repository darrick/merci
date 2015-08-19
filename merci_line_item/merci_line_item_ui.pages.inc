<?php

/**
 * Form callback: create or edit a entity_type.
 *
 * @param $entity_type
 *   The entity_type object to edit or for a create form an empty entity_type object
 *     with only a entity_type type defined.
 */
function merci_line_item_form($form, &$form_state, $line_item) {

  // Add custom css to prettify the form.
  drupal_add_css(drupal_get_path('module', 'merci_line_item') . '/merci_line_item.css');

  $entity_type = $form_state['entity_type'];
  $entity_info = $line_item->entityInfo();
  $path = $entity_info['admin ui']['path'];
  $form['#entity'] = $line_item;
  $account = user_load($line_item->uid);

  if (empty($line_item->owner)) {
    $line_item->owner = !empty($account) ? $account->name : '';
  }

  $form['owner'] = array(
    '#type' => 'textfield', 
    '#access' => user_access('edit any merci_reservation entities'),
    '#title' => t('Reservation for ...'), 
    '#maxlength' => 60, 
    '#autocomplete_path' => 'user/autocomplete', 
    '#default_value' => !empty($line_item->owner) ? $line_item->owner : '', 
    '#description' => t('Leave blank for %anonymous.', array('%anonymous' => variable_get('anonymous', t('Anonymous')))),
  );

  $form['quantity'] = array(
    '#type' => 'textfield',
    '#datatype' => 'integer',
    '#title' => t('Quantity'),
    '#description' => t('The quantity of the selected resource to reserve.'),
    '#default_value' => (int) $line_item->quantity,
    '#size' => 4,
    '#maxlength' => max(4, strlen($line_item->quantity)),
    '#required' => TRUE,
  );

  // Add the field related form elements.
  $form_state[$entity_type] = $line_item;
  field_attach_form($entity_type, $line_item, $form, $form_state);

  // Only show return date field if the checkout status warrents it.
  $form_parents = array();
  $parents = array_merge($form_parents, array(MERCI_CHECKOUT_STATUS, 'und'));
  $head = array_shift($parents);
  $field_path = implode('][', $parents);

  $form[MERCI_RETURN_DATE]['#states'] = array(
    'visible' => array(
      ':input[name="' . $head . '[' . $field_path .  ']"]' => array('value' => 'returned'),
    ),
  );
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

  if (!empty($line_item->id)) {
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
  
  $account = user_load_by_name($form_state['values']['owner']);
  // Validate the "authored by" field.
  if (!$account) {
    // The use of empty() is mandatory in the context of usernames
    // as the empty string denotes the anonymous user. In case we
    // are dealing with an anonymous user we set the user ID to 0.
    form_set_error('name', t('The username %name does not exist.', array('%name' => $form_state['values']['owner'])));
  }

  $entity->uid = $account->uid;

  // Notify field widgets to validate their data.
  field_attach_form_validate($entity_type, $entity, $form, $form_state);


  // Do no validation if their errors from the main validation function.
  if (form_get_errors()) {
    return;
  }

  // Determine if they are any conflicts. 
  try {
    merci_line_item_validate($entity);
  } catch ( MerciException $e) {
    $parents_path = implode('][', $e->getParents());
    $conflicts = $e->getData();
    $quantity = $entity->quantity;
    $line_item_wrapper = entity_metadata_wrapper($entity_type, $entity);
    $resource  = $line_item_wrapper->{MERCI_RESOURCE_REFERENCE};

    // Display an error for not enough available items.
    if (count($conflicts) < $quantity) {
      $available = $quantity - count($conflicts);
      form_set_error($parents_path, t('You reserved %count but only %available are available.',
                      array('%count' => $quantity, '%available' => $available)));
    } else {
      // Display error showing conflicts.
      $ids = array();
      foreach ($conflicts as $conflict) {
        foreach ($conflict as $item) {
          $ids[] = $item->parent_id;
        }
      }

      // Load the entities which hold the conflicting item.
      $entities = entity_load($entity_type, $ids);

      foreach ($entities as $id => $line_item) {
        $entity_uri = entity_uri($entity_type, $line_item);
        $entity_label = entity_label($entity_type, $line_item);
        $line_items[] = l(t("@label", array('@label' => $entity_label)), $entity_uri['path']);
      }

      form_set_error($parents_path, t('%name is already checked out or reserved by: !orders', array('%name' => $resource->label(), '!orders' => implode(', ', $line_items))));
    }
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

