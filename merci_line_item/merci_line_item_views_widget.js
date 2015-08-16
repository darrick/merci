(function($) {
  Drupal.behaviors.merciLineItemViewsWidget = {
    attach: function (context, settings) {
      if (settings.entityReferenceViewWidget) {
        var ervwSetttings = settings.entityReferenceViewWidget.settings;
        if (ervwSetttings.element == 'merci_line_item_reference') {
          $('#edit-ervw-submit', context).mousedown(function (event) {
            selected_ids = $("#modal-content input.entity-reference-view-widget-select:checked").map(function () {return this.value;}).get().join(",");
            $('input[name="merci_line_item_reference[und][add_form][selected_entity_ids]"]').val(selected_ids);
            Drupal.CTools.Modal.dismiss();
            //$("#merci-reservation-edit-checkout-form").submit();
            $('input[name="merci_add_selected_entity_ids"]').trigger('mousedown');
          });
        }
      }
    }
  };
})(jQuery);
