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
    //$options = parent::getReferencableEntities($match, $match_operator, $limit);
    //extract(get_object_vars($this));
    $options = array();
    $entity_type = $this->instance['entity_type'];
    $bundle = $this->instance['bundle'];
    $entity = $this->entity;

    //$entity_type = $this->entity_type;
    $merci_settings = merci_settings_load($entity_type, $bundle);
    $field_item_name = $merci_settings['target_field'];

    $query = $this->buildEntityFieldQuery($match, $match_operator);
    if ($limit > 0) {
      $query->range(0, $limit);
    }

    $results = $query->execute();
    $items = array();
    //$this->entity_type = $entity_type;

    foreach($results[$entity_type] as $id => $target) {
      $items[] = array('target_id' => $id);
    }

    $errors = array();

    module_load_include('inc', 'merci', 'merci.validate');
    merci_api_validate_items($entity_type, $entity, $items, &$errors);
    //merci_api_validate_restrictions($entity_type, $entity, $field, $instance, $langcode, $items, &$errors);
    $langcode = $entity ? $entity->language : LANGUAGE_NONE;
    foreach($items as $delta => $target) {
      if (isset($errors[$field_item_name][$langcode][$delta])) {
        unset($results[$entity_type][$target['target_id']]);
      }
    }
    if (!empty($results[$entity_type])) {
      $entities = entity_load($entity_type, array_keys($results[$entity_type]));
      foreach ($entities as $entity_id => $entity) {
        list(,, $bundle) = entity_extract_ids($entity_type, $entity);
        $options[$bundle][$entity_id] = check_plain($this->getLabel($entity));
      }
    }
    return $options;
  }

}
