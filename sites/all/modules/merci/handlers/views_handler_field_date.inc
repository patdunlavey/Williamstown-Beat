<?php
// $Id: views_handler_field_date.inc,v 1.1 2009/01/23 21:16:22 civicpixel Exp $

/**
 * Views assumes dates are always Unix timestamps, so convert our DATETIME-formatted columns.
 *
 */
class views_handler_field_datetime extends views_handler_field_date {
  function query() {
    $this->ensure_my_table();
    $this->field_alias = $this->query->add_field(NULL, "UNIX_TIMESTAMP(".$this->table_alias.'.'.$this->field.')', $this->table_alias . '_' . $this->field);
  }
}
