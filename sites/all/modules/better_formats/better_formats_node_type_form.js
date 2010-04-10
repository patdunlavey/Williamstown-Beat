// $Id: better_formats_node_type_form.js,v 1.4.2.2 2009/02/25 06:44:17 dragonwize Exp $

/**
 * @file
 * Enhances the default format selection on content type edit forms.
 *
 * Fixes bug that shows weight field when drag and drop is enabled
 * because the field is hidden by default.
 * Also hides formats that are not available per the Allowed checkboxes.
 */

/**
 * Initialize Better Formats setttings and defaults.
 */
function better_formats_init() {
  // Set default settings check for use of global allowed formats.
  Drupal.settings.better_formats = {"num_checked" : $('input.bf-allowed-formats:checked').length};

  // Collapsing the input format setting after the weight columns have been hidden.
  $('.input-format-settings > legend > a').click();

  // Add hide/show events for allowed formats.
  var format_boxes = $('input.bf-allowed-formats');
  format_boxes.click(function() {
    better_formats_toggle_formats($(this));
  });
  if (Drupal.settings.better_formats.num_checked > 0) {
    format_boxes.each(function() {
      better_formats_toggle_formats($(this), true);
    });
  }
}

/**
 * Toggle format display in dropdowns in sync with allowed checkboxes.
 *
 * @param el
 *  DOM element of event.
 * @param init
 *  Boolean value to determine first toggle.
 */
function better_formats_toggle_formats(el, init) {
  // Hide all formats except site default when the first box is checked.
  if (Drupal.settings.better_formats.num_checked === 0) {
    $('select.bf-default-formats option[value != "0"][value != "' + el.val() + '"]').removeAttr('selected').hide();
  }

  $('select.bf-default-formats option[value = "' + el.val() + '"]').each(function() {
    var option = $(this);
      if (el.attr('checked')) {
        option.show();
      }
      else {
        option.removeAttr('selected').hide();
      }
  });

  // Do not modify count on intial run.
  if ( ! init) {
    if (el.attr('checked')) {
      Drupal.settings.better_formats.num_checked += 1;
    }
    else if (Drupal.settings.better_formats.num_checked > 0) {
      // Keep num_checked from going below zero.
      Drupal.settings.better_formats.num_checked -= 1;
    }
  }

  // Show all globally allowed formats if no boxes are checked.
  if (Drupal.settings.better_formats.num_checked === 0) {
    // Show global formats available to roles because no format allowed boxes are checked.
    $('select.bf-default-formats option').show();
  }
}


$(document).ready(better_formats_init);