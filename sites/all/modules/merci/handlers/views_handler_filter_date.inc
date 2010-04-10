<?php
// $Id: views_handler_filter_date.inc,v 1.1 2009/01/23 21:16:22 civicpixel Exp $

class views_handler_filter_datetime extends views_handler_filter_date {
  function op_simple($field) {
    $value = intval(strtotime($this->value['value'], 0));
    $value = date_convert($value, DATE_UNIX, DATE_DATETIME);
    if ($this->value['type'] == 'offset') {
      $value = 'NOW()' . sprintf('%+d', $value); // keep sign
      $this->query->add_where($this->options['group'], "$field $this->operator %s", $value);
    } else {
      $this->query->add_where($this->options['group'], "$field $this->operator '%s'", $value);
    }
  }
}