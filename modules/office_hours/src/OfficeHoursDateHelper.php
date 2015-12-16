<?php
/**
 * @file
 * Contains \Drupal\office_hours\OfficeHoursDateHelper.
 *
 * Lots of helpful functions for use in massaging dates.
 * For formatting options, see http://www.php.net/manual/en/function.date.php
 */
namespace Drupal\office_hours;

use Drupal\Component\Datetime\DateTimePlus;
use Drupal\Core\Datetime\DateHelper;

/**
 * Defines time conversions from numeric to other format.
 */
class OfficeHoursDateHelper extends DateHelper {

  /**
   * Gets the day number of first day of the week.
   *
   * @return int
   */
  public static function getFirstDay() {
    return \Drupal::config('system.date')->get('first_day');
  }

  /**
   * Helper function to get the proper format_date() format from the settings.
   *
   * For formatting options, see http://www.php.net/manual/en/function.date.php
   *
   * @param string $time_type
   *
   * @return string
   */
  public static function getTimeFormat($time_type) {
    switch ($time_type) {
      case 'G':
        // 24hr without leading zero.
        $timeformat = 'G:i';
        break;

      case 'H':
        // 24hr with leading zero.
        $timeformat = 'H:i';
        break;

      case 'g':
        // 12hr ampm without leading zero.
        $timeformat = 'g:i a';
        break;

      case 'h':
        // 12hr ampm with leading zero.
        $timeformat = 'h:i a';
        break;
    }
    return $timeformat;
  }

  /**
   * Helper function to convert a time to a given format.
   *
   * For formatting options, see http://www.php.net/manual/en/function.date.php
   *
   * @param $time
   *   Time, in 24hr format '0800', '800', '08:00' or '8:00'
   * @param $timeformat
   *   The requested time format.
   *
   * @return string
   *   The formatted time.
   */
  public static function format($time, $timeformat) {
    // Convert '800' or '0800' to '08:00'
    if (!strstr($time, ':')) {
      $time = substr('0000' . $time, -4);
      $hour = substr($time, 0, -2);
      $min = substr($time, -2);
      $time = $hour . ':' . $min;
    }
    $date = new DateTimePlus($time);
    return $date->format($timeformat);
  }

  /**
   * Gets the (limited) hours of a day.
   *
   * Mimics DateHelper::hours() function, but that function does not support limiting
   * the hours. The limits are set in the Widget settings form, and used in the
   * Widget form.
   *
   * {@inheritdoc}
   */
  public static function hours($format = 'H', $required = FALSE, $start = 0, $end = 23) {
    $hours = array();

    // Get the valid hours. DateHelper API doesn't provide a straight method for this.
    $start = ($start == '') ? 0 : max(0, $start);
    $end = ($start == '') ? 23 : min(23, $end);

    // Begin modified copy from date_hours().
    if ($format == 'h' || $format == 'g') {
      // 12-hour format.
      $min = 1;
      $max = 24;
      for ($i = $min; $i <= $max; $i++) {
        if ((($i >= $start) && ($i <= $end)) || ($end - $start >= 11)) {
          $hour = ($i <= 12) ? $i : $i - 12;
          $hours[$hour] = $hour < 10 && ($format == 'H' || $format == 'h') ? "0$hour" : $hour;
        }

      }
      $hours = array_unique($hours);
    }
    else {
      $min = $start;
      $max = $end;
      for ($i = $min; $i <= $max; $i++) {
        $hour = $i;
        $hours[$hour] = $hour < 10 && ($format == 'H' || $format == 'h') ? "0$hour" : $hour;
      }
    }

    $none = array('' => '');
    $hours = !$required ? $none + $hours : $hours;
    // End modified copy from date_hours().

    return $hours;
  }

}
