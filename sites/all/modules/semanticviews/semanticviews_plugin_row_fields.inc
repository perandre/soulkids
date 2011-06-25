<?php
// $Id: semanticviews_plugin_row_fields.inc,v 1.1.2.8 2009/09/26 22:17:39 bangpound Exp $
/**
 * @file
 * Contains the semantic row style plugin.
 */

/**
 * The semantic 'fields' row plugin
 *
 * This displays fields one after another, giving options for HTML element and
 * class.
 *
 * @ingroup views_row_plugins
 */
class semanticviews_plugin_row_fields extends views_plugin_row {
  function option_definition() {
    $options = parent::option_definition();
    $options['skip_blank'] = array('default' => FALSE);
    // Field element_type and classes cannot be defined in the options
    // definition because the field handlers are not attached when the option
    // defaults are set up in the object's init() method.
    return $options;
  }

  /**
   * Provide a form for setting options.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['semantic_html'] = array(
      '#tree' => TRUE,
    );
    foreach ($this->display->handler->get_handlers('field') as $field => $handler) {
      if (!$handler->options['exclude']) {
        $default_value = is_array($this->options['semantic_html'][$field]) ?
          $this->options['semantic_html'][$field] :
          array('element_type' => 'div', 'class' => '');
        $form['semantic_html'][$field] = array(
          '#title' => $handler->label() ? $handler->label() : $handler->ui_name(),
          '#type' => 'fieldset',
          '#attributes' => array(
            'class' => 'clear-block',
          ),
        );
        $form['semantic_html'][$field]['element_type'] = array(
          '#prefix' => '<div class="views-left-30">',
          '#suffix' => '</div>',
          '#title' => 'Element',
          '#type' => 'textfield',
          '#size' => '10',
          '#default_value' => $default_value['element_type'],
        );
        $form['semantic_html'][$field]['class'] = array(
          '#prefix' => '<div class="views-right-70">',
          '#suffix' => '</div>',
          '#title' => 'Class attribute',
          '#type' => 'textfield',
          '#size' => '30',
          '#default_value' => $default_value['class'],
        );
      }
    }
    $form['skip_blank'] = array(
      '#type' => 'checkbox',
      '#default_value' => $this->options['skip_blank'],
      '#title' => t('Skip empty fields'),
      '#description' => t('Do not output anything when a field has no content.'),
    );
  }

  /**
   * Validate the options form.
   */
  function options_validate(&$form, &$form_state) {
    parent::options_validate($form, $form_state);
    // TODO: validate that the elements and classes are valid HTML. This is not
    // a substitute for output filtering.
  }
}
