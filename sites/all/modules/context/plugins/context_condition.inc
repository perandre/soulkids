<?php
// $Id: context_condition.inc,v 1.1.2.1 2009/12/14 22:34:05 yhahn Exp $

/**
 * Base class for a context condition.
 */
class context_condition {
  var $plugin;
  var $title;
  var $description;
  var $values = array();

  /**
   * Constructor. Do not override.
   */
  function __construct($plugin, $info) {
    $this->plugin = $plugin;
    $this->title = isset($info['title']) ? $info['title'] : $plugin;
    $this->description = isset($info['description']) ? $info['description'] : '';
  }

  /**
   * Condition values.
   */
  function condition_values() {
    return array();
  }

  /**
   * Condition form.
   */
  function condition_form($context) {
    return array(
      '#title' => $this->title,
      '#description' => $this->description,
      '#options' => $this->condition_values(),
      '#type' => 'checkboxes',
      '#default_value' => $this->fetch_from_context($context, 'values'),
    );
  }

  /**
   * Condition form submit handler.
   */
  function condition_form_submit($values) {
    // Editor forms are generally checkboxes -- do some checkbox processing.
    return drupal_map_assoc(array_keys(array_filter($values)));
  }

  /**
   * Options form. Provide additional options for your condition.
   */
  function options_form($context) {
    return array();
  }

  /**
   * Options form submit handler.
   */
  function options_form_submit($values) {
    return $values;
  }

  /**
   * Context editor form for conditions.
   */
  function editor_form($context) {
    $form = array();
    if (!empty($this->values)) {
      $options = $this->condition_values();
      foreach ($this->values as $value => $contexts) {
        $label = "{$this->title}: ";
        $label .= isset($options[$value]) ? trim($options[$value], ' -') : $value;
        $form[$value] = array(
          '#type' => 'checkbox',
          '#title' => check_plain($label),
          '#default_value' => empty($context->name) ? TRUE : in_array($context->name, $contexts, TRUE),
        );
      }
    }
    return $form;
  }

  /**
   * Context editor form submit handler.
   */
  function editor_form_submit(&$context, $values) {
    // Editor forms are generally checkboxes -- do some checkbox processing.
    return drupal_map_assoc(array_keys(array_filter($values)));
  }

  /**
   * Public method that is called from hooks or other integration
   * points with Drupal.
   */
  function execute($value) {
    $contexts = $this->get_contexts($value);
    $this->values[$value] = array();
    foreach ($contexts as $context) {
      $this->values[$value][] = $context->name;
      context_set('context', $context->name, $context);
    }
  }

  /**
   * Retrieve all contexts with the condition value provided.
   */
  function get_contexts($value) {
    $map = context_condition_map();
    $map = isset($map[$this->plugin]) ? $map[$this->plugin] : array();

    $contexts = array();
    if ((is_string($value) || is_numeric($value)) && isset($map[$value]) && is_array($map[$value])) {
      foreach ($map[$value] as $name) {
        $context = context_load($name);
        $contexts[$context->name] = $context;
      }
    }
    return $contexts;
  }

  /**
   * Retrieve options from the context provided.
   */
  function fetch_from_context($context, $key = NULL) {
    // Allow other modules to alter the value just before it's returned.
    if (!context_isset('context_ui', 'no_alter')) {
      drupal_alter('context_condition_fetch', $this, $context);
    }
    if (isset($key)) {
      return isset($context->conditions[$this->plugin][$key]) ? $context->conditions[$this->plugin][$key] : array();
    }
    return isset($context->conditions[$this->plugin]) ? $context->conditions[$this->plugin] : array();
  }
}
