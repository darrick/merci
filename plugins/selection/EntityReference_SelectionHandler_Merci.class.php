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
    $options = array();
    $items = array();
    $entity = $this->entity;
    $entity_type = $this->entity_type;
    list($entity_id,,) = entity_extract_ids($entity_type, $entity);

    $preset = $this->instance['settings']['behaviors']['merci']['preset'];
    $target_type = $this->instance['entity_type'];
    $settings = merci_preset_load_settings($preset);

    $date_field = $settings['date_field'];
    $dates = property_exists($entity, $date_field) ? $entity->{$date_field}[LANGUAGE_NONE][0] : NULL;


    $query = $this->buildEntityFieldQuery($match, $match_operator);
    if ($limit > 0) {
      $query->range(0, $limit);
    }

    $result = $query->execute();

    if (!empty($result[$target_type])) {
      $items = entity_load($target_type, array_keys($result[$target_type]));
    }

    foreach ($items as $target_id => $target) {
      $handler = Merci_ReservableHandler_Generic::getInstance($target_type, $target, $preset); 

      // Check availability
      if (!$handler->available($dates, $entity_id)) {
        continue;
      }

      list(,, $bundle) = entity_extract_ids($target_type, $target);
      $options[$bundle][$target_id] = check_plain($this->getLabel($target));
    }
    return $options;
  }
}
