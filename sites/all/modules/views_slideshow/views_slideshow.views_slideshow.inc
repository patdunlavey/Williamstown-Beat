<?php
// $Id: views_slideshow.views_slideshow.inc,v 1.1.2.2.2.2 2009/08/24 19:46:54 aaron Exp $

/**
 *  @file
 *  The default options available with Views Slideshow.
 */


function views_slideshow_views_slideshow_option_definition() {
  $options['mode'] = array('default' => VIEWS_SLIDESHOW_DEFAULT_MODE);
  return $options;
}
