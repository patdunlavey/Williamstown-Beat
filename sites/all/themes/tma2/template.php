<?php
// $Id: template.php,v 1.13 2008/05/13 09:19:13 johnalbin Exp $

/**
 * @file
 *
 * OVERRIDING THEME FUNCTIONS
 *
 * The Drupal theme system uses special theme functions to generate HTML output
 * automatically. Often we wish to customize this HTML output. To do this, we
 * have to override the theme function. You have to first find the theme
 * function that generates the output, and then "catch" it and modify it here.
 * The easiest way to do it is to copy the original function in its entirety and
 * paste it here, changing the prefix from theme_ to STARTERKIT_. For example:
 *
 *   original: theme_breadcrumb()
 *   theme override: STARTERKIT_breadcrumb()
 *
 * where STARTERKIT is the name of your sub-theme. For example, the zen_classic
 * theme would define a zen_classic_breadcrumb() function.
 *
 * If you would like to override any of the theme functions used in Zen core,
 * you should first look at how Zen core implements those functions:
 *   theme_breadcrumbs()      in zen/template.php
 *   theme_menu_item_link()   in zen/template-menus.php
 *   theme_menu_local_tasks() in zen/template-menus.php
 */
/*
function tma2_breadcrumb($breadcrumb) {
  $show_breadcrumb = theme_get_setting('zen_breadcrumb');
  $show_breadcrumb_home = theme_get_setting('zen_breadcrumb_home');
  $breadcrumb_separator = theme_get_setting('zen_breadcrumb_separator');
  $trailing_separator = (theme_get_setting('zen_breadcrumb_trailing') || theme_get_setting('zen_breadcrumb_title')) ? $breadcrumb_separator : '';
  $leading_separator = theme_get_setting('zen_breadcrumb_leading') ? $breadcrumb_separator : '';

  // Determine if we are to display the breadcrumb
  if ($show_breadcrumb == 'yes' || $show_breadcrumb == 'admin' && arg(0) == 'admin') {
    if (!$show_breadcrumb_home) {
      // Optionally get rid of the homepage link
      array_shift($breadcrumb);
    }
    if (!empty($breadcrumb)) {
      // Return the breadcrumb with separators
      return '<div class="breadcrumb">' . $leading_separator . implode($breadcrumb_separator, $breadcrumb) . "$trailing_separator</div>";
    }
  }
  // Otherwise, return an empty string
  return '';
}
*/

/*
 * Add any conditional stylesheets you will need for this sub-theme.
 *
 * To add stylesheets that ALWAYS need to be included, you should add them to
 * your .info file instead. Only use this section if you are including
 * stylesheets based on certain conditions.
 */

/* -- Delete this line if you want to use and modify this code
// Example: optionally add a fixed width CSS file.
if (theme_get_setting('STARTERKIT_fixed')) {
  drupal_add_css(path_to_theme() . '/layout-fixed.css', 'theme', 'all');
}
// */


/**
 * Implementation of HOOK_theme().
 */
function tma2_theme(&$existing, $type, $theme, $path) {
  return zen_theme($existing, $type, $theme, $path);
}

function tma2_preprocess_content_field(&$variables) {  
  // Some customisations for story content type
  // Turn off some field labels for story
  if ($variables['node']->type == 'story' && in_array($variables['field_name'], array('field_subtitle', 'field_source', 'field_date'))) {
    $variables['label_display'] = 'hidden';
  }

  // CCK field items default to using <div>
  $variables['field_list_start_tag'] = '';
  $variables['field_list_end_tag'] = '';
  $variables['field_list_item_tag'] = 'div';

  // For some cases, change this to <ul> and <li>
  if (($variables['field_name'] == 'field_headlines' && $variables['teaser']) ||
      ($variables['field_name'] == 'field_stories')) {
    $variables['field_list_start_tag'] = '<ul>';
    $variables['field_list_end_tag'] = '</ul>';
    $variables['field_list_item_tag'] = 'li';
  }
}

/**
 * Override or insert PHPTemplate variables into all templates.
 *
 * @param $vars
 *   A sequential array of variables to pass to the theme template.
 * @param $hook
 *   The name of the theme function being called (name of the .tpl.php file.)
 */
/* -- Delete this line if you want to use this function
function tma2_preprocess(&$vars, $hook) {
  $vars['sample_variable'] = t('Lorem ipsum.');
}
// */

/**
 * Override or insert PHPTemplate variables into the page templates.
 *
 * @param $vars
 *   A sequential array of variables to pass to the theme template.
 * @param $hook
 *   The name of the theme function being called ("page" in this case.)
 */
/* -- Delete this line if you want to use this function
function tma2_preprocess_page(&$variables, $hook) {
}
// */

/**
 * Override or insert PHPTemplate variables into the node templates.
 *
 * @param $vars
 *   A sequential array of variables to pass to the theme template.
 * @param $hook
 *   The name of the theme function being called ("node" in this case.)
 */

function tma2_preprocess_node(&$variables, $hook) {
  zen_preprocess_node($variables, $hook);
  $node = $variables['node'];

  // Add a node-type-teaser suggestion
  if ($node->teaser) {
    $variables['template_files'][] = 'node-'. $node->type . '-teaser';
  }

  $variables['body_content'] = $node->content['body']['#value'];

  // If there is an extracted teaser image, do some processing
  if (isset($node->teaser_pp_img)) {
    $variables['teaser_pp_img'] = $node->content['teaser_pp_img']['#value'];
  }  
}


/**
 * Override or insert PHPTemplate variables into the comment templates.
 *
 * @param $vars
 *   A sequential array of variables to pass to the theme template.
 * @param $hook
 *   The name of the theme function being called ("comment" in this case.)
 */
/* -- Delete this line if you want to use this function
function tma2_preprocess_comment(&$vars, $hook) {
  $vars['sample_variable'] = t('Lorem ipsum.');
}
// */

/**
 * Override or insert PHPTemplate variables into the block templates.
 *
 * @param $vars
 *   A sequential array of variables to pass to the theme template.
 * @param $hook
 *   The name of the theme function being called ("block" in this case.)
 */
/* -- Delete this line if you want to use this function
function tma2_preprocess_block(&$vars, $hook) {
  $vars['sample_variable'] = t('Lorem ipsum.');
}
// */


/**
 * The rel="nofollow" attribute is missing from anonymous users' URL in Drupal 6.0-6.2.
 */
/* -- Delete this line if you want to use this function
function tma2_username($object) {

  if ($object->uid && $object->name) {
    // Shorten the name when it is too long or it will break many tables.
    if (drupal_strlen($object->name) > 20) {
      $name = drupal_substr($object->name, 0, 15) . '...';
    }
    else {
      $name = $object->name;
    }

    if (user_access('access user profiles')) {
      $output = l($name, 'user/' . $object->uid, array('attributes' => array('title' => t('View user profile.'))));
    }
    else {
      $output = check_plain($name);
    }
  }
  else if ($object->name) {
    // Sometimes modules display content composed by people who are
    // not registered members of the site (e.g. mailing list or news
    // aggregator modules). This clause enables modules to display
    // the true author of the content.
    if (!empty($object->homepage)) {
      $output = l($object->name, $object->homepage, array('attributes' => array('rel' => 'nofollow')));
    }
    else {
      $output = check_plain($object->name);
    }

    $output .= ' (' . t('not verified') . ')';
  }
  else {
    $output = variable_get('anonymous', t('Anonymous'));
  }

  return $output;
}
// */

function tma2_tinymce_content_css() 
{
  return path_to_theme() . '/tinymce_content.css';
}

/**
 * Override theme_custom_local_tasks to make the css look like zen_menu_local_tasks()
 */
function tma2_custom_local_tasks($primary = '', $secondary = '') {
  $output = '';

  if ($primary) {
    $output .= '<ul class="tabs primary clear-block">' . $primary . '</ul>';
  }
  if ($secondary) {
    $output .= '<ul class="tabs secondary clear-block">' . $secondary . '</ul>';
  }

  return $output;
}

function tma2_date_all_day_label() {
  return '<!-- ('. date_t('All day', 'datetime') .') -->';
}


