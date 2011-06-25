<?php

// $Id: flvmediaplayer_menu.inc,v 1.1.2.1.2.7 2009/03/02 13:39:21 arthuregg Exp $

/**
 * @FILE this is the menu include- makes tracking changes a bit
 *       easier as well as decreasing the amount of code parsed
 */


/**
 * Implementation of hook_menu().
 * called from flvmediaplayer_menu()
 *
 * @param boolean $may_cache
 * @return array
 */
function flvmediaplayer_menu_inc() {
   // default page
  $items['admin/settings/flvmediaplayer'] = array(
    'title' => 'FLV Media Player settings',
    'description' => 'Settings for the FLV Media Player.',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('flvmediaplayer_admin_settings'),
    'access arguments' => array('administer flvmediaplayer'),
  );

  $items['admin/settings/flvmediaplayer/default'] = array (
    'title' => 'FLV Media Player settings',
    'description' => 'Settings for the FLV Media Player.',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('flvmediaplayer_admin_settings'),
    'access arguments' => array('administer flvmediaplayer'),
    'type' => MENU_DEFAULT_LOCAL_TASK,
  );

  // profiles
  $items['admin/settings/flvmediaplayer/profiles'] = array (
    'title' => 'Profiles',
    'description' => 'Profiles for FLV Media Player.',
    'page callback' => 'flvmediaplayer_admin_profiles',
    'access arguments' => array('administer flvmediaplayer'),
    'type' => MENU_LOCAL_TASK,
    'file' => 'flvmediaplayer_profiles.inc',
  );

  // get all the individual profiles
  if ($profiles = flvmediaplayer_profile_get_configs()) {
    foreach ($profiles as $pid => $name) {
      $items['admin/settings/flvmediaplayer/profile/'. $pid] = array (
        'title' => $name,
        'description' => 'FLV Media Player profile.',
        'page callback' => 'flvmediaplayer_admin_profiles',
        'page arguments' => array(4),
        'access arguments' => array('administer flvmediaplayer'),
        'type' => MENU_LOCAL_TASK,
        'file' => 'flvmediaplayer_profiles.inc',
      );
    }
  }

  // export pages
  $items['admin/settings/flvmediaplayer/export'] = array(
    'page callback' => 'drupal_get_form',
    'page arguments' => array('flvmediaplayer_export_form', 4),
    'access arguments' => array('administer flvmediaplayer'),
    'type' => MENU_LOCAL_TASK,
    'title' => 'Export',
    'description' => 'Export flvmediaplayer profile',
    'weight' => 9,
    'file' => 'flvmediaplayer_io.inc',
  );

  // export pages
  $items['admin/settings/flvmediaplayer/import'] = array(
    'page callback' => 'drupal_get_form',
    'page arguments' => array('flvmediaplayer_import_form', arg(4)),
    'access arguments' => array('administer flvmediaplayer'),
    'type' => MENU_LOCAL_TASK,
    'title' => 'Import',
    'description' => 'Import flvmediaplayer profile',
    'weight' => 9,
    'file' => 'flvmediaplayer_io.inc',
  );

  // external configuration page
  $items['node/%/flvmediaplayer'] = array (
    'page callback' => 'flvmediaplayer_config_xml_page',
    'page arguments' => array(0, 1),
    'access arguments' => array('access content'),
    'type' => MENU_CALLBACK,
  );

  return $items;
}