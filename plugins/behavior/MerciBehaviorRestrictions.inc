<?php
$plugin = array(
  'label' => t('Content type restrictions'),
  'description' => t('Restrictions based on content type.'),
  'handler' => array(
    'class' => 'MerciBehaviorRestrictions',
  ),
);

class MerciBehaviorRestrictions extends MerciBehavior {

  public function validate($form, &$form_state) {
    dpm("Hello");
    // Do no validation if their errors from the main validation function.
    if (form_get_errors()) {
      return;
    }
    $node = (object) $form_state['values'];

    $entity = $this->entity;

    $target_field = 'merci_reservation_items';
    $field_info = field_info_field($target_field);
    $target_type = $field_info['settings']['target_type'];

    $ids = array();
    foreach ($entity->{$target_field}->raw() as $delta => $target_id) {
      if (!empty($target_id) and intval($target_id)) {
        $ids[$delta] = $target_id;
      }
    }
    $targets = entity_load($target_type, $ids);

    $dates = $entity->{MERCI_DATE_FIELD}->raw();

    $start_date = new DateObject($dates['value'], 'UTC');
    $end_date = new DateObject($dates['value2'], 'UTC');

    foreach ($entity->{$target_field}->raw() as $delta => $target_id) {
      if (!empty($target_id) and intval($target_id)) {
        $target = entity_metadata_wrapper($target_type, $targets[$target_id]);

        $interval = $target->{MERCI_MAX_TIME_FIELD}->value();

        $max_date = new DateObject($dates['value'] . ' ' . $interval, 'UTC'); 

        $context = array('form' => $form, 'form_state' => $form_state, 'target' => $target, 'start_date' => $start_date);
        drupal_alter('merci_override_max_time', $max_date, $context);

        if ($max_date->format('U') < $end_date->format('U')) {
          $interval = $max_date->format('U') - $start_date->format('U');
          $message = t('%title can only be reserved for up to %date.', array('%title' => $targets[$target_id]->title, '%date' => format_interval($interval))); 
          form_set_error("merci_reservation_items][und][$delta][target_id", $message);
        }
      }
    }

  }
}
