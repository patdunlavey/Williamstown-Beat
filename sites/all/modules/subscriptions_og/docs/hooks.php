<?php
// $Id: hooks.php,v 1.1.2.6 2008/03/15 17:22:01 develCuy Exp $

/**
 * @file
 * These are the hooks that are invoked by the Subscription module.
 *
 * Hooks are typically called in all modules at once using module_invoke_all().
 */

/**
 * @defgroup hooks Hooks
 * @{
 * Allow modules to interact with the Drupal core.
 *
 * Please refer to @link http://api.drupal.org/api/group/hooks/5 online help @endlink.
 */
/**
 * @} End of "defgroup hooks".
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Declare a subscription event handler and plugin hook dispatcher.
 *
 * This hook is called by subscriptions module to retrieve parameters for
 * events. Parameter $op is used to identify the event, the other three
 * arguments have dependency on $op (detailed below).
 *
 * It is also used to call _hook_$op in which case returns what such hook
 * provides. Note that those hooks have prefix: '_'.
 *
 * @param $op
 *   What kind of information to retrieve about the event.
 *   Possible values:
 *   - 'queue': Occurs when a notification event is going to be added.
 *   - 'fields': Occurs when notification emails are generated.
 *   - 'stype': Occurs when a subscription is added.
 *   - 'stypes': Alternative call to retrieve all types a plugin may have.
 *   - 'node_options', 'types': redirects arguments to _hook_$op.
 * @param $arg0
 *   If $op equals 'queue' then must be an array of the form:
 *   array(
 *     'module' => 'node' | 'comment',
 *     'node' => (node object),
 *     'type' => 'node' | 'comment',
 *     'action' => 'update', // Used with 'type' to filter by send_updates
 *   );
 *   If $op equals 'fields' must be 'node' or 'comment'.
 *   If $op equals 'stype' must be an array key of $stypes.
 *   If $op equals 'stypes' it is not used.
 * @param $arg1
 *   Used only if $op equals 'stype' to be included in return array.
 * @param $arg2
 *   Used only if $op equals 'stype' to be included in return array.
 *
 * @return
 *   If $op equals 'queue' returns the array structure of the join part of
 *   subscriptions queue query.
 *   If $op equals 'fields' returns the array structure to call mailvars
 *   handler.
 *   If $op equals 'stype' returns corresponding key $stypes.
 *   If $op equals 'stypes' returns $stypes.
 */
function hook_subscriptions($op, $arg0 = NULL, $arg1 = NULL, $arg2 = NULL) {
  static $stypes = array('og' => array('node', 'group_nid'));
  $function = '_subscriptions_'. $op;
  // Hook dispatcher, must be copied as is.
  if (function_exists($function)) {
    return $function($arg0, $arg1, $arg2);
  }
  // Event handling
  switch ($op) {
    case 'queue':
      // $arg0 is $event array.
      if ($arg0['module'] == 'node') {
        $node = $arg0['node'];
        $params['node']['group_nid'] = array(
          'join' => 'INNER JOIN {og_ancestry} a ON s.value = a.group_nid',
          'where' => 'a.nid = %d',
          'args' => array($node->nid),
        );
        if ($arg0['type'] == 'comment') {
          $where = ' AND s.send_comments = 1';
        }
        elseif ($arg0['type'] == 'node' && $arg0['action'] == 'update') {
          $where  = ' AND s.send_updates = 1';
        }
        if (isset($where)) {
          $params['node']['group_nid']['where'] .= $where;
        }
        return $params;
      }
      break;
    case 'fields': // Parameter is module
      if ($arg0 == 'node' || $arg0 == 'comment') {
        $tr = 't';
        return array(
          'group_nid' => array(
            'mailvars_function' => '_subscriptions_content_node_mailvars',
            '!type' => $tr('category'),
          ),
        );
      }
      break;
    case 'stypes':
      return $stypes;
    case 'stype':
      return (isset($stypes[$arg0]) ? array_merge( $stypes[$arg0], array($arg1, $arg2)) : NULL);
  }
}

/**
 * Constructs node options to be displayed in the node subscriptions controls.
 *
 * This hook is called by subscriptions_ui module to retrieve parameters for
 * node form (a.k.a subscriptions controls). Subscriptions_OG module provides a
 * convenient function to construct a valid array structure, it is
 * _subscriptions_append_node_option().
 *
 * @param $account
 *   User account to handle permissions if needed.
 * @param $node
 *   A valid node object which let identify the kind of parameters to return.
 *
 * @return
 *   Array structure of node form options.
 *
 * @ingroup form
 *
 * @see _subscriptions_append_node_option()
 */
function _hook_node_options($account, $node) {
  $options = array();
  $omits   = variable_get('subscriptions_omitted_og', array());
    
  if (!isset($node->og_description)) {
    // Organic Groups node
    $_node_groups = og_get_node_groups($node);
    if (count($_node_groups)) {
      // Fetch group nodes
      $_query = "
  SELECT
    *
  FROM
    {node} n
  WHERE
    nid IN(". str_repeat("%d,", count($_node_groups)-1) ."%d)
      ";
      $_rs = call_user_func_array('db_query', array_merge(array($_query), array_keys($_node_groups)));
      $_groups_types = array();
      while ($_group = db_fetch_object($_rs)) {
        $_groups_types[$_group->nid] = $_group->type;
      }
      foreach ($_node_groups as $_group_nid => $_label) {
        if (!in_array($_groups_types[$_group_nid], $omits)) {
        _module_append_node_option($options, t('To group: ') . $_label, $_group_nid);
        }
      }
    }
  }
  else {
    if (!in_array($node->type, $omits)) {
      // Organic Groups "group" node
      _module_append_node_option($options, t('To this group'), $node->nid);
    }
  }
  // It allows our options to be displayed at the very first of node form.
  $options['group_nid']['weight'] = -5;
  return $options;
}

/**
 * Returns an array with information of types for subscriptions module
 * user interface.
 *
 * This is called by subscriptions module to construct user and admin interface
 * for managing subscriptions and defaults. This is also used to label items in
 * user overview page.
 *
 * It have not parameters.
 *
 * @return
 *   A valid subscription "types" array structure.
 *
 * @ingroup form
 *
 * @see subscriptions_types()
 */
function _hook_types() {
  $tr = 't';
  $types['group'] = array(
    'title' => $tr('Groups'),
    'access' => $tr('subscribe to organic groups'),
    'page' => 'module_page_group',
    'fields' => array('group', 'group_nid'),
    'weight' => -5,
  );
  return $types;
}

/**
 * Declare a subscription plugin count handler.
 *
 * When subscriptions module generates page user overview, it calls every
 * plug-in available to provide the amount of subscriptions a user have. Note
 * that this implementation uses _module_types() in order of construct
 * resulting array.
 *
 * @param $count
 *   It is the array where we can store our count. WARNING! must not be
 *   overriten, but only inserted with your plugin array key.
 * @param $uid
 *   User account ID to filter subscriptions used to count.
 *
 * @return
 *   Array with count of current plugin subscriptions.
 *
 * @see subscriptions_page_user_overview()
 */
function hook_count_user_subscriptions($count, $uid) {
  $type   = _module_types();
  $fields = $type['group']['fields'];
  $_query = "
SELECT count(*)
FROM {subscriptions}
WHERE
  module = 'node' AND
  field = 'group_nid' AND
  recipient_uid = %d
  ";
  $count[$fields[0]][$fields[1]] = db_result(db_query($_query, $uid));
  return $count;
}


/**
 * Declare a Organic Groups event handler.
 *
 * When a user is inserted in a group OG module invokes this hook to let other
 * modules take some action according to $op. In this particular case,
 * Subscriptons OG module validate settings before auto-subscribe the user
 * envolved in this event.
 *
 * @param $op
 *   What kind of event happens.
 *   Posible values: 'user insert' and 'user update'.
 * @param $gid
 *   The Group ID is the Node ID associated to a content type configured to be
 *   group (a.k.a group_nid or gid).
 * @param $uid
 *   User account ID who is triggers the event.
 * @param $args
 *   Addional parameters inherited from og_save_subscription().
 *
 * @see og_save_subscription()
 */
function hook_og($op, $gid, $uid, $args) {
  if ($op == 'user insert') {
    $_args = func_get_args();
    global $_module_history;
    global $_module_history_values;
    $_module_history[] = 'module_og';
    
    $_no_autosubs = FALSE;
    
    // If user is subscribed to group from og_confirm_subscribe.
    if (isset(
      $_module_history_values['module_og_confirm_subscribe_submit']
    )) {
      // Recover needed data
      $_form_values =& $_module_history_values['module_og_confirm_subscribe_submit'];
      $_params =& $_form_values['params'];
      $_subscriptions =& $_form_values['subscriptions'];
      
      for ($_i = 0; $_i < count($_params); $_i++) {
        if (
          $_params[$_i+1]['field'] == 'group_nid' &&
          !$_subscriptions[$_i+1]
        ) {
          $_no_autosubs = TRUE;
        }
      }
    }
    
    if (
      variable_get('subscriptions_autosubs_og', 0) &&
      user_access('subscribe to organic groups') &&
      !$_no_autosubs
    ) {
      subscriptions_autosubscribe('node', 'group_nid', $gid, 'on_post');
    }
  }
}

/**
 * Install the current version of the database schema. (Copied from api.drupal.org).
 *
 * The hook will be called the first time a module is installed, and the
 * module's schema version will be set to the module's greatest numbered update
 * hook. Because of this, anytime a hook_update_N() is added to the module, this
 * function needs to be updated to reflect the current version of the database
 * schema.
 *
 * Table names in the CREATE queries should be wrapped with curly braces so that
 * they're prefixed correctly, see db_prefix_tables() for more on this.
 *
 * Note that since this function is called from a full bootstrap, all functions
 * (including those in modules enabled by the current page request) are
 * available when this hook is called. Use cases could be displaying a user
 * message, or calling a module function necessary for initial setup, etc.
 */
function hook_install() {
  switch ($GLOBALS['db_type']) {
    case 'mysql':
    case 'mysqli':
      db_query("CREATE TABLE {event} (
                  nid int(10) unsigned NOT NULL default '0',
                  event_start int(10) unsigned NOT NULL default '0',
                  event_end int(10) unsigned NOT NULL default '0',
                  timezone int(10) NOT NULL default '0',
                  PRIMARY KEY (nid),
                  KEY event_start (event_start)
                ) TYPE=MyISAM /*!40100 DEFAULT CHARACTER SET utf8 */;"
      );
      break;

    case 'pgsql':
      db_query("CREATE TABLE {event} (
                  nid int NOT NULL default '0',
                  event_start int NOT NULL default '0',
                  event_end int NOT NULL default '0',
                  timezone int NOT NULL default '0',
                  PRIMARY KEY (nid)
                );"
      );
      break;
  }
}

/**
 * Remove any tables or variables that the module sets. (Copied from api.drupal.org).
 *
 * The uninstall hook will fire when the module gets uninstalled.
 */
function hook_uninstall() {
  db_query('DROP TABLE {profile_fields}');
  db_query('DROP TABLE {profile_values}');
  variable_del('profile_block_author_fields');
}

/**
 * Perform necessary actions before module is disabled. (Copied from api.drupal.org).
 *
 * The hook is called everytime module is disabled.
 */
function hook_disable() {
  mymodule_cache_rebuild();
}

/**
 * Perform necessary actions before module is disabled. (Copied from
 * api.drupal.org).
 *
 * The hook is called everytime module is disabled.
 */
function hook_disable() {
  mymodule_cache_rebuild();
}

/**
 * Act on nodes defined by other modules.
 *
 * Despite what its name might make you think, hook_nodeapi() is not
 * reserved for node modules. On the contrary, it allows modules to react
 * to actions affecting all kinds of nodes, regardless of whether that
 * module defined the node.
 *
 * @param &$node
 *   The node the action is being performed on.
 * @param $op
 *   What kind of action is being performed. Possible values:
 *   - "delete": The node is being deleted.
 *   - "delete revision": The revision of the node is deleted. You can delete data
 *     associated with that revision.
 *   - "insert": The node is being created (inserted in the database).
 *   - "load": The node is about to be loaded from the database. This hook
 *     can be used to load additional data at this time.
 *   - "prepare": The node is about to be shown on the add/edit form.
 *   - "search result": The node is displayed as a search result. If you
 *     want to display extra information with the result, return it.
 *   - "print": Prepare a node view for printing. Used for printer-friendly
 *     view in book_module
 *   - "update": The node is being updated.
 *   - "submit": The node passed validation and will soon be saved. Modules may
 *      use this to make changes to the node before it is saved to the database.
 *   - "update index": The node is being indexed. If you want additional
 *     information to be indexed which is not already visible through
 *     nodeapi "view", then you should return it here.
 *   - "validate": The user has just finished editing the node and is
 *     trying to preview or submit it. This hook can be used to check
 *     the node data. Errors should be set with form_set_error().
 *   - "view": The node content is being assembled before rendering. The module
 *     may add elements $node->content prior to rendering. This hook will be
 *     called after hook_view().  The format of $node->content is the same as
 *     used by Forms API.
 *   - "alter": the $node->content array has been rendered, so the node body or
 *     teaser is filtered and now contains HTML. This op should only be used when
 *     text substitution, filtering, or other raw text operations are necessary.
 *   - "rss item": An RSS feed is generated. The module can return properties
 *     to be added to the RSS item generated for this node. See comment_nodeapi()
 *     and upload_nodeapi() for examples. The $node passed can also be modified
 *     to add or remove contents to the feed item.
 * @param $a3
 *   - For "view", passes in the $teaser parameter from node_view().
 *   - For "validate", passes in the $form parameter from node_validate().
 * @param $a4
 *   - For "view", passes in the $page parameter from node_view().
 * @return
 *   This varies depending on the operation.
 *   - The "submit", "insert", "update", "delete", "print' and "view"
 *     operations have no return value.
 *   - The "load" operation should return an array containing pairs
 *     of fields => values to be merged into the node object.
 *
 * If you are writing a node module, do not use this hook to perform
 * actions on your type of node alone. Instead, use the hooks set aside
 * for node modules, such as hook_insert() and hook_form().
 */
function hook_nodeapi(&$node, $op, $a3 = NULL, $a4 = NULL) {
  switch ($op) {
    case 'submit':
      if ($node->nid && $node->moderate) {
        // Reset votes when node is updated:
        $node->score = 0;
        $node->users = '';
        $node->votes = 0;
      }
      break;
    case 'insert':
    case 'update':
      if ($node->moderate && user_access('access submission queue')) {
        drupal_set_message(t('The post is queued for approval'));
      }
      elseif ($node->moderate) {
        drupal_set_message(t('The post is queued for approval. The editors will decide whether it should be published.'));
      }
      break;
    case 'view':
      $node->content['my_additional_field'] = array(
        '#value' => theme('mymodule_my_additional_field', $additional_field),
        '#weight' => 10,
      );
      break;
  }
}

/**
 * Perform alterations before a form is rendered. (Copied from api.drupal.org).
 *
 * One popular use of this hook is to add form elements to the node form. When
 * altering a node form, the node object retrieved at from $form['#node'].
 *
 * @param $form_id
 *   String representing the name of the form itself. Typically this is the
 *   name of the function that generated the form.
 * @param $form
 *   Nested array of form elements that comprise the form.
 * @return
 *   None.
 */
function hook_form_alter($form_id, &$form) {
  if (isset($form['type']) && $form['type']['#value'] .'_node_settings' == $form_id) {
    $form['workflow']['upload_'. $form['type']['#value']] = array(
      '#type' => 'radios',
      '#title' => t('Attachments'),
      '#default_value' => variable_get('upload_'. $form['type']['#value'], 1),
      '#options' => array(t('Disabled'), t('Enabled')),
    );
  }
}

/**
 * Define menu items and page callbacks. (Copied from api.drupal.org).
 *
 * This hook enables modules to register paths, which determines whose
 * requests are to be handled. Depending on the type of registration
 * requested by each path, a link is placed in the the navigation block and/or
 * an item appears in the menu administration page (q=admin/menu).
 *
 * Drupal will call this hook twice: once with $may_cache set to TRUE, and once
 * with it set to FALSE. Therefore, each menu item should be registered when
 * $may_cache is either TRUE or FALSE, not both times. Setting a menu item
 * twice will result in unspecified behavior.
 *
 * This hook is also a good place to put code which should run exactly once
 * per page view. Put it in an if (!may_cache) block.
 *
 * @param $may_cache
 *   A boolean indicating whether cacheable menu items should be returned. The
 *   menu cache is per-user, so items can be cached so long as they are not
 *   dependent on the user's current location. See the local task definitions
 *   in node_menu() for an example of uncacheable menu items.
 * @return
 *   An array of menu items. Each menu item is an associative array that may
 *   contain the following key-value pairs:
 *   - "path": Required. The path to link to when the user selects the item.
 *   - "title": Required. The translated title of the menu item.
 *   - "callback": The function to call to display a web page when the user
 *     visits the path. If omitted, the parent menu item's callback will be used
 *     instead.
 *   - "callback arguments": An array of arguments to pass to the callback
 *     function.
 *   - "access": A boolean value that determines whether the user has access
 *     rights to this menu item. Usually determined by a call to user_access().
 *     If omitted and "callback" is also absent, the access rights of the parent
 *     menu item will be used instead.
 *   - "weight": An integer that determines relative position of items in the
 *     menu; higher-weighted items sink. Defaults to 0. When in doubt, leave
 *     this alone; the default alphabetical order is usually best.
 *   - "type": A bitmask of flags describing properties of the menu item.
 *     Many shortcut bitmasks are provided as constants in menu.inc:
 *     - MENU_NORMAL_ITEM: Normal menu items show up in the menu tree and can be
 *       moved/hidden by the administrator.
 *     - MENU_ITEM_GROUPING: Item groupings are used for pages like "node/add"
 *       that simply list subpages to visit.
 *     - MENU_CALLBACK: Callbacks simply register a path so that the correct
 *       function is fired when the URL is accessed.
 *     - MENU_DYNAMIC_ITEM: Dynamic menu items change frequently, and so should
 *       not be stored in the database for administrative customization.
 *     - MENU_SUGGESTED_ITEM: Modules may "suggest" menu items that the
 *       administrator may enable.
 *     - MENU_LOCAL_TASK: Local tasks are rendered as tabs by default.
 *     - MENU_DEFAULT_LOCAL_TASK: Every set of local tasks should provide one
 *       "default" task, that links to the same path as its parent when clicked.
 *     If the "type" key is omitted, MENU_NORMAL_ITEM is assumed.
 *
 * For a detailed usage example, see page_example.module.
 *
 */
function hook_menu($may_cache) {
  global $user;
  $items = array();

  if ($may_cache) {
    $items[] = array('path' => 'node/add/blog', 'title' => t('blog entry'),
      'access' => user_access('maintain personal blog'));
    $items[] = array('path' => 'blog', 'title' => t('blogs'),
      'callback' => 'blog_page',
      'access' => user_access('access content'),
      'type' => MENU_SUGGESTED_ITEM);
    $items[] = array('path' => 'blog/'. $user->uid, 'title' => t('my blog'),
      'access' => user_access('maintain personal blog'),
      'type' => MENU_DYNAMIC_ITEM);
    $items[] = array('path' => 'blog/feed', 'title' => t('RSS feed'),
      'callback' => 'blog_feed',
      'access' => user_access('access content'),
      'type' => MENU_CALLBACK);
  }
  return $items;
}

/**
 * Provide mailkeys definitions.
 *
 * The mailkeys hook will fire when mail_edit constructs overview of mail
 * templates.
 *
 * @return
 *   A valid array of mailkeys.
 *
 * @see mail_edit_overview()
 */
function hook_mailkeys() {
  return array('module-key1', 'module-key2');
}

/**
 * Given a node object prevents or not OG module sending notifications. (Based
 * on OG documentation).
 *
 * Subscriptions_OG module returns TRUE without retrictions in order of let
 * Subscriptions module take over control of all notifications.
 *
 * @param $node
 *   Must be a valid $node object.
 *
 * @return
 *   Any module that returns TRUE from its hook_og_notify($node) will prevent
 *   sending notifications.
 */
function hook_og_notify($node) {
  return TRUE;
}

/**
 * Called just before a query begins to be built for every view. (Copied from
 * Views 1.x module developer API)
 *
 * This is called even if the query is cached. If the query is cached, it will
 * be available on $view->query.
 *
 * @param &$view
 *   The view the query is being run for.
 *
 * @return
 *   A string that will be prepended to the view.
 *
 * @see views_build_view()
 */
function hook_views_pre_query(&$view) {
  if ($view->name == 'my_view') {
    // Override view my_view's some field
    $key = 0;
    $view->field[$key] = array(
      'tablename' => 'mytable',
      'field' => 'myfield',
      'label' => $view->field[3]['label'],
      'fullname' => 'mytable.myfield',
      'id' => 'mytable.recipient_uid',
      'queryname' => 'mytable_myfield',
    );
  }
  
}

/**
 * Returns an array of table objects. (Adapted from Views 1.x module developer
 * API).
 *
 * The keys to the array are the names of the tables, and each one must be unique.
 *
 * @return
 *   An array of table objects.
 *
 * @see _views_get_tables()
 */
function hook_views_tables() {
  $table = array(
    'name' => 'example_table_a',
    //more...
  );
  $tables["example_table_a"] = $table;

  $table = array(
    'name' => 'example_table_b',
    //more...
  );
  $tables["example_table_b"] = $table;
  return $tables;
}
/**
 * @} End of "addtogroup hooks".
 */
