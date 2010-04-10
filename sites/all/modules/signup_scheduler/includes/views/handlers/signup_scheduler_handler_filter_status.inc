<?php
// $Id: signup_scheduler_handler_filter_status.inc,v 1.1.2.1 2008/11/20 05:18:57 jrbeeman Exp $

/**
 * Filter nodes based on their signup scheduler status (enabled vs. disabled).
 */
class signup_scheduler_handler_filter_status extends views_handler_filter_boolean_operator {
  function admin_summary() {
    if (!empty($this->options['exposed'])) {
      return t('exposed');
    }
    return (empty($this->value) ? t('Disabled') : t('Enabled'));
  }
}

