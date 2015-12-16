<?php

/**
 * @file
 * Contains \Drupal\office_hours\Plugin\Validation\Constraint\OfficeHoursConstraintValidator.
 */

namespace Drupal\office_hours\Plugin\Validation\Constraint;

use Symfony\Component\Validator\Constraint;
use Symfony\Component\Validator\ConstraintValidator;

/**
 * Checks for conflicts when validating a entity with reservable items.
 */
class OfficeHoursConstraintValidator extends ConstraintValidator {

  /**
   * {@inheritdoc}
   */
  public function validate($value, Constraint $constraint) {
    /* @var \Drupal\Core\Field\FieldItemInterface $value */
    if (!isset($value)) {
      return;
    }


    //foreach ($value as $delta => $item) {
    $startdate = clone $value->startdate;
    $enddate = clone $value->enddate;
    //$value->startdate->setTimeZone(timezone_open(drupal_get_user_timezone()));
    //$value->enddate->setTimeZone(timezone_open(drupal_get_user_timezone()));
      if ($value->startdate > $value->enddate) {
        $this->context->buildViolation($constraint->message)
          ->atPath((string) $delta)
          ->addViolation();
        return;
      }
    //}
  }
}
