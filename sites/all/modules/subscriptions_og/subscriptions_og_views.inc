<?php
// $Id: subscriptions_og_views.inc,v 1.1.2.4 2008/03/15 05:25:10 develCuy Exp $

/**
 * Implementation of hook_views_tables().
 *
 * @ingroup hooks
 */
function subscriptions_og_views_tables() {
  $t = get_t();
  
  // Add new field "Email" which must replace the original one in view og_my
  $tables['subscriptions'] = array(
    'name' => 'subscriptions',
    'join' => array(
      'left' => array(
        'table' => 'og_uid',
        'field' => 'uid',
      ),
      'right' => array(
        'field' => 'recipient_uid',
      ),
      'extra' => array(
        'value = og_uid.nid' => NULL,
        'module' => 'node',
        'field' => 'group_nid',
      ),
    ),
    'fields' => array(
      'recipient_uid' => array(
        'name' => $t('Subscriptions OG: Subscription email'),
        'handler' => 'og_handler_field_yesempty',
        'help' => $t('Does subscriber receive email notifications for a group.'),
      ),
    ),
  );
  
  return $tables;
}

/**
 * Implementation of hook_views_pre_query().
 *
 * @ingroup hooks
 */
function subscriptions_og_views_pre_query(&$view) {
  if ($view->name == 'og_my') {
    // Override view og_my's Email field
    $view->field[3] = array(
      'tablename' => 'subscriptions',
      'field' => 'recipient_uid',
      'label' => $view->field[3]['label'],
      'fullname' => 'subscriptions.recipient_uid',
      'id' => 'subscriptions.recipient_uid',
      'queryname' => 'subscriptions_recipient_uid',
    );
  }
}
