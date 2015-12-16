<?php

/**
 * @file
 * Contains \Drupal\office_hours\Plugin\Field\FieldWidget\OfficeHoursSelectWidget.
 */

namespace Drupal\office_hours\Plugin\Field\FieldWidget;

use Drupal\datetime\Plugin\Field\FieldWidget\DateTimeWidgetBase;
use Drupal\Core\Field\FieldItemListInterface;
use Drupal\Core\Form\FormStateInterface;

/**
 * Plugin implementation of the 'office_hours_week' widget.
 *
 * @FieldWidget(
 *   id = "office_hours_select",
 *   label = @Translation("Office hours (select)"),
 *   field_types = {
 *     "office_hours",
 *     "datetime",
 *   }
 * )
 */
class OfficeHoursSelectWidget extends DateTimeWidgetBase {

  /**
   * {@inheritdoc}
   */
  public function formElement(FieldItemListInterface $items, $delta, array $element, array &$form, FormStateInterface $form_state) {
    $element = parent::formElement($items, $delta, $element, $form, $form_state);
    $element['value']['#type'] = 'office_hours_select';
    return $element;
  }

}
