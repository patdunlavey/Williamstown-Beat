// $Id$

Drupal.behaviors.contact = function(context) {
  var cookie_parts = new Array("name", "mail");
  var form_parts = new Array("from", "mail");
  var cookie = '';
  for (i=0;i<2;i++) {
    cookie = Drupal.contact.getCookie('comment_info_' + cookie_parts[i]);
    if (cookie != '') {
      $("#contact-user-anon-mail-user input[name=" + form_parts[i] + "]:not(.comment-processed)", context)
        .val(cookie)
        .addClass('comment-processed');
    }
  }
};

Drupal.contact = {};

Drupal.contact.getCookie = function(name) {
  var search = name + '=';
  var returnValue = '';

  if (document.cookie.length > 0) {
    offset = document.cookie.indexOf(search);
    if (offset != -1) {
      offset += search.length;
      var end = document.cookie.indexOf(';', offset);
      if (end == -1) {
        end = document.cookie.length;
      }
      returnValue = decodeURIComponent(document.cookie.substring(offset, end).replace(/\+/g, '%20'));
    }
  }

  return returnValue;
};
