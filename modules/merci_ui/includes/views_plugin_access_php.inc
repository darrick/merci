<?php
// $Id$

/**
 * Access plugin that provides PHP-based access control.
 */
class views_plugin_access_php extends views_plugin_access {
  function access($account) {
    return merci_ui_php_eval($this->options['code'], $account);
  }

  function get_access_callback() {
    return array('merci_ui_php_eval', array($this->options['code']));
  }

  function summary_title() {
    return t('PHP code');
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['code'] = array('default' => '<?php\r\nreturn TRUE;\r\n?>');
    return $options;
  }

  function options_form(&$form, &$form_state) {
    $form['code'] = array(
      '#type' => 'textarea',
      '#title' => t('PHP code'),
      '#default_value' => $this->options['code'],
      '#description' => t('Users will be able to access this display if PHP code returns TRUE. Enclose your code within <code>&lt;?php</code> and <code>?&gt;</code> tags.'),
      '#required' => TRUE,
    );
  }
}

