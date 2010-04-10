<?php
// $Id$

/**
 * @file
 * User contact page callbacks for the contact_user_anon module.
 */

/**
 * Personal contact page.
 */
function contact_user_anon_page($account) {
  global $user;

  if (!flood_is_allowed('contact', variable_get('contact_hourly_threshold', 3))) {
    $output = t('You cannot contact more than %number users per hour. Please try again later.', array('%number' => variable_get('contact_hourly_threshold', 3)));
  }
  else {
    drupal_set_title(check_plain($account->name));
    $output = drupal_get_form('contact_user_anon_mail_user', $account);
  }

  return $output;
}

/**
 * Provides the form definition for the user contact page.
 */
function contact_user_anon_mail_user(&$form_state, $recipient) {
  global $user;
  $form['#token'] = $user->uid ? $user->name . $user->mail : '';
  $form['recipient'] = array(
    '#type' => 'value',
    '#value' => $recipient
  );
  $form['from'] = array(
    '#type' => 'textfield',
    '#title' => t('Your name'),
    '#maxlength' => 255,
    '#default_value' => $user->uid ? $user->name : '',
    '#required' => TRUE,
  );
  $form['mail'] = array(
    '#type' => 'textfield',
    '#title' => t('Your e-mail address'),
    '#maxlength' => 255,
    '#default_value' => $user->uid ? $user->mail : '',
    '#required' => TRUE,
  );
  $form['to'] = array(
    '#type' => 'item',
    '#title' => t('To'),
    '#value' => check_plain($recipient->name),
  );
  $form['subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#maxlength' => 50,
    '#required' => TRUE,
  );
  $form['message'] = array(
    '#type' => 'textarea',
    '#title' => t('Message'),
    '#rows' => 15,
    '#required' => TRUE,
  );
  if ($user->uid) {
    $form['copy'] = array(
      '#type' => 'checkbox',
      '#title' => t('Send yourself a copy.'),
    );
  }
  else {
    drupal_add_js(drupal_get_path('module', 'contact_user_anon') .'/contact_user_anon.js');
    $form['copy'] = array(
      '#type' => 'value',
      '#value' => FALSE,
    );
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Send e-mail'),
  );
  return $form;
}

/**
 * Validate the user contact page form submission.
 */
function contact_user_anon_mail_user_validate($form, &$form_state) {
  if (!valid_email_address($form_state['values']['mail'])) {
    form_set_error('mail', t('You must enter a valid e-mail address.'));
  }
  global $user;
  if (!$user->uid) {
    foreach (array('from' => 'name', 'mail' => 'mail') as $form_field => $cookie_field) {
      // Set cookie for 365 days.
      if (isset($form_state['values'][$form_field])) {
        setcookie('comment_info_'. $cookie_field, $form_state['values'][$form_field], time() + 31536000, '/');
      }
    }
  }
}

/**
 * Process the personal contact page form submission.
 */
function contact_user_anon_mail_user_submit($form, &$form_state) {
  global $user, $language;

  $values = $form_state['values'];
  $account = $values['recipient'];

  // Send from the current user to the requested user.
  $to = $account->mail;
  $from = $values['mail'];

  // Save both users and all form values for email composition.
  $values['account'] = $account;
  if (!$user->uid) {
    $user->mail = $values['mail'];
    $user->name = $values['from'];
  }
  $values['user'] = $user;

  // Send the e-mail in the requested user language.
  drupal_mail('contact_user_anon', 'user_mail', $to, user_preferred_language($account), $values, $from);

  // Send a copy if requested, using current page language.
  if ($values['copy']) {
    drupal_mail('contact_user_anon', 'user_copy', $from, $language, $values, $from);
  }

  flood_register_event('contact');
  watchdog('mail', '%name-from sent %name-to an e-mail.', array('%name-from' => $user->name, '%name-to' => $account->name));
  drupal_set_message(t('The message has been sent.'));

  // Back to the requested users profile page or the homepage if the
  // user does not have access to user profiles.
  $form_state['redirect'] = user_access('access user profiles') ? "user/$account->uid" : '';
}
