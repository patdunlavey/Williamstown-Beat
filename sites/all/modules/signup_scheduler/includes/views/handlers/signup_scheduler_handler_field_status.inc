<?php
// $Id: signup_scheduler_handler_field_status.inc,v 1.1.2.1 2008/11/20 05:18:57 jrbeeman Exp $

/**
 * Field handler to display the current signup scheduler status of a node.
 */
class signup_scheduler_handler_field_status extends views_handler_field {
  function render($values) {
    return ($values->{$this->field_alias} == 0) ? t('Disabled') : t('Enabled');
  }
}

