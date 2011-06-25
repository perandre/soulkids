<?php
// $Id: theme.inc,v 1.1.2.16 2010/01/29 14:22:40 swentel Exp $

/**
 * @file
 * Theming functions for ds.
 */

/**
 * Theme function to render the field content.
 *
 * @param string $content The content to render.
 * @param string $field_key The name of the field key.
 * @param array $field The current field.
 */
function theme_ds_field($content, $field_key, $field) {

  $output = '';
  if (!empty($content)) {
    if ($field['type'] == 'ds') {
      $output .= '<div class="field field-'. $field_key .'">';
      // Above label.
      if ($field['labelformat'] == 'above') {
        $output .= '<div class="field-label">'. $field['title'] .': </div>';
      }
      // Inline label
      if ($field['labelformat'] == 'inline') {
        $output .= '<div class="field-label-inline-first">'. $field['title'] .': </div>';
      }
      $output .= $content;
      $output .= '</div>';
    }
    else {
      $output = $content;
    }
  }

  return $output;
}

/**
 * Theme author name.
 */
function theme_ds_author_nolink(&$object) {
  $object->author_rendered = $object->name;
}

/**
 * Theme author name linked.
 */
function theme_ds_author_link(&$object) {
  $object->author_rendered = theme('username', $object);
}

/**
 * Template preprocess function for theme_views_view_row_object().
 */
function template_preprocess_ds_row_object(&$vars) {

  $options = $vars['options'];
  $vars['object'] = ''; // make sure var is defined.

  // We delegate the manipulation of the object to a function
  // which every module implementing the ds_api should define.
  // This can either be the simple selector or the advanced selector.
  // The advanced selector searches for a function build like this:
  // ds_views_row_adv_VIEWSNAME.
  if ($options['advanced']) {
    $row_function = 'ds_views_row_adv_'. $vars['view']->name;
    $build_mode = $options['build_mode'];
    $row_function($vars);
  }
  // Simple selector.
  else {
    $row_function = 'ds_views_row_'. $vars['view']->base_table;
    $build_mode = $options['build_mode'];
    $row_function($vars, $build_mode);
  }
}

/**
 * Theme preprocess function for ds-display-overview-form.tpl.php.
 */
function template_preprocess_ds_display_overview_form(&$vars) {
  $form = &$vars['form'];

  $build_mode = $form['#build_mode'];

  // Order the fields.
  $order = array();
  foreach ($form['#fields'] as $key => $field) {
    $order[$field] = $form[$field]['ds_weight']['#default_value'];
  }
  asort($order);

  // Status.
  $vars['settings_status'] = drupal_render($form['settings_status']);

  $rows = array();
  foreach ($order as $key => $field) {
    $element = &$form[$key];
    $row = new stdClass();

    // Each field will have a region, store it temporarily
    $region = $element[$build_mode]['region']['#default_value'];

    foreach (element_children($element) as $child) {

      // Render the display fields
      if ($child == $build_mode) {
        $row->{$child}->label = drupal_render($element[$child]['label']);
        $row->{$child}->label_edit = drupal_render($element[$child]['label_edit']);
        $row->{$child}->label_value = drupal_render($element[$child]['label_value']);
        $row->{$child}->format = drupal_render($element[$child]['format']);
        $row->{$child}->region = drupal_render($element[$child]['region']);
      }
      // Render the rest of the fields
      else {
        // Process weight.
        if ($child == 'ds_weight') {
          $element['ds_weight']['#attributes']['class'] = 'field-weight field-weight-'. $region;
          $element['ds_weight'] = process_weight($element['ds_weight']);
        }
        $row->{$child} = drupal_render($element[$child]);
      }
    }

    // Add draggable.
    $row->class = 'draggable';
    if ($region == 'disabled') {
      $row->class .= ' region-css-'. $region;
    }

    $row->label_class = 'label-field';
    $rows[$region][] = $row;
  }

  drupal_add_js('misc/tableheader.js');
  drupal_add_js(drupal_get_path('module', 'ds') .'/js/ds.js');
  drupal_add_css(drupal_get_path('module', 'ds') .'/css/ds.css');

  $regions = ds_regions(variable_get('ds_build_mode_'. $build_mode, 'all'));
  foreach ($regions as $region => $title) {
    drupal_add_tabledrag('fields', 'match', 'sibling', 'field-region-select', 'field-region-'. $region, NULL, FALSE);
    // We need to have subgroups to make the dragging available within parents
    drupal_add_tabledrag('fields', 'order', 'sibling', 'field-weight', 'field-weight-'. $region);
  }

  // Plugins available.
  $vars['plugins_tabs'] = array();
  $vars['plugins_content'] = '';
  if ($form['#plugins'] == TRUE) {
    foreach ($form['#plugin_keys'] as $key => $title) {
      $vars['plugins_tabs'][$key] = $title;
      $vars['plugins_content'][$key] = drupal_render($form[$key]);
    }
  }

  $vars['rows'] = $rows;
  $vars['submit'] = drupal_render($form);
  $vars['regions'] = $regions;
  $vars['build_mode'] = $build_mode;
}
