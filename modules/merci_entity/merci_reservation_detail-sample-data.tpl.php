<?php

/**
 * @file
 * Example tpl file for theming a single merci_reservation_detail-specific theme
 *
 * Available variables:
 * - $status: The variable to theme (while only show if you tick status)
 * 
 * Helper variables:
 * - $merci_reservation_detail: The MerciReservationDetail object this status is derived from
 */
?>

<div class="merci_reservation_detail-status">
  <?php print '<strong>MerciReservationDetail Sample Data:</strong> ' . $merci_reservation_detail_sample_data = ($merci_reservation_detail_sample_data) ? 'Switch On' : 'Switch Off' ?>
</div>
