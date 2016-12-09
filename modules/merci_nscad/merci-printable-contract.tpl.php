<?php 
global $base_path;
$logourl = theme_get_setting('logo_path', '');
?>
  <html>
    <head>
      <title>Contract</title>
      <link type="text/css" rel="stylesheet" href="<?php print $base_path ?><?php print drupal_get_path('module', 'merci_printable_contract'); ?>/contract.css" />
    </head>
    <body>
      <div id="page">
        <div id="header">
        <?php if ($logourl) { ?>
           <img src="<?php print $base_path ?><?php print $logourl ?>">
        <?php } ?>
        <h2><?php print variable_get('site_name', ''); ?> Equipment Rental Contract</h2>
        <?php if (module_exists('token')) { 
          print token_replace(variable_get('merci_contract_header', ''), array('node' => $node));
        }
        else {
          print variable_get('merci_contract_header','');
        }
        ?>
        Start: <?php print $start_date . '<br />'; ?>
        Returned by: <?php print $end_date . '<br />'; ?>
        Name: <?php print $username ?><br />
        Email: <?php print $email ?><br />
        <?php print isset($phone) ? "Phone: $phone" . '<br />' : '' ?>
        
        </div>
        <table id="cost">
          <thead>
            <tr>
              <th>Item</th>
              <th>Returned?</th>
            </tr>
          </thead>
          <tbody>
          <?php

  $even_odd = 'even';

  foreach ($kits as $kit) {
?>
            <tr class="<?php print $even_odd; ?>">
              <td>
                <div><?php print $kit['#title']; ?></div>
              </td>
              <td>
              </td>
            </tr>
                <?php
    $even_odd = ($even_odd == 'even') ? 'odd' : 'even';
    if (count($kit['items']) > 0) {

      foreach ($kit['items'] as $item) {

        ?>
            <tr class="<?php print $even_odd; ?>">
              <td>
                  <ul class="accessories">
                    <li><?php print $item['item_title']; ?></li>
                  </ul>
                  <ul>
<?php
        foreach ($item['accessories'] as $accessory) {
?>
                  <li><?php print $accessory; ?></li>
<?php
        }
?>
                  </ul>
              </td>
              <td>
              </td>
            </tr>
                    <?php
    $even_odd = ($even_odd == 'even') ? 'odd' : 'even';
      }
      // foreach

    }
    // if
  }

  foreach ($items as $item) {


    if ($item['item_title']) {
      $title = htmlspecialchars($item['item_title']);
    }
    else {
      $title = '<b>SPECIFIC ITEM NOT SELECTED FROM BUCKET</b>';
    }
    ?>
            <tr class="<?php print $even_odd; ?>">
              <td>
                <div><?php print $title; ?></div>
              </td>
              <td>
              </td>
            </tr>
                <?php
    $even_odd = ($even_odd == 'even') ? 'odd' : 'even';
    if (count($item['accessories']) > 0) {

      foreach ($item['accessories'] as $accessory) {

        ?>
            <tr class="<?php print $even_odd; ?>">
              <td>
                  <ul class="accessories">
                    <li><?php print $accessory; ?></li>
                  </ul>
              </td>
              <td>
              </td>
            </tr>
                    <?php
    $even_odd = ($even_odd == 'even') ? 'odd' : 'even';
      }
      // foreach

    }
    // if

  }
  // foreach

  ?>
          </tbody>
        </table>
        <div id="boilerplate"><?php if (module_exists('token')) { echo token_replace(variable_get('merci_contract_boilerplate', ''), array('node' => $node)); }
  else  { echo variable_get('merci_contract_boilerplate',''); } ?></div>
        <div id="footer"><?php if (module_exists('token')) { echo token_replace(variable_get('merci_contract_footer', ''), array('node' => $node)); }
  else  { echo variable_get('merci_contract_footer',''); } ?></div>
      </div>
    </body>
