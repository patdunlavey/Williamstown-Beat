<?php
// $Id: og_expire.views.inc,v 1.1.2.1 2009/04/15 09:34:49 meba Exp $

/**
  * @file
  *  Views integration for OG Expiration.
  */

/**
 * Implementation of hook_views_data().
 */
function og_expire_views_data() {
  // Table definition.
  $data['og_expire']['table']['group'] = t('Organic groups');
  $data['og_expire']['table']['join'] = array(
      'node' => array(
        'left_field' => 'nid',
        'field' => 'nid',
        ),
      'users' => array(
        'left_field' => 'uid',
        'field' => 'uid',
        ),
      );
  // Fields.
  $data['og_expire']['uid'] = array(
      'title' => t('Expiration dates for current user'),
      'filter' => array(
        'handler' => 'views_handler_filter_user_current',
        'help' => t("Use when you need to list expiration dates for currently logged in user."),
        ),
      );

  $data['og_expire']['expire'] = array(
      'title' => t('OG: Membership expiration date'),
      'help' => t("Membership expiration date."),
      'field' => array(
        'handler' => 'views_handler_field_og_expire',
        'click sortable' => TRUE,
        ),
      'sort' => array(
        'handler' => 'views_handler_sort_date',
        ),
      'filter' => array(
        'handler' => 'views_handler_filter_date',
        ),

      );

  return $data;
}
