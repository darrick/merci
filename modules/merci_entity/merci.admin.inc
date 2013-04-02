<?php

/**
 * @file
 * Merci editing UI.
 *
 * We make very little use of the EntityAPI interface for this - preferring instead to use
 * views. That offers more flexibility to change a UI that will, more often than not,
 * be end-user facing.
 */

/**
 * UI controller.
 */
class MerciUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults. Main reason for doing this is that
   * parent class hook_menu() is optimized for entity type administration.
   */
  public function hook_menu() {
    
    $items = array();
    $id_count = count(explode('/', $this->path));
    $wildcard = isset($this->entityInfo['admin ui']['menu wildcard']) ? $this->entityInfo['admin ui']['menu wildcard'] : '%' . $this->entityType;

    $items[$this->path] = array(
      'title' => 'Merci Reservations',
      'description' => 'Add edit and update merci reservations.',
      'page callback' => 'system_admin_menu_block_page',
      'access arguments' => array('access administration pages'),
      'file path' => drupal_get_path('module', 'system'),
      'file' => 'system.admin.inc',
    );
    
    // Change the overview menu type for the list of merci reservations.
    $items[$this->path]['type'] = MENU_LOCAL_TASK;
    
    // Change the add page menu to multiple types of entities
    $items[$this->path . '/add'] = array(
      'title' => 'Add a merci reservation',
      'description' => 'Add a new merci reservation',
      'page callback'  => 'merci_add_page',
      'access callback'  => 'merci_access',
      'access arguments' => array('edit'),
      'type' => MENU_NORMAL_ITEM,
      'weight' => 20,
      'file' => 'merci.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])

    );
    
    // Add menu items to add each different type of entity.
    foreach (merci_get_types() as $type) {
      $items[$this->path . '/add/' . $type->type] = array(
        'title' => 'Add ' . $type->label,
        'page callback' => 'merci_form_wrapper',
        'page arguments' => array(merci_create(array('type' => $type->type))),
        'access callback' => 'merci_access',
        'access arguments' => array('edit', 'edit ' . $type->type),
        'file' => 'merci.admin.inc',
        'file path' => drupal_get_path('module', $this->entityInfo['module'])
      );
    }

    // Loading and editing merci entities
    $items[$this->path . '/merci/' . $wildcard] = array(
      'page callback' => 'merci_form_wrapper',
      'page arguments' => array($id_count + 1),
      'access callback' => 'merci_access',
      'access arguments' => array('edit', $id_count + 1),
      'weight' => 0,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
      'file' => 'merci.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])
    );
    $items[$this->path . '/merci/' . $wildcard . '/edit'] = array(
      'title' => 'Edit',
      'type' => MENU_DEFAULT_LOCAL_TASK,
      'weight' => -10,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    );
    
    $items[$this->path . '/merci/' . $wildcard . '/delete'] = array(
      'title' => 'Delete',
      'page callback' => 'merci_delete_form_wrapper',
      'page arguments' => array($id_count + 1),
      'access callback' => 'merci_access',
      'access arguments' => array('edit', $id_count + 1),
      'type' => MENU_LOCAL_TASK,
      'context' => MENU_CONTEXT_INLINE,
      'weight' => 10,
      'file' => 'merci.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])
    );
    
    // Menu item for viewing merci reservations
    $items['merci/' . $wildcard] = array(
      //'title' => 'Title',
      'title callback' => 'merci_page_title',
      'title arguments' => array(1),
      'page callback' => 'merci_page_view',
      'page arguments' => array(1),
      'access callback' => 'merci_access',
      'access arguments' => array('view', 1),
      'type' => MENU_CALLBACK,
    );
    return $items;
  }
  
  
  /**
   * Create the markup for the add Merci Entities page within the class
   * so it can easily be extended/overriden.
   */ 
  public function addPage() {
    $item = menu_get_item();
    $content = system_admin_menu_block($item);

    if (count($content) == 1) {
      $item = array_shift($content);
      drupal_goto($item['href']);
    }    
        
    return theme('merci_add_list', array('content' => $content));
  }
  
}


/**
 * Form callback wrapper: create or edit a merci resrvation.
 *
 * @param $merci
 *   The merci object being edited by this form.
 *
 * @see merci_edit_form()
 */
function merci_form_wrapper($merci) {
  // Add the breadcrumb for the form's location.
  merci_set_breadcrumb();
  return drupal_get_form('merci_edit_form', $merci);
}


/**
 * Form callback wrapper: delete a merci.
 *
 * @param $merci
 *   The merci object being edited by this form.
 *
 * @see merci_edit_form()
 */
function merci_delete_form_wrapper($merci) {
  // Add the breadcrumb for the form's location.
  //merci_set_breadcrumb();
  return drupal_get_form('merci_delete_form', $merci);
}


/**
 * Form callback: create or edit a merci.
 *
 * @param $merci
 *   The merci object to edit or for a create form an empty merci object
 *     with only a merci type defined.
 */
function merci_edit_form($form, &$form_state, $merci) {
  // Add the default field elements.
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Merci Name'),
    '#default_value' => isset($merci->name) ? $merci->name : '',
    '#maxlength' => 255,
    '#required' => TRUE,
    '#weight' => -5,
  );
  
  $form['data']['#tree'] = TRUE;
  $form['data']['sample_data'] = array(
    '#type' => 'checkbox',
    '#title' => t('An interesting merci switch'),
    '#default_value' => isset($merci->data['sample_data']) ? $merci->data['sample_data'] : 1,
  );

  // Add the field related form elements.
  $form_state['merci'] = $merci;
  field_attach_form('merci', $merci, $form, $form_state);

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
    '#value' => t('Save merci reservation'),
    '#submit' => $submit + array('merci_edit_form_submit'),
  );
  
  if (!empty($merci->name)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete merci reservation'),
      '#suffix' => l(t('Cancel'), 'admin/content/merci'),
      '#submit' => $submit + array('merci_form_submit_delete'),
      '#weight' => 45,
    );
  }

  // We append the validate handler to #validate in case a form callback_wrapper
  // is used to add validate handlers earlier.
  $form['#validate'][] = 'merci_edit_form_validate';
  return $form;
}


/**
 * Form API validate callback for the merci form
 */
function merci_edit_form_validate(&$form, &$form_state) {
  $merci = $form_state['merci'];
  
  // Notify field widgets to validate their data.
  field_attach_form_validate('merci', $merci, $form, $form_state);
}


/**
 * Form API submit callback for the merci form.
 * 
 * @todo remove hard-coded link
 */
function merci_edit_form_submit(&$form, &$form_state) {
  
  $merci = entity_ui_controller('merci')->entityFormSubmitBuildEntity($form, $form_state);
  // Save the merci and go back to the list of mercis
  
  // Add in created and changed times.
  if ($merci->is_new = isset($merci->is_new) ? $merci->is_new : 0){
    $merci->created = time();
  }

  $merci->changed = time();
  
  $merci->save();
  $form_state['redirect'] = 'admin/content/merci';
}

/**
 * Form API submit callback for the delete button.
 * 
 * @todo Remove hard-coded path
 */
function merci_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/content/merci/merci/' . $form_state['merci']->merci_id . '/delete';
}


/**
 * Form callback: confirmation form for deleting a merci.
 *
 * @param $merci
 *   The merci to delete
 *
 * @see confirm_form()
 */
function merci_delete_form($form, &$form_state, $merci) {
  $form_state['merci'] = $merci;

  $form['#submit'][] = 'merci_delete_form_submit';

  $form = confirm_form($form,
    t('Are you sure you want to delete merci %name?', array('%name' => $merci->name)),
    'admin/content/merci/merci',
    '<p>' . t('This action cannot be undone.') . '</p>',
    t('Delete'),
    t('Cancel'),
    'confirm'
  );
  
  return $form;
}

/**
 * Submit callback for merci_delete_form
 */
function merci_delete_form_submit($form, &$form_state) {
  $merci = $form_state['merci'];

  merci_delete($merci);

  drupal_set_message(t('The merci %name has been deleted.', array('%name' => $merci->name)));
  watchdog('merci', 'Deleted merci %name.', array('%name' => $merci->name));

  $form_state['redirect'] = 'admin/content/merci';
}



/**
 * Page to add Merci Entities.
 *
 * @todo Pass this through a proper theme function
 */
function merci_add_page() {
  $controller = entity_ui_controller('merci');
  return $controller->addPage();
}


/**
 * Displays the list of available merci types for merci creation.
 *
 * @ingroup themeable
 */
function theme_merci_add_list($variables) {
  $content = $variables['content'];
  $output = '';
  if ($content) {
    $output = '<dl class="merci-type-list">';
    foreach ($content as $item) {
      $output .= '<dt>' . l($item['title'], $item['href']) . '</dt>';
      $output .= '<dd>' . filter_xss_admin($item['description']) . '</dd>';
    }
    $output .= '</dl>';
  }
  else {
    if (user_access('administer merci types')) {
      $output = '<p>' . t('Merci Entities cannot be added because you have not created any merci types yet. Go to the <a href="@create-merci-type">merci type creation page</a> to add a new merci type.', array('@create-merci-type' => url('admin/structure/merci_types/add'))) . '</p>';
    }
    else {
      $output = '<p>' . t('No merci types have been created yet for you to use.') . '</p>';
    }
  }

  return $output;
}





/**
 * Sets the breadcrumb for administrative merci pages.
 */
function merci_set_breadcrumb() {
  $breadcrumb = array(
    l(t('Home'), '<front>'),
    l(t('Administration'), 'admin'),
    l(t('Content'), 'admin/content'),
    l(t('Merci'), 'admin/content/merci'),
  );

  drupal_set_breadcrumb($breadcrumb);
}



