<?php
// $Id: ds.registry.inc,v 1.1.2.24 2010/01/29 14:22:40 swentel Exp $

/**
 * @file
 * Registry functions.
 */

/**
 * Return permissions.
 */
function _ds_perm() {
  $perms = array(
    'access display suite',
    'use PHP in custom fields',
    'revert overridden settings',
    'export and import settings',
  );
  foreach (module_implements('ds_api') as $module) {
    $perms[] = 'administer '. $module;
    $perms[] = 'configure layout for '. $module;
  }
  return $perms;
}

/**
 * Return menu items and import default settings.
 */
function _ds_menu() {

  ds_import_default_data();

  $items = array();
  $all_build_modes = ds_get_build_modes(NULL, TRUE);

  // Display suite main menu block.
  $items['admin/ds'] = array(
    'title' => 'Display suite',
    'description' => 'Manage your site display.',
    'page callback' => 'ds_general_page',
    'access arguments' => array('access display suite'),
    'file' => 'includes/ds.admin.inc',
  );

  // Import / Export
  $items['admin/ds/tools'] = array(
    'title' => 'Tools',
    'description' => 'Tools for Display suite',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('ds_import'),
    'access arguments' => array('export and import settings'),
    'file' => 'includes/ds.tools.inc',
    'weight' => -1,
  );
  $items['admin/ds/tools/import'] = array(
    'title' => 'Import',
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => 0,
  );
  $items['admin/ds/tools/export'] = array(
    'title' => 'Export',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('ds_export'),
    'access arguments' => array('export and import settings'),
    'file' => 'includes/ds.tools.inc',
    'type' => MENU_LOCAL_TASK,
    'weight' => 1,
  );
  $items['admin/ds/tools/revert'] = array(
    'title' => 'Revert',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('ds_revert_form'),
    'access arguments' => array('revert overridden settings'),
    'file' => 'includes/ds.tools.inc',
    'type' => MENU_CALLBACK,
  );

  // Display layout menu item.
  $items['admin/ds/layout'] = array(
    'title' => 'Layout',
    'page callback' => 'ds_layout_overview',
    'access arguments' => array('access display suite'),
    'file' => 'includes/ds.display.inc',
    'weight' => 2,
  );

  foreach (module_implements('ds_api') as $module) {
    $api_info = ds_api_info($module);
    $module = $api_info['module'];
    $title = $api_info['title'];

    // Info.
    $items['admin/ds/'. $module] = array(
      'title' => $title,
      'page callback' => 'ds_info',
      'page arguments' => array($module),
      'access arguments' => array('administer '. $module),
      'file' => 'includes/ds.info.inc',
    );
    $items['admin/ds/'. $module .'/info'] = array(
      'title' => 'Info',
      'type' => MENU_DEFAULT_LOCAL_TASK,
      'weight' => 0,
    );

    // Fields.
    $items['admin/ds/'. $module .'/fields'] = array(
      'title' => 'Fields',
      'page callback' => 'ds_fields',
      'page arguments' => array($module),
      'access callback' => '_ds_menu_access',
      'access arguments' => array('administer '. $module, 'use PHP in custom fields'),
      'file' => 'includes/ds.fields.inc',
      'type' => MENU_LOCAL_TASK,
      'weight' => 1,
    );

    // Build modes.
    $items['admin/ds/'. $module .'/buildmodes'] = array(
      'title' => 'Build modes',
      'page callback' => 'ds_build_modes',
      'page arguments' => array($module),
      'access arguments' => array('administer '. $module),
      'file' => 'includes/ds.buildmodes.inc',
      'type' => MENU_LOCAL_TASK,
      'weight' => 2,
    );

    // Plugins.
    $items['admin/ds/'. $module .'/plugins'] = array(
      'title' => 'Plugins',
      'page callback' => 'drupal_get_form',
      'page arguments' => array('ds_plugins', $module),
      'access arguments' => array('administer '. $module),
      'file' => 'includes/ds.plugins.inc',
      'type' => MENU_LOCAL_TASK,
      'weight' => 3,
    );

    // Displays.
    $build_modes = $all_build_modes[$module];
    $first_build_mode = key($build_modes);
    if (!empty($build_modes)) {
      foreach ($api_info['types']() as $tkey => $type) {

        $type_name = $type->type;
        $type_url_str = str_replace('_', '-', $type_name);

        // Global exclude.
        if (variable_get($module .'_type_'. $type->type, FALSE)) {
          continue;
        }

        $first = 0;
        $weight = 20;
        $local_task_items = array();
        $exclude_build_modes = variable_get($module .'_buildmodes_exclude', array());
        foreach ($build_modes as $key => $value) {

          // Check if build mode is excluded for this object type.
          $excluded = (isset($exclude_build_modes[$type_name][$key]) && $exclude_build_modes[$type_name][$key] == TRUE) ? TRUE : FALSE;
          if ($excluded) {
            continue;
          }

          // Tabs.
          $local_task_items['admin/ds/layout/'. $type_url_str .'/'. $key] = array(
            'title' => $value['title'],
            'page callback' => 'drupal_get_form',
            'page arguments' => array('ds_display_overview_form', $type_name, "$key", $module),
            'access arguments' => array('configure layout for '. $module),
            'type' => $first == 0 ? MENU_DEFAULT_LOCAL_TASK : MENU_LOCAL_TASK,
            'file' => 'includes/ds.display.inc',
            'weight' => (isset($value['weight']) && !empty($value['weight'])) ? $value['weight'] : $weight++,
          );

          // Store the first build_mode
          if ($first == 0) {
            $first++;
            $first_build_mode = $key;
          }
        }

        // Default callback.
        $items['admin/ds/layout/'. $type_url_str] = array(
          'title' => drupal_ucfirst($type_url_str),
          'page callback' => 'drupal_get_form',
          'page arguments' => array('ds_display_overview_form', $type_name, "$first_build_mode", $module),
          'access arguments' => array('configure layout for '. $module),
          'file' => 'includes/ds.display.inc',
          'weight' => 2,
        );
        $items += $local_task_items;

      }
    }
  }

  return $items;
}

/**
 * Return theme functions.
 */
function _ds_theme() {
  $path = drupal_get_path('module', 'ds');

  $theme_functions = array(
    // Display overview form.
    'ds_display_overview_form' => array(
      'template' => 'ds-display-overview-form',
      'file' => 'theme.inc',
      'path' => $path .'/theme',
      'arguments' => array('form' => NULL),
    ),
    // Build modes matrix.
    'ds_buildmodes_matrix_form' => array(
      'file' => 'ds.buildmodes.inc',
      'path' => $path .'/includes',
      'arguments' => array('form' => NULL),
    ),
    // Plugins form.
    'ds_plugins' => array(
      'file' => 'ds.plugins.inc',
      'path' => $path .'/includes',
      'arguments' => array('form' => NULL),
    ),

    // General theming field.
    'ds_field' => array(
      'arguments' => array(
        'content' => NULL,
        'field_key' => NULL,
        'field' => NULL,
      ),
      'file' => 'theme.inc',
      'path' => $path .'/theme',
    ),


    // Theming for fields which are shared across objects (name, picture etc).
    'ds_author_link' => array(
      'arguments' => array('object' => NULL),
      'file' => 'theme.inc',
      'path' => $path .'/theme',
    ),
    'ds_author_nolink' => array(
      'arguments' => array('object' => NULL),
      'file' => 'theme.inc',
      'path' => $path .'/theme',
    ),
  );

  return $theme_functions;
}

/**
 * Register build modes.
 */
function _ds_register_build_modes() {

  $weight = 10;
  $build_modes = array();

  // Build modes defined in hooks.
  foreach (module_implements('content_build_modes') as $module) {

    $module_data = array();
    $function = $module .'_content_build_modes';
    $temp_data = $function();
    // We need to do some fiddling existing build modes.
    if (!empty($temp_data)) {
      foreach ($temp_data as $key => $value) {
        // Sane build modes.
        foreach ($value['build modes'] as $build_mode => $build_mode_value) {
          // If no module key is set, we are adding it ourselves. This hook only applies
          // to nodes anyway, so yes, we use nd as the $module key.
          $module_name = isset($build_mode_value['module']) ? $build_mode_value['module'] : 'nd';
          $build_modes[$module_name][$build_mode] = array(
            'weight' => isset($build_mode_value['weight']) ? $build_mode_value['weight'] : $weight++,
            'title' => $build_mode_value['title'],
          );
        }
      }
    }
  }

  // Custom build modes via the UI.
  foreach (module_implements('ds_api') as $module) {
    $weight = 10;
    $db_build_modes = variable_get($module .'_build_modes', array());
    if (!empty($db_build_modes)) {
      foreach ($db_build_modes as $key => $name) {
        $build_modes[$module][$key] = array(
          'title' => check_plain($name),
          'build modes' => array(
            $key => array(
              'weight' => $weight++,
              'title' => check_plain($name),
            ),
          ),
        );
      }
    }
    asort($build_modes[$module]);
  }

  // Give modules a change to alter build_modes.
  drupal_alter('ds_buildmodes', $build_modes);

  // Cache all build modes.
  variable_set('ds_all_build_modes', $build_modes);

  return $build_modes;
}

/**
 * Return ds plugins.
 */
function _ds_plugins() {
  $path = drupal_get_path('module', 'ds');
  return array(
    'emptyregionrender' => array(
      'title' => t('Empty region'),
      'class' => 'ds_emptyregionrender',
      'description' => t('Renders a region when there is no content in it.'),
      'file' => 'ds_emptyregionrender.inc',
      'path' => $path .'/plugins',
      'target' => 'all',
    ),
    'cssoverrider' => array(
      'title' => t('Simple CSS overrider'),
      'class' => 'ds_cssoverrider',
      'description' => t('Override region CSS with inline styles or add extra classes.'),
      'file' => 'ds_cssoverrider.inc',
      'path' => $path .'/plugins',
      'target' => 'all',
    ),
  );
}
