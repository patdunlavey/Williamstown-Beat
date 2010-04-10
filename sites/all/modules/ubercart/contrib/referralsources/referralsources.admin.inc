<?php
// $Id: referralsources.admin.inc,v 1.1 2009/12/18 22:04:43 adamo Exp $

/**
 * @file
 *
 * Contains callback functions for Referral Sources admin panels
 */

 /**
 * Display tabular list of referral sources
 */
function referralsources_admin_table_form(&$form_state) {
  // Build table header
  $form['header'] = array(
    '#type' => 'value',
    '#value' => array(
      array('data' => t('Code'), 'field' => 'code'),
      array('data' => t('Title'), 'field' => 'title'),
      array('data' => t('Description'), 'field' => 'description'),
      array('data' => t('Weight'), 'field' => 'weight', 'sort' => 'asc'),
      array('data' => ' ', 'field' => 'edit'),
      array('data' => ' ', 'field' => 'delete'),
    ),
  );

  // Build table rows
  $result = db_query("SELECT rid, code, title, description, weight FROM {referralsources}". tablesort_sql($form['header']['#value']));
  while ($row = db_fetch_object($result)) {
    $form['code'][$row->rid] = array('#type' => 'markup', '#value' => $row->code);
    $form['title'][$row->rid] = array('#type' => 'markup', '#value' => $row->title);
    $form['description'][$row->rid] = array(
      '#type' => 'markup',
      '#value' => drupal_strlen($row->description) > 75 ? drupal_substr($row->description, 0, 75) .' (...)' : $row->description,
    );
    $form['weight'][$row->rid] = array(
      '#type' => 'weight',
      '#default_value' => $row->weight,
      '#delta' => 10,
      '#tree' => TRUE,
    );
    $form['edit'][$row->rid] = array('#type' => 'markup', '#value' => l('Edit', 'admin/content/referralsources/'. $row->rid .'/edit'));
    $form['delete'][$row->rid] = array('#type' => 'markup', '#value' => l('Delete', 'admin/content/referralsources/'. $row->rid .'/delete'));
  }

  // Add save button to save reordered weights
  $form['save'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
    '#submit' => array('referralsources_admin_table_form_save'),
  );

  // Enable tree mode on form so submit callback can loop through $form_state['values']['weight'] array
  $form['#tree'] = TRUE;

  return $form;
}

/**
 * Form for adding/editing referral source
 */
function referralsources_admin_form(&$form_state, $rid = NULL) {
  if ($rid != NULL) {
    // Editing existing referral source, load referral source from DB to set default values for form
    $referralsources = db_fetch_array(db_query("SELECT * FROM {referralsources} WHERE rid = %d", $rid));
  }

  $form = array();
  $form['rid'] = array(
    '#type' => 'value',
    '#value' => $referralsources['rid'],
  );
  $form['code'] = array(
    '#type' => 'textfield',
    '#title' => t('Code'),
    '#description' => t('Enter a code for the referral source.  For integration with other systems.'),
    '#default_value' => $referralsources['code'],
    '#size' => 30,
    '#maxlength' => 30,
    '#required' => TRUE,
  );
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#description' => t('Enter a title for the referral source.  This will be used in select lists.'),
    '#default_value' => $referralsources['title'],
    '#size' => 50,
    '#maxlength' => 50,
    '#required' => TRUE,
  );
  $form['grouping'] = array(
    '#type' => 'textfield',
    '#title' => t('Grouping'),
    '#description' => t('If entered, the referral source will appear under the specified group in select lists.'),
    '#default_value' => $referralsources['grouping'],
    '#size' => 50,
    '#maxlength' => 50,
  );
  $form['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#description' => t('Weight affects the position of this item in referral source select lists.  Heavier items fall to the bottom.'),
    '#default_value' => isset($referralsources['weight']) ? $referralsources['weight'] : 0,
    '#delta' => 10,
  );
  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#description' => t('Enter any additional information about the referral source that may be useful to admins.'),
    '#cols' => 100,
    '#rows' => 2,
    '#maxlength' => 255,
    '#default_value' => $referralsources['description'],
  );
  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#submit' => array('referralsources_form_save'),
  );
  $form['cancel'] = array(
    '#value' => l(t('Cancel'), 'admin/content/referralsources/list'),
  );

  return $form;
}

/**
 * Form for referral source delete confirmation
 */
function referralsources_admin_delete_form(&$form_state, $rid) {
  // Get data so form can display list of referral sources to be deleted
  $result = db_fetch_object(db_query("SELECT rid, title FROM {referralsources} WHERE rid = %d", $rid));
  $todelete = array($result->code .' : '. $result->title);

  drupal_set_title('Are you sure you want to delete these items?');
  // Build form
  $form = array();
  $form['rid'] = array(
    '#type' => 'hidden',
    '#value' => $rid,
  );
  $form['dialog'] = array(
    '#type' => 'markup',
    '#value' => theme_item_list($todelete) .'<p>'. t('This action cannot be undone.') .'</p>',
  );
  $form['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#submit' => array('referralsources_admin_delete'),
  );
  $form['cancel'] = array(
    '#value' => l(t('Cancel'), 'admin/content/referralsources/list'),
  );

  return $form;
}

/**
 * Form for referral source admin settings.
 */
function referralsources_admin_settings_form(&$form_state) {
  $form = array();

  $form['general'] = array(
    '#type' => 'fieldset',
    '#title' => t('General'),
    '#collapsible' => TRUE,
    '#weight' => 0,
  );
  $form['general']['referralsource_fieldset_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Fieldset title'),
    '#description' => t('Sets the title of the referral source fieldset on forms. Default is "Referral source".'),
    '#default_value' => check_plain(variable_get('referralsource_fieldset_title', REFERRALSOURCE_FIELDSET_TITLE_DEFAULT)),
  );
  $form['general']['referralsource_fieldset_description'] = array(
    '#type' => 'textarea',
    '#title' => t('Fieldset description'),
    '#description' => t('Sets the description of the referral source fieldset on forms.'),
    '#default_value' => check_plain(variable_get('referralsource_fieldset_description', REFERRALSOURCE_FIELDSET_DESCRIPTION_DEFAULT)),
    '#rows' => 2,
    '#cols' => 50,
  );
  $form['general']['referralsource_select_label'] = array(
    '#type' => 'textfield',
    '#title' => t('Select label'),
    '#description' => t('Sets the label of the referral source select list on forms. Default is "Referral source".'),
    '#default_value' => check_plain(variable_get('referralsource_select_label', REFERRALSOURCE_SELECT_LABEL_DEFAULT)),
  );
  $form['general']['referralsource_other_label'] = array(
    '#type' => 'textfield',
    '#title' => t('Other label'),
    '#description' => t('Sets the label of the "other" text field on forms. Default is "Other".'),
    '#default_value' => check_plain(variable_get('referralsource_other_label', REFERRALSOURCE_OTHER_LABEL_DEFAULT)),
  );
  $form['general']['referralsource_group_select_list'] = array(
    '#type' => 'checkbox',
    '#title' => t('Group items in select list'),
    '#description' => t('If checked, referral sources in the select list will be grouped according to the group set on each referral source.'),
    '#default_value' => TRUE,
  );

  $form['register'] = array(
    '#type' => 'fieldset',
    '#title' => t('User registration'),
    '#collapsible' => TRUE,
    '#weight' => 1,
  );
  $form['register']['referralsource_register_enable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable referral source selection'),
    '#description' => t('If checked, users will be prompted to select a referral source when creating an account.'),
    '#default_value' => check_plain(variable_get('referralsource_register_enable', REFERRALSOURCE_REGISTER_ENABLE_DEFAULT)),
  );
  $form['register']['referralsource_register_weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#description' => t('Sets the weight of the referral source fieldset on the registration page.'),
    '#default_value' => check_plain(variable_get('referralsource_register_weight', REFERRALSOURCE_REGISTER_WEIGHT_DEFAULT)),
    '#delta' => 10,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#weight' => 10,
  );

  return $form;
}

/**
 * Callback to generate referral source statistics page.
 */
function referralsources_admin_statistics(&$form_state) {
  $form = array();

  // Build global stats section
  $form['global'] = array(
    '#type' => 'fieldset',
    '#title' => t('Global Stats'),
    '#description' => t('Global stats include all types of referral source submissions.'),
    '#weight' => -10,
    '#collapsible' => TRUE,
    '#collaped' => FALSE,
  );
  $form['global']['selections'] = array(
    '#type' => 'markup',
    '#value' => referralsources_statistics_selections(),
  );
  $form['global']['other'] = array(
    '#type' => 'markup',
    '#value' => referralsources_statistics_other(),
  );

  // Build a stats section for each referralsource type
  $submission_types = module_invoke_all('referralsource_submission_type');
  foreach ($submission_types as $rstid => $submission_type) {
    $form[$rstid] = array(
      '#type' => 'fieldset',
      '#title' => check_plain($submission_type['title']),
      '#description' => check_plain($submission_type['description']),
      '#weight' => $submission_type['weight'],
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form[$rstid]['selections'] = array(
      '#type' => 'markup',
      '#value' => referralsources_statistics_selections($rstid, $submission_type),
    );
    $form[$rstid]['other'] = array(
      '#type' => 'markup',
      '#value' => referralsources_statistics_other($rstid, $submission_type),
    );

    // Provide stats for individual forms.
    if ($rstid == REFERRALSOURCE_SUBMISSION_TYPE_WEBFORM) {
      // Loop through webforms with referral source submissions, display stats for each
      $result = db_query("SELECT DISTINCT nid FROM {referralsources_submissions} WHERE rstid = '%s'", REFERRALSOURCE_SUBMISSION_TYPE_WEBFORM);
      $webforms = array();
      while ($webform = db_fetch_object($result)) {
        // Look up the latest submitted title of this particular webform.
        $query = "SELECT title FROM {referralsources_submissions} WHERE rstid = '%s' AND nid = %d ORDER BY created_datetime DESC";
        $title = db_result(db_query_range($query, array(REFERRALSOURCE_SUBMISSION_TYPE_WEBFORM, $webform->nid), 0, 1));

        // Create array of distinct nids that have webform submissions, with the latest title for each.
        $webforms[$webform->nid] = check_plain($title);
      }
      // Display stats for each webform
      foreach ($webforms as $nid => $title) {
        $form[$rstid][$nid] = array(
          '#type' => 'fieldset',
          '#title' => (!empty($title) ? $title : t('Title missing for Webform node @nid', array('@nid' => $nid))),
          '#description' => t('Submissions collected through @webform webform, node @nid.', array('@webform' => (!empty($title) ? $title : t('untitled')), '@nid' => $nid)),
          '#collapsible' => TRUE,
          '#collapsed' => TRUE,
        );
        $form[$rstid][$nid]['selections'] = array(
          '#type' => 'markup',
          '#value' => referralsources_statistics_selections($rstid, $submission_type, NULL, $nid),
        );
        $form[$rstid][$nid]['other'] = array(
          '#type' => 'markup',
          '#value' => referralsources_statistics_other($rstid, $submission_type, NULL, $nid),
        );
      }
    }
    elseif ($rstid == REFERRALSOURCE_SUBMISSION_TYPE_FAPI) {
      // Loop through generic FAPI forms with referral source submissions, display stats for each
      $result = db_query("SELECT DISTINCT form_id FROM {referralsources_submissions} WHERE rstid = '%s'", $rstid);
      while ($submission_form = db_fetch_object($result)) {
        $form_id = check_plain($submission_form->form_id);
        $form[$rstid][$form_id] = array(
          '#type' => 'fieldset',
          '#title' => $form_id,
          '#description' => t('Submissions collected through @form form.', array('@form' => $form_id)),
          '#collapsible' => TRUE,
          '#collapsed' => TRUE,
        );
        $form[$rstid][$form_id]['selections'] = array(
          '#type' => 'markup',
          '#value' => referralsources_statistics_selections($rstid, $submission_type, $form_id, NULL),
        );
        $form[$rstid][$form_id]['other'] = array(
          '#type' => 'markup',
          '#value' => referralsources_statistics_other($rstid, $submission_type, $form_id, NULL),
        );
      }
    }
  }

  return $form;
}

/**
 * Create table of statistics for selected referral sources.
 */
function referralsources_statistics_selections($rstid = NULL, $submission_type = NULL, $form_id = NULL, $webform_nid = NULL) {
  $output = '';
  $rows = array();

  // Build referral source selections table
  $header = array(
      array('data' => t('Referral source'), 'field' => 'title'),
      array('data' => t('Count'), 'field' => 'count_rid', 'sort' => 'desc'),
  );

  $query = "SELECT r.title, COUNT(rs.rid) AS count_rid "
          ."FROM {referralsources_submissions} rs INNER JOIN {referralsources} r ON rs.rid = r.rid "
          ."WHERE rs.rid > 0 ". (!empty($rstid) ? "AND rstid = '%s' " : '')
          . (!empty($form_id) ? "AND form_id = '%s' " : '')
          . (!empty($webform_nid) ? "AND nid = %d " : '')
          ."GROUP BY rs.rid ". tablesort_sql($header);

  $args = array();
  if (!empty($rstid)) $args[] = $rstid;
  if (!empty($form_id)) $args[] = $form_id;
  if (!empty($webform_nid)) $args[] = $webform_nid;

  $result = db_query($query, $args);
  while ($data = db_fetch_object($result)) {
    $row = array();
    $row[] = check_plain($data->title);
    $row[] = $data->count_rid;
    $rows[] = array('data' => $row);
  }

  $output .= '<h3>'. t('Referral source selections:') .'</h3>';
  if (count($rows) > 0) {
    // Rows, display table of referral source selections
    $output .= theme('table', $header, $rows);
  }
  else {
    // No rows, display message instead of empty table
    $output .= '<p>'. t('There are no referral source selections on record.') .'</p>';
  }

  return $output;
}

/**
 * Create table of statistics for other referral sources specified.
 */
function referralsources_statistics_other($rstid = NULL, $submission_type = NULL, $form_id = NULL, $webform_nid = NULL) {
  $output = '';
  $rows = array();

  // Build referral source other text table
  $header = array(
    array('data' => t('Other'), 'field' => 'other'),
    array('data' => t('Count'), 'field' => 'count_other', 'sort' => 'desc'),
  );

  $query = "SELECT other, COUNT(other) AS count_other FROM {referralsources_submissions} rs "
          ."WHERE LTRIM(RTRIM(rs.other)) <> '' ". (!empty($rstid) ? "AND rstid = '%s' " : '')
          . (!empty($form_id) ? "AND form_id = '%s' " : '')
          . (!empty($webform_nid) ? "AND nid = %d " : '')
          ."GROUP BY other " . tablesort_sql($header);

  $args = array();
  if (!empty($rstid)) $args[] = $rstid;
  if (!empty($form_id)) $args[] = $form_id;
  if (!empty($webform_nid)) $args[] = $webform_nid;

  $result = db_query($query, $args);
  while ($data = db_fetch_object($result)) {
    $row = array();
    $row[] = check_plain($data->other);
    $row[] = $data->count_other;
    $rows[] = array('data' => $row);
  }

  $output .= '<h3>'. t('Other referral sources specified:') .'</h3>';
  if (count($rows) > 0) {
    // Rows, display table of other referral sources specified
    $output .= theme('table', $header, $rows);
  }
  else {
    $output .= '<p>'. t('There have been no other referral sources specified.'). '</p>';
  }

  return $output;
}

/**
 * Submit handler for referral source admin settings form
 */
function referralsources_admin_settings_form_submit($form, &$form_state) {
  variable_set('referralsource_fieldset_title', $form_state['values']['referralsource_fieldset_title']);
  variable_set('referralsource_fieldset_description', $form_state['values']['referralsource_fieldset_description']);
  variable_set('referralsource_select_label', $form_state['values']['referralsource_select_label']);
  variable_set('referralsource_other_label', $form_state['values']['referralsource_other_label']);
  variable_set('referralsource_register_enable', $form_state['values']['referralsource_register_enable']);
  variable_set('referralsource_register_weight', $form_state['values']['referralsource_register_weight']);
  variable_set('referralsource_group_select_list', $form_state['values']['referralsource_group_select_list']);
}

/**
 * Submit handler for Save button on referral source table form
 */
function referralsources_admin_table_form_save($form, &$form_state) {
  foreach ($form_state['values']['weight'] as $k => $v) {
    db_query("UPDATE {referralsources} SET weight = %d WHERE rid = %d", $v, $k);
  }
  drupal_set_message("Referral source sort order saved.");
}

/**
 * Submit handler for Save button on referral source add/edit form
 */
function referralsources_form_save($form, &$form_state) {
  db_query("UPDATE {referralsources} SET code = '%s', title = '%s', grouping = '%s', description = '%s', weight = %d WHERE rid = %d",
    $form_state['values']['code'], $form_state['values']['title'], $form_state['values']['grouping'], $form_state['values']['description'], $form_state['values']['weight'], $form_state['values']['rid']);
  if (db_affected_rows() == 0) {
    db_query("INSERT INTO {referralsources} (code, title, grouping, description, weight) VALUES ('%s', '%s', '%s', '%s', %d)",
      $form_state['values']['code'], $form_state['values']['title'], $form_state['values']['grouping'], $form_state['values']['description'], $form_state['values']['weight']);
    // New referral source added, stay on Add page for convenience when adding multiple referral sources.
    drupal_set_message('New referral source '. $form_state['values']['code'] .' added.');
    $form_state['redirect'] = 'admin/content/referralsources/add';
  }
  else {
    // Updated existing referral source.
    drupal_set_message('Referral source '. $form_state['values']['code'] .' updated.');
    $form_state['redirect'] = 'admin/content/referralsources/list';
  }
}

/**
 * Submit handler for Delete button on referral source delete confirmation form
 */
function referralsources_admin_delete($form, &$form_state) {
  db_query("DELETE FROM {referralsources} WHERE rid = %d", $form_state['values']['rid']);
  drupal_set_message('Referral source '. $form_state['values']['code'] .' has been deleted.');
  $form_state['redirect'] = 'admin/content/referralsources/list';
}

/**
 * Theme function for displaying referral sources table in admin
 */
function theme_referralsources_admin_table_form($form) {
  foreach (element_children($form['code']) as $key) {
    $form['weight'][$key]['#attributes']['class'] = 'referral-source-weight';

    $row = array();
    $row[] = drupal_render($form['code'][$key]);
    $row[] = drupal_render($form['title'][$key]);
    $row[] = drupal_render($form['description'][$key]);
    $row[] = drupal_render($form['weight'][$key]);
    $row[] = drupal_render($form['edit'][$key]);
    $row[] = drupal_render($form['delete'][$key]);
    $rows[] = array('data' => $row, 'class' => 'draggable');
  }

  if (is_array($rows)) {
    // Rows, display table of referral sources
    $output = theme('table', $form['header']['#value'], $rows, array('id' => 'referral-source-table'));
    $output .= drupal_render($form);

    drupal_add_tabledrag('referral-source-table', 'order', 'sibling', 'referral-source-weight', NULL, NULL, TRUE);
  }
  else {
    // No rows, display message instead of empty table
    $output = '<p>'. t('There are currently no referral sources set up.') .'</p>';
    $output .= '<p>'. l('Click here to add a referral source.', 'admin/content/referralsources/add') .'</p>';
  }

  return $output;
}
