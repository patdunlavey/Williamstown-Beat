<?php
// $Id: FeedsNodeProcessor.inc,v 1.26 2010/04/12 00:30:33 alexb Exp $

/**
 * @file
 * Class definition of FeedsNodeProcessor.
 */

// Create or delete FEEDS_NODE_BATCH_SIZE at a time.
define('FEEDS_NODE_BATCH_SIZE', 50);

/**
 * Creates nodes from feed items.
 */
class FeedsNodeProcessor extends FeedsProcessor {

  /**
   * Implementation of FeedsProcessor::process().
   */
  public function process(FeedsImportBatch $batch, FeedsSource $source) {

    // Keep track of processed items in this pass.
    $processed = 0;

    while ($item = $batch->shiftItem()) {

      // Create/update if item does not exist or update existing is enabled.
      if (!($nid = $this->existingItemId($item, $source)) || $this->config['update_existing']) {

        $node = new stdClass();
        $hash = $this->hash($item);

        if (!empty($nid)) {

          // If hash of this item is same as existing hash there is no actual
          // change, skip.
          if ($hash == $this->getHash($nid)) {
            continue;
          }

          // If updating populate nid and vid avoiding an expensive node_load().
          $node->nid = $nid;
          $node->vid = db_result(db_query("SELECT vid FROM {node} WHERE nid = %d", $nid));
          $batch->updated++;
        }
        else {
          $batch->created++;
        }

        // Populate and prepare node object.
        $node->type = $this->config['content_type'];
        $node->created = FEEDS_REQUEST_TIME;
        $node->feeds_node_item = new stdClass();
        $node->feeds_node_item->hash = $hash;
        $node->feeds_node_item->id = $this->id;
        $node->feeds_node_item->imported = FEEDS_REQUEST_TIME;
        $node->feeds_node_item->feed_nid = $source->feed_nid;
        static $included;
        if (!$included) {
          module_load_include('inc', 'node', 'node.pages');
          $included = TRUE;
        }
        node_object_prepare($node);

        // Populate properties that are set by node_object_prepare().
        $node->log = 'Created/updated by FeedsNodeProcessor';
        $node->uid = 0;

        // Execute mappings from $item to $node.
        $this->map($item, $node);

        // Add attributes inherited from the source feed node to the node.
        if ($source->feed_nid) {
          $feed_node = node_load($source->feed_nid);
          $this->inherit($feed_node, $node);
        }

        // Save the node.
        node_save($node);
      }

      $processed++;
      if ($processed >= variable_get('feeds_node_batch_size', FEEDS_NODE_BATCH_SIZE)) {
        return (1.0 / ($batch->total + 1)) * ($batch->updated + $batch->created); // Add + 1 to make sure that result is not 1.0 = finished.
      }
    }

    // Set messages.
    if ($batch->created) {
      drupal_set_message(t('Created !number !type nodes.', array('!number' => $batch->created, '!type' => node_get_types('name', $this->config['content_type']))));
    }
    elseif ($batch->updated) {
      drupal_set_message(t('Updated !number !type nodes.', array('!number' => $batch->updated, '!type' => node_get_types('name', $this->config['content_type']))));
    }
    else {
      drupal_set_message(t('There is no new content.'));
    }

    return FEEDS_BATCH_COMPLETE;
  }

  /**
   * Implementation of FeedsProcessor::clear().
   */
  public function clear(FeedsBatch $batch, FeedsSource $source) {
    if (empty($batch->total)) {
      $batch->total = db_result(db_query("SELECT COUNT(nid) FROM {feeds_node_item} WHERE id = '%s' AND feed_nid = %d", $source->id, $source->feed_nid));
    }
    $result = db_query_range("SELECT nid FROM {feeds_node_item} WHERE id = '%s' AND feed_nid = %d", $source->id, $source->feed_nid, 0, variable_get('feeds_node_batch_size', FEEDS_NODE_BATCH_SIZE));
    while ($node = db_fetch_object($result)) {
      _feeds_node_delete($node->nid);
      $batch->deleted++;
    }
    if (db_result(db_query_range("SELECT nid FROM {feeds_node_item} WHERE id = '%s' AND feed_nid = %d", $source->id, $source->feed_nid, 0, 1))) {
      return (1.0 / ($batch->total + 1)) * $batch->deleted;
    }

    // Set message.
    drupal_get_messages('status');
    if ($batch->deleted) {
      drupal_set_message(t('Deleted !number nodes.', array('!number' => $batch->deleted)));
    }
    else {
      drupal_set_message(t('There is no content to be deleted.'));
    }
    return FEEDS_BATCH_COMPLETE;
  }

  /**
   * Implement expire().
   */
  public function expire($time = NULL) {
    if ($time === NULL) {
      $time = $this->expiryTime();
    }
    if ($time == FEEDS_EXPIRE_NEVER) {
      return;
    }
    $result = db_query_range('SELECT n.nid FROM {node} n JOIN {feeds_node_item} fni ON n.nid = fni.nid WHERE fni.id = "%s" AND n.created < %d', $this->id, FEEDS_REQUEST_TIME - $time, 0, variable_get('feeds_node_batch_size', FEEDS_NODE_BATCH_SIZE));
    while ($node = db_fetch_object($result)) {
      _feeds_node_delete($node->nid);
    }
    if (db_result(db_query_range('SELECT n.nid FROM {node} n JOIN {feeds_node_item} fni ON n.nid = fni.nid WHERE fni.id = "%s" AND n.created < %d', $this->id, FEEDS_REQUEST_TIME - $time, 0, 1))) {
      return FEEDS_BATCH_ACTIVE;
    }
    return FEEDS_BATCH_COMPLETE;
  }

  /**
   * Override parent::map() to load all available add-on mappers.
   */
  protected function map($source_item, $target_node) {
    self::loadMappers();
    return parent::map($source_item, $target_node);
  }

  /**
   * Return expiry time.
   */
  public function expiryTime() {
    return $this->config['expire'];
  }

  /**
   * Override parent::configDefaults().
   */
  public function configDefaults() {
    $types = node_get_types('names');
    $type = isset($types['story']) ? 'story' : key($types);
    return array(
      'content_type' => $type,
      'update_existing' => 0,
      'expire' => FEEDS_EXPIRE_NEVER,
      'mappings' => array(),
      'inherit' => array(),
    );
  }

  /**
   * Override parent::configForm().
   */
  public function configForm(&$form_state) {
    $types = node_get_types('names');
    $form = array();
    $form['content_type'] = array(
      '#type' => 'select',
      '#title' => t('Content type'),
      '#description' => t('Choose node type to create from this feed. <strong>Note:</strong> Users with "import !feed_id feeds" permissions will be able to <strong>import</strong> nodes of the content type selected here regardless of the node level permissions. Further, users with "clear !feed_id permissions" will be able to <strong>delete</strong> imported nodes regardless of their node level permissions.', array('!feed_id' => $this->id)),
      '#options' => $types,
      '#default_value' => $this->config['content_type'],
    );
    $form['update_existing'] = array(
      '#type' => 'checkbox',
      '#title' => t('Update existing items'),
      '#description' => t('Check if existing items should be updated from the feed.'),
      '#default_value' => $this->config['update_existing'],
    );
    $period = drupal_map_assoc(array(FEEDS_EXPIRE_NEVER, 3600, 10800, 21600, 43200, 86400, 259200, 604800, 604800 * 4, 604800 * 12, 604800 * 24, 31536000), 'feeds_format_expire');
    $form['expire'] = array(
      '#type' => 'select',
      '#title' => t('Expire nodes'),
      '#options' => $period,
      '#description' => t('Select after how much time nodes should be deleted. The node\'s published date will be used for determining the node\'s age, see Mapping settings.'),
      '#default_value' => $this->config['expire'],
    );
    $disabled = !feeds_importer($this->id)->config['content_type'];
    $form['inherit'] = array(
      '#type' => 'fieldset',
      '#title' => t('Inherit from feed node'),
      '#description' => $disabled ? t('Only available for importers that are attached to a content type (see Basic settings).') : t('Copy one or more properties from the feed node to feed item nodes created by this importer.'),
      '#tree' => TRUE,
    );
    foreach ($this->getInheritors() as $key => $info) {
      $form['inherit'][$key] = array(
       '#type' => 'checkbox',
       '#title' => $info['name'],
       '#description' => $info['description'],
       '#default_value' => $disabled ? 0 : $this->config['inherit'][$key],
       '#disabled' => $disabled,
     );
    }
    return $form;
  }

  /**
   * Override setTargetElement to operate on a target item that is a node.
   */
  public function setTargetElement($target_node, $target_element, $value) {
    if (in_array($target_element, array('url', 'guid'))) {
      $target_node->feeds_node_item->$target_element = $value;
    }
    elseif ($target_element == 'body') {
      $target_node->teaser = $value;
      $target_node->body = $value;
    }
    elseif (in_array($target_element, array('title', 'status', 'created'))) {
      $target_node->$target_element = $value;
    }
  }

  /**
   * Return available mapping targets.
   *
   * Static cached, may be called multiple times in a page load.
   */
  public function getMappingTargets() {
    $targets = array(
      'title' => array(
        'name' => t('Title'),
        'description' => t('The title of the node.'),
       ),
     );
    // Include body field only if available.
    $type = node_get_types('type',  $this->config['content_type']);
    if ($type->has_body) {
      // Using 'teaser' instead of 'body' forces entire content above the break.
      $targets['body'] = array(
        'name' => t('Body'),
        'description' => t('The body of the node. The teaser will be the same as the entire body.'),
      );
    }
    $targets += array(
      'status' => array(
        'name' => t('Published status'),
        'description' => t('Whether a node is published or not. 1 stands for published, 0 for not published.'),
      ),
      'created' => array(
        'name' => t('Published date'),
        'description' => t('The UNIX time when a node has been published.'),
      ),
      'url' => array(
        'name' => t('URL'),
        'description' => t('The external URL of the node. E. g. the feed item URL in the case of a syndication feed. May be unique.'),
        'optional_unique' => TRUE,
      ),
      'guid' => array(
        'name' => t('GUID'),
        'description' => t('The external GUID of the node. E. g. the feed item GUID in the case of a syndication feed. May be unique.'),
        'optional_unique' => TRUE,
      ),
    );

    // Let other modules expose mapping targets.
    self::loadMappers();
    drupal_alter('feeds_node_processor_targets', $targets, $this->config['content_type']);

    return $targets;
  }

  /**
   * Get nid of an existing feed item node if available.
   */
  protected function existingItemId($source_item, FeedsSource $source) {

    // Iterate through all unique targets and test whether they do already
    // exist in the database.
    foreach ($this->uniqueTargets($source_item) as $target => $value) {
      switch ($target) {
        case 'url':
          $nid = db_result(db_query("SELECT nid FROM {feeds_node_item} WHERE feed_nid = %d AND id = '%s' AND url = '%s'", $source->feed_nid, $source->id, $value));
          break;
        case 'guid':
          $nid = db_result(db_query("SELECT nid FROM {feeds_node_item} WHERE feed_nid = %d AND id = '%s' AND guid = '%s'", $source->feed_nid, $source->id, $value));
          break;
      }
      if ($nid) {
        // Return with the first nid found.
        return $nid;
      }
    }
    return 0;
  }

  /**
   * Loads on-behalf implementations from mappers/
   */
  protected static function loadMappers() {
    static $loaded = FALSE;

    if (!$loaded) {
      $path = drupal_get_path('module', 'feeds') .'/mappers';
      $files = drupal_system_listing('.*\.inc$', $path, 'name', 0);
      foreach ($files as $file) {
        if (strstr($file->filename, '/mappers/')) {
  	      require_once("./$file->filename");
        }
      }
      // Rebuild cache.
      module_implements('', FALSE, TRUE);
    }
    $loaded = TRUE;
  }

  /**
   * Create MD5 hash of $item array.
   * @return Always returns a hash, even with empty, NULL, FALSE:
   *  Empty arrays return 40cd750bba9870f18aada2478b24840a
   *  Empty/NULL/FALSE strings return d41d8cd98f00b204e9800998ecf8427e
   */
  protected function hash($item) {
    return hash('md5', serialize($item));
  }

  /**
   * Retrieve MD5 hash of $nid from DB.
   * @return Empty string if no item is found, hash otherwise.
   */
  protected function getHash($nid) {
    $hash = db_result(db_query("SELECT hash FROM {feeds_node_item} WHERE nid = %d", $nid));
    if ($hash) {
      // Return with the hash.
      return $hash;
    }
    return '';
  }

  /**
   * Invoke inheritors depending on configuration.
   */
  protected function inherit($item_node, $feed_node) {
    $inheritors = $this->getInheritors();
    foreach ($this->config['inherit'] as $key => $value) {
      if ($value && function_exists($inheritors[$key]['callback'])) {
        $inheritors[$key]['callback']($feed_node, $item_node);
      }
    }
  }

  /**
   * Get a list of available inheritors.
   */
  protected function getInheritors() {
    $inheritors = array();
    if (module_exists('og')) {
     $inheritors['og'] = array(
       'name' => t('Inherit organic group(s)'),
       'description' => t('Feed item nodes inherit organic group settings from feed node.'),
       'callback' => 'feeds_inherit_og',
     );
    }
    if (module_exists('translation')) {
      $inheritors['translation'] = array(
        'name' => t('Inherit language'),
        'description' => t('Feed nodes inherit language settings from feed node.'),
        'callback' => 'feeds_inherit_translation',
      );
    }
    $inheritors['taxonomy'] = array(
      'name' => t('Inherit taxonomy'),
      'description' => t('Feed item nodes inherit taxonomy terms from feed node.'),
       'callback' => 'feeds_inherit_taxonomy',
    );
    $inheritors['user'] = array(
      'name' => t('Inherit author'),
      'description' => t('Feed item nodes will be authored by the same user as the feed node.'),
      'callback' => 'feeds_inherit_user',
    );
    drupal_alter('feeds_node_processor_inheritors', $inheritors);
    return $inheritors;
  }
}

/**
 * @defgroup inheritors Inheritors for Feeds node processor.
 * @{
 */

/**
 * Inherit OG properties.
 */
function feeds_inherit_og($item_node, $feed_node) {
  if (isset($feed_node->og_public) ) {
    if (!isset($item_node->og_public)) {
      $item_node->og_public = 1;
    }
    $item_node->og_public = $item_node->og_public & $feed_node->og_public;
  }
  if (isset($feed_node->og_groups)) {
    if (!(isset($item_node->og_groups) && is_array($item_node->og_groups))) {
      $item_node->og_groups = array();
    }
    $item_node->og_groups = array_merge($feed_node->og_groups, $item_node->og_groups);
    foreach ($item_node->og_groups as $gid) {
      if ($gid != 0) {
        $transformed_groups[$gid] = $gid;
      }
    }
    $item_node->og_groups = $transformed_groups;
  }
  if (isset($feed_node->og_groups_names)) {
    if (!is_array($item_node->og_groups_names)) {
      $item_node->og_groups_names = array();
    }
    $item_node->og_groups_names = array_merge($feed_node->og_groups_names, $item_node->og_groups_names);
  }
}

/**
 * Inherit translation properties.
 */
function feeds_inherit_translation($item_node, $feed_node) {
  $item_node->language = $feed_node->language;
}

/**
 * Inherit taxonomy properties.
 */
function feeds_inherit_taxonomy($item_node, $feed_node) {
  $terms = taxonomy_node_get_terms($feed_node);
  if (!(isset($item_node->taxonomy) && is_array($item_node->taxonomy))) {
    $item_node->taxonomy = array();
  }
  foreach ($terms as $tid => $term) {
    $vid = $term->vid;
    $vocabulary = taxonomy_vocabulary_load($vid);
    if ($vocabulary->multiple) {
      if (!(isset($item_node->taxonomy[$vid]) && is_array($item_node->taxonomy[$vid]))) {
        $item_node->taxonomy[$vid] = array();
      }
      if (!in_array($tid, $item_node->taxonomy[$vid])) {
        $item_node->taxonomy[$vid][$tid] = $tid;
      }
    }
    else {
      if (!$item_node->taxonomy[$vid]) {
        $item_node->taxonomy[$vid] = $tid;
      }
    }
  }
}

/**
 * Inherit user properties.
 */
function feeds_inherit_user($item_node, $feed_node) {
  $item_node->uid = $feed_node->uid;
}

/**
 * @}
 */

/**
 * Copy of node_delete() that circumvents node_access().
 *
 * Used for batch deletion.
 */
function _feeds_node_delete($nid) {

  $node = node_load($nid);

  db_query("DELETE FROM {node} WHERE nid = %d", $node->nid);
  db_query("DELETE FROM {node_revisions} WHERE nid = %d", $node->nid);

  // Call the node-specific callback (if any):
  node_invoke($node, 'delete');
  node_invoke_nodeapi($node, 'delete');

  // Clear the page and block caches.
  cache_clear_all();

  // Remove this node from the search index if needed.
  if (function_exists('search_wipe')) {
    search_wipe($node->nid, 'node');
  }
  watchdog('content', '@type: deleted %title.', array('@type' => $node->type, '%title' => $node->title));
  drupal_set_message(t('@type %title has been deleted.', array('@type' => node_get_types('name', $node), '%title' => $node->title)));
}
