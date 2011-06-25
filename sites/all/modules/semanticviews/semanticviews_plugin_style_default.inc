<?php
// $Id: semanticviews_plugin_style_default.inc,v 1.1.2.12 2009/10/29 21:42:41 bangpound Exp $
/**
 * @file
 * Contains the default style plugin.
 */

/**
 * Default style plugin to render rows one after another with no decorations.
 *
 * @ingroup views_style_plugins
 */
class semanticviews_plugin_style_default extends views_plugin_style {
  /**
   * Set default options
   */
  function options(&$options) {
    parent::options($options);
  }

  /**
   * Set default options
   */
  function option_definition() {
    $options = parent::option_definition();

    // Group option definition.
    $options['group'] = array(
      'contains' => array(
        'element_type' => array('default' => 'h3'),
        'class' => array('default' => 'title'),
      ),
    );

    // List option definition.
    $options['list'] = array(
      'contains' => array(
        'element_type' => array('default' => ''),
        'class' => array('default' => ''),
      ),
    );

    // Row option definition.
    $options['row'] = array(
      'contains' => array(
        'element_type' => array('default' => 'div'),
        'class' => array('default' => ''),
        'first_class' => array('default' => 'first'),
        'last_class' => array('default' => 'last'),
        'last_every_nth' => array('default' => '0'),
        'striping_classes' => array('default' => 'odd even'),
      ),
    );

    return $options;
  }

  /**
   * Render the given style.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    // Group options.
    $form['group'] = array(
      '#type' => 'fieldset',
      '#title' => 'Grouping title',
      '#description' => 'If using groups, the view will insert the grouping&rsquo;s title field.',
      '#attributes' => array(
        'class' => 'clear-block',
      ),
    );
    $form['group']['element_type'] = array(
      '#prefix' => '<div class="views-left-30">',
      '#suffix' => '</div>',
      '#title' => 'Element',
      '#type' => 'textfield',
      '#size' => '10',
      '#default_value' => $this->options['group']['element_type'],
    );
    $form['group']['class'] = array(
      '#prefix' => '<div class="views-right-70">',
      '#suffix' => '</div>',
      '#title' => 'Class attribute',
      '#type' => 'textfield',
      '#size' => '30',
      '#default_value' => $this->options['group']['class'],
    );

    // List options.
    $form['list'] = array(
      '#type' => 'fieldset',
      '#title' => 'List',
      '#description' => t('If the output should be a HTML list, select the element and class attribute. The row element should also be set to %li.', array('%li' => 'li')),
      '#attributes' => array(
        'class' => 'clear-block',
      ),
    );
    $form['list']['element_type'] = array(
      '#prefix' => '<div class="views-left-30">',
      '#suffix' => '</div>',
      '#type' => 'radios',
      '#title' => t('List type'),
      '#options' => array(
        '' => t('None'),
        'ul' => t('Unordered list'),
        'ol' => t('Ordered list'),
        'dl' => t('Definition list'),
      ),
      '#default_value' => $this->options['list']['element_type'],
    );
    $form['list']['class'] = array(
      '#prefix' => '<div class="views-right-70">',
      '#suffix' => '</div>',
      '#title' => 'Class attribute',
      '#type' => 'textfield',
      '#size' => '30',
      '#default_value' => $this->options['list']['class'],
    );

    // Row options.
    $form['row'] = array(
      '#type' => 'fieldset',
      '#title' => 'Row',
      '#attributes' => array(
        'class' => 'clear-block',
      ),
    );
    $form['row']['element_type'] = array(
      '#prefix' => '<div class="clear-block"><div class="views-left-30">',
      '#suffix' => '</div>',
      '#title' => 'Element',
      '#type' => 'textfield',
      '#size' => '10',
      '#default_value' => $this->options['row']['element_type'],
    );
    $form['row']['class'] = array(
      '#prefix' => '<div class="views-right-70">',
      '#suffix' => '</div></div>',
      '#title' => 'Class attribute',
      '#type' => 'textfield',
      '#size' => '30',
      '#default_value' => $this->options['row']['class'],
      '#description' => t('Insert a %hash where you want row enumeration.', array('%hash' => '#')),
    );

    // First and last class options.
    $form['row']['first_last'] = array(
      '#type' => 'fieldset',
      '#title' => 'First and last classes',
      // The #parents attribute must be set to avoid another array layer around
      // the group of FIRST/LAST class attribute options.
      '#parents' => array('style_options', 'row'),
      '#description' => t('If the %last_every_nth option is empty or zero, the %first_class and %last_class are added once to only the first and last rows in the pager set. If this option is greater than 1, these classes are added to every n<sup>th</sup> row, which may be useful for grid layouts where the initial and final unit&rsquo;s lateral margins must be 0.', array(
          '%last_every_nth' => 'FIRST/LAST every nth',
          '%first_class' => 'FIRST class attribute',
          '%last_class' => 'LAST class attribute',
        )
      ),
      '#attributes' => array(
        'class' => 'clear-block',
      ),
    );
    $form['row']['first_last']['last_every_nth'] = array(
      '#type' => 'textfield',
      '#size' => '10',
      '#title' => t('FIRST/LAST every n<sup>th</sup>'),
      '#default_value' => $this->options['row']['last_every_nth'],
    );
    $form['row']['first_last']['first_class'] = array(
      '#prefix' => '<div class="views-left-50">',
      '#suffix' => '</div>',
      '#title' => 'FIRST class attribute',
      '#type' => 'textfield',
      '#size' => '30',
      '#default_value' => $this->options['row']['first_class'],
    );
    $form['row']['first_last']['last_class'] = array(
      '#prefix' => '<div class="views-right-50">',
      '#suffix' => '</div>',
      '#title' => 'LAST class attribute',
      '#type' => 'textfield',
      '#size' => '30',
      '#default_value' => $this->options['row']['last_class'],
    );

    // Striping class options.
    $form['row']['striping_classes'] = array(
      '#title' => 'Striping class attributes',
      '#type' => 'textfield',
      '#size' => '30',
      '#default_value' => $this->options['row']['striping_classes'],
      '#description' => 'One striping class attribute is applied to each row. Separate multiple attributes with a space.',
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
