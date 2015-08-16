(function ($) {
  // Array of cached dates per calendar and month-year tuples.
  var workCalendarDates = new Array();
  var workCalendarTimes = new Array();

  Drupal.behaviors.WorkCalendarDatePopup = {
    attach: function (context) {
      $('.merci-hours-date-popup').each(function() {
        // Initialize workCalendarDates array for the calendar of this popup.
        wc = Drupal.settings.datePopup[this.id].settings.workCalendar;
        workCalendarDates[wc] = new Array();
        Drupal.settings.datePopup[this.id].settings.beforeShowDay = workCalendarCheckDate;
      });
      $('.merci-hours-timefield').each(function() {
        wc = Drupal.settings.datePopup[this.id].settings.workCalendar;
        workCalendarTimes[wc] = new Array();
        Drupal.settings.datePopup[this.id].settings.beforeShow = workCalendarCheckTime;
        Drupal.settings.datePopup[this.id].settings.beforeSetTime = workCalendarSetTime;
      });
    }
  };

  function workCalendarCheckTime(input) {
    selector = Drupal.settings.datePopup[this.id].settings.dateid;
    var date = $('#' + selector).val();

    var date_object = new Date(date);
    var day = date_object.getDay();//('0' + date.getDate().toString()).slice(-2);
    var wc = Drupal.settings.datePopup[this.id].settings.workCalendar;
    // If we don't have the month-year dates for the requested date,
    // ask them to Drupal.

    if (!(day in workCalendarTimes[wc])) {
      workCalendarTimes[wc][day] = (function () {
        var val = null;
        $.ajax({
          'async': false,
          'global': false,
          'url': '/merci_hours_open/' + wc + '/' + day,
          'success': function (data) {
            val = data;
          }
        });
        return val;
      })();
    }
    if (day in workCalendarTimes[wc]) {
      return {minTime : workCalendarTimes[wc][day]['open'],
        maxTime : workCalendarTimes[wc][day]['close']};
    }
  }

  function workCalendarSetTime(oldTime, newTime, minTime, maxTime) {
    console.log("set time " + this.id);
    return newTime;
  }
  function workCalendarCheckDate(date) {
    // Build a key to identify the month-year tuple for the requested date.
    var day = ('0' + date.getDate().toString()).slice(-2);
    var month = ('0' + (date.getMonth() + 1).toString()).slice(-2)
    var year = date.getFullYear();
    var key = year + '-' + month;

    // If we don't have the month-year dates for the requested date,
    // ask them to Drupal.
    var wc = Drupal.settings.datePopup[this.id].settings.workCalendar;
    if (!(key in workCalendarDates[wc])) {
      workCalendarDates[wc][key] = (function () {
        var val = null;
        $.ajax({
          'async': false,
          'global': false,
          'url': '/merci_hours_dates/' + wc + '/' + year + '/' + month,
          'success': function (data) {
            val = data;
          }
        });
        return val;
      })();
    }

    // We want the date in this format 2013-12-31.
    if (workCalendarDates[wc][key].indexOf(key + '-' + day) == -1) {
      return [false, ''];
    }
    return [true, ''];
  }
})(jQuery);
