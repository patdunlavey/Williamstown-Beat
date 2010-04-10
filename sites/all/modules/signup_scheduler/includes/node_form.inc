<?php
// $Id: node_form.inc,v 1.1.2.3 2008/11/20 06:41:27 jrbeeman Exp $

/**
 * @file
 * Alterations for the node edit form.
 */

/**
 * Alter the node edit form
 *
 * Add scheduler form items to the signup form.
 */
function signup_scheduler_alter_node_form(&$form, &$form_state) {
  // Simple check to see if signups are enabled
  // All complex logic for this is handled in signup.module
  if (isset($form['signup'])) {
    global $user;

    // Load the node if it already exists.
    if (isset($form['nid']['#value'])) {
      $node = node_load($form['nid']['#value']);
    }
    else {
      $node = NULL;
    }

    $timezone = date_default_timezone_name();
    $open = $node->signup_scheduler_open ? $node->signup_scheduler_open : time();
    $close = $node->signup_scheduler_close ? $node->signup_scheduler_close : time();
    
    $open = date_make_date($open, 'UTC', DATE_UNIX);
    date_timezone_set($open, timezone_open($timezone));
    $open = date_convert($open, DATE_OBJECT, DATE_ISO);
    
    $close = date_make_date($close, 'UTC', DATE_UNIX);
    date_timezone_set($close, timezone_open($timezone));
    $close = date_convert($close, DATE_OBJECT, DATE_ISO);

    $form['signup']['node_settings']['schedule'] = array(
      '#type' => 'fieldset',
      '#title' => t('Schedule'),
    );
    $form['signup']['node_settings']['schedule']['signup_scheduler_status'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable scheduling of signups'),
      '#default_value' => $node->signup_scheduler_status,
      '#description' => t('If enabled, signups will automatically be turned on and off at midnight on the dates selected.'),
    );
    require_once('./'. drupal_get_path('module', 'date_api') .'/date_api_elements.inc');
    $form['signup']['node_settings']['schedule']['signup_scheduler_open'] = array(
      '#type' => module_exists('date_popup') ? 'date_popup' : 'date_select',
      '#date_timezone' => $timezone,
      '#title' => t('Open signups on'),
      '#default_value' => $open,
      '#description' => t('If this date is later than now, signups will be enabled, but closed, for this node until the specified date.'),
    );
    $form['signup']['node_settings']['schedule']['signup_scheduler_close'] = array(
      '#type' => module_exists('date_popup') ? 'date_popup' : 'date_select',
      '#date_timezone' => $timezone,
      '#title' => t('Close signups on'),
      '#default_value' => $close,
    );
  }
}