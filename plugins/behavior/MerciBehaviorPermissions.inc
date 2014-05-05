<?php
$plugin = array(
  'label' => t('Content type restrictions'),
  'description' => t('Restrictions based on content type.'),
  'handler' => array(
    'class' => 'MerciBehaviorPermissions',
  ),
);

class MerciBehaviorPermissions extends MerciBehavior {

  public function validate($form, &$form_state) {

    try {
      $entity = $this->entity;

      $target_field = 'merci_reservation_items';
      $field_info = field_info_field($target_field);
      $target_type = $field_info['settings']['target_type'];

      $ids = array();
      foreach ($entity->{$target_field}->raw() as $delta => $target_id) {
        if (!empty($target_id) and intval($target_id)) {
          $ids[] = $target_id;
        }
      }
      $targets = entity_load($target_type, $ids);
      $user_roles = $entity->author->roles->value();
      //$user_roles = array_keys($account->roles);

      foreach ($entity->{$target_field}->raw() as $delta => $target_id) {
        if (!empty($target_id) and intval($target_id)) {
          $target = entity_metadata_wrapper($target_type, $targets[$target_id]);
          $roles = $target->{MERCI_ROLE_FIELD}->raw();
          if(!count(array_intersect($roles, $user_roles))) {
            $messages[$delta] = t('User %user does not have permission to reserve %title.', array('%user' => $account->name, '%title' => $targets[$target_id]->title)); 
            form_set_error("merci_reservation_items][und][$delta][target_id", $messages[$delta]);
          }
        }
      }
    }
    catch (EntityMetadataWrapperException $exc) {
      watchdog(
        'MODULE_NANE',
        'See '  . __FUNCTION__ . '() ' .  $exc->getTraceAsString(),
        NULL, WATCHDOG_ERROR
      );
    }
  }

}
