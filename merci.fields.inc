<?php
/**
 * @file
 * An example field using the Field Types API.
 */

/**
 * @defgroup field_example Example: Field Types API
 * @ingroup examples
 * @{
 * Examples using Field Types API.
 *
 * This is updated from Barry Jaspan's presentation at Drupalcon Paris,
 * @link http://acquia.com/community/resources/acquia-tv/intro-field-api-module-developers Video Presentation @endlink
 *
 * Providing a field requires:
 * - Defining a field:
 *   - hook_field_info()
 *   - hook_field_schema()
 *   - hook_field_validate()
 *   - hook_field_is_empty()
 *
 * - Defining a formatter for the field (the portion that outputs the field for
 *   display):
 *   - hook_field_formatter_info()
 *   - hook_field_formatter_view()
 *
 * - Defining a widget for the edit form:
 *   - hook_field_widget_info()
 *   - hook_field_widget_form()
 *
 * Our module defines the field in field_example_field_info(),
 * field_example_field_validate() and field_example_field_is_empty().
 * field_example_field_schema() is implemented in field_example.install.
 *
 * Our module sets up a formatter in field_example_field_formatter_info() and
 * field_example_field_formatter_view(). These are the API hooks that present
 * formatted and themed output to the user.

 * And finally, our module defines the widet in
 * field_example_field_widget_info() and field_example_field_widget_form().
 * The widget is the form element used to receive input from the user
 * when the field is being populated.
 *
 * @see field_types
 * @see field
 */

/***************************************************************
 * Field Type API hooks
 ***************************************************************/

/**
 * Implements hook_field_widget_info().
 *
 * Three widgets are provided.
 * - A simple text-only widget where the user enters the '#ffffff'.
 * - A 3-textfield widget that gathers the red, green, and blue values
 *   separately.
 * - A farbtastic colorpicker widget that chooses the value graphically.
 *
 * These widget types will eventually show up in hook_field_widget_form,
 * where we will have to flesh them out.
 *
 * @see field_example_field_widget_form()
 */
function merci_field_widget_info_alter(&$info) {
  dpm($info);
  return;
  return array(
    'merci_type_select' => array(
      'label' => t('Merci Type Select'),
      'field types' => array('entityreference'),
    ),
  );
  $widgets = array(
    'entityreference' => array('list_integer', 'list_float', 'list_text'), 
  );

  foreach ($widgets as $widget => $field_types) {
    $info[$widget]['field types'] = array_merge($info[$widget]['field types'], $field_types);
  }
}

/**
 * Implements hook_field_widget_form().
 *
 * hook_widget_form() is where Drupal tells us to create form elements for
 * our field's widget.
 *
 * We provide one of three different forms, depending on the widget type of
 * the Form API item provided.
 *
 * The 'field_example_colorpicker' and 'field_example_text' are essentially
 * the same, but field_example_colorpicker adds a javascript colorpicker
 * helper.
 *
 * field_example_3text displays three text fields, one each for red, green,
 * and blue. However, the field type defines a single text column,
 * rgb, which needs an HTML color spec. Define an element validate
 * handler that converts our r, g, and b fields into a simulated single
 * 'rgb' form element.
 */
function merci_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  $options = entityreference_options_list($field);
  dpm($options);
}


