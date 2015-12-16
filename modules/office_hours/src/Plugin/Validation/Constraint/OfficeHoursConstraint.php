<?php

/**
 * @file
 * Contains \Drupal\office_hours\Plugin\Validation\Constraint\OfficeHoursConstraint.
 */

namespace Drupal\office_hours\Plugin\Validation\Constraint;

use Symfony\Component\Validator\Constraint;

/**
 * Checks that the node is assigned only a "leaf" term in the forum taxonomy.
 *
 * @Constraint(
 *   id = "OfficeHours",
 *   label = @Translation("OfficeHours", context = "Validation"),
 * )
 */
class OfficeHoursConstraint extends Constraint {
  public $message = 'End time cannot be less than start time.';

}
