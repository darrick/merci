<?php

/**
 * @file
 * Contains \Drupal\merci_entityref_behavior\Plugin\Validation\Constraint\ReservationConflictConstraint.
 */

namespace Drupal\merci_entityref_behavior\Plugin\Validation\Constraint;

use Symfony\Component\Validator\Constraint;

/**
 * Checks that the node is assigned only a "leaf" term in the forum taxonomy.
 *
 * @Constraint(
 *   id = "ReservationConflict",
 *   label = @Translation("ReservationConflict", context = "Validation"),
 * )
 */
class ReservationConflictConstraint extends Constraint {

}
