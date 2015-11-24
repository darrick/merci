<?php

/**
 * @file
 * Contains \Drupal\merci_entityref_behavior\Plugin\Validation\Constraint\ReservationConflictConstraintValidator.
 */

namespace Drupal\merci_entityref_behavior\Plugin\Validation\Constraint;

use Symfony\Component\Validator\Constraint;
use Symfony\Component\Validator\ConstraintValidator;
use Drupal\merci_core\ReservationConflicts;

/**
 * Checks for conflicts when validating a entity with reservable items.
 */
class ReservationConflictConstraintValidator extends ConstraintValidator {

  /**
   * Overrides EntityReference_BehaviorHandler_Abstract::access().
   *
   * Ensure that it is only enabled for ER instances on nodes targeting
   * terms, and the core variable to maintain index is enabled.
  public function access($field, $instance) {
    return true;
  }

  public function presave($entity_type, $entity, $field, $instance, $langcode, &$items) {
   // merci_api_presave_reference($entity_type, $entity, $field, $instance, $langcode, $items, &$errors);
  }
  public function insert($entity_type, $entity, $field, $instance, $langcode, &$items) {

    //merci_api_insert_reference($entity_type, $entity, $field, $instance, $langcode, $items, &$errors);
  }
  /**
   * Overrides EntityReference_BehaviorHandler_Abstract::entityPostInsert().
   *
   * Runs after hook_node_insert() used by taxonomy module.
  public function entityPostInsert($entity_type, $entity, $field, $instance) {
  }

  /**
   * Overrides EntityReference_BehaviorHandler_Abstract::entityPostUpdate().
   *
   * Runs after hook_node_update() used by taxonomy module.
  public function entityPostUpdate($entity_type, $entity, $field, $instance) {
  }

  /**
   * Overrides EntityReference_BehaviorHandler_Abstract::settingsForm().
  public function settingsForm($field, $instance) {
    $form = array();
    $fields_info = field_info_instances($instance['entity_type'], $instance['bundle']);
    $date_fields = array();
    $date_fields = array('disabled' => t('Disabled'));
    foreach ($fields_info as $field_name => $info) {
      $more_info = field_info_field($field_name);
      if ( $more_info['type'] == 'datetime' || $more_info['type'] == 'date' || $more_info['type'] == 'datestamp') {
        $date_fields[$field_name] = $info['label'];
      }
    }

    $settings = $field['settings']['handler_settings']['behaviors']['merci'];
    $default_value = (array_key_exists('merci', $settings) and array_key_exists('merci_date_field', $settings['merci'])) ? $settings['merci']['merci_date_field'] : 'disabled';
    $form['merci'] = array(
      '#type' => 'fieldset',
      '#title' => t('Merci'),
      '#collapsible' => TRUE,
      '#collapsed' => $default_value == 'disabled' ? true : false, 
    );

    if (empty($date_fields)) {
      $form['merci']['merci_date_field'] = array(
        '#markup' => t('Please add a date field to this entity to use to filter reservations.')
      );
    } else {
      $form['merci']['merci_date_field'] = array(
        '#type' => 'select',
        '#title' => t('Date field'),
        '#options' => $date_fields,
        '#default_value' => $default_value,
        '#description' => t('Select the date field to use to limit reservations of this entity field.'),
      );
    }

    return $form;
  }
   */

  /**
   * {@inheritdoc}
   */
  public function validate($value, Constraint $constraint) {
    /* @var \Drupal\Core\Field\FieldItemInterface $value */
    if (!isset($value)) {
      return;
    }
    $id = $value->target_id;
    // '0' or NULL are considered valid empty references.
    if (empty($id)) {
      return;
    }
    /* @var \Drupal\Core\Entity\FieldableEntityInterface $referenced_entity */
    $referenced_entity = $value->entity;

    $context = array(
      'quantity_field' => 'field_quantity',
      'date_field' => 'merci_reservation_date', //$value->getFieldDefinition()->getSetting('merci_date_field'),
      'item_field' => $value->getFieldDefinition()->getName(),
    );


    $conflicts = \Drupal::service('merci_core.reservation_conflicts');
    $conflicts->setEntity($value->getEntity());
    $conflicts->setDateField('merci_reservation_date');
    $conflicts->setItemField('merci_reservation_items');


    foreach ($conflicts->getErrors() as $delta => $errors) {
      $msg = array();

      /*
      if (array_key_exists(MERCI_ERROR_TOO_MANY, $errors)) {
        $msg[] = $errors[MERCI_ERROR_TOO_MANY];
      } elseif (array_key_exists(MERCI_ERROR_CONFLICT, $errors)) {
        foreach ($errors[MERCI_ERROR_CONFLICT] as $date_start => $message) {
          $msg[] = $message;
        }
      }
      $errors[$field['field_name']][$langcode][$delta][] = array(
        'error' => 'merci',
        'message' => implode('<br>', $msg),
      );
       */
      //$this->context->addViolation($constraint->message, array('%type' => $type, '%id' => $id));
      $this->context->addViolation('Conflict');

    }
  }
}
