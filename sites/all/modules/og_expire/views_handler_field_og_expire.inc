<?php
// $Id: views_handler_field_og_expire.inc,v 1.1.2.1 2009/04/15 09:34:49 meba Exp $

/**
  * @file
  *  Views integration for OG Expiration.
  */

class views_handler_field_og_expire extends views_handler_field {

  function option_definition() {
    $options = parent::option_definition();
    $options['label'] = array('default' => t('Expiration date'), 'translatable' => TRUE);
    $options['include_link'] = array('default' => FALSE);
    return $options;
  }
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['include_link'] = array(
        '#title' => t('If viewer is group admin, include link to change expiration date.'),
        '#type' => 'checkbox',
        '#default_value' => $this->options['include_link'],
        );
  }

  function render($values) {
    global $user;
    if ($user->og_groups[$values->og_uid_nid]['is_admin'] && $this->options['include_link']) {
      // User is admin of displayed group.
      $link = ' ' . l(t('Change'), 'og/expire/' . $values->og_uid_nid . '/' . $values->og_uid_uid);
    }
    else {
      $link = '';
    }
    if ($values->og_uid_is_admin) {
      return t('Never');
    }
    if ($values->og_expire_expire) {
      return date('d.m.Y', $values->og_expire_expire) . $link;
    }
    else {
      return t('Never') . $link;
    }
  }
}

