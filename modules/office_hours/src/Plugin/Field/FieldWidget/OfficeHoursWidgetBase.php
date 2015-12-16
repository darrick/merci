<?php

/**
 * @file
 * Contains \Drupal\office_hours\Plugin\Field\FieldWidget\OfficeHoursWidgetBase.
 */

namespace Drupal\office_hours\Plugin\Field\FieldWidget;

use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Field\FieldItemListInterface;
use Drupal\Core\Field\WidgetBase;
use Drupal\office_hours\OfficeHoursDateHelper;


/**
 * Base class for the 'office_hours_*' widgets.
 */
abstract class OfficeHoursWidgetBase extends WidgetBase {

  /**
   * {@inheritdoc}
   */
  public static function defaultSettings() {
    $settings = array(
      'time_type' => 'G',
      'increment' => 60,
      'date_element_type' => 'datelist',
    ) + parent::defaultSettings();

    return $settings;
  }

  /**
   * Returns the array of field settings, added with hours data.
   *
   * @return array
   *   The array of settings.
   */
  public function getFieldSettings() {
    $settings = parent::getFieldSettings();
    return $settings;
  }
  /**
   * {@inheritdoc}
   */
  public function settingsForm(array $form, FormStateInterface $form_state) {
    $element = parent::settingsForm($form, $form_state);

    $element['date_element_type'] = array(
      '#type' => 'select',
      '#title' => t('Time element type'),
      '#description' => t('Select the widget type for inputing time.'),
      '#options' => array(
        'datelist' => 'Select list',
        'datetime' => 'HTML5 time input',
      ),
      '#default_value' => $this->getSetting('date_element_type'),
    );

    // @todo D8: aligen with DateTimeDatelistWidget.
    $element['time_type'] = array(
      '#type' => 'select',
      '#title' => t('Time type'),
      '#options' => array(
        'G' => t('24 hour time') . ' (9:00)', // D7: key = 0
        'H' => t('24 hour time') . ' (09:00)', // D7: key = 2
        'g' => t('12 hour time') . ' (9:00 am)', // D7: key = 1
        'h' => t('12 hour time') . ' (09:00 am)', // D7: key = 1
      ),
      '#default_value' => $this->getSetting('time_type'),
      '#required' => FALSE,
      '#description' => t('Format of the time in the widget.'),
    );

    // @todo D8: align with DateTimeDatelistWidget.
    $element['increment'] = array(
      '#type' => 'select',
      '#title' => t('Time increments'),
      '#default_value' => $this->getSetting('increment'),
      '#options' => array(
        1 => t('1 minute'),
        5 => t('5 minute'),
        15 => t('15 minute'),
        30 => t('30 minute'),
        60 => t('60 minute')
      ),
      '#required' => FALSE,
      '#description' => t('Restrict the input to fixed fractions of an hour.'),
    );

    return $element;
  }

  /**
   * {@inheritdoc}
   */
  public function settingsSummary() {
    $summary = parent::settingsSummary();
    $summary[] = t('Time element type: @date_element_type', array('@date_element_type' => $this->getSetting('date_element_type')));
    return $summary;
  }

}
