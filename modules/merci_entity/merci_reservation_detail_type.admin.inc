<?php

/**
 * @file
 * MerciReservationDetail type editing UI.
 */

/**
 * UI controller.
 */
class MerciReservationDetailTypeUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
		$items[$this->path]['description'] = 'Manage merci_reservation_detail entity types, including adding
		and removing fields and the display of fields.';
    return $items;
  }
}

/**
 * Generates the merci_reservation_detail type editing form.
 */
function merci_reservation_detail_type_form($form, &$form_state, $merci_reservation_detail_type, $op = 'edit') {

  if ($op == 'clone') {
    $merci_reservation_detail_type->label .= ' (cloned)';
    $merci_reservation_detail_type->type = '';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $merci_reservation_detail_type->label,
    '#description' => t('The human-readable name of this merci_reservation_detail type.'),
    '#required' => TRUE,
    '#size' => 30,
  );
  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($merci_reservation_detail_type->type) ? $merci_reservation_detail_type->type : '',
    '#maxlength' => 32,
//    '#disabled' => $merci_reservation_detail_type->isLocked() && $op != 'clone',
    '#machine_name' => array(
      'exists' => 'merci_reservation_detail_get_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this merci_reservation_detail type. It must only contain lowercase letters, numbers, and underscores.'),
  );

  $form['data']['#tree'] = TRUE;
  $form['data']['sample_data'] = array(
    '#type' => 'checkbox',
    '#title' => t('An interesting merci_reservation_detail switch'),
    '#default_value' => !empty($merci_reservation_detail_type->data['sample_data']),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save merci_reservation_detail type'),
    '#weight' => 40,
  );

  //Locking not supported yet
  /*if (!$merci_reservation_detail_type->isLocked() && $op != 'add') {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete merci_reservation_detail type'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('merci_reservation_detail_type_form_submit_delete')
    );
  }*/
  return $form;
}

/**
 * Form API submit callback for the type form.
 */
function merci_reservation_detail_type_form_submit(&$form, &$form_state) {
  $merci_reservation_detail_type = entity_ui_form_submit_build_entity($form, $form_state);
  dpm($merci_reservation_detail_type);
  $merci_reservation_detail_type->save();
  $form_state['redirect'] = 'admin/structure/merci_reservation_detail_types';
}

/**
 * Form API submit callback for the delete button.
 */
function merci_reservation_detail_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/merci_reservation_detail_types/manage/' . $form_state['merci_reservation_detail_type']->type . '/delete';
}
