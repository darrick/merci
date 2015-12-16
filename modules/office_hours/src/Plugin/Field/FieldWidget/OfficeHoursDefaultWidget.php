<?php

/**
 * @file
 * Contains \Drupal\office_hours\Plugin\Field\FieldWidget\OfficeHoursDefaultWidget.
 */

namespace Drupal\office_hours\Plugin\Field\FieldWidget;

use Drupal\Core\Field\FieldItemListInterface;
use Drupal\Core\Field\FieldStorageDefinitionInterface;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Render\Element;
use Drupal\office_hours\OfficeHoursDateHelper;
use Drupal\Core\Field\FieldDefinitionInterface;
use Symfony\Component\Validator\ConstraintViolationInterface;
use Drupal\Component\Utility\NestedArray;

/**
 * Plugin implementation of the 'office_hours_default' widget.
 *
 * @FieldWidget(
 *   id = "office_hours_default",
 *   label = @Translation("Office hours (week)"),
 *   field_types = {
 *     "office_hours",
 *   },
 *   multiple_values = TRUE,
 * )
 */
class OfficeHoursDefaultWidget extends OfficeHoursListWidget {

  /**
   * {@inheritdoc}
   */
  public function formElement(FieldItemListInterface $items, $delta, array $element, array &$form, FormStateInterface $form_state) {
    //return parent::formElement($items, $delta, $element, $form, $form_state);

    // In D8, we have an anomaly in the widget.
    // We prepare 1 widget for the whole week, but the field has unlimited
    // cardinality. So with $delta = 0, we show ALL values.
    // @todo D8: make the table widget correct again. See string 'link'.
    // @see https://www.drupal.org/node/2064123 "Field types are now plugins"
    // @see https://www.drupal.org/node/2112677 "Dynamic/Virtual field values using computed field property classes"
    if ($delta > 0) {
      return;
    }

    // Get field settings, to make it accessible for each element in other functions.
    //$settings = $this->getSettings() + $this->getFieldDefinition()->getFieldStorage()->getSetting();

    $day_cardinality = $this->getFieldSetting('cardinality_per_day');

    $day_delta = 0;

    // Sort items by day and hours.
    $items->sortByDays();

    $indexed_items = array_fill_keys(range(0, 6), array());

    foreach ($items as $delta => $item) {
      $indexed_items[$item->day][] = $delta;
    }

    // Add items for missing days.
    foreach ($indexed_items as $day => $indexed_item) {
      for ($index = 0; $index < max(2, $day_cardinality); $index++) {
        if ($index >= count($indexed_item)) {
          $items->appendItem(array('day' => $day));
        }
      }
    }

    // Sort items by day and hours.  Since items have been appended.
    $items->sortByDays();

    $elements = array();
    $day_delta = 0;
    $element['#day_hidden'] = TRUE;
    unset($element['#field_parents']);
    foreach ($items as $delta => $item) {

      if ($items[$delta]->day == $day_delta) {
        $element['#day_delta'] = 0;
        $day_delta++;
      } else {
        $element['#day_delta']++;
      }

      $elements[$delta] = parent::formElement($items, $delta, $element, $form, $form_state);
    }

    /*
    $id = -1;
    foreach ($days as $index => $day) {
      // todo: theme_function clears values above cardinality. move it here.
      for ($daydelta = 0; $daydelta < max(2, $day_cardinality); $daydelta++) {
      }
    }
     */

// BEGIN D7 Code
    // Create an indexed two level array of time slots
    // First level are day numbers. Second level contains field values arranged by daydelta.
    /*
    $indexed_items = array_fill_keys(range(0, 6), array());
    foreach ($items as $index => $item) {
      $value_of_item = $item->getValue();
      if ($item && !empty($value_of_item)) {
        $indexed_items[(int) $value_of_item['day']][] = $item;
      }
    }

    // Build elements, sorted by first_day_of_week.
    $elements = array();
    $days = OfficeHoursDateHelper::weekDaysOrdered(range(0, 6));
    $daynames = OfficeHoursDateHelper::weekDays(FALSE);
    $id = -1;
    foreach ($days as $index => $day) {
      // todo: theme_function clears values above cardinality. move it here.
      for ($daydelta = 0; $daydelta < max(2, $day_cardinality); $daydelta++) {
        $id++;
        $elements[$id]['#day'] = $day;
        $elements[$id]['#daydelta'] = $daydelta;
        $elements[$id]['#dayname'] = $daynames[$day];

        $elements[$id]['#type'] = 'office_hours_slot';
        $elements[$id]['#default_value'] = isset($indexed_items[$day][$daydelta]) ? $indexed_items[$day][$daydelta]->getValue() : NULL;
        $elements[$id]['#field_settings'] = $settings;
        $elements[$id]['#date_element_type'] = $this->getSetting('date_element_type');
    $elements[$id]['#attached']['library'][] = 'office_hours/officehours';
      }
    }
     */
    // Build multi element widget.
    $element['value'] = [
/*
        // Begin D7-code.
        //      '#theme' => 'office_hours_week',
        '#field_name' => $items->getName(),
        '#title' => $this->fieldDefinition->getLabel(),
        //      '#required' => $this->fieldDefinition->isRequired(),
        '#description' => $this->fieldDefinition->getDescription(),
        // End   D7-code.
*/
        '#type' => 'office_hours_table',
        '#header' => array(
          '',
          t($element['#title']),
          // t('Day'),
          t('From'),
          t('To'),
        ),
        '#tableselect' => FALSE,
        '#tabledrag' => FALSE,
      ] + $elements;

    return $element;
  }

  /**
   * {@inheritdoc}
   */
  public function errorElement(array $element, ConstraintViolationInterface $error, array $form, FormStateInterface $form_state) {
    $error_item = $error->getInvalidValue();
    $input = NestedArray::getValue($form_state->getValues(), $element['#parents'], $input_exists);
    foreach ($input['value'] as $key => $item) {

      if ($item['starthours'] instanceof \Drupal\Core\Datetime\DrupalDateTime) {
        if ($error_item->day == $item['day'] 
          && (string) $error_item->startdate == (string) $item['starthours'] 
          && (string) $error_item->enddate == (string) $item['endhours']) {
          return $element['value'][$key];
        }
      }

    }
    return $element;
  }

  /**
   * {@inheritdoc}
   */
  public function massageFormValues(array $values, array $form, FormStateInterface $form_state) {
    return parent::massageFormValues($values['value'], $form, $form_state);
  }

}
