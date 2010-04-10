<?php
// $Id: node_output.inc,v 1.1.2.3 2008/11/20 06:41:27 jrbeeman Exp $

/**
 * @file
 * Signup Scheduler-related output when viewing a node.
 */

/**
 * Theme the node view with scheduler info
 *
 * @param $node
 *   The node object.
 * @return
 *   HTML
 */
function theme_signup_scheduler_node_view($node, $open, $close) {
  // Are signups open and is the current time before the close date?
  if ($node->signup_status && (time() < $close)) {
    return "<h3>". t('Signup schedule') ."</h3>\n
    <p>". t('Signups open until !date', array('!date' => format_date($close))) ."</p>";
  }

  // Are signups closed and is the current time before the open date?
  if (!$node->signup_status && (time() < $open)) {
    return "<h3>". t('Signup schedule') ."</h3>\n
    <p>". t('Signups open from !open to !close', array('!open' => format_date($open), '!close' => format_date($close))) ."</p>";
  }
}