<?php
// $Id: template.php,v 1.1.2.2 2009/03/29 15:46:36 dvessel Exp $


/**
 * Preprocessor for page.tpl.php template file.
 */
function ninesixty_preprocess_page(&$vars, $hook) {
  // For easy printing of variables.
  $vars['logo_img']         = $vars['logo'] ? theme('image', substr($vars['logo'], strlen(base_path())), t('Home'), t('Home')) : '';
  $vars['linked_logo_img']  = $vars['logo_img'] ? l($vars['logo_img'], '<front>', array('rel' => 'home', 'title' => t('Home'), 'html' => TRUE)) : '';
  $vars['linked_site_name'] = $vars['site_name'] ? l($vars['site_name'], '<front>', array('rel' => 'home', 'title' => t('Home'))) : '';
  $vars['main_menu_links']      = theme('links', $vars['primary_links'], array('class' => 'links main-menu'));
  $vars['secondary_menu_links'] = theme('links', $vars['secondary_links'], array('class' => 'links secondary-menu'));

  // Make sure framework styles are placed above all others.
  $vars['css_alt'] = ninesixty_css_reorder($vars['css']);
  $vars['styles'] = drupal_get_css($vars['css_alt']);
}

/**
 * Contextually adds 960 Grid System classes.
 *
 * The first parameter passed is the *default class*. All other parameters must
 * be set in pairs like so: "$variable, 3". The variable can be anything available
 * within a template file and the integer is the width set for the adjacent box
 * containing that variable.
 *
 *  class="<?php print ns('grid-16', $var_a, 6); ?>"
 *
 * If $var_a contains data, the next parameter (integer) will be subtracted from
 * the default class. See the README.txt file.
 */
function ns() {
  $args = func_get_args();
  $default = array_shift($args);
  // Get the type of class, i.e., 'grid', 'pull', 'push', etc.
  // Also get the default unit for the type to be procesed and returned.
  list($type, $return_unit) = explode('-', $default);

  // Process the conditions.
  $flip_states = array('var' => 'int', 'int' => 'var');
  $state = 'var';
  foreach ($args as $arg) {
    if ($state == 'var') {
      $var_state = !empty($arg);
    }
    elseif ($var_state) {
      $return_unit = $return_unit - $arg;
    }
    $state = $flip_states[$state];
  }

  $output = '';
  // Anything below a value of 1 is not needed.
  if ($return_unit > 0) {
    $output = $type . '-' . $return_unit;
  }
  return $output;
}

/**
 * This rearranges how the style sheets are included so the framework styles
 * are included first.
 *
 * Sub-themes can override the framework styles when it contains css files with
 * the same name as a framework style. This can be removed once Drupal supports
 * weighted styles.
 */
function ninesixty_css_reorder($css) {
  global $theme_info, $base_theme_info;

  // Dig into the framework .info data.
  $framework = !empty($base_theme_info) ? $base_theme_info[0]->info : $theme_info->info;

  // Pull framework styles from the themes .info file and place them above all stylesheets.
  if (isset($framework['stylesheets'])) {
    foreach ($framework['stylesheets'] as $media => $styles_from_960) {
      // Setup framework group.
      if (isset($css[$media])) {
        $css[$media] = array_merge(array('framework' => array()), $css[$media]);
      }
      else {
        $css[$media]['framework'] = array();
      }
      foreach ($styles_from_960 as $style_from_960) {
        // Force framework styles to come first.
        if (strpos($style_from_960, 'framework') !== FALSE) {
          $framework_shift = $style_from_960;
          $remove_styles = array($style_from_960);
          // Handle styles that may be overridden from sub-themes.
          foreach ($css[$media]['theme'] as $style_from_var => $preprocess) {
            if ($style_from_960 != $style_from_var && basename($style_from_960) == basename($style_from_var)) {
              $framework_shift = $style_from_var;
              $remove_styles[] = $style_from_var;
              break;
            }
          }
          $css[$media]['framework'][$framework_shift] = TRUE;
          foreach ($remove_styles as $remove_style) {
            unset($css[$media]['theme'][$remove_style]);
          }
        }
      }
    }
  }

  return $css;
}

/*!
 * Dynamic display block preprocess functions
 * Copyright (c) 2008 - 2009 P. Blaauw All rights reserved.
 * Version 1.3 (18-MAR-2009)
 * Licenced under GPL license
 * http://www.gnu.org/licenses/gpl.html
 */

 /**
 * Override or insert variables into the ddblock_cycle_block_content templates.
 *   Used to convert variables from view_fields to slider_items template variables
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * 
 */
function soulkids_preprocess_ddblock_cycle_block_content(&$vars) {
  if ($vars['output_type'] == 'view_fields') {
    $content = array();
    // Add slider_items for the template 
    // If you use the devel module uncomment the following line to see the theme variables
    // dsm($vars['settings']['view_name']);  
    // dsm($vars['content'][0]);
    // If you don't use the devel module uncomment the following line to see the theme variables
    // drupal_set_message('<pre>' . var_export($vars['settings']['view_name'], true) . '</pre>');
    // drupal_set_message('<pre>' . var_export($vars['content'][0], true) . '</pre>');
    if ($vars['settings']['view_name'] == 'News') {
      if (!empty($vars['content'])) {
        foreach ($vars['content'] as $key1 => $result) {
          // add slide_image variable 
          if (isset($result->node_data_field_image_field_image_fid)) {
            // get image id
            $fid = $result->node_data_field_image_field_image_fid;
            // get path to image
            $filepath = db_result(db_query("SELECT filepath FROM {files} WHERE fid = %d", $fid));
            //  use imagecache (imagecache, preset_name, file_path, alt, title, array of attributes)
            if (module_exists('imagecache') && is_array(imagecache_presets()) && $vars['imgcache_slide'] <> '<none>'){
              $slider_items[$key1]['slide_image'] = 
              theme('imagecache', 
                    $vars['imgcache_slide'], 
                    $filepath,
                    $result->node_title);
            }
            else {          
              $slider_items[$key1]['slide_image'] = 
                '<img src="' . base_path() . $filepath . 
                '" alt="' . $result->node_title . 
                '"/>';     
            }          
          }
          // add slide_text variable
          if (isset($result->node_data_field_pager_item_text_field_slide_text_value)) {
            $slider_items[$key1]['slide_text'] =  $result->node_data_field_pager_item_text_field_slide_text_value;
          }
          // add slide_title variable
          if (isset($result->node_title)) {
            $slider_items[$key1]['slide_title'] =  $result->node_title;
          }
          // add slide_read_more variable and slide_node variable
          if (isset($result->nid)) {
            $slider_items[$key1]['slide_read_more'] =  l('Read more...', 'node/' . $result->nid);
            $slider_items[$key1]['slide_node'] =  'node/' . $result->nid;
          }
        }
        $vars['slider_items'] = $slider_items;
      }
    }    
  }
}  
/**
 * Override or insert variables into the ddblock_cycle_pager_content templates.
 *   Used to convert variables from view_fields  to pager_items template variables
 *  Only used for custom pager items
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 *
 */
function soulkids_preprocess_ddblock_cycle_pager_content(&$vars) {
  if (($vars['output_type'] == 'view_fields') && ($vars['pager_settings']['pager'] == 'custom-pager')){
    $content = array();
    // Add pager_items for the template 
    // If you use the devel module uncomment the following lines to see the theme variables
    // dsm($vars['pager_settings']['view_name']);     
    // dsm($vars['content'][0]);     
    // If you don't use the devel module uncomment the following lines to see the theme variables
    // drupal_set_message('<pre>' . var_export($vars['pager_settings'], true) . '</pre>');
    // drupal_set_message('<pre>' . var_export($vars['content'][0], true) . '</pre>');
    if ($vars['pager_settings']['view_name'] == 'News') {
      if (!empty($vars['content'])) {
        foreach ($vars['content'] as $key1 => $result) {
          // add pager_item_image variable
          if (isset($result->node_data_field_pager_item_text_field_image_fid)) {
            $fid = $result->node_data_field_pager_item_text_field_image_fid;
            $filepath = db_result(db_query("SELECT filepath FROM {files} WHERE fid = %d", $fid));
            //  use imagecache (imagecache, preset_name, file_path, alt, title, array of attributes)
            if (module_exists('imagecache') && 
                is_array(imagecache_presets()) && 
                $vars['imgcache_pager_item'] <> '<none>'){
              $pager_items[$key1]['image'] = 
                theme('imagecache', 
                      $vars['pager_settings']['imgcache_pager_item'],              
                      $filepath,
                      $result->node_data_field_pager_item_text_field_pager_item_text_value);
            }
            else {          
              $pager_items[$key1]['image'] = 
                '<img src="' . base_path() . $filepath . 
                '" alt="' . $result->node_data_field_pager_item_text_field_pager_item_text_value . 
                '"/>';     
            }          
          }
          // add pager_item _text variable
          if (isset($result->node_data_field_pager_item_text_field_pager_item_text_value)) {
            $pager_items[$key1]['text'] =  $result->node_data_field_pager_item_text_field_pager_item_text_value;
          }
        }
      }
      $vars['pager_items'] = $pager_items;
    }
  }    
}


/**
 * Intercept comment template variables
 */
function phptemplate_preprocess_comment(&$vars, $hook) {
  static $comment_count = 1; // keep track the # of comments rendered
  
  // if the author of the node comments as well, highlight that comment
  $node = node_load($vars['comment']->nid);
  if ($vars['comment']->uid == $node->uid) {
    $vars['author_comment'] = TRUE;
  }
  // only show links for users that have permission to post or administer comments
  if (!user_access('post comments')) {
    $vars['links'] = '';
  }

  // if subjects in comments are turned off, don't show the title then
  if (variable_get('comment_subject_field', 1) == 0) {
    $vars['title'] = '';
  }

  // if user has no picture, remove div.picture
  if (empty($vars['comment']->picture)) {
    $vars['picture'] = '';
  }

  $vars['comment_count'] = $comment_count++;  
}

/**
 *  Create some custom classes for comments
 */
function comment_classes($comment) {
  global $user;

  $node = node_load($comment->nid);
  $output .= ($comment->new) ? ' comment-new' : ''; 
  $output .=  ' '. $status .' '; 
  if ($node->name == $comment->name) {	
    $output .= 'node-author';
  }
  if ($user->name == $comment->name) {	
    $output .=  ' mine';
  }
  return $output;
}

/**
 * Override comment wrapper to show you must login to comment.
 */
function beach_comment_wrapper($content, $node) {
  global $user;
  $output = '';

  if ($node = menu_get_object()) {
    $count = $node->comment_count .' '. format_plural($node->comment_count, 'comment', 'comments');

    if ($node->comment_count < 1) {
      $output .= $content;
    }
    else {
      $output .= '<h3 id="comment-number">'.$count.'</h3>';
      $output .= '<div id="comments">'.$content.'</div>';
    }
  }
  return $output;
}
