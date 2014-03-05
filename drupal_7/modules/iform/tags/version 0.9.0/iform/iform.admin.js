(function ($) {(function ($) {
  $(document).ready(function() {
    $('input[name="indicia_warehouse"]').change(function(evt) {
      if (evt.target.value=='other') {
        $('#warehouse_details').removeClass('collapsed');
      } else {
        $('#warehouse_details').addClass('collapsed');
      }
    });
  });
})(jQuery);})(jQuery);