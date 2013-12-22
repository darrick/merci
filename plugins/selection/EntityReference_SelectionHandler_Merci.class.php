<?php

/**
 * A generic Entity handler.
 *
 * The generic base implementation has a variety of overrides to workaround
 * core's largely deficient entity handling.
 */
class EntityReference_SelectionHandler_Merci extends EntityReference_SelectionHandler_Generic {

  public static function getInstance($field, $instance = NULL, $entity_type = NULL, $entity = NULL) {
    return new EntityReference_SelectionHandler_Merci($field, $instance, $entity_type, $entity);
  }
  /**
   * Implements EntityReferenceHandler::getReferencableEntities().
   */
  public function getReferencableEntities($match = NULL, $match_operator = 'CONTAINS', $limit = 0) {

    $instance = $this->instance;

    $target_type = $this->field['settings']['target_type'];

    $query = $this->buildEntityFieldQuery($match, $match_operator);
    if ($limit > 0) {
      $query->range(0, $limit);
    }

    $result = $query->execute();

    $targets = array();
    if (!empty($result[$target_type])) {
      foreach (array_keys($result[$target_type]) as $target_id) {
        $targets[$target_type][$target_id] = array ('target_id' => $target_id); 
      }
    }

    $errors = array();

    $context = $instance['settings']['behaviors']['merci'];

    $context += array(
      'target_field' => $this->field['field_name'],
      'field' => $this->field,
      'instance' => $instance,
      'langcode' => $this->entity->language,
    );

    merci_items_validate($targets, $this->entity_type, $this->entity, $context, &$errors);

    $options = array();
    $items = array();
    foreach (array_keys($result[$target_type]) as $target_id) {
        //$errors[$this->context['field']['field_name']][$this->context['langcode']][$delta][] = array(
      if ($errors and array_key_exists($target_id, $errors[$context['field']['field_name']][$context['langcode']])) {
        continue;
      }
      $items[] = $target_id;
    }
    $items = entity_load($target_type, $items);


    foreach ($items as $target_id => $target) {
      list(,, $bundle) = entity_extract_ids($target_type, $target);
      $options[$bundle][$target_id] = check_plain($this->getLabel($target));
    }
    return $options;
  }
}
