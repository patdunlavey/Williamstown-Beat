<?php
// $Id: cron.inc,v 1.1.2.3 2008/11/20 04:13:27 jrbeeman Exp $

/**
 * @file
 * Cron-related methods for Signup Scheduler.
 */

/**
 * Change "enabled" status of signup-enabled modules based on scheduled open
 * and close dates.
 */
function signup_scheduler_process_all_nodes() {
  signup_scheduler_close_open_signups();
  signup_scheduler_open_closed_signups();
}


/**
 * Close open signups, if scheduler is enabled for them.
 */
function signup_scheduler_close_open_signups() {
  $sql = "SELECT ss.nid 
          FROM {signup_scheduler} ss, {signup} s 
          WHERE ss.status = 1 
            AND ss.close < %d 
            AND ss.nid = s.nid 
            AND s.status = 1";
  $result = db_query($sql, time());
  
  while ($signup = db_fetch_object($result)) {
    $node = node_load($signup->nid);
    $node->signup_status = 0;
    signup_close_signup($signup->nid, $cron = 'yes');
    foreach (module_implements('signup_close') as $module) {
      $function = $module .'_signup_close';
      $function($node);
    }
    watchdog('signup', 'Signups closed for %event by cron.', array('%event' => $node->title), WATCHDOG_NOTICE, l(t('view'), 'node/'. $node->nid));
  }
}


/**
 * Open closed signups (only open if the close date is after now).
 */
function signup_scheduler_open_closed_signups() {
  $sql = "SELECT ss.nid 
          FROM {signup_scheduler} ss, {signup} s 
          WHERE ss.status = 1 
            AND ss.open < %d 
            AND ss.close > %d 
            AND ss.nid = s.nid 
            AND s.status = 0";
  $result = db_query($sql, time(), time());
  
  while ($signup = db_fetch_object($result)) {
    $nid = $signup->nid;
    $node = node_load($nid);
    // Don't open signups if they're full
    $open = FALSE;
    if ($node->signup_close_signup_limit == 0) {
      $open = TRUE;
    }
    if ($node->signup_close_signup_limit > 0 && ($node->signup_total < $node->signup_close_signup_limit)) {
      $open = TRUE;
    }
    if ($open) {
      $node->signup_status = 1;
      db_query("UPDATE {signup} SET status = 1 WHERE nid = %d", $nid);
      foreach (module_implements('signup_open') as $module) {
        $function = $module .'_signup_open';
        $function($node);
      }
      watchdog('signup', 'Signups opened for %title by cron.', array('%title' => $node->title), WATCHDOG_NOTICE, l(t('view'), 'node/'. $nid));
    }
  }
}