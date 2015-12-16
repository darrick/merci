<?php

/**
 * @file
 * Contains \Drupal\office_hours\Plugin\Field\FieldWidget\OfficeHoursListWidget.
 */

namespace Drupal\office_hours\Plugin\Field\FieldWidget;

use Drupal\Core\Field\AllowedTagsXssTrait;
use Drupal\Core\Field\FieldItemListInterface;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Render\Element;
use Drupal\office_hours\OfficeHoursDateHelper;
use Drupal\Core\Datetime\DrupalDateTime;
use Drupal\Core\Plugin\ContainerFactoryPluginInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;
use Symfony\Component\Validator\ConstraintViolationInterface;
use Drupal\Component\Utility\NestedArray;

/**
 * Plugin implementation of the 'office_hours_week' widget.
 *
 * @FieldWidget(
 *   id = "office_hours_list",
 *   label = @Translation("Office hours (list)"),
 *   field_types = {
 *     "office_hours",
 *     "date_combo",
 *   }
 * )
 */
class OfficeHoursListWidget extends OfficeHoursWidgetBase {

  /**
   * {@inheritdoc}
   */
  public function formElement(FieldItemListInterface $items, $delta, array $element, array &$form, FormStateInterface $form_state) {
    // Get field settings, to make it accessible for each element in other functions.
    $settings = $this->getSettings() + $items->getFieldDefinition()->getFieldStorageDefinition()->getSettings();

    $value = isset($items[$delta]) ? $items[$delta] : NULL;
    //$element['#type'] = 'fieldset';
    $element['#attributes']['class'][] = 'container-inline';
    $element['#office_hours'] = $value;

    if ($element['#day_hidden']) {
      $daynames = OfficeHoursDateHelper::weekDays(FALSE);
      $element['day'] = array(
        '#type' => 'value',
        '#value' => $value->day,
      );
      $element['day_label'] = array(
        '#markup' => $element['#day_delta'] == 0 ? $daynames[$items[$delta]->day] : t('And'),
      );

    } else {
      $element['day'] = array(
        '#type' => 'select',
        '#options' => OfficeHoursDateHelper::weekDays(FALSE),
        '#default_value' => $value->day,
        '#description' => '',
      );
    }

    $element['starthours'] = array(
      '#type' => 'office_hours_select', // datelist, datetime.
      '#default_value' => $value->startdate,
      '#field_settings' => $settings,
      '#date_element_type' => $this->getSetting('date_element_type'),
      '#date_increment' => $this->getSetting('increment'),
      '#date_timezone' => date_default_timezone_get(),
    );
    if ($items[$delta]->startdate) {
      $date = $items[$delta]->startdate;
      $date->setTimezone(new \DateTimeZone($element['starthours']['#date_timezone']));
      $element['starthours']['#default_value'] = $date;
    }
    $element['endhours'] = array(
      '#type' => 'office_hours_select', // datelist, datetime.
      '#default_value' => $value->enddate,
      '#field_settings' => $settings,
      '#date_element_type' => $this->getSetting('date_element_type'),
      '#date_increment' => $this->getSetting('increment'),
      '#date_timezone' => date_default_timezone_get(),
    );
    if ($items[$delta]->enddate) {
      $date = $items[$delta]->enddate;
      $date->setTimezone(new \DateTimeZone($element['endhours']['#date_timezone']));
      $element['endhours']['#default_value'] = $date;
    }

    return $element;
  }


  /**
   * {@inheritdoc}
   */
  public function massageFormValues(array $values, array $form, FormStateInterface $form_state) {
    // The widget form element type has transformed the value to a
    // DrupalDateTime object at this point. We need to convert it back to the
    // storage timezone and format.
    foreach ($values as &$item) {
      if (!empty($item['starthours']) && !empty($item['endhours']) && $item['starthours'] instanceof DrupalDateTime && $item['endhours'] instanceof DrupalDateTime) {
        $format = DATETIME_DATETIME_STORAGE_FORMAT;
        $date = $item['starthours'];
        $date2 = $item['endhours'];
        // Adjust the date for storage.
        $date->setTimezone(new \DateTimezone(DATETIME_STORAGE_TIMEZONE));
        $date2->setTimezone(new \DateTimezone(DATETIME_STORAGE_TIMEZONE));
        $item['starthours'] = $date->format($format);
        $item['endhours'] = $date2->format($format);
      }
    }
    return $values;
  }
}
