<?php 
global $base_path;
$logourl = theme_get_setting('logo');
?>
  <html>
    <head>
      <title>Contract</title>
      <link type="text/css" rel="stylesheet" href="<?php print $base_path ?><?php print drupal_get_path('module', 'merci_printable_contract'); ?>/contract.css" />
    </head>
    <body>
      <div id="page">
        <?php if ($logourl) { ?>
        <div id="logo">
           <img src="<?php print $logourl ?>">
        </div>
        <?php } ?>
        <div id="header">
        <?php if (module_exists('token')) { 
          print token_replace(variable_get('merci_contract_header', ''), array('merci_reservation' => $checkout), array('clear' => TRUE));
        }
        else {
          print variable_get('merci_contract_header','');
        }
?>
  
        </div>
        <div id="boilerplate"><?php if (module_exists('token')) { echo token_replace(variable_get('merci_contract_boilerplate', ''), array('merci_reservation' => $checkout)); }
  else  { echo variable_get('merci_contract_boilerplate',''); } ?></div>
        <div id="footer"><?php if (module_exists('token')) { echo token_replace(variable_get('merci_contract_footer', ''), array('merci_reservation' => $checkout)); }
  else  { echo variable_get('merci_contract_footer',''); } ?></div>
      </div>
    </body>
