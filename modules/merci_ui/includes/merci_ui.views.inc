<?php
/**
* Implementation of hook_views_plugin().
*/
function merci_ui_views_plugins() {
  return array(
    'access' => array(
      'php' => array(
        'title' => t('PHP code'),
        'help' => t('Access will be granted if PHP code returns TRUE.'),
        'handler' => 'views_plugin_access_php',
        'theme path' => 'sites/all/modules/views/theme',
        'theme file' => 'theme.inc',
        'uses options' => TRUE,
        'path' => drupal_get_path('module', 'merci_ui') . '/includes',
      ),
    ),
  );
}
