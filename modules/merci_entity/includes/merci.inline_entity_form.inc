<?php

/**
 * @file
 * Defines the inline entity form controller for Nodes.
 */

class MerciReservationDetailInlineEntityFormController extends EntityInlineEntityFormController {

  /**
  * Overrides EntityInlineEntityFormController::labels().
   */
  public function labels() {
    $labels = array(
      'singular' => t('node'),
      'plural' => t('nodes'),
    );
    return $labels;
  }

  /**
   * Overrides EntityInlineEntityFormController::tableFields().
   */
  public function tableFields($bundles) {
    $fields = parent::tableFields($bundles);
    dpm($bundles);
    /*
    $fields['status'] = array(
      'type' => 'property',
      'label' => t('Status'),
      'weight' => 100,
    );
    */
    return $fields;
  }


  /**
   * Overrides EntityInlineEntityFormController::entityForm().
   */
  public function entityForm($entity_form, &$form_state) {
    $field_name = 'field_merci_reservation_items';
    $bundle = $form_state['values'][$field_name]['und']['actions']['bundle'];
    $merci_reservation_detail = merci_reservation_detail_create(array('type' => $bundle));
    dpm($merci_reservation_detail);
    dpm($entity_form);
    dpm($form_state);
    dpm($form_state['node']);
    $entity_type = 'merci_reservation_detail';
    $view_mode = 'default';
    dpm(field_info_instances($entity_type, $bundle));

    // Add the field related form elements.
    $form_state['merci_reservation_detail'] = $merci_reservation_detail;
    field_attach_form('merci_reservation_detail', $merci_reservation_detail, $entity_form, $form_state);
    return $entity_form;

  }

  /**
   * Overrides EntityInlineEntityFormController::entityFormSubmit().
   */
  public function entityFormSubmit(&$entity_form, &$form_state) {
    parent::entityFormSubmit($entity_form, $form_state);

    $merci_reservation_detail = entity_ui_controller('merci_reservation_detail')->entityFormSubmitBuildEntity($form, $form_state);
    // Save the merci_reservation_detail and go back to the list of merci_reservation_details

    // Add in created and changed times.
    if ($merci_reservation_detail->is_new = isset($merci_reservation_detail->is_new) ? $merci_reservation_detail->is_new : 0){
      $merci_reservation_detail->created = time();
    }

    $merci_reservation_detail->changed = time();

    $merci_reservation_detail->save();
  }
}
