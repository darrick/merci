<?php
$plugin = array(
  'label' => t('Unique'),
  'description' => t('Only unique items.'),
  'handler' => array(
    'class' => 'MerciBehaviorUnique',
  ),
);

class MerciBehaviorUnique extends MerciBehavior {

  public function merci_validate_unique_items($form, &$form_state) {

    // Do no validation if their errors from the main validation function.
    if (form_get_errors()) {
      return;
    }
    $node = (object) $form_state['values'];

    $entity_type  = 'node';

    $entity       = entity_metadata_wrapper($entity_type, $node);

    $target_field = 'merci_reservation_items';
    $field_info = field_info_field($target_field);
    $target_type = $field_info['settings']['target_type'];

    $ids = array();
    foreach ($entity->{$target_field}->raw() as $delta => $target_id) {
      if (!empty($target_id) and intval($target_id)) {
        $ids[$delta] = $target_id;
      }
    }
    $unique = array_unique($ids);
    if (count($ids) != count($unique)) {
      foreach ($ids as $delta => $target_id) {
        if (!array_key_exists($delta, $unique)) {
          $targets = entity_load($target_type, array($target_id));
          $messages[$delta] = t('%title is overbooked.  You can only reserve one of this item.', array('%title' => $targets[$target_id]->title)); 
          form_set_error("merci_reservation_items][und][$delta][target_id", $messages[$delta]);
        }
      }
    }
  }

}
