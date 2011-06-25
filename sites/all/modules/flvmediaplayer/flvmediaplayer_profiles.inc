<?php

// $Id: flvmediaplayer_profiles.inc,v 1.1.2.2.2.5 2009/01/21 22:29:28 arthuregg Exp $

/* ************************************************************** */
/* PROFILE FUNCTIONS */
/* ************************************************************** */


/**
 * Creates the profile listing page
 * @param int $pid is a profile id
 * @return string
 */
function flvmediaplayer_admin_profiles ($pid = null) {
  // return a profile if it is passed in
  if ($pid) {
    // get config data to pass to the form
    $configuration = flvmediaplayer_profile_get_config($pid);
    drupal_set_title(t('Edit profile: !name', array('!name' => $configuration['name'])));
    return drupal_get_form('flvmediaplayer_profile_edit_form', $configuration);
  }

  // set title for the profile lists
  drupal_set_title(t('Profile list'));
  // get the create form
  $output = drupal_get_form('flvmediaplayer_profile_create_form');
  // get existing profiles
  if ($configurations = flvmediaplayer_profile_get_configs()) {
    foreach($configurations as $pid => $name) {
      $rows[] = array($pid, l($name, 'admin/settings/flvmediaplayer/profiles/'. $pid));
    }
    $header = array(t('Profile Id'), t('Name (edit)'));
    $output .= theme('table', $header, $rows);
  }
  return $output;
}


/**
 * form for creating a new profile
 */
function flvmediaplayer_profile_create_form() {
  $form = array();
  $form['create'] = array(
    '#type' => 'fieldset',
    '#title' => t('Create new profile'),
  );
  $form['create']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#description' => t('Give your profile a distinct name. Use _ for spaces if you need them.'),
  );
  $form['create']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Create'),
  );
  return $form;
}


/**
 * validates the name on the create
 */
function flvmediaplayer_profile_create_form_validate($form, &$form_state) {
  if (strstr($form_state['values']['name'], ' ')) {
    form_set_error('name', t('Please do not use spaces in the name.'));
  }
  // check to make sure this name is not in use
  $result = flvmediaplayer_profile_get_config($form_state['values']['name']);
  if ($result['name']) {
    form_set_error('name', t('This name is already in use, please choose another.'));
  }
}


/**
 * submits a new profile and returns user to the edit screen
 * @param string $from_id
 *   the form id being passed by drupal
 * @param array $form_values
 *   full drupal form array
 * @return string drupal goto
 */
function flvmediaplayer_profile_create_form_submit($form, &$form_state) {
  foreach ($form as $key => $value) {
    if (strstr($key, 'flvmp_')) {
      $data[str_replace('flvmp_', '', $key)] = $value;
    }
  }
  $data = serialize($data);
  $type = 'profile';
  db_query('INSERT INTO {flvmediaplayer_profiles} (type, name, config) VALUES ("%s", "%s", "%s")', $type, $form_state['values']['name'], $data);
  $pid = db_last_insert_id('flvmediaplayer_profiles', 'pid');
  drupal_set_message(t('Created your profile !name', array('!name' => $form_state['values']['name'])));
  $form_state['redirect'] = 'admin/settings/flvmediaplayer/profiles/'. $pid;
}


/**
 * handles the profile config save/delete
 */
function flvmediaplayer_profile_edit_form(&$form_state, $configuration) {
  $form = array();
  // create the fieldset
  $form['profiles'] = array(
    '#type' => 'fieldset',
    '#title' => $configuration['name'],
  );
  $form['profiles']['flvmp_name'] = array(
    '#type' => 'textfield',
    '#default_value' => $configuration['name'],
    '#description' => t('The name of your configuration'),
  );

  // grab the settings form
  $form['profiles']['data'] = flvmdiaplayer_get_player_form($configuration);

  // set the pid
  $form['profiles']['flvmp_pid'] = array(
    '#type' => 'hidden',
    '#value' => $configuration['pid'],
  );

  $form['profiles']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  $form['profiles']['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#submit' => array('flvmediaplayer_profile_delete_submit'),
  );
  return $form;
}


/**
 * handles the submit of an individual profile form
 */
function flvmediaplayer_profile_edit_form_submit($form, &$form_state) {
  // loop through all the form data and strip out form prefixes
  foreach ($form_state['values'] as $name => $value) {
    if (strstr($name, 'flvmp_')) {
      $data[str_replace('flvmp_', '', $name)] = $value;
    }
  }
  db_query('UPDATE {flvmediaplayer_profiles} SET name = "%s", config = "%s" WHERE pid = %d', $data['name'], serialize($data), $data['pid']);
  // clear cached nodes
  cache_clear_all('flvmp_%'. $data['name'], 'cache_flvmediaplayer', true);
  drupal_set_message(t('Cleared configuration cache'));
  drupal_set_message(t('Saved your player profile'));
}


/**
 * Delete the requested profile
 * @param $form
 * @param $form_state
 */
function flvmediaplayer_profile_delete_submit($form, &$form_state) {
  cache_clear_all('flvmp_%'. $form_state['values']['name'], 'cache_flvmediaplayer', true);
  db_query('DELETE FROM {flvmediaplayer_profiles} WHERE pid = %d', $form_state['values']['pid']);
  drupal_set_message(t('Deleted your player profile'));
  $form_state['redirect'] = 'admin/settings/flvmediaplayer/profiles';
}