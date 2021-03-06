<?php
// $Id: context_condition_menu.inc,v 1.1.2.1 2009/12/14 22:34:05 yhahn Exp $

/**
 * Expose menu items as a context condition.
 */
class context_condition_menu extends context_condition {
  function condition_values() {
    $menus = menu_parent_options(array_reverse(menu_get_menus()), NULL);
    $root_menus = array();
    foreach ($menus as $key => $name) {
      $id = explode(':', $key);
      if ($id[1] == '0') {
        $root_menus[$id[0]] = check_plain($name);
      }
      else {
        $link = menu_link_load($id[1]);
        $root_menu = $root_menus[$id[0]];
        $menus[$root_menu][$link['link_path']] = $name;
      }
      unset($menus[$key]);
    }
    array_unshift($menus, "-- ". t('None') ." --");
    return $menus;
  }

  function condition_form($context) {
    $form = parent::condition_form($context);
    $form['#type'] = 'select';
    $form['#multiple'] = TRUE;
    return $form;
  }

  function execute() {
    // Menu trail condition integration. Note that because of the way
    // menu_get_active_trail() is written this will often not work for active
    // menu items outside the standard navigation tree. See menu_set_active_trail()
    // and menu_get_active_menu_name() for the details.
    $trail = menu_get_active_trail();
    foreach ($trail as $item) {
      $this->values[$item['href']] = array();
      if (!empty($item['href']) && $contexts = $this->get_contexts($item['href'])) {
        foreach ($contexts as $context) {
          $this->values[$item['href']][] = $condition->name;
          context_set('context', $context->name, $context);
        }
      }
    }
  }
}
