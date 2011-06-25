<?php
// $Id: template.php,v 1.1.4.7.2.1 2009/05/05 18:01:38 couzinhub Exp $
    
// Auto-rebuild the theme registry during theme development.
if (theme_get_setting('basic_rebuild_registry')) {
  drupal_rebuild_theme_registry();
}

//
//	from ZEN // Override or insert PHPTemplate variables into the page templates.
//	
//	 This function creates the body classes that are relative to each page
//	
//	@param $vars
//	  A sequential array of variables to pass to the theme template.
//	@param $hook
//	  The name of the theme function being called ("page" in this case.)
//

function basic_preprocess_page(&$vars, $hook) {
  global $theme;

  // Don't display empty help from node_help().
  if ($vars['help'] == "<div class=\"help\"><p></p>\n</div>") {
    $vars['help'] = '';
  }

  // Classes for body element. Allows advanced theming based on context
  // (home page, node of certain type, etc.)
  $body_classes = array($vars['body_classes']);
  if (user_access('administer blocks')) {
	  $body_classes[] = 'admin';
	}
  if (!$vars['is_front']) {
    // Add unique classes for each page and website section
    $path = drupal_get_path_alias($_GET['q']);
    list($section, ) = explode('/', $path, 2);
    $body_classes[] = basic_id_safe('page-'. $path);
    $body_classes[] = basic_id_safe('section-'. $section);

    if (arg(0) == 'node') {
      if (arg(1) == 'add') {
        if ($section == 'node') {
          array_pop($body_classes); // Remove 'section-node'
        }
        $body_classes[] = 'section-node-add'; // Add 'section-node-add'
      }
      elseif (is_numeric(arg(1)) && (arg(2) == 'edit' || arg(2) == 'delete')) {
        if ($section == 'node') {
          array_pop($body_classes); // Remove 'section-node'
        }
        $body_classes[] = 'section-node-'. arg(2); // Add 'section-node-edit' or 'section-node-delete'
      }
    }
  }
  //// Check what the user's browser is and add it as a body class
  //// DEACTIVATED - Only works if page cache is deactivated
  //$user_agent = $_SERVER['HTTP_USER_AGENT'];
  //if($user_agent) {
  //  if (strpos($user_agent, 'MSIE')) {
  //    $body_classes[] = 'browser-ie';
  //  } else if (strpos($user_agent, 'MSIE 6.0')) {
  //    $body_classes[] = 'browser-ie6';
  //  } else if (strpos($user_agent, 'MSIE 7.0')) {
  //    $body_classes[] = 'browser-ie7';
  //  } else if (strpos($user_agent, 'MSIE 8.0')) {
  //    $body_classes[] = 'browser-ie8'; 
  //  } else if (strpos($user_agent, 'Firefox/2')) {
  //    $body_classes[] = 'browser-firefox2';
  //  } else if (strpos($user_agent, 'Firefox/3')) {
  //    $body_classes[] = 'browser-firefox3';
  //  }else if (strpos($user_agent, 'Safari')) {
  //    $body_classes[] = 'browser-safari';
  //  } else if (strpos($user_agent, 'Opera')) {
  //    $body_classes[] = 'browser-opera';
  //  }
  //}
  
  // Add template suggestions based on content type
  // You can use a different page template depending on the
  // content type or the node ID
  // For example, if you wish to have a different page template
  // for the story content type, just create a page template called
  // page-type-story.tpl.php
  // For a specific node, use the node ID in the name of the page template
  // like this : page-node-22.tpl.php (if the node ID is 22)
  
  if ($vars['node']->type != "") {
      $vars['template_files'][] = "page-type-" . $vars['node']->type;
    }
  if ($vars['node']->nid != "") {
      $vars['template_files'][] = "page-node-" . $vars['node']->nid;
    }
  $vars['body_classes'] = implode(' ', $body_classes); // Concatenate with spaces
}



//
//	from ZEN // Override or insert PHPTemplate variables into the node templates.
//	
//	 This function creates the NODES classes, like 'node-unpublished' for nodes
//	 that are not published, or 'node-mine' for node posted by the connected user...
//	
//	@param $vars
//	  A sequential array of variables to pass to the theme template.
//	@param $hook
//	  The name of the theme function being called ("node" in this case.)
//

function basic_preprocess_node(&$vars, $hook) {
  global $user;

  // Special classes for nodes
  $node_classes = array();
  if ($vars['sticky']) {
    $node_classes[] = 'sticky';
  }
  if (!$vars['node']->status) {
    $node_classes[] = 'node-unpublished';
    $vars['unpublished'] = TRUE;
  }
  else {
    $vars['unpublished'] = FALSE;
  }
  if ($vars['node']->uid && $vars['node']->uid == $user->uid) {
    // Node is authored by current user
    $node_classes[] = 'node-mine';
  }
  if ($vars['teaser']) {
    // Node is displayed as teaser
    $node_classes[] = 'node-teaser';
  }
  // Class for node type: "node-type-page", "node-type-story", "node-type-my-custom-type", etc.
  $node_classes[] = 'node-type-'. $vars['node']->type;
  $vars['node_classes'] = implode(' ', $node_classes); // Concatenate with spaces
}


//
// from ZEN // Override or insert PHPTemplate variables into the block templates.
//
//	This function create the EDIT LINKS for blocks and menus blocks.
//	When overing a block (except in IE6), some links appear to edit
//	or configure the block. You can then edit the block, and once you are
//	done, brought back to the first page.
//
// @param $vars
//   A sequential array of variables to pass to the theme template.
// @param $hook
//   The name of the theme function being called ("block" in this case.)
// 

function basic_preprocess_block(&$vars, $hook) {
  $block = $vars['block'];

  if (theme_get_setting('basic_block_editing') && user_access('administer blocks')) {
    // Display 'edit block' for custom blocks
    if ($block->module == 'block') {
      $edit_links[] = l( t('edit block'), 'admin/build/block/configure/'. $block->module .'/'. $block->delta, array('title' => t('edit the content of this block'), 'class' => 'block-edit'), drupal_get_destination(), NULL, FALSE, TRUE);
    }
    // Display 'configure' for other blocks
    else {
      $edit_links[] = l(t('configure'), 'admin/build/block/configure/'. $block->module .'/'. $block->delta, array('title' => t('configure this block'), 'class' => 'block-config'), drupal_get_destination(), NULL, FALSE, TRUE);
    }

    // Display 'edit menu' for menu blocks
    if (($block->module == 'menu' || ($block->module == 'user' && $block->delta == 1)) && user_access('administer menu')) {
      $edit_links[] = l(t('edit menu'), 'admin/build/menu', array('title' => t('edit the menu that defines this block'), 'class' => 'block-edit-menu'), drupal_get_destination(), NULL, FALSE, TRUE);
    }
    $vars['edit_links_array'] = $edit_links;
    $vars['edit_links'] = '<div class="edit">'. implode(' ', $edit_links) .'</div>';
  }
}


//
//  Create some custom classes for comments
//

function comment_classes($comment) {
  $node = node_load($comment->nid);
  global $user;
 
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


// 	
// 	Customize the PRIMARY and SECONDARY LINKS, to allow the admin tabs to work on all browsers
// 	An implementation of theme_menu_item_link()
// 	
// 	@param $link
// 	  array The menu item to render.
// 	@return
// 	  string The rendered menu item.
// 	

function basic_menu_item_link($link) {
  if (empty($link['options'])) {
    $link['options'] = array();
  }

  // If an item is a LOCAL TASK, render it as a tab
  if ($link['type'] & MENU_IS_LOCAL_TASK) {
    $link['title'] = '<span class="tab">'. check_plain($link['title']) .'</span>';
    $link['options']['html'] = TRUE;
  }

  if (empty($link['type'])) {
    $true = TRUE;
  }

  return l($link['title'], $link['href'], $link['options']);
}

/**
 * Duplicate of theme_menu_local_tasks() but adds clear-block to tabs.
 */
function basic_menu_local_tasks() {
  $output = '';

  if ($primary = menu_primary_local_tasks()) {
    $output .= "<ul class=\"tabs primary clear-block\">\n". $primary ."</ul>\n";
  }
  if ($secondary = menu_secondary_local_tasks()) {
    $output .= "<ul class=\"tabs secondary clear-block\">\n". $secondary ."</ul>\n";
  }

  return $output;
}

//	
//	Add custom classes to menu item
//	
	
function basic_menu_item($link, $has_children, $menu = '', $in_active_trail = FALSE, $extra_class = NULL) {
  $class = ($menu ? 'expanded' : ($has_children ? 'collapsed' : 'leaf'));
  if (!empty($extra_class)) {
    $class .= ' '. $extra_class;
  }
  if ($in_active_trail) {
    $class .= ' active-trail';
  }
#New line added to get unique classes for each menu item
  $css_class = basic_id_safe(str_replace(' ', '_', strip_tags($link)));
  return '<li class="'. $class . ' ' . $css_class . '">' . $link . $menu ."</li>\n";
}


//	
//	Converts a string to a suitable html ID attribute.
//	
//	 http://www.w3.org/TR/html4/struct/global.html#h-7.5.2 specifies what makes a
//	 valid ID attribute in HTML. This function:
//	
//	- Ensure an ID starts with an alpha character by optionally adding an 'n'.
//	- Replaces any character except A-Z, numbers, and underscores with dashes.
//	- Converts entire string to lowercase.
//	
//	@param $string
//	  The string
//	@return
//	  The converted string
//	


function basic_id_safe($string) {
  // Replace with dashes anything that isn't A-Z, numbers, dashes, or underscores.
  $string = strtolower(preg_replace('/[^a-zA-Z0-9_-]+/', '-', $string));
  // If the first character is not a-z, add 'n' in front.
  if (!ctype_lower($string{0})) { // Don't use ctype_alpha since its locale aware.
    $string = 'id'. $string;
  }
  return $string;
}


//
//  Return a themed breadcrumb trail.
//	Alow you to customize the breadcrumb markup
//

function basic_breadcrumb($breadcrumb) {
  if (!empty($breadcrumb)) {
    return '<div class="breadcrumb">'. implode(' » ', $breadcrumb) .'</div>';
  }
}