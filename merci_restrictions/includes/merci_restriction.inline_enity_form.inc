<?php

/**
 * @file
 * Defines the inline entity form controller for Nodes.
 */

class MerciRestrictionInlineEntityFormController extends EntityInlineEntityFormController {

  /**
  * Overrides EntityInlineEntityFormController::defaultLabels().
   */
  public function defaultLabels() {
    $labels = array(
      'singular' => t('merci restriction'),
      'plural' => t('merci restrictions'),
    );
    return $labels;
  }

  /**
   * Overrides EntityInlineEntityFormController::tableFields().
   */
  public function tableFields($bundles) {
    //$fields = parent::tableFields($bundles);
    $fields = array();

    $fields[MERCI_ALLOWED_ROLES] = array(
      'type' => 'field',
      'label' => t('Role'),
      'formatter' => 'list_default',
      'weight' => 4,
    );
    $fields[MERCI_CHECKOUT_MAX_LENGTH] = array(
      'type' => 'field',
      'label' => t('Checkout Max Length'),
      'formatter' => 'text_default',
      'weight' => 5,
    );
    $fields[MERCI_CHECKOUT_OFFSITE] = array(
      'type' => 'field',
      'label' => t('Checkout Offsite'),
      'formatter' => 'boolean_yes_no',
      'weight' => 6,
    );

    return $fields;
  }

  /**
   * Overrides EntityInlineEntityFormController::defaultSettings().
   */
  public function defaultSettings() {
    $defaults = parent::defaultSettings();

    // Line items should always be deleted when the order is deleted, they
    // are never managed alone.
    //$defaults['delete_references'] = TRUE;

    return $defaults;
  }

  /**
   * Overrides EntityInlineEntityFormController::settingsForm().
   */
  public function settingsForm($field, $instance) {
    $form = parent::settingsForm($field, $instance);

    // Adding existing entities is not supported for line items.
    //$form['allow_existing']['#access'] = FALSE;
    //$form['match_operator']['#access'] = FALSE;

    return $form;
  }

  /**
   * Overrides EntityInlineEntityFormController::entityForm().
   */
  public function entityForm($entity_form, &$form_state) {
    $form = parent::entityForm($entity_form, $form_state);

    return $form;
  }
  /**
   * Overrides EntityInlineEntityFormController::entityFormSubmit().
   */
  public function entityFormSubmit(&$entity_form, &$form_state) {
    $line_item = $entity_form['#entity'];
    $line_item->tid = $form_state['term']->tid;
    parent::entityFormSubmit($entity_form, $form_state);
  }
}
