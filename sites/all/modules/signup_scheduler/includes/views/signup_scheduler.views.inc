<?php
// $Id: signup_scheduler.views.inc,v 1.1.2.1 2008/11/20 05:18:57 jrbeeman Exp $

/**
 * @file
 * Signup Scheduler Views integration.
 */

/**
 * Implementation of hook_views_data().
 */
function signup_scheduler_views_data() {
  $data = array();
  $data['signup_scheduler']['table']['group'] = t('Signup');
  $data['signup_scheduler']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
  );
  $data['signup_scheduler']['open'] = array(
    'title' => t('Scheduler: Open date'),
    'help' => t('The date upon which the signups for this node are scheduled to open.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'signup_scheduler_handler_filter_status',
      'help' => t('Filter on if Signup Scheduler is enabled for each node.'),
      'label' => 'Enabled',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
      'help' => t('Sort by if Signup Scheduler is enabled.'),
    ),
  );
  $data['signup_scheduler']['close'] = array(
    'title' => t('Scheduler: Close date'),
    'help' => t('The date upon which the signups for this node are scheduled to close.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
      'help' => t('Time when signups are scheduled to close.'),
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
      'help' => t('Sort by time signups are scheduled to close.'),
    ),
  );
  $data['signup_scheduler']['status'] = array(
    'title' => t('Scheduler: Status: Enabled / Disabled'),
    'help' => t('Is Signup Scheduler enabled for this node?'),
    'field' => array(
      'handler' => 'signup_scheduler_handler_field_status',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'signup_scheduler_handler_filter_status',
      'help' => t('Filter on if signups are open or closed for each node.'),
      'label' => 'Enabled',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
      'help' => t('Sort by if signups are closed or open.'),
    ),
  );
  return $data;
}


/**
 * Implementation of hook_views_handlers().
 */
function signup_scheduler_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'signup_scheduler') .'/includes/views/handlers',
    ),
    'handlers' => array(
      'signup_scheduler_handler_field_status' => array(
        'parent' => 'views_handler_field',
      ),
      'signup_scheduler_handler_filter_status' => array(
        'parent' => 'views_handler_filter_boolean_operator',
      ),
    ),
  );
}