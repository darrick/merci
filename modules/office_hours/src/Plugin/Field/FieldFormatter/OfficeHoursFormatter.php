<?php

/**
 * @file
 * Contains \Drupal\office_hours\Plugin\Field\FieldFormatter\OfficeHoursFormatter.
 */

namespace Drupal\office_hours\Plugin\Field\FieldFormatter;

use Drupal\Component\Utility\Unicode;
use Drupal\Component\Utility\Xss;
use Drupal\Core\Field\FormatterBase;
use Drupal\Core\Field\FieldItemListInterface;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Render\Element;
use Drupal\office_hours\OfficeHoursDateHelper;
use Drupal\datetime\Plugin\Field\FieldFormatter\DateTimeDefaultFormatter;

/**
 * Plugin implementation of the formatter.
 *
 * @FieldFormatter(
 *   id = "office_hours",
 *   label = @Translation("Office hours"),
 *   field_types = {
 *     "office_hours",
 *   }
 * )
 */
class OfficeHoursFormatter extends DateTimeDefaultFormatter {

  /**
   * {@inheritdoc}
   */
  public static function defaultSettings() {
    $parent_settings = parent::defaultSettings();
    $parent_settings['format_type'] = '24_hour_time_9_00';
    return array(
      'daysformat' => 'long',
      'time_type' => 'G',
      'compress' => FALSE,
      'grouped' => FALSE,
      'showclosed' => 'all',
      'closedformat' => 'Closed',
      // The html-string for closed/empty days.
      'separator' => array(
        'days' => '<br />',
        'grouped_days' => ' - ',
        'day_hours' => ': ',
        'hours_hours' => '-',
        'more_hours' => ', ',
      ),
      'current_status' => array(
        'position' => 'hide',
        'open_text' => 'Currently open!',
        'closed_text' => 'Currently closed',
      ),
      'timezone_field' => '',
      'office_hours_first_day' => '',
    ) + $parent_settings;
  }

  /**
   * {@inheritdoc}
   */
  public function settingsForm(array $form, FormStateInterface $form_state) {
    $element = parent::settingsForm($form, $form_state);;

    $settings = $this->getSettings();
    $daynames = OfficeHoursDateHelper::weekDays(FALSE);
    $daynames[''] = t("- system's Regional settings -");

    // todo D8: per view mode ophalen uit settings.
//    $display = $instance['display'][$view_mode];
//    $settings = _office_hours_field_formatter_defaults($instance['display'][$view_mode]['settings']);

    /*
      // Find timezone fields, to be used in 'Current status'-option.
      $fields = field_info_instances( (isset($form['#entity_type']) ? $form['#entity_type'] : NULL), (isset($form['#bundle']) ? $form['#bundle'] : NULL));
      $timezone_fields = array();
      foreach ($fields as $field_name => $timezone_instance) {
        if ($field_name == $field['field_name']) {
          continue;
        }
        $timezone_field = field_read_field($field_name);

        if (in_array($timezone_field['type'], array('tzfield'))) {
          $timezone_fields[$timezone_instance['field_name']] = $timezone_instance['label'] . ' (' . $timezone_instance['field_name'] . ')';
        }
      }
      if ($timezone_fields) {
        $timezone_fields = array('' => '<None>') + $timezone_fields;
      }
     */

    // @TODO: The settings could go under the several 'core' settings,
    // as above in the implemented hook_FORMID_form_alter functions.
/*    $element = array(
      '#type' => 'fieldset',
      '#title' => t('Office hours formatter settings'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#weight' => 5,
    );
*/

    $element['showclosed'] = array(
      '#type' => 'select',
      '#title' => t('Number of days to show'),
      '#options' => array(
        'all' => t('Show all days'),
        'open' => t('Show only open days'),
        'next' => t('Show next open day'),
        'none' => t('Hide all days'),
      ),
      '#default_value' => $settings['showclosed'],
      '#description' => t('The days to show in the formatter. Useful in combination with the Current Status block.'),
    );
    // First day of week, copied from system.variable.inc.
    $element['office_hours_first_day'] = array(
      '#type' => 'select',
      '#options' => $daynames,
      '#title' => t('First day of week'),
      '#default_value' => $this->getSetting('office_hours_first_day'),
    );
    $element['daysformat'] = array(
      '#type' => 'select',
      '#title' => t('Day notation'),
      '#options' => array(
        'long' => t('long'),
        'short' => t('short'),
        'number' => t('number'),
        'none' => t('none'),
      ),
      '#default_value' => $settings['daysformat'],
    );
    $element['time_type'] = array(
      '#type' => 'select',
      '#title' => t('Time type'),
      '#options' => array(
        'G' => t('24 hour time') . ' (9:00)', // D7: key = 0
        'H' => t('24 hour time') . ' (09:00)', // D7: key = 2
        'g' => t('12 hour time') . ' (9:00 am)', // D7: key = 1
        'h' => t('12 hour time') . ' (09:00 am)', // D7: key = 1
      ),
      '#default_value' => $settings['time_type'],
      '#required' => FALSE,
      '#description' => t('Format of the clock in the formatter.'),
    );
    $element['compress'] = array(
      '#title' => t('Compress all hours of a day into one set'),
      '#type' => 'checkbox',
      '#default_value' => $settings['compress'],
      '#description' => t('Even if more hours is allowed, you might want to show a compressed form. E.g.,  7:00-12:00, 13:30-19:00 becomes 7:00-19:00.'),
      '#required' => FALSE,
    );
    $element['grouped'] = array(
      '#title' => t('Group consecutive days with same hours into one set'),
      '#type' => 'checkbox',
      '#default_value' => $settings['grouped'],
      '#description' => t('E.g., Mon: 7:00-19:00; Tue: 7:00-19:00 becomes Mon-Tue: 7:00-19:00.'),
      '#required' => FALSE,
    );
    $element['closedformat'] = array(
      '#type' => 'textfield',
      '#size' => 30,
      '#title' => t('Empty days notation'),
      '#default_value' => $settings['closedformat'],
      '#required' => FALSE,
      '#description' => t('Format of empty (closed) days. You can use translatable text and HTML in this field.'),
    );

    // Taken from views_plugin_row_fields.inc.
    // Show a 'Current status' option.
    $element['separator'] = array(
      '#title' => t('Separators'),
      '#type' => 'details',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $element['separator']['days'] = array(
      '#type' => 'textfield',
      '#size' => 10,
      '#default_value' => $settings['separator']['days'],
      '#description' => t('This separator will be placed between the days. Use &#39&ltbr&gt&#39 to show each day on a new line.'),
    );
    $element['separator']['grouped_days'] = array(
      '#type' => 'textfield',
      '#size' => 10,
      '#default_value' => $settings['separator']['grouped_days'],
      '#description' => t('This separator will be placed between the labels of grouped days.'),
    );
    $element['separator']['day_hours'] = array(
      '#type' => 'textfield',
      '#size' => 10,
      '#default_value' => $settings['separator']['day_hours'],
      '#description' => t('This separator will be placed between the day and the hours.'),
    );
    $element['separator']['hours_hours'] = array(
      '#type' => 'textfield',
      '#size' => 10,
      '#default_value' => $settings['separator']['hours_hours'],
      '#description' => t('This separator will be placed between the hours of a day.'),
    );
    $element['separator']['more_hours'] = array(
      '#type' => 'textfield',
      '#size' => 10,
      '#default_value' => $settings['separator']['more_hours'],
      '#description' => t('This separator will be placed between the hours and more_hours of a day.'),
    );

    // Show a 'Current status' option.
    $element['current_status'] = array(
      '#title' => t('Current status'),
      '#type' => 'details',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $element['current_status']['position'] = array(
      '#type' => 'select',
      '#title' => t('Current status position'),
      '#options' => array(
        'hide' => t('Hidden'),
        'before' => t('Before hours'),
        'after' => t('After hours'),
      ),
      '#default_value' => $settings['current_status']['position'],
      '#description' => t('Where should the current status be located?'),
    );
    $element['current_status']['open_text'] = array(
      '#title' => t('Formatting'),
      '#type' => 'textfield',
      '#size' => 40,
      '#default_value' => $settings['current_status']['open_text'],
      '#description' => t('Format of the message displayed when currently open. You can use translatable text and HTML in this field.'),
    );
    $element['current_status']['closed_text'] = array(
      '#type' => 'textfield',
      '#size' => 40,
      '#default_value' => $settings['current_status']['closed_text'],
      '#description' => t('Format of message displayed when currently closed. You can use translatable text and HTML in this field.'),
    );

    /*
      if ($timezone_fields) {
        $element['timezone_field'] = array(
          '#type' => 'select',
          '#title' => t('Timezone') . ' ' . t('Field'),
          '#options' => $timezone_fields,
          '#default_value' => $settings['timezone_field'],
          '#description' => t('Should we use another field to set the timezone for these hours?'),
        );
      }
      else {
        $element['timezone_field'] = array(
          '#type' => 'hidden',
          '#value' => $settings['timezone_field'],
        );
      }
     */

    return $element;
  }

  /**
   * {@inheritdoc}
   */
  public function settingsSummary() {
    // @todo: Return more info, like the Date module does.
//    $summary = array();
//    $settings = $this->getSettings();
    $summary[] = t('Display Office hours in different formats.');
//    $summary = implode('<br />', $summary);
    return $summary;
  }

  /**
   * {@inheritdoc}
   */
//  public function prepareView(array $entities_items) {
//    return parent::prepareView($entities_items);
//  }

  /**
   * {@inheritdoc}
   */
  public function viewElements(FieldItemListInterface $items, $langcode) {
    $elements = array();

    // Initialize formatter settings.
    $settings = $this->getSettings();

    // @todo d8
    $display = array();

    if (!$items) {
      return $elements;
    }

    // Get field settings, to make it accessible for each element in other functions.
    $settings = $this->getSettings();

    $day_cardinality = $this->getFieldSetting('cardinality_per_day');

    $day_delta = 0;

    // Sort items by day and hours.
    $items->sortByDays();

    switch ($settings['showclosed']) {
      case 'open':
        $indexed_items = array();
        break;

      case 'none':
      case 'next':
        // @FIXME
        break;

      case 'all':
      default:
        $indexed_items = array_fill_keys(range(0, 6), array());
        break;
    }

    foreach ($items as $delta => $item) {
      if (!array_key_exists($item->day, $indexed_items)) {
        $indexed_items[$item->day] = array();
      }
      $indexed_items[$item->day][] = $item;
    }


    // Initialize daynames, using date_api as key: 0=Sun - 6=Sat.
    // Be careful: date_api uses PHP: 0=Sunday, and DateObject uses ISO: 1=Sunday.
    switch ($settings['daysformat']) {
      case 'number':
        // ISO-8601 numerical representation.
        $daynames = range(1, 7);
        break;

      case 'none':
        $daynames = array_fill(0, 7, '');
        break;

      case 'long':
        $daynames = OfficeHoursDateHelper::weekDays(TRUE);
        break;

      case 'short':
      default:
        $daynames = OfficeHoursDateHelper::weekDaysAbbr(TRUE);
        break;
    }

    $first_day = $settings['office_hours_first_day'] == '' ? \Drupal::config('system.date')->get('first_day') : $settings['office_hours_first_day'];

    $index = 0;
    foreach ($indexed_items as $day => $day_items) {
      if (count($day_items)) {
        $hours = array();
        if ($settings['compress']) {
          // Compress the open/close times.
          $first = $day_items[0];
          $last = end($day_items);
          $this->setTimeZone($first->startdate);
          $starttime = $this->formatDate($first->startdate);
          $this->setTimeZone($last->enddate);
          $endtime = $this->formatDate($last->enddate);
          $hours = array(
            array(
              'open' => $starttime,
              'close' => $endtime,
            ),
          );
        } else {
          // Show all open/close times with separator.
          foreach ($day_items as $item) {
            if ($item->startdate) {
              $startdate = $item->startdate;
              $this->setTimeZone($startdate);
              $starttime = $this->formatDate($startdate);
            }
            if ($item->enddate) {
              $enddate   = $item->enddate;
              $this->setTimeZone($enddate);
              $endtime = $this->formatDate($enddate);
            }

            $hours[] = array(
              'open' => $starttime,
              'close' => $endtime,
            );
          }
        }

        $markup_hours = array();
        foreach ($hours as $times) {
          $markup_hours[] = implode($settings['separator']['hours_hours'], $times);
        }
        $markup = implode($settings['separator']['more_hours'], $markup_hours);
      } else {
        $markup = $settings['closedformat'];

      }
      $element[$day] = array(
        '#markup' => $daynames[$day] . $settings['separator']['day_hours'] . $markup,
        '#weight' => $first_day > $day ? $day + 7 : $day,
      );
    }
    $element['#sorted'] = FALSE;
    return $element;

    $day_delta = 0;
    $element['#day_hidden'] = TRUE;
    foreach ($items as $delta => $item) {

      if ($items[$delta]->day == $day_delta) {
        $element['#day_delta'] = 0;
        $day_delta++;
      } else {
        $element['#day_delta']++;
      }

      $elements[$delta] = parent::formElement($items, $delta, $element, $form, $form_state);
    }


    // Initialize days and times, using date_api as key (0=Sun, 6-Sat)
    // Empty days are not yet present in $items, and are now added in $days.
    $days = array();
    for ($day = 0; $day < 7; $day++) {
      $days[$day] = array(
        'startday' => $day,
        'endday' => NULL,
        'times' => NULL,
        'current' => FALSE,
        'next' => FALSE,
      );
    }

    // @TODO: support timezones.
    $timezone = NULL;

    // Avoid repetitive calculations, use static.
    // See http://drupal.org/node/1969956.
    // And even better, avoid the expensive DateObject.
    $today = (int) idate('w', $_SERVER['REQUEST_TIME']); // Get daynumber sun=0 - sat=6.
    $now = date('Gi', $_SERVER['REQUEST_TIME']); // 'Gi' format.

    $open = FALSE;
    $next = NULL;

    // Loop through all lines. Detect the current line and the open/closed status.
    // Convert the daynumber to (int) to get '0' for Sundays, not 'false'.
    foreach ($items->getValue() as $key => $item) {
      // Calculate start and end times.
      $day = $item['day'];
      // @todo: Is this (expensive) conversion necessary? It only adds the preceding 0 to an integer.
      $start = OfficeHoursDateHelper::format($item['starthours'], 'Hi'); // 'Hi' format (0900).
      $end = OfficeHoursDateHelper::format($item['endhours'], 'Hi'); // 'Hi' format (0900).

      $days[$day]['times'][] = array(
        'start' => $start,
        'end' => $end,
      );

      // Are we currently open? If not, when is the next time?
      // Remember: empty days are not in $items; they are present in $days.
      if ($day < $today) {
        // Initialize to first day of (next) week, in case we're closed
        // the rest of the week.
        if ($next === NULL) {
          $next = (int) $day;
        }
      }

      if ($day - $today == -1 || ($day - $today == 6)) {
        // We were open yesterday evening, check if we are still open.
        if ($start >= $end && $end >= $now) {
          $open = TRUE;
          $days[$day]['current'] = TRUE;
          $next = (int) $day;
        }
      }
      elseif ($day == $today) {
        if ($start <= $now) {
          // We were open today, check if we are still open.
          if (($start > $end)    // We are open until after midnight.
            || ($start == $end) // We are open 24hrs per day.
            || (($start < $end) && ($end > $now))
          ) {
            // We have closed already.
            $open = TRUE;
            $days[$day]['current'] = TRUE;
            $next = (int) $day;
          }
          else {
            // We have already closed.
          }
        }
        else {
          // We will open later today.
          $next = (int) $day;
        }
      }
      elseif ($day > $today) {
        if ($next === NULL || $next < $today) {
          $next = (int) $day;
        }
      }
    }
    if ($next !== NULL) {
      $days[(int) $next]['next'] = TRUE;
    }

    // Reorder weekdays to match the first day of the week, using formatter settings;
    // $days = DateHelper::weekDaysOrdered($days);
    $first_day = ($settings['office_hours_first_day'] == '') ? OfficeHoursDateHelper::getFirstDay() : $settings['office_hours_first_day'];
    if ($first_day > 0) {
      for ($i = 1; $i <= $first_day; $i++) {
        $last = array_shift($days);
        array_push($days, $last);
      }
    }

    // Check if we're compressing times. If so, combine lines of the same day into one.
    if ($settings['compress']) {
      foreach ($days as $day => &$info) {
        if (is_array($info['times'])) {
          // Initialize first slot of the day.
          $day_times = $info['times'][0];
          // Compress other slot in first slot.
          foreach ($info['times'] as $index => $slot_times) {
            $day_times['start'] = min($day_times['start'], $slot_times['start']);
            $day_times['end'] = max($day_times['end'], $slot_times['end']);
          }
          $info['times'] = array(0 => $day_times);
        }
      }
    }

    // Check if we're grouping days.
    if ($settings['grouped']) {
      $times = array();
      for ($i = 0; $i < 7; $i++) {
        if ($i == 0) {
          $times = $days[$i]['times'];
        }
        elseif ($times != $days[$i]['times']) {
          $times = $days[$i]['times'];
        }
        else {
          // N.B. for 0=Sundays, we need to (int) the indices.
          $days[$i]['endday'] = $days[(int) $i]['startday'];
          $days[$i]['startday'] = $days[(int) $i - 1]['startday'];
          $days[$i]['current'] = $days[(int) $i]['current'] || $days[(int) $i - 1]['current'];
          $days[$i]['next'] = $days[(int) $i]['next'] || $days[(int) $i - 1]['next'];
          unset($days[(int) $i - 1]);
        }
      }
    }

    // Theme the result.
    $elements[] = array(
//      '#markup' => _theme(
//        'office_hours_field_formatter_default',
      '#markup' => _office_hours_theme_formatter_default(
        array(
          'element' => $items,
          'display' => $display,
          'days' => $days,
          'settings' => $settings,
          'daynames' => $daynames,
          'open' => $open,
          'timezone' => $timezone,
        )
      ),
    );

    return $elements;
  }

}

/**
 * Theme function for field formatter.
 * @param $vars
 * @return string
 */
function _office_hours_theme_formatter_default($vars) {
  $days = $vars['days'];
  $settings = $vars['settings'];
  $daynames = $vars['daynames'];
  $open = $vars['open'];
  $time_format = OfficeHoursDateHelper::getTimeFormat($settings['time_type']);

  // Minimum width for day labels. Adjusted when adding new labels.
  $max_label_length = 3;

  $html_hours = '';
  foreach ($days as $day => &$info) {
    // Format the label.
    $label = $daynames[$info['startday']];
    $label .= !isset($info['endday']) ? '' : $settings['separator']['grouped_days'] . $daynames[$info['endday']];
    $label .= $settings['separator']['day_hours'];
    $max_label_length = max($max_label_length, Unicode::strlen($label));

    // Format the time.
    if (!$info['times']) {
      $times = Xss::filter(t($settings['closedformat']));
    }
    else {
      $times = array();
      foreach ($info['times'] as $slot_times) {
        $time_range = array(
          '#type' => 'office_hours_time_range',
          'times' => $slot_times,
          'format' => $time_format,
          'separator' => $settings['separator']['hours_hours'],
        );
//        $times[] = \Drupal::service('renderer')->render($time_range);
        $times[] = _office_hours_theme_time_range($time_range);
        // D7- $times[] = _theme('office_hours_time_range', $time_range);
      }
      $times = implode($settings['separator']['more_hours'], $times);
    }

    $info['output_label'] = $label;
    $info['output_times'] = $times;
  }

  // Start the loop again - only now we have the correct $max_label_length.
  foreach ($days as $day => &$info) {
    // Remove unwanted lines.
    switch ($settings['showclosed']) {
      case 'all':
        break;

      case 'open':
        if (!isset($info['times'])) {
          continue 2;
        }
        break;

      case 'next':
        if (!$info['current'] && !$info['next']) {
          continue 2;
        }
        break;

      case 'none':
        continue 2;
        break;
    }

    // Generate HTML for Hours.
    $width = ($max_label_length * 0.60);
    $html_hours .= '<span class="office-hours-display">'
      . '<span class="office-hours-display-label" style="width:' . $width . 'em;">'
      . $info['output_label']
      . '</span>'
      . '<span class="office-hours-display-times office-hours-display-' . (!$info['times'] ? 'closed' : 'hours')
      . ($info['current'] ? ' office-hours-display-current' : '')
      . '">'
      . $info['output_times'] . $settings['separator']['days']
      . '</span>'
      . '</span>';
  }

  $html_hours = '<span class="office-hours-wrapper' . ($settings['grouped'] ? ' office-hours-display-grouped' : '') . '">' . $html_hours . '</span>';

  // Generate HTML for CurrentStatus.
  if ($open) {
    $html_current_status = '<div class="office-hours-current-open">' . t($settings['current_status']['open_text']) . '</div>';
  }
  else {
    $html_current_status = '<div class="office-hours-current-closed">' . t($settings['current_status']['closed_text']) . '</div>';
  }

  switch ($settings['current_status']['position']) {
    case 'before':
      $html = $html_current_status . $html_hours;
      break;

    case 'after':
      $html = $html_hours . $html_current_status;
      break;

    case 'hide':
    default: // Not shown.
      $html = $html_hours;
      break;
  }

  return $html;
}

/**
 * Theme function for formatter: time ranges.
 * @param array $vars
 * @return string
 */
function _office_hours_theme_time_range($vars = array()) {
  // Add default values to $vars if not set already.
  $vars += array(
    'times' => array(
      'start' => '',
      'end' => '',
    ),
    'format' => 'G:i',
    'separator' => ' - ',
  );

  $starttime = OfficeHoursDateHelper::format($vars['times']['start'], $vars['format']);
  $endtime = OfficeHoursDateHelper::format($vars['times']['end'], $vars['format']);
  if ($endtime == '0:00' || $endtime == '00:00') {
    $endtime = '24:00';
  }
  $result = $starttime . $vars['separator'] . $endtime;
  return $result;
}
