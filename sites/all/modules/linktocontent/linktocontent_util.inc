<?php
// $Id: linktocontent_util.inc,v 1.3 2007/10/08 15:18:09 stborchert Exp $

/*
* @file
* Linktocontent Module for Drupal 6.x (patched for FCKeditor)
*/

/**
 * Register a plugin for a special function.
 *
 * @ingroup linktocontent_utility
 * @param $plugin the name of the plugin to register
 * @param $function The function to register to.
 **/
function linktocontent_register($plugin, $function = 'global') {
  if (!$function) {
    $function = 'global';
  }
  $registered = variable_get('linktocontent_registered_plugins', array());
  if (empty($registered[$function][$plugin])) {
    $registered[$function][$plugin] = $plugin;
    variable_set('linktocontent_registered_plugins', $registered);
  }
}

/**
 * Unregister a plugin for a special function.
 *
 * @ingroup linktocontent_utility
 * @param $plugin the name of the plugin to unregister
 * @param $function The function to remove registration for.
 **/
function linktocontent_unregister($plugin, $function = 'global') {
  if (!$function) {
    $function = 'global';
  }
  $registered = variable_get('linktocontent_registered_plugins', array());
  if ($function === 'global') {
    // remove all registered functions of this plugin
    foreach ($registered as $func => $val) {
      unset($registered[$func][$plugin]);
    }
  }
  else {
    unset($registered[$function][$plugin]);
  }
  variable_set('linktocontent_registered_plugins', $registered);
}

/**
 * Install a plugin.
 *
 * @ingroup linktocontent_utility
 * @param $plugin The name of the plugin to install.
 **/
function _linktocontent_autoinstall($plugin) {
  if (module_exists('fckeditor') || !module_exists('tinymce')) {
    return TRUE;
  }
  $plugin_reg = drupal_get_path('module', 'tinymce') .'/plugin_reg.php';
  // check if plugin_reg.php exists
  if (!file_exists($plugin_reg) && module_exists('tinymce')) {
    drupal_set_message(t('The file %file could not be found. Please make sure !tinymce is installed correct.',
      array('%file' => $file, '!tinymce' => t('TinyMCE'))), 'error');
    return FALSE;
  }

  // are we allowed to change plugin_reg.php?
  if (!@is_writable($plugin_reg)) {
    // try to set write-permission
    if (!@chmod($plugin_reg, 0766)) {
      drupal_set_message(t('The file %file is not writable. Please read INSTALL.txt (section "How to install manually") for instructions, how to install the module manually.',
        array('%file' => $file)), 'error');
      return FALSE;
    }
  }
  
  // the text to insert in plugin_reg.php
  $insert = '
// linktocontent.module: '. $plugin .'
if (is_dir(drupal_get_path(\'module\', \'tinymce\') . \'/tinymce/jscripts/tiny_mce/plugins/'. $plugin .'/\')) {
  $plugins[\''. $plugin .'\'] = array();
  $plugins[\''. $plugin .'\'][\'theme_advanced_buttons3\'] = array(\''. $plugin .'\');
}

';
  // content of plugin_reg.php
  $content = file_get_contents($plugin_reg, FALSE);
  if (!$content || (strpos($content, 'function _tinymce_plugins()') === FALSE)) {
    drupal_set_message(t('Could not open %file or %file isn\'t correct. Please make sure !tinymce is installed correct.',
      array('%file' => 'plugin_reg.php', '!tinymce' => t('TinyMCE'))), 'error');
    return FALSE;
  }

  // check whether plugin already exists in plugin_reg.php
  if (strpos($content, '// linktocontent.module: '. $plugin) !== FALSE) {
    drupal_set_message(t('The plugin %plugin has been already installed with this installation of !tinymce.<br />If you try to do a multi-site installation read INSTALL.txt section "multi-site installation" for further instructions.',
      array('%plugin' => $plugin, '!tinymce' => t('TinyMCE'))), 'error');
    return FALSE;
  }

  $insert_pos = strpos($content, 'return $plugins;');
  if ($insert_pos !== FALSE) {
    $content = substr($content, 0, $insert_pos - 1) . $insert . substr($content, $insert_pos);
    if (_linktocontent_install_write($plugin_reg, $content) === FALSE) {
      return FALSE;
    }
  }
}

/**
 * Uninstall a plugin.
 *
 * @ingroup linktocontent_utility
 * @param $plugin The name of the plugin to uninstall.
 **/
function _linktocontent_autouninstall($plugin) {
  $plugin_reg = drupal_get_path('module', 'tinymce') .'/plugin_reg.php';
  // check if plugin_reg.php exists
  if (!file_exists($plugin_reg) && module_exists('tinymce')) {
    drupal_set_message(t('The file %file could not be found. Please make sure !tinymce is installed correct.',
      array('%file' => $plugin_reg, '!tinymce' => t('TinyMCE'))), 'error');
    return FALSE;
  }


  $insert = '
// linktocontent.module: '. $plugin .'
if (is_dir(drupal_get_path(\'module\', \'tinymce\') . \'/tinymce/jscripts/tiny_mce/plugins/'. $plugin .'/\')) {
  $plugins[\''. $plugin .'\'] = array();
  $plugins[\''. $plugin .'\'][\'theme_advanced_buttons3\'] = array(\''. $plugin .'\');
}

';
  $content = file_get_contents($plugin_reg, FALSE);
  if (!$content || (strpos($content, 'function _tinymce_plugins()') === FALSE)) {
    drupal_set_message(t('Could not open %file or %file isn\'t correct. Please make sure !tinymce is installed correct.',
      array('%file' => 'plugin_reg.php', '!tinymce' => t('TinyMCE'))), 'error');
    return FALSE;
  }

  // check whether plugin exists in plugin_reg.php
  $pos = strpos($content, $insert);
  if ($pos !== FALSE) {
    $return_pos = strpos($content, 'return $plugins;');
    if ($return_pos !== FALSE) {
      $content = substr($content, 0, $pos + 1) . substr($content, $return_pos);
      _linktocontent_install_write($plugin_reg, $content);
    }
  }
}

/**
 * Write content to file.
 *
 * @ingroup linktocontent_utility
 * @param $file Path of the file to write to.
 * @param $content The content to be written.
 **/
function _linktocontent_install_write($file, $content) {
  if (!$handle = fopen($file, 'wb+')) {
    drupal_set_message(t('Could not open %file for writing.', array('%file' => $file)), 'error');
    return FALSE;
  }
  if (!fwrite($handle, $content)) {
    drupal_set_message(t('Could not write to %file.', array('%file' => $file)), 'error');
    return FALSE;
  }
  fclose($handle);
}
