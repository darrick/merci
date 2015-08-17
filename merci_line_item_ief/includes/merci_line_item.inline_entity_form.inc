<?php

/**
 * @file
 * Defines the inline entity form controller for Commerce Line Items.
 */

class MerciLineItemInlineEntityFormController extends EntityInlineEntityFormController {

  /**
   * Overrides EntityInlineEntityFormController::tableFields().
   */
  public function tableFields($bundles) {
    $fields = array();

    $bundle = reset($bundles);
    $entity_type = $this->entityType;
    $options = array(
      'default' => FALSE,
      'deleted' => FALSE,
      'language' => NULL,
    );
    $instances = _field_invoke_get_instances($entity_type, $bundle, $options);
    foreach ($instances as $instance) {
      $display = field_get_display($instance, 'list', NULL);
      if ($display['type'] != 'hidden') {
        $fields[$instance['field_name']] = array(
          'type' => 'field',
          'label' => $instance['label'],
          //'formatter' => 'list_default',
          'weight' => $display['weight'],
          'settings' => $display['settings'],
        );

      }
    }
    $extra_fields = field_info_extra_fields($entity_type, $bundle, 'display');

    foreach ($extra_fields as $field_name => $field) {

      if ($field['display']['list']['visible']) {
        $fields[$field_name] = array(
          'type' => 'property',
          'label' => $field['label'],
          //'formatter' => 'list_default',
          'weight' => $field['display']['list']['weight'],
        );
      }
    }
    return $fields;
  }

  /**
   * Overrides EntityInlineEntityFormController::defaultSettings().
   */
  public function defaultSettings() {
    $defaults = parent::defaultSettings();

    // Line items should always be deleted when the order is deleted, they
    // are never managed alone.
    $defaults['delete_references'] = TRUE;

    return $defaults;
  }

  /**
   * Overrides EntityInlineEntityFormController::settingsForm().
   */
  public function settingsForm($field, $instance) {
    $form = parent::settingsForm($field, $instance);

    // Adding existing entities is not supported for line items.
    $form['allow_existing']['#access'] = FALSE;
    $form['match_operator']['#access'] = FALSE;

    return $form;
  }

  /**
   * Overrides EntityInlineEntityFormController::entityForm().
   */
  public function entityForm($entity_form, &$form_state) {
    drupal_add_css(drupal_get_path('module', 'merci_line_item') . '/merci_line_item.css');
    $line_item = $entity_form['#entity'];
    $extra_fields = field_info_extra_fields('merci_line_item', $line_item->type, 'form');

    $entity_form['line_item_details'] = array(
      '#type' => 'fieldset',
      '#title' => t('Line item details'),
      '#attributes' => array('class' => array('ief-line_item-details', 'ief-entity-fieldset')),
    );

    $entity_form['quantity'] = array(
      '#type' => 'textfield',
      '#datatype' => 'integer',
      '#title' => t('Quantity'),
      '#description' => t('The quantity of line items.'),
      '#default_value' => (int) $line_item->quantity,
      '#size' => 4,
      '#maxlength' => max(4, strlen($line_item->quantity)),
      '#required' => TRUE,
      '#weight' => $extra_fields['quantity']['weight'],
      '#fieldset' => 'line_item_details',
    );

    field_attach_form('merci_line_item', $line_item, $entity_form, $form_state);

    $line_item_wrapper = entity_metadata_wrapper('merci_line_item', $line_item);

    unset($entity_form[MERCI_RESOURCE_REFERENCE]);
    unset($entity_form[MERCI_RESOURCE_DISPLAY]);

    $parents = $entity_form[MERCI_CHECKOUT_STATUS]['und']['#field_parents'];
    $head = array_shift($parents);
    $parents = implode('][', $parents);

    $field_path = $head . '[' . $parents . '][' . MERCI_CHECKOUT_STATUS . '][und]';

    $entity_form[MERCI_RETURN_DATE]['#states'] = array(
      'visible' => array(
        ':input[name="' . $field_path .  '"]' => array('value' => 'returned'),
      ),
    );

    // Add all fields to the main fieldset.
    foreach (field_info_instances('merci_line_item', $line_item->type) as $a => $instance) {
      $entity_form[$instance['field_name']]['#fieldset'] = 'line_item_details';

      // TODO Move elsewhere.
      if ($instance['field_name'] == MERCI_RETURNED_ACCESSORIES or $instance['field_name'] == MERCI_CHECKED_OUT_ACCESSORIES) {
        try {
          $product_wrapper = entity_metadata_wrapper('merci_line_item', $line_item)->{MERCI_RESOURCE_DISPLAY};
          $terms = array();
          foreach ($product_wrapper->merci_resource_accessories->value() as $index => $term) {
            $terms[$term->tid] = $term;
          }
          if (count($terms) == 1 and reset($terms)->name == 'None') {
            unset($entity_form[$instance['field_name']]);
          } else {
            $entity_form[$instance['field_name']]['und']['#options'] = array_intersect_key($entity_form[$instance['field_name']]['und']['#options'], $terms);
          }
        }
        catch (EntityMetadataWrapperException $exc) {
          $entity_form[$instance['field_name']]['und']['#options'] = array();
          $entity_form[$instance['field_name']]['und'] = array();
        }
      }

    }

    // Add a css class to widget form elements for all fields of type mytype.
    //

    return $entity_form;
  }

  /**
   * Overrides EntityInlineEntityFormController::entityFormValidate().
   *
   * @todo Remove once Commerce gets a quantity #element_validate function.
   */
  public function entityFormValidate($entity_form, &$form_state) {
    $line_item = $entity_form['#entity'];
    $line_item_values = drupal_array_get_nested_value($form_state['values'], $entity_form['#parents']);

    $line_item->quantity = $line_item_values['quantity'];

    // Validate attached fields.
    field_attach_form_validate('merci_line_item', $line_item, $entity_form, $form_state);

    // Return if any errors from validation.
    if (form_get_errors()) {
      return t('Form error');
    }

    // Allow other modules to validate.
    foreach (module_implements('merci_line_item_validate') as $module) {
      $function = $module . '_merci_line_item_validate';
      $function($line_item, $entity_form, $form_state);
    }

    // Submit the attached fields.
    field_attach_submit('merci_line_item', $line_item, $entity_form, $form_state);

  }

  /**
   * Overrides EntityInlineEntityFormController::entityFormSubmit().
   */
  public function entityFormSubmit(&$entity_form, &$form_state) {
    $line_item = $entity_form['#entity'];

    //field_attach_submit('merci_line_item', $line_item, $entity_form, $form_state);


  }

  /**
   * Overrides EntityInlineEntityFormController::removeForm().
   */
  public function removeForm($remove_form, &$form_state) {
    // EntityInlineEntityFormController::removeForm uses the entity label
    // in the confirmation message, but line items don't have any.
    $remove_form['message'] = array(
      '#markup' => '<div>' . t('Are you sure you want to remove this line item?') . '</div>',
    );

    return $remove_form;
  }

  /**
   * Permanently saves the given entity.
   *
   * @param $entity
   *   The entity to save.
   * @param array $context
   *   Available keys:
   *   - parent_entity_type: The type of the parent entity.
   *   - parent_entity: The parent entity.
   */
  public function save($entity, $context) {
    $wrapper = entity_metadata_wrapper($context['parent_entity_type'], $context['parent_entity']);
    $entity->entity_id = $wrapper->getIdentifier();
    parent::save($entity, $context);
  }
}
