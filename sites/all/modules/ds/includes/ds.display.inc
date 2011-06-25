<?php
// $Id: ds.display.inc,v 1.1.2.32 2010/01/29 21:11:35 swentel Exp $

/**
 * @file
 * Display overview form.
 */

/**
 * General overview page.
 */
function ds_layout_overview() {
  $content = '';

  drupal_add_css(drupal_get_path('module', 'ds') .'/css/ds.css');

  $rows = array();
  foreach (module_implements('ds_api') as $module) {
    if (user_access('configure layout for '. $module)) {
      $api_info = ds_api_info($module);

      // Build modes.
      $build_modes = ds_get_build_modes($module);

      foreach ($api_info['types']() as $tkey => $otype) {

        $type_name = $otype->type;
        $type = $type_name;
        $type_url_str = str_replace('_', '-', $type_name);

        // Global exclude.
        if (variable_get($module .'_type_'. $type, FALSE)) {
          continue;
        }

        $exclude_build_modes = variable_get($module .'_buildmodes_exclude', array());

        if (!empty($build_modes)) {
          $rows[] = array(array('data' => $otype->name, 'colspan' => '3', 'class' => 'ds-row-title'));
          foreach ($build_modes as $bkey => $build_mode) {

            $excluded = (isset($exclude_build_modes[$type_name][$bkey]) && $exclude_build_modes[$type_name][$bkey] == TRUE) ? TRUE : FALSE;
            if ($excluded) {
              continue;
            }

            // Settings en status.
            $display_settings = variable_get($module .'_display_settings_'. $type, array());
            $settings_status = isset($display_settings[$bkey]['status']) ? $display_settings[$bkey]['status'] : DS_SETTINGS_UI;

            $row = array();
            $row[] = $build_mode['title'];
            $row[] = ds_settings_status($settings_status);

            $operations = l('Edit', 'admin/ds/layout/'. $type .'/'. $bkey);
            if (user_access('revert overridden settings') && $settings_status == DS_SETTINGS_OVERRIDDEN) {
              $operations .= ' - '.l('Revert', 'admin/ds/tools/revert/'.$module .'/'. $type .'/'. $bkey);
            }
            $row[] = $operations;
            $rows[] = array('data' => $row, 'class' => 'ds-row-type');
          }
        }
      }
      if (!empty($rows)) {
        $content = theme('table', array(), $rows, array('id' => 'ds-overview-page'));
      }
    }
  }

  if (empty($content)) {
    $content = t('No items found.');
  }

  return $content;
}

/**
 * Helper function to return readable name of status.
 *
 * @param boolean $status The status.
 */
function ds_settings_status($status) {
  $settings_status_descriptions = array(
    DS_SETTINGS_UI => t('Normal'),
    DS_SETTINGS_DEFAULT => t('Default'),
    DS_SETTINGS_OVERRIDDEN => t('Overridden'),
  );

  return $settings_status_descriptions[$status];
}

/**
 * Menu callback; presents a listing of fields display settings for an object type.
 *
 * Form includes form widgets to select which fields appear for teaser, full node
 * etc and how the field labels should be rendered.
 *
 * This function is based on content_display_overview_form from
 * content.admin.inc of cck, but altered to have weights and regions.
 */
function ds_display_overview_form(&$form_state, $type, $build_mode = 'full', $module = 'nd') {

  // Gather information.
  $api_info = ds_api_info($module);
  $display_settings = variable_get($module .'_display_settings_'. $type, array());
  $settings_status = isset($display_settings[$build_mode]['status']) ? $display_settings[$build_mode]['status'] : DS_SETTINGS_UI;

  $form = array(
    '#tree' => TRUE,
    '#module' => $module,
    '#type_name' => $type,
    '#fields' => array(),
    '#build_mode' => $build_mode,
    '#plugins' => FALSE,
    '#extra' => array(),
    '#status' => $settings_status,
  );

  // Extra info.
  if (isset($api_info['extra'])) {
    $types = $api_info['types']();
    $type_info = $types[$type];
    foreach ($api_info['extra'] as $key) {
      $form['#extra'][$key] = $type_info->{$key};
    }
  }

  // Add the fields.
  ds_fields_display_form($form, $display_settings);

  // Plugins.
  $plugins = TRUE;
  if (isset($api_info['plugins_exclude']) && in_array($build_mode, $api_info['plugins_exclude'])) {
    $plugins = FALSE;
  }
  if ($plugins)
  ds_plugins_display_form($form, $display_settings);

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Add fields to display overview form.
 */
function ds_fields_display_form(&$form, $display_settings) {

  global $base_url;

  $module = $form['#module'];
  $build_mode = $form['#build_mode'];
  $type = $form['#type_name'];
  $extra = $form['#extra'];

  $fields = ds_get_fields($module, $type, $build_mode, $extra, TRUE);

  // Break form build in case no fields
  if (empty($fields)) {
    return;
  }

  $default_properties = ds_field_default_form_properties($build_mode);
  $default = (!empty($form[key($fields)])) ? $form[key($fields)] : $default_properties;

  foreach ($fields as $field => $value) {

    // Add form variable to let the template system know about the fields
    $form['#fields'][] = $field;

    // Every field starts with the default value
    $form[$field] = $default;

    // Populate/override the fields with the saved values
    $form[$field][$build_mode]['label']['format']['#default_value'] = isset($value['display_settings'][$build_mode]['label']['format']) ? $value['display_settings'][$build_mode]['label']['format'] : ds_default_value($display_settings, $build_mode, 'fields', $field, 'labelformat', DS_DEFAULT_LABEL_FORMAT);

    $label_value = ds_default_value($display_settings, $build_mode, 'fields', $field, 'label_value', '');
    $form[$field][$build_mode]['label_value'] = array(
      '#type' => 'hidden',
      '#default_value' => $label_value,
    );
    $form[$field][$build_mode]['label_edit']['#value'] = l(t('Edit'), $base_url . request_uri(), array(
      'attributes' => array('onclick' => 'javascript: Drupal.DisplaySuite.changeLabel(this, "' . $label_value. '"); return false;')
    ));

    $form[$field]['human_name']['#value'] = !empty($label_value) ? $label_value : $value['title'];

    $form[$field]['weight']['#default_value'] = ds_default_value($display_settings, $build_mode, 'fields', $field, 'weight', DS_DEFAULT_WEIGHT);

    $form[$field]['ds_weight']['#default_value'] = ds_default_value($display_settings, $build_mode, 'fields', $field, 'weight', DS_DEFAULT_WEIGHT);

    $form[$field][$build_mode]['format']['#options'] = isset($value['formatters']) ? $value['formatters'] : array();
    $format = ds_default_value($display_settings, $build_mode, 'fields', $field, 'format', '');
    if (empty($format) && isset($value['formatters'])) {
      $format = key($value['formatters']);
    }
    $form[$field][$build_mode]['format']['#default_value'] = $format;
    $form[$field][$build_mode]['format']['#access'] = (count($form[$field][$build_mode]['format']['#options']) == 0) ? FALSE : TRUE;

    $form[$field][$build_mode]['region']['#default_value'] = ds_default_value($display_settings, $build_mode, 'fields', $field, 'region', DS_DEFAULT_REGION);
  }
}

/**
 * Add plugins to display overview form.
 *
 * @param array $form The display form.
 * @param array $display_settings Current display settings.
 */
function ds_plugins_display_form(&$form, $display_settings) {
  $module = $form['#module'];
  $plugins = variable_get($module .'_plugin_settings', array());
  if (!empty($plugins)) {
    $form['#plugins'] = TRUE;
    $form['#plugin_keys'] = array();
  }

  foreach ($plugins as $key => $data) {
    if (isset($data['filename'])) {
      require_once($data['filename']);
    }
    $class = $data['class'];
    $plugin = new $class();
    $plugin_form = $plugin->plugin_form($form, $display_settings);
    $form['#plugin_keys'][$key] = $data['title'];
  }
}

/**
 * Save fields & plugins for a build mode.
 */
function ds_display_overview_form_submit($form, &$form_state) {

  $module = $form['#module'];
  $build_mode = $form['#build_mode'];
  $type = $form['#type_name'];
  $extra = $form['#extra'];

  $display_settings = variable_get($module .'_display_settings_'. $type, array());
  unset($display_settings[$build_mode]);

  // Fields.
  $fields = ds_get_fields($module, $type, $build_mode, $extra);
  if (!empty($fields)) {
    foreach ($fields as $key => $field) {
      $display_settings[$build_mode]['fields'][$key]['weight'] = $form_state['values'][$key]['ds_weight'];
      $display_settings[$build_mode]['fields'][$key]['format'] = $form_state['values'][$key][$build_mode]['format'];
      $display_settings[$build_mode]['fields'][$key]['region'] = $form_state['values'][$key][$build_mode]['region'];
      if (!empty($form_state['values'][$key][$build_mode]['label_value'])) {
      $display_settings[$build_mode]['fields'][$key]['label_value'] = $form_state['values'][$key][$build_mode]['label_value'];
      }
      $display_settings[$build_mode]['fields'][$key]['labelformat'] = $form_state['values'][$key][$build_mode]['label']['format'];

      // Ditch this field, unless it has its storage in another module (ie CCK)
      if (!isset($field['storage']))
      unset($form_state['values'][$key]);
    }
  }

  // Plugins
  if ($form['#plugins'] == TRUE)
  ds_plugins_display_submit($form, $form_state, $display_settings);

  // Status.
  $status = ($form['#status'] == DS_SETTINGS_DEFAULT) ? DS_SETTINGS_OVERRIDDEN : $form['#status'];
  $display_settings[$build_mode]['status'] = $status;

  // Save all these settings.
  variable_set($module .'_display_settings_'. $type, $display_settings);

  drupal_set_message(t('Your settings have been saved.'));
}

/**
 * Save plugins settings.
 *
 * @param array $form The display form.
 * @param array $form_state The submitted values of the form.
 * @param array $display_settings Current display settings being saved.
 */
function ds_plugins_display_submit($form, $form_state, &$display_settings) {
  $module = $form['#module'];

  $plugins = variable_get($module .'_plugin_settings', array());

  foreach ($plugins as $key => $data) {

    if (isset($data['filename'])) {
      require_once($data['filename']);
    }

    $class = $data['class'];
    $plugin = new $class();
    $plugin->plugin_form_submit($form, $form_state, $display_settings);
  }
}

/**
 * Return array of available label options.
 */
function ds_label_options() {
  return array(
    'above' => t('Above'),
    'inline' => t('Inline'),
    'hidden' => t('Hidden'),
  );
}

/**
 * Function to load default form properties for a field in a context
 */
function ds_field_default_form_properties($build_mode) {

  $field = array(
    'label' => array(
      'format' => array(
        '#type' => 'select',
        '#options' => ds_label_options(),
        '#default_value' => DS_DEFAULT_LABEL_FORMAT,
      ),
    ),
    'format' => array(
      '#type' => 'select',
      '#options' => array(),
      '#default_value' => DS_DEFAULT_FORMAT,
    ),
    'region' => array(
      '#type' => 'select',
      '#options' => ds_regions(variable_get('ds_build_mode_'. $build_mode, 'all')),
      '#default_value' => DS_DEFAULT_REGION,
      '#attributes' => array(
        'class' => 'field-region-select field-region-'. DS_DEFAULT_REGION,
      ),
    ),
  );

  // If build mode is not empty, this means we're looking at
  // a content type which has no cck fields defined. Add
  // ds_weight and build mode key to the default field array
  // so the draghandling & formatters start working.
  if (!empty($build_mode)) {
    $field['ds_weight'] = array(
      '#type' => 'weight',
      '#default_value' => DS_DEFAULT_WEIGHT,
      '#delta' => 30,
    );
    $field[$build_mode] = array(
      'label' => array(
        'format' => array(
          '#type' => 'select',
          '#options' => ds_label_options(),
          '#default_value' => DS_DEFAULT_LABEL_FORMAT,
        ),
      ),
      'format' => array(
        '#type' => 'select',
        '#options' => array(),
        '#default_value' => DS_DEFAULT_FORMAT,
      ),
      'region' => array(
        '#type' => 'select',
        '#options' => ds_regions(variable_get('ds_build_mode_'. $build_mode, 'all')),
        '#default_value' => DS_DEFAULT_WEIGHT,
        '#attributes' => array(
          'class' => 'field-region-select field-region-'. DS_DEFAULT_REGION,
        ),
      ),
    );
  }

  return $field;
}
