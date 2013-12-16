<?php

/**
 * @file
 * Work calendar admin settings UI.
 */

/**
 * Form builder for the site work calendar settings.
 */
function merci_hours_sitewide_settings() {
  $options = db_select('merci_hours', 'wc')
    ->fields('wc', array('name', 'label'))
    ->execute()
    ->fetchAllKeyed(0, 1);

  if (empty($options)) {
    $form['message'] = array(
      '#markup' => t("There is no merci calendar created yet. Create one at <a href='!url'>admin/structure/merci-hours</a>.", array('!url' => url('admin/structure/merci-hours'))),
    );
  }

  $form['merci_hours_default'] = array(
    '#type' => 'select',
    '#title' => t('Default merci calendar'),
    '#empty_value' => '',
    '#default_value' => variable_get('merci_hours_default', ''),
    '#options' => $options,
    '#disabled' => empty($options),
  );

  return system_settings_form($form);
}
