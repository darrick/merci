<?php

/**
 * @file
 * Contains \Drupal\date_combo\Plugin\Field\FieldType\OfficeHoursItemList.
 */

namespace Drupal\office_hours\Plugin\Field\FieldType;

use Drupal\Core\Datetime\DrupalDateTime;
use Drupal\Core\Entity\FieldableEntityInterface;
use Drupal\Core\Field\FieldDefinitionInterface;
use Drupal\Core\Field\FieldItemList;
use Drupal\Core\Form\FormStateInterface;

/**
 * Represents a configurable entity datetime field.
 */
class OfficeHoursItemList extends FieldItemList {

  /**
   * Defines the default value as now.
   */
  const DEFAULT_VALUE_NOW = 'now';

  /**
   * Defines the default value as relative.
   */
  const DEFAULT_VALUE_CUSTOM = 'relative';

  /**
   * {@inheritdoc}
  public function defaultValuesForm(array &$form, FormStateInterface $form_state) {
    if (empty($this->getFieldDefinition()->getDefaultValueCallback())) {
      $default_value = $this->getFieldDefinition()->getDefaultValueLiteral();

      $element = array(
        '#parents' => array('default_value_input'),
        'default_date_type' => array(
          '#type' => 'select',
          '#title' => t('Default date for start'),
          '#description' => t('Set a default value for the start date.'),
          '#default_value' => isset($default_value[0]['default_date_type']) ? $default_value[0]['default_date_type'] : '',
          '#options' => array(
            static::DEFAULT_VALUE_NOW => t('Current date'),
            static::DEFAULT_VALUE_CUSTOM => t('Relative date'),
          ),
          '#empty_value' => '',
        ),
        'default_date' => array(
          '#type' => 'textfield',
          '#title' => t('Relative default start value'),
          '#description' => t("Describe a time by reference to the current day, like '+90 days' (90 days from the day the field is created) or '+1 Saturday' (the next Saturday). See <a href=\"@url\">@strtotime</a> for more details.", array('@strtotime' => 'strtotime', '@url' => 'http://www.php.net/manual/en/function.strtotime.php')),
          '#default_value' => (isset($default_value[0]['default_date_type']) && $default_value[0]['default_date_type'] == static::DEFAULT_VALUE_CUSTOM) ? $default_value[0]['default_date'] : '',
          '#states' => array(
            'visible' => array(
              ':input[id="edit-default-value-input-default-date-type"]' => array('value' => static::DEFAULT_VALUE_CUSTOM),
            )
          )
        ),
        'default_date2' => array(
          '#type' => 'textfield',
          '#title' => t('Relative default end value'),
          '#description' => t("Relative to start date. Describe a time by reference to the current day, like '+90 days' (90 days from the day the field is created) or '+1 Saturday' (the next Saturday). See <a href=\"@url\">@strtotime</a> for more details.", array('@strtotime' => 'strtotime', '@url' => 'http://www.php.net/manual/en/function.strtotime.php')),
          '#default_value' =>  $default_value[0]['default_date2'],
          )
      );

      return $element;
    }
  }

  /**
   * {@inheritdoc}
  public function defaultValuesFormValidate(array $element, array &$form, FormStateInterface $form_state) {
  if ($form_state->getValue(['default_value_input', 'default_date_type']) == static::DEFAULT_VALUE_CUSTOM) {
      $is_strtotime = @strtotime($form_state->getValue(array('default_value_input', 'default_date')));
      if (!$is_strtotime) {
        $form_state->setErrorByName('default_value_input][default_date', t('The relative date value entered is invalid.'));
      }
    }
  }

  /**
   * {@inheritdoc}
  public function defaultValuesFormSubmit(array $element, array &$form, FormStateInterface $form_state) {
    if ($form_state->getValue(array('default_value_input', 'default_date_type'))) {
      if ($form_state->getValue(array('default_value_input', 'default_date_type')) == static::DEFAULT_VALUE_NOW) {
        $form_state->setValueForElement($element['default_date'], static::DEFAULT_VALUE_NOW);
      }
      return array($form_state->getValue('default_value_input'));
    }
    return array();
  }

  /**
   * {@inheritdoc}
  */
  public static function processDefaultValue($default_value, FieldableEntityInterface $entity, FieldDefinitionInterface $definition) {
    $default_value = parent::processDefaultValue($default_value, $entity, $definition);
    if (empty($default_value)) {
      $default_value = array(
        array(
          'day' => 0,
        )
      );
    }
    return $default_value;
  }

  public function sortByDays() {
    uasort($this->list, array($this, 'dayCompareMethod'));
    parent::reKey();
  }

  public function dayCompareMethod($a, $b) {
    if ($a->day == $b->day) {
      if ($a->enddate && $b->startdate) {
        return ($a->enddate < $b->startdate) ? -1 : 1;
      } else if ($a->enddate) {
        return -1;
      } else if ($b->startdate) {
        return 1;
      }
      return 0;
    }

    return ($a->day < $b->day) ? -1 : 1;
  }

}
