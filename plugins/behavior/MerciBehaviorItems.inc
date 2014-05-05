<?php
$plugin = array(
  'label' => t('Item restrictions'),
  'description' => t('Limits reservations if item is available.'),
  'handler' => array(
    'class' => 'MerciBehaviorItems',
  ),
);

class MerciBehaviorItems extends MerciBehavior {

  function validate($form, &$form_state) {

    $entity = $this->entity;
    $entity_type = $entity->type();

    $date_field   = 'field_merci_date';
    $target_field = 'merci_reservation_items';
    $status_field = MERCI_STATUS_FIELD;

    $field_info = field_info_field($target_field);
    $target_type = $field_info['settings']['target_type'];

    $offset = variable_get('merci_workflow_state_offset', 0);
    if ($entity->{MERCI_STATUS_FIELD}->value() > MERCI_STATUS_CHECKED_OUT + $offset) {
      return;
    }

    //TODO do not validate if status is greater then MERCI_STATUS_CHECKED_OUT.
    dpm($entity->value());
    dpm($entity->value(array('identifier' => TRUE)));
    $controller = merci_get_controller($entity_type, $entity, $date_field, $target_field);
    $ids =  $controller->conflictingTargets();

    $conflicts = entity_load($target_type, $ids);

    foreach ($entity->{$target_field}->raw() as $delta => $target_id) {
      if (in_array($target_id, $ids)) {
        $messages[$delta] = t('%title is not available at this time.', array('%title' => $conflicts[$target_id]->title)); 
        form_set_error("merci_reservation_items][und][$delta][target_id", $messages[$delta]);
      }
    }
  }

}
