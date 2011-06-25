<?php
// $Id: ds.fields.inc,v 1.1.2.24 2010/01/30 17:05:08 swentel Exp $

/**
 * @file
 * Manage fields.
 */

/**
 *
 * @param integer $integer An integer which identifies a constant.
 * @param string $type A string to identify the category.
 */
function ds_human_name($integer, $type) {
  $human_names = array(
    'type' => array(
      DS_FIELD_TYPE_CODE => t('Code field'),
      DS_FIELD_TYPE_BLOCK => t('Block field'),
    ),
    'status' => array(
      DS_FIELD_STATUS_DEFAULT => t('Default'),
      DS_FIELD_STATUS_CUSTOM => t('Custom'),
      DS_FIELD_STATUS_OVERRIDDEN => t('Overridden'),
    ),
  );
  return $human_names[$type][$integer];
}

/**
 * Fields overview.
 */
function ds_fields($module) {
  $output = '';
  $action = arg(4);
  $field = arg(5);

  drupal_add_js(drupal_get_path('module', 'ds') .'/js/fields.js');

  // Get API information.
  $api_info = ds_api_info($module);

  // Extra info.
  $extra = array();

  // Get fields.
  $db_fields = variable_get($module .'_fields', array());
  $all_fields = ds_get_fields($module, NULL, NULL, $extra, TRUE);

  // Compare with db_fields.
  foreach ($all_fields as $key => $ds_field) {
    if ($ds_field['status'] == DS_FIELD_STATUS_DEFAULT && !isset($db_fields[$key])) {
      $db_fields[$key] = array(
        'title' => $ds_field['title'],
        'properties' => $ds_field['properties'],
        'type' => $ds_field['type'],
        'status' => $ds_field['status'],
        'exclude' => isset($ds_field['exclude']) ? $ds_field['exclude'] : array(),
      );
    }
  }

  // Delete form.
  if (in_array($field, array_keys($db_fields)) && $action == 'delete') {
    $output .= drupal_get_form('ds_field_delete_reset_form', $module, $field, $db_fields[$field]);
  }
  // Edit form.
  elseif (in_array($field, array_keys($db_fields)) && $action == 'edit') {
    if ($db_fields[$field]['type'] == DS_FIELD_TYPE_BLOCK) $form_id = 'ds_block_field_form';
    else $form_id = 'ds_code_field_form';
    $output .= drupal_get_form($form_id, $module, $all_fields, $api_info['object'], $field, $db_fields[$field]);
  }
  // Custom fields overview form
  else {
    if (empty($db_fields)) {
      $output .= '<p>'. t('You have not defined any custom fields.') .'</p>';
    }
    else {
      $output .= '<p>'. t('This is a list of custom fields either defined in code or via the UI.') .'</p>';

      $header = array(
        t('Title'), t('Key'), t('Type'), t('Status'), t('Operations'),
      );

      $rows = array();
      foreach ($db_fields as $key => $value) {
        $row = array();
        $row[] = check_plain($value['title']);
        $row[] = $key;
        $row[] = ds_human_name($value['type'], 'type');
        $row[] = ds_human_name($value['status'], 'status');
        $operations = l(t('Edit'), 'admin/ds/'. $module .'/fields/edit/'. $key);
        if ($value['status'] != DS_FIELD_STATUS_DEFAULT) {
          $text = ($value['status'] == DS_FIELD_STATUS_OVERRIDDEN) ? 'Reset' : 'Delete';
          $operations .= ' - '. l(t($text), 'admin/ds/'. $module .'/fields/delete/'. $key);
        }
        $row[] = $operations;
        $rows[] = $row;
      }

      $output .= theme('table', $header, $rows);
    }

    // New fields form.
    $output .= drupal_get_form('ds_code_field_form', $module, $all_fields, $api_info['object']);
    // Block field form.
    $output .= drupal_get_form('ds_block_field_form', $module, $all_fields);
  }

  return $output;
}

/**
 * Code field form.
 *
 * @param string $module The module the field is for.
 * @param array $all_fields All fields for this module.
 * @param string $object The object name (ie node, user, comment).
 * @param string $key The key of the field.
 * @param array $field The field with title and code keys.
 */
function ds_code_field_form($form_state, $module, $all_fields, $object = '', $key = '', $field = array()) {
  $form = array();

  if (empty($field)) {
    $field = array(
      'title' => '',
      'exclude' => array(),
      'type' => DS_FIELD_TYPE_CODE,
      'status' => DS_FIELD_STATUS_CUSTOM,
      'properties' => array('code' => '')
    );
  }

  $form['code_identity'] = array(
    '#type' => 'fieldset',
    '#title' => (empty($key)) ? t('Add new code field'): t('Update code field'),
    '#collapsible' => (empty($key)) ? TRUE : FALSE,
    '#collapsed' => (empty($key)) ? TRUE : FALSE,
  );
  $form['code_identity']['code_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Field key'),
    '#description' => t('The machine-readable name of this field.'),
    '#required' => TRUE,
  );
  if (!empty($key)) {
    $form['code_identity']['code_key']['#disabled'] = TRUE;
    $form['code_identity']['code_key']['#value'] = $key;
    $form['code_identity']['code_key']['#description'] = t('The machine-readable name of this field. Note: you can not edit this field.');
  }
  $form['code_identity']['code_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Field title'),
    '#description' => t('The title to use when rendering the output and on the display administration screen.'),
    '#required' => TRUE,
    '#default_value' => $field['title']
  );
  $api_info = ds_api_info($module);
  if (isset($api_info['types']) && count($api_info['types']()) > 1) {
    $types = array();
    foreach ($api_info['types']() as $tkey => $type) {
      $types[$tkey] = $type->name;
    }
    $form['code_identity']['code_exclude'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Field exclude'),
      '#options' => $types,
      '#default_value' => $field['exclude'],
      '#attributes' => array('class' => 'exclude-types')
    );
    $form['code_identity']['code_exclude_all'] = array(
      '#type' => 'checkbox',
      '#title' => t('Select all'),
      '#description' => t('Toggle types which you don\'t want the field to appear in.'),
      '#attributes' => array('class' => 'select-all')
    );
  }
  else {
    $form['code_identity']['code_exclude'] = array(
      '#type' => 'value',
      '#value' => array(),
    );
  }
  $form['code_identity']['code_code'] = array(
    '#type' => 'textarea',
    '#title' => t('Field code'),
    '#required' => TRUE,
    '#default_value' => $field['properties']['code'],
  );

  _ds_field_object_info($form, $object);

  $form['code_identity']['code_submit'] = array(
    '#type' => 'submit',
    '#submit' => array('ds_code_field_form_submit'),
    '#value' => t('Save code field'),
  );

  $form['#field_status'] = ($field['status'] == DS_FIELD_STATUS_DEFAULT) ? DS_FIELD_STATUS_OVERRIDDEN : (($field['type'] == DS_FIELD_STATUS_OVERRIDDEN) ? DS_FIELD_STATUS_OVERRIDDEN : DS_FIELD_STATUS_CUSTOM);
  $form['#form_type'] = (empty($key)) ? 'insert' : 'update';
  $form['#module'] = $module;
  $form['#all_fields'] = $all_fields;

  return $form;
}

/**
 * Return info about the value field, including token info.
 * @param array $form The current form.
 * @param string $object The object name (ie node, user, comment)
 */
function _ds_field_object_info(&$form, $object) {
  $form['code_identity']['help'] = array(
    '#type' => 'item',
    '#value' => t('Every field will be wrapped in &lt;div class="field field-key"&gt;VALUE&lt;/div&gt;.<br/>Enter PHP code between &lt;?php ?&gt;. If you are using PHP, the variable for the '. $object .' is called $object.<br/>Note that executing incorrect PHP-code can break your Drupal site.'),
  );

  if (module_exists('token')) {
    $form['code_identity']['help']['tokens'] = array(
      '#type' => 'fieldset',
      '#title' => t('Placeholder tokens'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['code_identity']['help']['tokens']['info'] = array(
      '#type' => 'item',
      '#value' => theme('token_help', $object),
    );
  }
  else {
    $form['code_identity']['help']['#value'] .= '<br />'. t("To use dynamic placeholder tokens in your custom fields (the ID or title of the current node, for example), download and install the <a href='@token'>Token module</a> from Drupal.org.", array('@token' => 'http://www.drupal.org/project/token'));
  }
}

/**
 * Validate code field submission, only on insert.
 */
function ds_code_field_form_validate($form, &$form_state) {
  if ($form['#form_type'] == 'insert') {

    $module = $form['#module'];
    $fields = $form['#all_fields'];
    $existing = array_keys($fields);

    if (in_array($form_state['values']['code_key'], $existing)) {
      form_set_error('code_key', t('This field already exists.'));
    }

    if (!preg_match('!^[a-z_]+$!', $form_state['values']['code_key'])) {
      form_set_error('code_key', t('The machine-readable name must contain only lowercase letters and underscores.'));
    }
  }
}

/**
 * Save code field.
 */
function ds_code_field_form_submit($form, &$form_state) {
  $module = $form['#module'];
  $fields = variable_get($module .'_fields', array());
  $fields[$form_state['values']['code_key']] = array(
    'title' => $form_state['values']['code_title'],
    'exclude' => $form_state['values']['code_exclude'],
    'type' => DS_FIELD_TYPE_CODE,
    'status' => $form['#field_status'],
    'properties' => array(
      'code' => $form_state['values']['code_code']
    ),
  );

  // Safe field and invalidate cached fields.
  variable_set($module .'_fields', $fields);
  variable_del('ds_fields_cached');

  // Redirect and confirmation message.
  $form_state['redirect'] = 'admin/ds/'. $form['#module'] .'/fields';
  drupal_set_message(t('Field %field has been saved.', array('%field' => $form_state['values']['code_title'])));
}

/**
 * Block field form.
 *
 * @param string $module The module the field is for.
 * @param array $all_fields All fields for this module.
 * @param string $object The object name (ie node, user, comment).
 * @param string $key The key of the field.
 * @param array $field The field with title and code keys.
 */
function ds_block_field_form($form_state, $module, $all_fields, $object = '', $key = '', $field = array()) {
  $form = array();

  if (empty($field)) {
    $field = array(
      'title' => '',
      'exclude' => array(),
      'type' => DS_FIELD_TYPE_BLOCK,
      'status' => DS_FIELD_STATUS_CUSTOM,
      'properties' => array(
        'block' => '',
        'render' => DS_BLOCK_TEMPLATE
      )
    );
  }

  $form['block_identity'] = array(
    '#type' => 'fieldset',
    '#title' => (empty($key)) ? t('Add new block field'): t('Update block field'),
    '#collapsible' => (empty($key)) ? TRUE : FALSE,
    '#collapsed' => (empty($key)) ? TRUE : FALSE,
  );
  $form['block_identity']['block_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Field key'),
    '#description' => t('The machine-readable name of this field.'),
    '#required' => TRUE,
  );
  if (!empty($key)) {
    $form['block_identity']['block_key']['#disabled'] = TRUE;
    $form['block_identity']['block_key']['#value'] = $key;
    $form['block_identity']['block_key']['#description'] = t('The machine-readable name of this field. Note: you can not edit this field.');
  }
  $form['block_identity']['block_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Field title'),
    '#description' => t('The title to use when rendering the output and on the display administration screen.'),
    '#required' => TRUE,
    '#default_value' => $field['title']
  );
  $api_info = ds_api_info($module);
  if (isset($api_info['types']) && count($api_info['types']()) > 1) {
    $types = array();
    foreach ($api_info['types']() as $tkey => $type) {
      $types[$tkey] = $type->name;
    }
    $form['block_identity']['block_exclude'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Field exclude'),
      '#options' => $types,
      '#default_value' => $field['exclude'],
      '#attributes' => array('class' => 'exclude-types')
    );
    $form['block_identity']['block_exclude_all'] = array(
      '#type' => 'checkbox',
      '#title' => t('Select all'),
      '#description' => t('Toggle types which you don\'t want the field to appear in.'),
      '#attributes' => array('class' => 'select-all')
    );
  }
  else {
    $form['code_identity']['code_exclude'] = array(
      '#type' => 'value',
      '#value' => array(),
    );
  }

  $blocks = array();
  foreach (module_list() as $drupal_module) {
    $module_blocks = module_invoke($drupal_module, 'block', 'list');
    if ($module_blocks) {
      foreach ($module_blocks as $module_key => $info) {
        $blocks[ucfirst($drupal_module)][$drupal_module .'|'. $module_key] = $info['info'];
      }
    }
  }
  ksort($blocks);

  $form['block_identity']['block_block'] = array(
    '#type' => 'select',
    '#options' => $blocks,
    '#title' => t('Block'),
    '#required' => TRUE,
    '#default_value' => $field['properties']['block']
  );
  $form['block_identity']['block_render'] = array(
    '#type' => 'select',
    '#options' => array(
      DS_BLOCK_TEMPLATE => t('Render through block template'),
      DS_BLOCK_TITLE_CONTENT => t('Show block title + content'),
      DS_BLOCK_CONTENT => t('Show only block content'),
    ),
    '#title' => t('Block render'),
    '#required' => TRUE,
    '#default_value' => $field['properties']['render'],
  );

  $form['block_identity']['block_submit'] = array(
    '#type' => 'submit',
    '#submit' => array('ds_block_field_form_submit'),
    '#value' => t('Save block field'),
  );

  $form['#field_status'] = ($field['status'] == DS_FIELD_STATUS_DEFAULT) ? DS_FIELD_STATUS_OVERRIDDEN : (($field['type'] == DS_FIELD_STATUS_OVERRIDDEN) ? DS_FIELD_STATUS_OVERRIDDEN : DS_FIELD_STATUS_CUSTOM);
  $form['#form_type'] = (empty($key)) ? 'insert' : 'update';
  $form['#module'] = $module;
  $form['#all_fields'] = $all_fields;

  return $form;
}

/**
 * Validate block field submission, only on insert.
 */
function ds_block_field_form_validate($form, &$form_state) {
  if ($form['#form_type'] == 'insert') {

    $module = $form['#module'];
    $fields = $form['#all_fields'];
    $existing = array_keys($fields);

    if (in_array($form_state['values']['block_key'], $existing)) {
      form_set_error('block_key', t('This field already exists.'));
    }

    if (!preg_match('!^[a-z_]+$!', $form_state['values']['block_key'])) {
      form_set_error('block_key', t('The machine-readable name must contain only lowercase letters and underscores.'));
    }
  }
}

/**
 * Save block field.
 */
function ds_block_field_form_submit($form, &$form_state) {
  $module = $form['#module'];
  $fields = variable_get($module .'_fields', array());
  $fields[$form_state['values']['block_key']] = array(
    'type' => DS_FIELD_TYPE_BLOCK,
    'status' => $form['#field_status'],
    'title' => $form_state['values']['block_title'],
    'exclude' => $form_state['values']['block_exclude'],
    'properties' => array(
      'block' => $form_state['values']['block_block'],
      'render' => $form_state['values']['block_render'],
    ),
  );

  // Safe field and invalidate cached fields.
  variable_set($module .'_fields', $fields);
  variable_del('ds_fields_cached');

  // Redirect and confirmation message.
  $form_state['redirect'] = 'admin/ds/'. $form['#module'] .'/fields';
  drupal_set_message(t('Field %field has been saved.', array('%field' => $form_state['values']['block_title'])));
}

/**
 * Field delete or reset form.
 *
 * @param string $module The module the fields are for.
 * @param string $object The object name (ie node, user, comment).
 * @param string $key The key of the field.
 * @param array $field The field with title and code keys.
 */
function ds_field_delete_reset_form($form_state, $module, $key, $field) {
  $form = array();

  $action = ($field['status'] == DS_FIELD_STATUS_OVERRIDDEN) ? t('reset') : t('delete');
  $form['question'] = array(
    '#type' => 'markup',
    '#value' => '<p>'. t('Are you sure you want to !action the field %field ?', array('!action' => $action, '%field' => $field['title'])) .'</p>',
  );
  $form['field'] = array(
    '#type' => 'value',
    '#value' => $key,
  );
  $form['button']['submit'] = array(
    '#prefix' => '<div>',
    '#type' => 'submit',
    '#value' => ($field['status'] == DS_FIELD_STATUS_OVERRIDDEN) ? t('Reset') : t('Delete'),
  );
  $form['buttons']['cancel'] = array(
    '#suffix' => '</div>',
    '#type' => 'markup',
    '#value' => l('cancel', 'admin/ds/'. $module .'/fields'),
  );
  $form['#module'] = $module;
  return $form;
}

/**
 * Delete or reset a field.
 */
function ds_field_delete_reset_form_submit($form, &$form_state) {
  $module = $form['#module'];
  $fields = variable_get($module .'_fields', array());
  $field = $fields[$form_state['values']['field']];
  $field_title = $field['title'];
  $action = ($field['status'] == DS_FIELD_STATUS_OVERRIDDEN) ? t('reset') : t('deleted');

  // Save and invalidate fields_cached.
  unset($fields[$form_state['values']['field']]);
  variable_set($module .'_fields', $fields);
  variable_del('ds_fields_cached');

  drupal_set_message(t('Field %field has been !action.', array('%field' => $field_title, '!action' => $action)));
  $form_state['redirect'] = 'admin/ds/'. $form['#module'] .'/fields';
}
