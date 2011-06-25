<?php

function views_gallery_views_default_views() {
$views = array();  

$view = new view;
$view->name = 'gallery';
$view->description = 'Photos belonging to a selected gallery';
$view->tag = '';
$view->view_php = '';
$view->base_table = 'node';
$view->is_cacheable = FALSE;
$view->api_version = 2;
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->override_option('fields', array(
  'title' => array(
    'label' => '',
    'alter' => array(
      'alter_text' => 0,
      'text' => '',
      'make_link' => 0,
      'path' => '',
      'alt' => '',
      'prefix' => '',
      'suffix' => '',
      'help' => '',
      'trim' => 0,
      'max_length' => '',
      'word_boundary' => 1,
      'ellipsis' => 1,
      'strip_tags' => 0,
      'html' => 0,
    ),
    'link_to_node' => 0,
    'exclude' => 0,
    'id' => 'title',
    'table' => 'node',
    'field' => 'title',
    'relationship' => 'none',
  ),
  'field_gallery_image_fid' => array(
    'label' => '',
    'alter' => array(
      'alter_text' => 0,
      'text' => '',
      'make_link' => 0,
      'path' => '',
      'alt' => '',
      'prefix' => '',
      'suffix' => '',
      'help' => '',
      'trim' => 0,
      'max_length' => '',
      'word_boundary' => 1,
      'ellipsis' => 1,
      'strip_tags' => 0,
      'html' => 0,
    ),
    'link_to_node' => 0,
    'label_type' => 'none',
    'format' => 'thumb_linked',
    'multiple' => array(
      'group' => TRUE,
      'multiple_number' => '',
      'multiple_from' => '',
      'multiple_reversed' => FALSE,
    ),
    'exclude' => 0,
    'id' => 'field_gallery_image_fid',
    'table' => 'node_data_field_gallery_image',
    'field' => 'field_gallery_image_fid',
    'relationship' => 'none',
  ),
  'created' => array(
    'label' => '',
    'alter' => array(
      'alter_text' => 0,
      'text' => '',
      'make_link' => 0,
      'path' => '',
      'alt' => '',
      'prefix' => '',
      'suffix' => '',
      'help' => '',
      'trim' => 0,
      'max_length' => '',
      'word_boundary' => 1,
      'ellipsis' => 1,
      'strip_tags' => 0,
      'html' => 0,
    ),
    'date_format' => 'time ago',
    'custom_date_format' => '2',
    'exclude' => 0,
    'id' => 'created',
    'table' => 'node',
    'field' => 'created',
    'relationship' => 'none',
  ),
));
$handler->override_option('sorts', array(
  'created' => array(
    'order' => 'DESC',
    'granularity' => 'second',
    'id' => 'created',
    'table' => 'node',
    'field' => 'created',
    'relationship' => 'none',
  ),
));
$handler->override_option('arguments', array(
  'field_gallery_nid' => array(
    'default_action' => 'not found',
    'style_plugin' => 'default_summary',
    'style_options' => array(),
    'wildcard' => 'all',
    'wildcard_substitution' => 'All',
    'title' => '',
    'default_argument_type' => 'fixed',
    'default_argument' => '',
    'validate_type' => 'node',
    'validate_fail' => 'not found',
    'break_phrase' => 0,
    'not' => 0,
    'id' => 'field_gallery_nid',
    'table' => 'node_data_field_gallery',
    'field' => 'field_gallery_nid',
    'validate_user_argument_type' => 'uid',
    'validate_user_roles' => array(
      '2' => 0,
    ),
    'relationship' => 'none',
    'default_options_div_prefix' => '',
    'default_argument_user' => 0,
    'default_argument_fixed' => '',
    'default_argument_php' => '',
    'validate_argument_node_type' => array(
      views_gallery_gallery_type() => views_gallery_gallery_type(),
    ),
    'validate_argument_node_access' => 1,
    'validate_argument_nid_type' => 'nid',
    'validate_argument_vocabulary' => array(),
    'validate_argument_type' => 'tid',
    'validate_argument_transform' => 0,
    'validate_user_restrict_roles' => 0,
    'validate_argument_php' => '',
  ),
));
$handler->override_option('filters', array(
  'status_extra' => array(
    'operator' => '=',
    'value' => '',
    'group' => '0',
    'exposed' => FALSE,
    'expose' => array(
      'operator' => FALSE,
      'label' => '',
    ),
    'id' => 'status_extra',
    'table' => 'node',
    'field' => 'status_extra',
    'relationship' => 'none',
  ),
));
$handler->override_option('access', array(
  'type' => 'none',
));
$handler->override_option('cache', array(
  'type' => 'none',
));
$handler->override_option('empty', 'No photos have been added to this gallery.');
$handler->override_option('empty_format', '1');
$handler->override_option('use_ajax', TRUE);
$handler->override_option('items_per_page', 6);
$handler->override_option('use_pager', '1');
$handler->override_option('style_plugin', 'grid');
$handler->override_option('style_options', array(
  'grouping' => '',
  'columns' => '3',
  'alignment' => 'horizontal',
));
$handler = $view->new_display('node_content', 'Full gallery', 'node_content_1');
$handler->override_option('types', array(
  'gallery' => 'gallery',
));
$handler->override_option('modes', array(
  '0' => 'full',
));
$handler->override_option('argument_mode', 'nid');
$handler->override_option('default_argument', 'nid');
$handler->override_option('show_title', 0);
$handler = $view->new_display('node_content', 'Latest image', 'node_content_2');
$handler->override_option('fields', array(
  'field_gallery_image_fid' => array(
    'label' => '',
    'alter' => array(
      'alter_text' => 0,
      'text' => '',
      'make_link' => 0,
      'path' => '',
      'alt' => '',
      'prefix' => '',
      'suffix' => '',
      'help' => '',
      'trim' => 0,
      'max_length' => '',
      'word_boundary' => 1,
      'ellipsis' => 1,
      'strip_tags' => 0,
      'html' => 0,
    ),
    'link_to_node' => 0,
    'label_type' => 'none',
    'format' => 'short_default',
    'multiple' => array(
      'group' => TRUE,
      'multiple_number' => '',
      'multiple_from' => '',
      'multiple_reversed' => FALSE,
    ),
    'exclude' => 0,
    'id' => 'field_gallery_image_fid',
    'table' => 'node_data_field_gallery_image',
    'field' => 'field_gallery_image_fid',
    'relationship' => 'none',
    'override' => array(
      'button' => 'Use default',
    ),
  ),
));
$handler->override_option('items_per_page', 1);
$handler->override_option('use_pager', '0');
$handler->override_option('style_plugin', 'default');
$handler->override_option('style_options', array(
  'grouping' => '',
));
$handler->override_option('types', array(
  'gallery' => 'gallery',
));
$handler->override_option('modes', array(
  '0' => 'teaser',
));
$handler->override_option('argument_mode', 'nid');
$handler->override_option('default_argument', 'nid');

$views[$view->name] = $view;
return $views;
}