<?php

/**
 * @file
 * Contains \Drupal\merci_max_length_constraint\Plugin\Validation\Constraint\MerciMaxLengthConstraint.
 */

namespace Drupal\merci_max_length_constraint\Plugin\Validation\Constraint;

use Symfony\Component\Validator\Constraint;

/**
 * Checks that the node is assigned only a "leaf" term in the forum taxonomy.
 *
 * @Constraint(
 *   id = "MerciMaxLength",
 *   label = @Translation("MerciMaxLength", context = "Validation"),
 * )
 */
class MerciMaxLengthConstraint extends Constraint {
  public $date_field;

  public $interval_field;

  /**
    ** {@inheritdoc}
    */
  public function getRequiredOptions() {
    return array('date_field', 'interval_field');
  }
}
