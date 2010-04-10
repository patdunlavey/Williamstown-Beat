<?php
// $Id: merci.views.inc,v 1.1 2009/01/23 21:16:22 civicpixel Exp $
/**
 * @file
 * Expose MERCI information to Views.
 */

/**
 * Implementation of hook_views_data().
 */
function merci_views_data() {
  $data = array();

  // merci_node_type
  $data['merci_node_type']['table']['group'] = t('MERCI');
  $data['merci_node_type']['table']['join']['node'] = array(
    'left_field' => 'type',
    'field' => 'type',
    'type' => 'INNER',
  );
  $data['merci_node_type']['type_setting'] = array(
    'title' => t('MERCI type'),
    'help' => t('The MERCI type (Bucket|Resource).'),
    'field' => array(
      'handler' => 'merci_handler_field_merci_node_type_type_setting',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'filter' => array(
      'handler' => 'merci_handler_filter_merci_node_type_type_setting',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['merci_node_type']['max_hours_per_reservation'] = array(
    'title' => t('Max hours per reservation'),
    'help' => t('The maximum hours that the item can be reserved for.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['merci_node_type']['allow_overnight'] = array(
    'title' => t('Allow overnight'),
    'help' => t('TRUE if the item can be kept overnight.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Allowed overnight'),
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['merci_node_type']['allow_weekends'] = array(
    'title' => t('Allow weekends'),
    'help' => t('TRUE if the item can be kept over weekends.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Allowed over weekends'),
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['merci_node_type']['late_fee_per_hour'] = array(
    'title' => t('Late fee per hour -- default'),
    'help' => t('Default per hour fee if the item is returned late (as defined by MERCI type).'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['merci_node_type']['rate_per_hour'] = array(
    'title' => t('Rate per hour -- default'),
    'help' => t('Default per hour fee for use of the item (as defined by MERCI type).'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['merci_node_type']['fee_free_hours'] = array(
    'title' => t('Fee free hours -- default'),
    'help' => t('Default number of hours the item can be used before fees are charged (as defined by MERCI type).'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['merci_node_type']['status'] = array(
    'title' => t('Bucket/Resource Status'),
    'help' => t('The status of the Bucket/Resource.'),
    'field' => array(
      'handler' => 'merci_handler_field_merci_node_type_status',
      'click sortable' => TRUE,
    ),
    // TODO: there really should be a custom handler for this.
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'filter' => array(
      'handler' => 'merci_handler_filter_merci_node_type_status',
    ),
    'sort' => array(
      // TODO: this needs a custom handler.
      'handler' => 'views_handler_sort',
    ),
  );
  $data['merci_node_type']['spare_items'] = array(
    'title' => t('Bucket spare items'),
    'help' => t('Number of spare items to leave unreserved for the bucket.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // merci_reservation
  $data['merci_reservation']['table']['group'] = t('MERCI');
  $data['merci_reservation']['table']['join']['node'] = array(
    'left_field' => 'vid',
    'field' => 'vid',
    'type' => 'INNER',
  );
  $data['merci_reservation']['status'] = array(
    'title' => t('Reservation status'),
    'help' => t('The status of the reservation.'),
    'field' => array(
      'handler' => 'merci_handler_field_merci_reservation_status',
      'click sortable' => TRUE,
    ),
    // TODO: there really should be a custom handler for this.
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'filter' => array(
      'handler' => 'merci_handler_filter_merci_reservation_status',
    ),
    'sort' => array(
      // TODO: this needs a custom handler.
      'handler' => 'views_handler_sort',
    ),
  );

  // merci_bucket_node
  $data['merci_bucket_node']['table']['group'] = t('MERCI');
  $data['merci_bucket_node']['table']['join']['node'] = array(
    'left_field' => 'vid',
    'field' => 'vid',
    'type' => 'INNER',
  );
  $data['merci_bucket_node']['default_availability'] = array(
    'title' => t('Default booking availability -- bucket'),
    'help' => t('The default booking availability of a Bucket type (ex. Available|Unavailable).'),
    'field' => array(
      'handler' => 'merci_handler_field_merci_bucket_resource_node_default_availability',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      // TODO: this needs a custom handler.
      'handler' => 'views_handler_argument_numeric',
    ),
    'filter' => array(
      'handler' => 'merci_handler_filter_merci_bucket_resource_node_default_availability',
    ),
    'sort' => array(
      // TODO: this needs a custom handler.
      'handler' => 'views_handler_sort',
    ),
  );
  $data['merci_bucket_node']['sub_type'] = array(
    'title' => t('Sub type -- bucket'),
    'help' => t('The sub type of the Bucket (Item|Reservation placeholder).'),
    'field' => array(
      'handler' => 'merci_handler_field_merci_bucket_resource_node_sub_type',
      'click sortable' => TRUE,
    ),
    // TODO: there really should be a custom handler for this.
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'filter' => array(
      'handler' => 'merci_handler_filter_merci_bucket_resource_node_sub_type',
    ),
    'sort' => array(
      // TODO: this needs a custom handler.
      'handler' => 'views_handler_sort',
    ),
  );

  // merci_resource_node
  $data['merci_resource_node']['table']['group'] = t('MERCI');
  $data['merci_resource_node']['table']['join']['node'] = array(
    'left_field' => 'vid',
    'field' => 'vid',
    'type' => 'INNER',
  );
  $data['merci_resource_node']['default_availability'] = array(
    'title' => t('Default booking availability -- resource'),
    'help' => t('The default booking availability of a Resource type (ex. Available|Unavailable).'),
    'field' => array(
      'handler' => 'merci_handler_field_merci_bucket_resource_node_default_availability',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      // TODO: this needs a custom handler.
      'handler' => 'views_handler_argument_numeric',
    ),
    'filter' => array(
      'handler' => 'merci_handler_filter_merci_bucket_resource_node_default_availability',
    ),
    'sort' => array(
      // TODO: this needs a custom handler.
      'handler' => 'views_handler_sort',
    ),
  );
  $data['merci_resource_node']['late_fee_per_hour'] = array(
    'title' => t('Late fee per hour -- resource'),
    'help' => t('Per hour fee if the item is returned late (as defined by the individual resource).'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['merci_resource_node']['rate_per_hour'] = array(
    'title' => t('Rate per hour -- resource'),
    'help' => t('Per hour fee for use of the item (as defined by the individual resource).'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['merci_resource_node']['fee_free_hours'] = array(
    'title' => t('Fee free hours -- resource'),
    'help' => t('Number of hours the item can be used before fees are charged (as defined by the individual resource).'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['merci_resource_node']['sub_type'] = array(
    'title' => t('Sub type -- resource'),
    'help' => t('The sub type of the Resource (Item|Reservation placeholder).'),
    'field' => array(
      'handler' => 'merci_handler_field_merci_bucket_resource_node_sub_type',
      'click sortable' => TRUE,
    ),
    // TODO: there really should be a custom handler for this.
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'filter' => array(
      'handler' => 'merci_handler_filter_merci_bucket_resource_node_sub_type',
    ),
    'sort' => array(
      // TODO: this needs a custom handler.
      'handler' => 'views_handler_sort',
    ),
  );

  return $data;
}
