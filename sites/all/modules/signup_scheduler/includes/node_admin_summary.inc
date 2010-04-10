<?php
// $Id: node_admin_summary.inc,v 1.1.2.2 2008/11/20 02:01:31 jrbeeman Exp $

/**
 * @file
 * Alterations to the node signup admin summary form.
 */

/**
 * Alter the signup admin form
 */
function signup_scheduler_alter_signup_node_admin_summary_form(&$form, &$form_state) {
  if (isset($form['nid']['#value'])) {
    $node = node_load($form['nid']['#value']);
  }
  if ($node->signup_scheduler_status) {
    if (isset($form['status']['#options'])) {
      $form['status']['#value'] = $form['status']['#options'][$form['status']['#default_value']];
      unset($form['status']['#type']);
      unset($form['status']['#options']);
      unset($form['status']['#default_value']);
    }
    $form['limit']['#value'] = $form['limit']['#default_value'];
    unset($form['limit']['#type']);
    $form['submit']['#value'] = t('<strong>Not available:</strong> Edit node to change signup settings. Signup open and close status is being managed by the scheduled open and close dates.');
    unset($form['submit']['#type']);
  }
}