Drupal.behaviors.merciDaypartPricing = function() {

  $('#edit-field-merci-date-0-value-datepicker-popup-0').change(merciDaypartPricingChange);
  $('#edit-field-merci-date-0-value-timeEntry-popup-1').change(merciDaypartPricingChange);
  $('#edit-field-merci-date-0-value2-datepicker-popup-0').change(merciDaypartPricingChange);
  $('#edit-field-merci-date-0-value2-timeEntry-popup-1').change(merciDaypartPricingChange);
  $('#merci-choices select').change(merciDaypartPricingChange);

} // function

function merciDaypartPricingChange() {

  var items = []
  var dateOne = $('#edit-field-merci-date-0-value-datepicker-popup-0').val();
  var timeOne = $('#edit-field-merci-date-0-value-timeEntry-popup-1').val();
  var dateTwo = $('#edit-field-merci-date-0-value2-datepicker-popup-0').val();
  var timeTwo = $('#edit-field-merci-date-0-value2-timeEntry-popup-1').val();
  var url = Drupal.settings.basePath + 'merci/daypart';
  var data = {'items': '', 'date[0]': dateOne, 'time[0]': timeOne, 'date[1]': dateTwo, 'time[1]': timeTwo};

  if (
    (dateOne != '') &&
    (timeOne != '') &&
    (dateTwo != '') &&
    (timeTwo != '')
  ) {

    $('#merci-choices select').each(function(){
  
      var val = $(this).val();
  
      if (val != '') {
        items.push(val);
      } // if
  
    });
    
    data.items = items.join(',');
  
    $.post(url, data, function(response) {
  
      $('#edit-field-merci-member-cost-0-value').val(response.member)
      $('#edit-field-merci-commercial-cost-0-value').val(response.commercial)
  
    }, 'json');

  } // if

} // merciDaypartPricingChange