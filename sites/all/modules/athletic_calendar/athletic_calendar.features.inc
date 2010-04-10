<?php

/**
 * Implementation of hook_content_default_fields().
 */
function athletic_calendar_content_default_fields() {
  module_load_include('inc', 'athletic_calendar', 'athletic_calendar.defaults');
  return _athletic_calendar_content_default_fields();
}

/**
 * Implementation of hook_node_info().
 */
function athletic_calendar_node_info() {
  module_load_include('inc', 'athletic_calendar', 'athletic_calendar.defaults');
  return _athletic_calendar_node_info();
}

/**
 * Implementation of hook_views_default_views().
 */
function athletic_calendar_views_default_views() {
  module_load_include('inc', 'athletic_calendar', 'athletic_calendar.defaults');
  return _athletic_calendar_views_default_views();
}
