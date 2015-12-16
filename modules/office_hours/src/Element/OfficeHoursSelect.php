<?php

/**
 * @file
 * Contains \Drupal\office_hours\Element\OfficeHoursSelect.
 */

namespace Drupal\office_hours\Element;

use Drupal\Core\Datetime\DrupalDateTime;
use Drupal\Core\Datetime\Element\Datelist;
use Drupal\Core\Datetime\Element\Datetime;
use Drupal\Core\Datetime\Entity\DateFormat;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Render\Element\FormElement;
use Drupal\Component\Datetime\DateTimePlus;
use Drupal\Component\Utility\NestedArray;
use Drupal\office_hours\OfficeHoursDateHelper;

/**
 * Provides a one-line text field form element.
 *
 * @FormElement("office_hours_select")
 */
class OfficeHoursSelect extends FormElement { //Datetime {

  /**
   * {@inheritdoc}
   */
  public function getInfo() {
    $class = get_class($this);

    $info = array(
      '#input' => TRUE,
      '#tree' => TRUE,
      '#process' => array(
        array($class, 'processOfficeHours'),
        array($class, 'processGroup'),
      ),
      '#element_validate' => array(
        array($class, 'validateOfficeHours'),
      ),
      '#pre_render' => array(
        array($class, 'preRenderGroup'),
      ),
//      '#attached' => array(
//        'library' => array(
//          'office_hours/office_hours',
//        ),
//      ),
      '#date_element_type' => 'datelist',
      '#after_build' => array(
        array($class, 'afterBuildOfficeHoursSelect'),
      ),
    );

    /*
    $parent_info = parent::getInfo();

    // #process, #validate bottom-up.
    $info['#process'] = array_merge($parent_info['#process'], $info['#process']);
    $info['#element_validate'] = array_merge($parent_info['#element_validate'], $info['#element_validate']);
    */
    return $info;// + $parent_info;
  }

  /**
   * Callback for office_hours_select element.
   *
   * @param array $element
   * @param mixed $input
   * @param FormStateInterface $form_state
   * @return array|mixed|null
   *
   * Takes the #default_value and dissects it in hours, minutes and ampm indicator.
   * Mimics the date_parse() function.
   *   g = 12-hour format of an hour without leading zeros 1 through 12
   *   G = 24-hour format of an hour without leading zeros 0 through 23
   *   h = 12-hour format of an hour with leading zeros    01 through 12
   *   H = 24-hour format of an hour with leading zeros    00 through 23
   */
  public static function valueCallback(&$element, $input, FormStateInterface $form_state) {

    if ($input !== FALSE) {

      if ($element['#date_element_type'] == 'datetime') {
        // Let child element set time via their valueCallback.
        unset($input['time']);
      }
      elseif($element['#date_element_type'] == 'datelist') {
        $time_element = $input['time'];

        if(empty($time_element['hour']) && empty($time_element['minute'])) {
          // Set our hidden values to empty. Let Datelist class return the empty
          // value.
          $input['time'] = array(
            'month' => '',
            'day' => '',
            'year' => '',
          );
        }
        else {
          // Let child element set time via their valueCallback.
          unset($input['time']);
        }
      }
    }
    else {
      $date = $element['#default_value'];

      if ($date instanceof DrupalDateTime && !$date->hasErrors()) {
        $input = array(
          'object' => $date,
        );
      } else {
        $input = array(
          'object' => NULL,
        );
      }
    }

    return $input;
  }

  /**
   * Process the office_hours_select element.
   *
   * @param $element
   * @param FormStateInterface $form_state
   * @param $form
   * @return
   */
  public static function processOfficeHours($element, FormStateInterface &$form_state, $form) {

    // The value callback has populated the #value array.
    $default_value = !empty($element['#value']['object']) ? $element['#value']['object'] : NULL;

    $settings = $element['#field_settings'];
    $time_type = $settings['time_type'];
    //$timezone = new \DateTimezone(DATETIME_STORAGE_TIMEZONE); // @todo:get from Widget.
    $timezone = !empty($element['#date_timezone']) ? $element['#date_timezone'] : NULL;

    // Render as a datelist element.
    if ($element['#date_element_type'] == 'datelist') {
      $element['time'] = array(
        '#type' => 'datelist',
        '#default_value' => $default_value,
        '#date_part_order' => (in_array($time_type, ['g','h']))
          ? array('hour', 'minute', 'ampm',)
          : array('hour', 'minute',),
        '#error_no_message' => TRUE,
        '#date_timezone' => $timezone,
        '#date_increment' => $settings['increment'],
      );
      $element['time']['#date_part_order'] = array('hour');
      if ($settings['increment'] != '60') {
        $element['time']['#date_part_order'][] = 'minute';
      }
      if (in_array($time_type, ['g','h'])) {
        $element['time']['#date_part_order'][] = 'ampm';
      }

      // Datelist element expects a date value, not just time.
      // So make something up.
      $element['time']['year'] = array(
        '#type' => 'hidden',
        '#value' => 2008,
      );
      $element['time']['month'] = array(
        '#type' => 'hidden',
        '#value' => 1,
      );
      $element['time']['day'] = array(
        '#type' => 'hidden',
        '#value' => 1,
      );
    }
    elseif ($element['#date_element_type'] == 'datetime') {
      $date_type = 'none';
      $time_type = 'time';
      $time_format = DateFormat::load('html_time')->getPattern();
      $element['time'] = array(
        '#type' => 'datetime',
        //'#title' => t('Start'),
        //'#date_date_format'=>  $date_format,
        '#date_date_element' => $date_type,
        '#date_date_callbacks' => array(),
        '#date_time_format' => $time_format,
        '#date_time_element' => $time_type,
        '#date_time_callbacks' => array(),
        '#date_timezone' => $timezone,
        '#date_increment' => 60 * $settings['increment'],
        '#default_value' => $default_value,
      );
      $timeformat = 'H:i:s';
      if (!empty($settings['limitstart'])) {
        $date = DrupalDateTime::createFromFormat('H', $settings['limitstart'], $timezone);
        $element['time']['#attributes']['min'] = $date->format($timeformat);
      }
      if (!empty($settings['limitend'])) {
        $date = DrupalDateTime::createFromFormat('H', $settings['limitend'], $timezone);
        $element['time']['#attributes']['max'] = $date->format($timeformat);
      }
    }

    return $element;
  }

  /**
   * Process an individual element.
   *
   * Build the form element. When creating a form using FAPI #process,
   * note that $element['#value'] is already set.
   */
  public static function afterBuildOfficeHoursSelect(&$element, FormStateInterface $form_state) {
    if ($element['#date_element_type'] == 'datelist') {
      $settings = $element['#field_settings'];
      $time_type = $settings['time_type'];
      // @TODO will need javascript to handle ampm limits.
      $limitstart = !empty($settings['limitstart']) ? $settings['limitstart'] : 0;
      $limitend = !empty($settings['limitend']) ? $settings['limitend'] : 23;
      if (in_array($time_type, ['G','H'])) {
        $timeformat = 'H:i:s';
        foreach ($element['time']['hour']['#options'] as $delta => $option) {
          if ($option !== '' && ($option < $limitstart || $option > $limitend)) {
            unset($element['time']['hour']['#options'][$delta]);
          }
        }
      }
    }
    return $element;
  }

  /**
   * Validate the hours selector element.
   *
   * @param $element
   * @param $form_state
   */
  public static function validateOfficeHours(&$element, FormStateInterface $form_state, &$complete_form) {

    $input_exists = FALSE;
    $input = NestedArray::getValue($form_state->getValues(), $element['#parents'], $input_exists);
    if ($input_exists) {
      if ($input['time'] instanceof DrupalDateTime) {
        //$form_state->setValueForElement($element, (int) $input['time']->format('Hi'));

        $form_state->setValueForElement($element, $input['time']);
      }
      else {
        $form_state->setValueForElement($element, '');
      }
    }
    else {
      $form_state->setValueForElement($element, '');
    }
  }

}
