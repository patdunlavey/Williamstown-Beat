<?php
// $Id: node_api.inc,v 1.1.2.4 2008/11/20 06:41:27 jrbeeman Exp $

/**
 * @file
 * Handlers for various operations of Drupal's nodeapi.
 */

function signup_scheduler_nodeapi_validate(&$node) {
  $timezone = date_default_timezone_name();
  $open = date_convert(date_make_date($node->signup_scheduler_open, $timezone, DATE_ISO), DATE_OBJECT, DATE_UNIX);
  $close = date_convert(date_make_date($node->signup_scheduler_close, $timezone, DATE_ISO), DATE_OBJECT, DATE_UNIX);

  // make sure close date is after open date
  if (!($close > $open)) {
    form_set_error('signup_scheduler_close', t('The signup scheduled close date must be after the scheduled open date.'));
  }

  if (!$node->signup_enabled) {
    form_set_error('signup_scheduler_status', t('You have enabled signup scheduling, but have not enabled signups.'));
  }
}


function signup_scheduler_nodeapi_update(&$node) {
  // delete existing and reinsert scheduler info
  if ($node->nid) {
    db_query("DELETE FROM {signup_scheduler} WHERE nid = %d", $node->nid);
  }

  $timezone = date_default_timezone_name();
  $open = date_make_date($node->signup_scheduler_open, $timezone, DATE_ISO);
  date_timezone_set($open, timezone_open('UTC'));
  $open = date_convert($open, DATE_OBJECT, DATE_UNIX);
  
  $close = date_make_date($node->signup_scheduler_close, $timezone, DATE_ISO);
  date_timezone_set($close, timezone_open('UTC'));
  $close = date_convert($close, DATE_OBJECT, DATE_UNIX);
  
  db_query("INSERT INTO {signup_scheduler} (nid, status, open, close) VALUES (%d, %d, %d, %d)", $node->nid, $node->signup_scheduler_status, $open, $close);

  if ($node->signup_scheduler_status) {
    // Close signups if the open date is later than now or the close date is before now
    if ($open > time() || $close < time()) {
      signup_close_signup($node->nid);
      drupal_set_message(t('Signups closed for !title because the time is currently outside of the signup scheduler window.', array('!title' => l($node->title, "node/$node->nid"))));
    }
    // Open signups, if applicable
    if (!$node->signup_status && $open < time() && $close > time() && ($node->signup_close_signup_limit == 0 || $node->signup_total < $node->signup_close_signup_limit)) {
      signup_open_signup($node->nid);
      drupal_set_message(t('Signups opened for !title because the time is currently inside the signup scheduler window.', array('!title' => l($node->title, "node/$node->nid"))));
    }
    // If using signup schedule, don't allow seat limit change to reopen the node
    // This is really fragile, but I'm not sure of another way around it without hacking signup module
    if (count($_SESSION['messages']['status'])) {
      foreach ($_SESSION['messages']['status'] as $key => $message) {
        if (stristr($message, t('signups re-opened'))) {
          if ($open > time() || $close < time()) {
            signup_close_signup($node->nid);
            unset($_SESSION['messages']['status'][$key]);
            if (!count($_SESSION['messages']['status'])) {
              unset($_SESSION['messages']['status']);
            }
          }
        }
      }
    }
  }
}


function signup_scheduler_nodeapi_delete(&$node) {
  db_query("DELETE FROM {signup_scheduler} WHERE nid = %d", $node->nid);
}


function signup_scheduler_nodeapi_load(&$node) {
  $result = db_query("SELECT * FROM {signup_scheduler} WHERE nid = %d", $node->nid);
  if ($result) {
    $schedule = db_fetch_object($result);
    $node->signup_scheduler_status = $schedule->status;
    $node->signup_scheduler_open = $schedule->open;
    $node->signup_scheduler_close = $schedule->close;
  }
}


function signup_scheduler_nodeapi_view(&$node) {
  $timezone = date_default_timezone_name();
  $open = $node->signup_scheduler_open;
  $close = $node->signup_scheduler_close;
  
  $open = date_make_date($open, 'UTC', DATE_UNIX);
  date_timezone_set($open, timezone_open($timezone));
  $open = date_convert($open, DATE_OBJECT, DATE_UNIX);
  
  $close = date_make_date($close, 'UTC', DATE_UNIX);
  date_timezone_set($close, timezone_open($timezone));
  $close = date_convert($close, DATE_OBJECT, DATE_UNIX);
  
  $node->content['signup_scheduler'] = array(
    '#value' => theme('signup_scheduler_node_view', $node, $open, $close),
    '#weight' => $node->content['signup']['#weight'],
  );
}