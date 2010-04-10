<?php
// $Id: views_handler_filter_boolean_operator.inc,v 1.1 2009/01/23 21:16:22 civicpixel Exp $

class views_handler_filter_bookings_resource_disabled extends views_handler_filter_boolean_operator {
  function construct() {
    $this->definition['label'] = 'Check to show only disabled, or leave empty to show only enabled.';
    parent::construct();
  }
}