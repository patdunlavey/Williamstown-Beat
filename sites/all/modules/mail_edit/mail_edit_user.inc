<?php
// $Id: mail_edit_user.inc,v 1.1.2.2 2009/09/14 00:29:43 litwol Exp $

//hook_mailkeys
function user_mailkeys() {
  return array(
    'register_admin_created' => t(' Welcome message for user created by the admin'),//has password
    'register_no_approval_required' => t(' Welcome message when user self-registers'),//has password
    'register_pending_approval' => t(' Welcome message, user pending admin approval'),//has password
    'register_pending_approval_admin' => t(' Message to admin, user is pending'),//has password
    'password_reset' => t(' Password recovery request'),
    'status_activated' => t(' Account activated'),
    'status_blocked' => t(' Account blocked'),
    'status_deleted' => t(' Account deleted'),
  );
}

//hook_mail_edit_tokens_list($mailkey, $options = array())
function user_mail_edit_tokens_list($mailkey, $options = array()) {
  $tokens = array();
  switch ($mailkey) {
    case 'register_admin_created':
    case 'register_no_approval_required':
    case 'register_pending_approval': 
    case 'register_pending_approval_admin':
      $tokens['!password'] = "User's raw password";
    case 'password_reset':
    case 'status_activated':
    case 'status_blocked':
    case 'status_deleted':
      $tokens['!username']= "User's username";
      $tokens['!site']= 'Site name';
      $tokens['!login_url']= 'One time login URL for password reset';
      $tokens['!uri_brief']= "Website's URL without the leading http://";
      $tokens['!uri']= "Website's URL";
      $tokens['!mailto']= "User's Email address (email address to which this email is being sent to)";
      $tokens['!date']= "Date and time when this email is sent";
      $tokens['!login_uri'] = "URL to user's profile or login form if user is not logged in";
      $tokens['!edit_uri'] = "URL to user's profile edit page";
      break;
  }
  return $tokens;
}

//hook_mail_edit_tokens_value($mailkey, $object, $options = array())
function user_mail_edit_tokens_value($mailkey, $mail, $options = array()) {
  global $base_url;
  $account = $mail['params']['account'];
  $language = $mail['language'];
  $tokens = array(
    '!username' => $account->name,
    '!site' => variable_get('site_name', 'Drupal'),
    '!login_url' => user_pass_reset_url($account),
    '!uri_brief' => substr($base_url, strlen('http://')),
    '!uri' => $base_url,
    '!mailto' => $account->mail,
    '!date' => format_date(time(), 'medium', '', NULL, $language->language),
    '!login_uri' => url('user', array('absolute' => TRUE, 'language' => $language)),
    '!edit_uri' => url('user/'. $account->uid .'/edit', array('absolute' => TRUE, 'language' => $language)),
  );
  if (!empty($account->password)) {
    $tokens['!password'] = $account->password;
  }
  return $tokens;
}

//hook_mail_edit_text($mailkey, $langcode)
function user_mail_edit_text($mailkey, $langcode) {
  $return = array();
  $return['subject'] = _user_mail_text($mailkey .'_subject', $langcode);
  $return['body'] = _user_mail_text($mailkey .'_body', $langcode);
  return $return;
}