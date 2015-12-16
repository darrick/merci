<?php

/**
 * @file
 * Contains \Drupal\office_hours\Plugin\Field\FieldType\OfficeHoursItem.
 */

namespace Drupal\office_hours\Plugin\Field\FieldType;

use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Field\FieldItemBase;
use Drupal\Core\Field\FieldStorageDefinitionInterface;
use Drupal\Core\TypedData\DataDefinition;
use Drupal\office_hours\OfficeHoursDateHelper;

/**
 * Plugin implementation of the 'office_hours' field type.
 *
 * @FieldType(
 *   id = "office_hours",
 *   label = @Translation("Office hours"),
 *   description = @Translation("This field stores weekly 'office hours' or 'opening hours' in the database."),
 *   default_widget = "office_hours_default",
 *   default_formatter = "office_hours",
 *   list_class = "\Drupal\office_hours\Plugin\Field\FieldType\OfficeHoursItemList"
 * )
 */
class OfficeHoursItem extends FieldItemBase {
  // implements EntityReferenceItem
//*   list_class = "\Drupal\datetime\Plugin\Field\FieldType\DateTimeFieldItemList"
//*                "\Drupal\Core\Field\FieldItemList
//*   list_class = "\Drupal\office_hours\Plugin\Field\FieldType\OfficeHoursFieldItemList"

  /**
   * {@inheritdoc}
   */
  public static function schema(FieldStorageDefinitionInterface $field_definition) {
    // todo D8: waar komt dit vandaan?  $maxlenght = $field_definition->getSetting('maxlength');

    return array(
      'columns' => array(
        'day' => array(
          'type' => 'int',
          'not null' => FALSE,
        ),
        'starthours' => array(
          'type' => 'varchar',
          'length' => 20,
        ),
        'endhours' => array(
          'type' => 'varchar',
          'length' => 20,
        ),
      ),
    );
  }

  /**
   * {@inheritdoc}
   */
  public static function propertyDefinitions(FieldStorageDefinitionInterface $field_definition) {
    //    $properties['value'] = DataDefinition::create('string')
    //      ->setLabel(t('Office hours'));
    $properties['day'] = DataDefinition::create('integer')
      ->setLabel(t('Day'))
      ->setRequired(TRUE)
      ->setDescription("Stores the day of the week's numeric representation (0-6)");
    $properties['starthours'] = DataDefinition::create('datetime_iso8601')
      ->setLabel(t('Start hours'))
      ->setRequired(TRUE)
      ->setDescription("Stores the start hours value");
    $properties['endhours'] = DataDefinition::create('datetime_iso8601')
      ->setLabel(t('End hours'))
      ->setRequired(TRUE)
      ->setDescription("Stores the end hours value");
    $properties['startdate'] = DataDefinition::create('any')
      ->setLabel(t('Computed start date'))
      ->setDescription(t('The computed start DateTime object.'))
      ->setComputed(TRUE)
      ->setClass('\Drupal\datetime\DateTimeComputed')
      ->setSetting('date source', 'starthours');
    $properties['enddate'] = DataDefinition::create('any')
      ->setLabel(t('Computed end date'))
      ->setDescription(t('The computed end DateTime object.'))
      ->setComputed(TRUE)
      ->setClass('\Drupal\datetime\DateTimeComputed')
      ->setSetting('date source', 'endhours');

    return $properties;
  }

  /**
   * {@inheritdoc}
   */
  public static function defaultStorageSettings() {
    $defaultStorageSettings = array(
        'cardinality_per_day' => 2,
        'valhrs' => 0,
        'limitstart' => '',
        'limitend' => '',
      ) + parent::defaultStorageSettings();

    return $defaultStorageSettings;
  }

  /**
   * {@inheritdoc}
   */
  public function storageSettingsForm(array &$form, FormStateInterface $form_state, $has_data) {
    $element = array();

    $settings = $this->getFieldDefinition()
      ->getFieldStorageDefinition()
      ->getSettings();


    $element['#element_validate'] = array(array($this, 'officeHoursValidate'));
    $description = t('The maximum number of slots, that are allowed per day.
      <br/><strong> Warning! Lowering this setting after data has been created
      could result in the loss of data! </strong><br/> Be careful when using
      more then 2 slots per day, since not all external services (like Google
      Places) support this.');
    $element['cardinality_per_day'] = array(
      '#type' => 'select',
      '#title' => t('Number of slots'),
      // @todo for 'multiple slots per day': add support for FIELD_CARDINALITY_UNLIMITED.
      // '#options' => array(FieldStorageDefinitionInterface::CARDINALITY_UNLIMITED => t('unlimited')) + drupal_map_assoc(range(1, 10)),
      '#options' => array_combine(range(1, 12), range(1, 12)),
      '#default_value' => $settings['cardinality_per_day'],
      '#description' => $description,
      // '#disabled' => $has_data,
    );

    $element['valhrs'] = array(
      '#type' => 'checkbox',
      '#title' => t('Validate hours'),
      '#required' => FALSE,
      '#default_value' => $this->getSetting('valhrs'),
      '#description' => t('Assure that endhours are later then starthours. Please note that this will work as long as the opening hours are not through midnight.'),
    );

    // Get a formatted list of hours.
    $hours = OfficeHoursDateHelper::hours('H', FALSE);
    foreach ($hours as $key => &$hour) {
      if (!empty($hour)) {
        $hrs = OfficeHoursDateHelper::format($hour . '00', 'H:i');
        $ampm = OfficeHoursDateHelper::format($hour . '00', 'g:i a');
        $hour = "$hrs ($ampm)";
      }
    }
    $element['limitstart'] = array(
      '#type' => 'select',
      '#title' => t('Limit widget hours - from'),
      '#description' => t('Restrict the hours available - select options will start from this hour.'),
      '#default_value' => $this->getSetting('limitstart'),
      '#options' => $hours,
    );
    $element['limitend'] = array(
      '#type' => 'select',
      '#title' => t('Limit widget hours - until'),
      '#description' => t('Restrict the hours available - select options will end at this hour.'),
      '#default_value' => $this->getSetting('limitend'),
      '#options' => $hours,
    );

    return $element;
  }

  public function getConstraints() {
    $constraints = parent::getConstraints();

    if ($this->getSetting('valhrs')) {

      $constraint_manager = $this->getTypedDataManager()->getValidationConstraintManager();
      $constraints[] = $constraint_manager->create('OfficeHours', []);
    }

    return $constraints;
  }

  /**
   * {@inheritdoc}
   */
  public function isEmpty() {
    $value = $this->get('starthours')->getValue();
    $value2 = $this->get('endhours')->getValue();
    return $value === NULL || $value === '' || $value2 === NULL || $value2 === '';
  }

  /**
    ** {@inheritdoc}
    **/
  public function onChange($property_name, $notify = TRUE) {
    // Enforce that the computed date is recalculated.
    if ($property_name == 'starthours') {
      $this->startdate = NULL;
    }
    if ($property_name == 'endhours') {
      $this->enddate = NULL;
    }
    parent::onChange($property_name,
      $notify);
  }

}
