<?php

/**
 * @file
 * Contains \Drupal\office_hours\Element\OfficeHoursTable.
 */

namespace Drupal\office_hours\Element;

use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Render\Element\Table;

/**
 * Provides a render element for a table.
 *
 * @FormElement("office_hours_table")
 */
class OfficeHoursTable extends Table {

  /**
   * {@inheritdoc}
   */
  public function getInfo() {
    $class = get_class($this);
    $info = array(
      '#element_validate' => array(
        array($class, 'validateOfficeHoursTable'),
      ),
      '#attached' => array(
        'library' => array(
          'office_hours/officehours',
        ),
      ),
    );
    return parent::getInfo() + $info;
  }

  public static function valueCallback(&$element, $input, FormStateInterface $form_state) {
    if ($input) {
//      $input = parent::valueCallback($element, $input, $form_state);
      // Let child elements set starthours and endhours via their valueCallback.
      foreach($input as $key => $value) {
        unset($input[$key]['starthours']);
        unset($input[$key]['endhours']);
      }
    }
    else {
      $input = parent::valueCallback($element, $input, $form_state);
    }

    return $input;
  }

}
