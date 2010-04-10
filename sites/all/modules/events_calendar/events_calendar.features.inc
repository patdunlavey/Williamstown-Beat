<?php

/**
 * Implementation of hook_content_default_fields().
 */
function events_calendar_content_default_fields() {
  module_load_include('inc', 'events_calendar', 'events_calendar.defaults');
  return _events_calendar_content_default_fields();
}

/**
 * Implementation of hook_node_info().
 */
function events_calendar_node_info() {
  module_load_include('inc', 'events_calendar', 'events_calendar.defaults');
  return _events_calendar_node_info();
}

/**
 * Implementation of hook_views_default_views().
 */
function events_calendar_views_default_views() {
  module_load_include('inc', 'events_calendar', 'events_calendar.defaults');
  return _events_calendar_views_default_views();
}
