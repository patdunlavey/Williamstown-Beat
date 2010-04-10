<?php
// $Id: merci_handler_field.inc,v 1.1 2009/01/23 21:16:22 civicpixel Exp $

/**
 * Type setting Bucket|Resource.
 */
class merci_handler_field_merci_node_type_type_setting extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['type_setting'] = 'type_setting';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    parent::render($values);
    $type_setting = $values->{$this->aliases['type_setting']};
    switch ($type_setting) {
      case 'bucket':
        return t('Bucket');
        break;
      case 'resource':
        return t('Resource');
        break;
      default:
        return t('Disabled');
        break;
    }
  }
}

/**
 * Item status.
 */
class merci_handler_field_merci_node_type_status extends views_handler_field {
  function construct() {
    parent::construct();
    // TODO: does this need an alias?
    $this->additional_fields['status'] = 'status';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    parent::render($values);
    $statuses = merci_item_status();
    $status = $values->{$this->aliases['status']};
    return $statuses[$status];
  }
}

/**
 * Reservation status.
 */
class merci_handler_field_merci_reservation_status extends views_handler_field {
  function construct() {
    parent::construct();
    // TODO: does this need an alias?
    $this->additional_fields['status'] = 'status';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    parent::render($values);
    $statuses = merci_record_status();
    $status = $values->{$this->aliases['status']};
    return $statuses[$status];
  }
}

/**
 * Bucket/resource default availability.
 */
class merci_handler_field_merci_bucket_resource_node_default_availability extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['default_availability'] = 'default_availability';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    parent::render($values);
    $availabilities = merci_item_status();
    $availability = $values->{$this->aliases['default_availability']};
    return $availabilities[$availability];
  }
}

/**
 * Bucket|Resource sub type.
 */
class merci_handler_field_merci_bucket_resource_node_sub_type extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['sub_type'] = 'sub_type';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    parent::render($values);
    $sub_type = $values->{$this->aliases['sub_type']};
    switch ($sub_type) {
      case MERCI_SUB_TYPE_ITEM:
        return t('Item');
        break;
      case MERCI_SUB_TYPE_RESERVATION:
        return t('Reservation placeholder');
        break;
      default:
        return t('Unknown');
        break;
    }
  }
}
