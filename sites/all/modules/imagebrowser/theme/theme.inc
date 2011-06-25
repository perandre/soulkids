<?php
// $Id: theme.inc,v 1.1.2.9 2009/07/19 14:30:51 starnox Exp $

/**
 * @file
 * Theme functions for Image Browser.
 */


/**
 * Setup variables for the Image Browser thumbnail template.
 *
 * @param array $vars
 */
function template_preprocess_imagebrowser_thumbnail(&$vars) {
  $node = node_load($vars['row']->nid);

  $vars['thumbnail'] = url('imagebrowser/view/image/'. $node->nid .'/thumbnail');
  $vars['path'] = url('imagebrowser/load/imagedetails/'. $node->nid);
  $vars['title'] = $node->title;
}