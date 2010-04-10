<?php

/**
 * Implementation of hook_content_default_fields().
 */
function views_gallery_content_default_fields() {
  module_load_include('inc', 'views_gallery', 'views_gallery.defaults');
  return _views_gallery_content_default_fields();
}

/**
 * Implementation of hook_context_default_contexts().
 */
function views_gallery_context_default_contexts() {
  module_load_include('inc', 'views_gallery', 'views_gallery.defaults');
  return _views_gallery_context_default_contexts();
}

/**
 * Implementation of hook_imagecache_default_presets().
 */
function views_gallery_imagecache_default_presets() {
  module_load_include('inc', 'views_gallery', 'views_gallery.defaults');
  return _views_gallery_imagecache_default_presets();
}

/**
 * Implementation of hook_node_info().
 */
function views_gallery_node_info() {
  module_load_include('inc', 'views_gallery', 'views_gallery.defaults');
  return _views_gallery_node_info();
}
