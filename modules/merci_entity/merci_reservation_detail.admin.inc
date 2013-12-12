<?php

/**
 * @file
 * MerciReservationDetail editing UI.
 *
 * We make very little use of the EntityAPI interface for this - preferring instead to use
 * views. That offers more flexibility to change a UI that will, more often than not,
 * be end-user facing.
 */

/**
 * UI controller.
 */
class MerciReservationDetailUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults. Main reason for doing this is that
   * parent class hook_menu() is optimized for entity type administration.
   */
  public function hook_menu() {
    
    $items = array();
    $id_count = count(explode('/', $this->path));
    $wildcard = isset($this->entityInfo['admin ui']['menu wildcard']) ? $this->entityInfo['admin ui']['menu wildcard'] : '%' . $this->entityType;

    $items[$this->path] = array(
      'title' => 'MerciReservationDetails',
      'description' => 'Add edit and update merci_reservation_details.',
      'page callback' => 'system_admin_menu_block_page',
      'access arguments' => array('access administration pages'),
      'file path' => drupal_get_path('module', 'system'),
      'file' => 'system.admin.inc',
    );
    
    // Change the overview menu type for the list of merci_reservation_details.
    $items[$this->path]['type'] = MENU_LOCAL_TASK;
    
    // Change the add page menu to multiple types of entities
    $items[$this->path . '/add'] = array(
      'title' => 'Add a merci_reservation_detail',
      'description' => 'Add a new merci_reservation_detail',
      'page callback'  => 'merci_reservation_detail_add_page',
      'access callback'  => 'merci_reservation_detail_access',
      'access arguments' => array('edit'),
      'type' => MENU_NORMAL_ITEM,
      'weight' => 20,
      'file' => 'merci_reservation_detail.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])

    );
    
    // Add menu items to add each different type of entity.
    foreach (merci_reservation_detail_get_types() as $type) {
      $items[$this->path . '/add/' . $type->type] = array(
        'title' => 'Add ' . $type->label,
        'page callback' => 'merci_reservation_detail_form_wrapper',
        'page arguments' => array(merci_reservation_detail_create(array('type' => $type->type))),
        'access callback' => 'merci_reservation_detail_access',
        'access arguments' => array('edit', 'edit ' . $type->type),
        'file' => 'merci_reservation_detail.admin.inc',
        'file path' => drupal_get_path('module', $this->entityInfo['module'])
      );
    }

    // Loading and editing merci_reservation_detail entities
    $items[$this->path . '/merci_reservation_detail/' . $wildcard] = array(
      'page callback' => 'merci_reservation_detail_form_wrapper',
      'page arguments' => array($id_count + 1),
      'access callback' => 'merci_reservation_detail_access',
      'access arguments' => array('edit', $id_count + 1),
      'weight' => 0,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
      'file' => 'merci_reservation_detail.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])
    );
    $items[$this->path . '/merci_reservation_detail/' . $wildcard . '/edit'] = array(
      'title' => 'Edit',
      'type' => MENU_DEFAULT_LOCAL_TASK,
      'weight' => -10,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    );
    
    $items[$this->path . '/merci_reservation_detail/' . $wildcard . '/delete'] = array(
      'title' => 'Delete',
      'page callback' => 'merci_reservation_detail_delete_form_wrapper',
      'page arguments' => array($id_count + 1),
      'access callback' => 'merci_reservation_detail_access',
      'access arguments' => array('edit', $id_count + 1),
      'type' => MENU_LOCAL_TASK,
      'context' => MENU_CONTEXT_INLINE,
      'weight' => 10,
      'file' => 'merci_reservation_detail.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])
    );
    
    // Menu item for viewing merci_reservation_details
    $items['merci_reservation_detail/' . $wildcard] = array(
      //'title' => 'Title',
      'title callback' => 'merci_reservation_detail_page_title',
      'title arguments' => array(1),
      'page callback' => 'merci_reservation_detail_page_view',
      'page arguments' => array(1),
      'access callback' => 'merci_reservation_detail_access',
      'access arguments' => array('view', 1),
      'type' => MENU_CALLBACK,
    );
    return $items;
  }
  
  
  /**
   * Create the markup for the add MerciReservationDetail Entities page within the class
   * so it can easily be extended/overriden.
   */ 
  public function addPage() {
    $item = menu_get_item();
    $content = system_admin_menu_block($item);

    if (count($content) == 1) {
      $item = array_shift($content);
      drupal_goto($item['href']);
    }    
        
    return theme('merci_reservation_detail_add_list', array('content' => $content));
  }
  
}


/**
 * Form callback wrapper: create or edit a merci_reservation_detail.
 *
 * @param $merci_reservation_detail
 *   The merci_reservation_detail object being edited by this form.
 *
 * @see merci_reservation_detail_edit_form()
 */
function merci_reservation_detail_form_wrapper($merci_reservation_detail) {
  // Add the breadcrumb for the form's location.
  merci_reservation_detail_set_breadcrumb();
  return drupal_get_form('merci_reservation_detail_edit_form', $merci_reservation_detail);
}


/**
 * Form callback wrapper: delete a merci_reservation_detail.
 *
 * @param $merci_reservation_detail
 *   The merci_reservation_detail object being edited by this form.
 *
 * @see merci_reservation_detail_edit_form()
 */
function merci_reservation_detail_delete_form_wrapper($merci_reservation_detail) {
  // Add the breadcrumb for the form's location.
  //merci_reservation_detail_set_breadcrumb();
  return drupal_get_form('merci_reservation_detail_delete_form', $merci_reservation_detail);
}


/**
 * Form callback: create or edit a merci_reservation_detail.
 *
 * @param $merci_reservation_detail
 *   The merci_reservation_detail object to edit or for a create form an empty merci_reservation_detail object
 *     with only a merci_reservation_detail type defined.
 */
function merci_reservation_detail_edit_form($form, &$form_state, $merci_reservation_detail) {
  // Add the default field elements.
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('MerciReservationDetail Name'),
    '#default_value' => isset($merci_reservation_detail->name) ? $merci_reservation_detail->name : '',
    '#maxlength' => 255,
    '#required' => TRUE,
    '#weight' => -5,
  );
  
  $form['data']['#tree'] = TRUE;
  $form['data']['sample_data'] = array(
    '#type' => 'checkbox',
    '#title' => t('An interesting merci_reservation_detail switch'),
    '#default_value' => isset($merci_reservation_detail->data['sample_data']) ? $merci_reservation_detail->data['sample_data'] : 1,
  );

  // Add the field related form elements.
  $form_state['merci_reservation_detail'] = $merci_reservation_detail;
  field_attach_form('merci_reservation_detail', $merci_reservation_detail, $form, $form_state);

  $form['actions'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('form-actions')),
    '#weight' => 400,
  );

  // We add the form's #submit array to this button along with the actual submit
  // handler to preserve any submit handlers added by a form callback_wrapper.
  $submit = array();

  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save merci_reservation_detail'),
    '#submit' => $submit + array('merci_reservation_detail_edit_form_submit'),
  );
  
  if (!empty($merci_reservation_detail->name)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete merci_reservation_detail'),
      '#suffix' => l(t('Cancel'), 'admin/content/merci_reservation_details'),
      '#submit' => $submit + array('merci_reservation_detail_form_submit_delete'),
      '#weight' => 45,
    );
  }

  // We append the validate handler to #validate in case a form callback_wrapper
  // is used to add validate handlers earlier.
  $form['#validate'][] = 'merci_reservation_detail_edit_form_validate';
  return $form;
}


/**
 * Form API validate callback for the merci_reservation_detail form
 */
function merci_reservation_detail_edit_form_validate(&$form, &$form_state) {
  $merci_reservation_detail = $form_state['merci_reservation_detail'];
  
  // Notify field widgets to validate their data.
  field_attach_form_validate('merci_reservation_detail', $merci_reservation_detail, $form, $form_state);
}


/**
 * Form API submit callback for the merci_reservation_detail form.
 * 
 * @todo remove hard-coded link
 */
function merci_reservation_detail_edit_form_submit(&$form, &$form_state) {
  
  $merci_reservation_detail = entity_ui_controller('merci_reservation_detail')->entityFormSubmitBuildEntity($form, $form_state);
  // Save the merci_reservation_detail and go back to the list of merci_reservation_details
  
  // Add in created and changed times.
  if ($merci_reservation_detail->is_new = isset($merci_reservation_detail->is_new) ? $merci_reservation_detail->is_new : 0){
    $merci_reservation_detail->created = time();
  }

  $merci_reservation_detail->changed = time();
  
  $merci_reservation_detail->save();
  $form_state['redirect'] = 'admin/content/merci_reservation_details';
}

/**
 * Form API submit callback for the delete button.
 * 
 * @todo Remove hard-coded path
 */
function merci_reservation_detail_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/content/merci_reservation_details/merci_reservation_detail/' . $form_state['merci_reservation_detail']->merci_reservation_detail_id . '/delete';
}


/**
 * Form callback: confirmation form for deleting a merci_reservation_detail.
 *
 * @param $merci_reservation_detail
 *   The merci_reservation_detail to delete
 *
 * @see confirm_form()
 */
function merci_reservation_detail_delete_form($form, &$form_state, $merci_reservation_detail) {
  $form_state['merci_reservation_detail'] = $merci_reservation_detail;

  $form['#submit'][] = 'merci_reservation_detail_delete_form_submit';

  $form = confirm_form($form,
    t('Are you sure you want to delete merci_reservation_detail %name?', array('%name' => $merci_reservation_detail->name)),
    'admin/content/merci_reservation_details/merci_reservation_detail',
    '<p>' . t('This action cannot be undone.') . '</p>',
    t('Delete'),
    t('Cancel'),
    'confirm'
  );
  
  return $form;
}

/**
 * Submit callback for merci_reservation_detail_delete_form
 */
function merci_reservation_detail_delete_form_submit($form, &$form_state) {
  $merci_reservation_detail = $form_state['merci_reservation_detail'];

  merci_reservation_detail_delete($merci_reservation_detail);

  drupal_set_message(t('The merci_reservation_detail %name has been deleted.', array('%name' => $merci_reservation_detail->name)));
  watchdog('merci_reservation_detail', 'Deleted merci_reservation_detail %name.', array('%name' => $merci_reservation_detail->name));

  $form_state['redirect'] = 'admin/content/merci_reservation_details';
}



/**
 * Page to add MerciReservationDetail Entities.
 *
 * @todo Pass this through a proper theme function
 */
function merci_reservation_detail_add_page() {
  $controller = entity_ui_controller('merci_reservation_detail');
  return $controller->addPage();
}


/**
 * Displays the list of available merci_reservation_detail types for merci_reservation_detail creation.
 *
 * @ingroup themeable
 */
function theme_merci_reservation_detail_add_list($variables) {
  $content = $variables['content'];
  $output = '';
  if ($content) {
    $output = '<dl class="merci_reservation_detail-type-list">';
    foreach ($content as $item) {
      $output .= '<dt>' . l($item['title'], $item['href']) . '</dt>';
      $output .= '<dd>' . filter_xss_admin($item['description']) . '</dd>';
    }
    $output .= '</dl>';
  }
  else {
    if (user_access('administer merci_reservation_detail types')) {
      $output = '<p>' . t('MerciReservationDetail Entities cannot be added because you have not created any merci_reservation_detail types yet. Go to the <a href="@create-merci_reservation_detail-type">merci_reservation_detail type creation page</a> to add a new merci_reservation_detail type.', array('@create-merci_reservation_detail-type' => url('admin/structure/merci_reservation_detail_types/add'))) . '</p>';
    }
    else {
      $output = '<p>' . t('No merci_reservation_detail types have been created yet for you to use.') . '</p>';
    }
  }

  return $output;
}





/**
 * Sets the breadcrumb for administrative merci_reservation_detail pages.
 */
function merci_reservation_detail_set_breadcrumb() {
  $breadcrumb = array(
    l(t('Home'), '<front>'),
    l(t('Administration'), 'admin'),
    l(t('Content'), 'admin/content'),
    l(t('MerciReservationDetails'), 'admin/content/merci_reservation_details'),
  );

  drupal_set_breadcrumb($breadcrumb);
}



