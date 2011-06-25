<?php
// $Id: semanticviews.views.inc,v 1.1.2.3 2009/09/20 22:22:59 bangpound Exp $

/**
 * Implementation of hook_views_plugins().
 */
function semanticviews_views_plugins() {
  return array(
    'style' => array(
      'semanticviews_default' => array(
        'title' => t('Semantic Views'),
        'help' => t('Displays rows one after another.'),
        'handler' => 'semanticviews_plugin_style_default',
        'theme' => 'semanticviews_view_unformatted',
        'uses row plugin' => TRUE,
        'uses options' => TRUE,
        'uses grouping' => TRUE,
        'type' => 'normal',
        'help topic' => 'style-unformatted',
      ),
    ),
    'row' => array(
      'semanticviews_fields' => array(
        'title' => t('Semantic Views : Fields'),
        'help' => t('Displays the fields with an optional template.'),
        'handler' => 'semanticviews_plugin_row_fields',
        'theme' => 'semanticviews_view_fields',
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal',
        'help topic' => 'style-row-fields',
      ),
    ),
  );
}
