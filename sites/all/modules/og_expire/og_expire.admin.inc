<?php
// $Id: og_expire.admin.inc,v 1.1.2.1 2009/04/15 09:34:49 meba Exp $

/**
  * @file
  * System settings form for expiration module.
  */
function og_expire_configure($form_state) {
  $form['og_expire_action'] = array(
      '#type' => 'radios',
      '#title' => t('Expiration action'),
      '#description' => t('After membership expires, delete user membership from the group or just set him inactive (effectively meaning that his membership needs approval again)?'),
      '#options' => array('0' => t('Set inactive'), '1' => t('Delete')),
      '#default_value' => variable_get('og_expire_action', 0),
      );

  $form['og_expire_default_value'] = array(
      '#type' => 'select',
      '#title' => t('Default expiration date offset'),
      '#description' => t('Whenever you are setting or changing expiration date, prolong membership by this value.'),
      '#options' => array(604800 => t('1 week'),
                          1209600 => t('2 weeks'),
                          2592000 => t('1 month'),
                          5184000 => t('2 months'),
                          7776000 => t('3 months'),
                          15552000 => t('6 months'),
                          31536000 => t('1 year'),
        ),
      '#default_value' => variable_get('og_expire_default_value', 31536000),
      );
  $form['og_expire_send_notice'] = array(
      '#type' => 'select',
      '#title' => t('Send expiration notice in advance'),
      '#description' => t('Send a notice to user this time in advance before the membership actually expires.'),
      '#options' => array(0 => t('Never'),
                          604800 => t('1 week'),
                          1209600 => t('2 weeks'),
                          2592000 => t('1 month'),
                          5184000 => t('2 months'),
                          7776000 => t('3 months'),
                          15552000 => t('6 months'),
                          31536000 => t('1 year'),
        ),
      '#default_value' => variable_get('og_expire_send_notice', 604800),
      );
  $form['og_expire_notify_after'] = array(
      '#type' => 'checkbox',
      '#title' => t('Notify users after their membership expires.'),
      '#default_value' => variable_get('og_expire_notify_admin', FALSE),
      );

  $form['og_expire_notify_admin'] = array(
      '#type' => 'checkbox',
      '#title' => t('Notify administrators too'),
      '#description' => t('Whenever sending expiration notice to user, Cc: the group administrators too'),
      '#default_value' => variable_get('og_expire_notify_admin', FALSE),
      );

  return system_settings_form($form);
}
