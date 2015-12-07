<?php

/**
 * @file
 * Contains \Drupal\merci_open_hours_constraint\Plugin\Validation\Constraint\MerciOpenHoursConstraintValidator.
 */

namespace Drupal\merci_open_hours_constraint\Plugin\Validation\Constraint;

use Symfony\Component\Validator\Constraint;
use Symfony\Component\Validator\ConstraintValidator;
use Drupal\Core\Datetime\Entity\DateFormat;
use Drupal\Core\Datetime\DrupalDateTime;

/**
 * Checks for conflicts when validating a entity with reservable items.
 */
class MerciOpenHoursConstraintValidator extends ConstraintValidator {

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
      'reservable_hours' => 'field_reservable_hours',
    );

    $datetime_start = $value->getEntity()->{$constraint->date_field}[0]->date;
    $datetime_end = $value->getEntity()->{$constraint->date_field}[0]->date2;
    $datetime_start->setTimeZone(timezone_open(drupal_get_user_timezone()));
    $datetime_end->setTimeZone(timezone_open(drupal_get_user_timezone()));
    $date_format = DateFormat::load('html_date')->getPattern();
    $time_format = DateFormat::load('html_time')->getPattern();
    $date_time_format = trim($date_format . ' ' . $time_format);
    $timezone = $datetime_start->getTimezone();

    $date_start = $datetime_start->format($date_format);
    $date_end   = $datetime_end->format($date_format);

    $start_error = FALSE;
    $end_error = FALSE;

    foreach ($referenced_entity->{$constraint->reservable_hours_field}[0]->entity->{$constraint->office_hours_field} as $open_hours) {

      $open_hours->startdate->setTimeZone(timezone_open(drupal_get_user_timezone()));
      $open_hours->enddate->setTimeZone(timezone_open(drupal_get_user_timezone()));
      $date_time_input = trim($date_start . ' ' . $open_hours->startdate->format($time_format));
      $open = DrupalDateTime::createFromFormat($date_time_format, $date_time_input, $timezone);
      $date_time_input = trim($date_start . ' ' . $open_hours->enddate->format($time_format));
      $close = DrupalDateTime::createFromFormat($date_time_format, $date_time_input, $timezone);
      if ($open >= $datetime_start && $datetime_start <= $close) {
        $start_error = TRUE;
      }
      if ($open >= $datetime_end && $datetime_end <= $close) {
        $end_error = TRUE;
      }
    }
    if ($start_error) {
      $this->context->addViolation('Start is outside open hours.');
    }
    if ($end_error) {
      $this->context->addViolation('End is outside open hours.');
    }

  }
}
