<?php

// $Id: flvmediaplayer_io.inc,v 1.1.2.1.2.1 2008/11/19 20:14:22 arthuregg Exp $


/* TODO db_next_id() is gone, and replaced as db_last_insert_id()
   Since db_next_id() introduce some problems, and the use of this function
   can be replaced by database level auto increment handling, db_next_id()
   is now gone and replaced as db_last_insert_id() with help of serial type
   under Schema API (check out http://drupal.org/node/149176 for more details).
   Please refer to drupal_write_record() as demonstration. */

/* TODO FormAPI image buttons are now supported.
   FormAPI now offers the 'image_button' element type, allowing developers to
   use icons or other custom images in place of traditional HTML submit buttons.

$form['my_image_button'] = array(
  '#type'         => 'image_button',
  '#title'        => t('My button'),
  '#return_value' => 'my_data',
  '#src'          => 'my/image/path.jpg',
); */

/* TODO Remove $row argument from db_result() method
   The $row argument of db_result() was removed from the database abstraction
   layer in 6.x core, as it was a database dependent option. Developers need to
   use other handling to replace the needs of this method. */

/* TODO Change 'Submit' to 'Save' on buttons
   It has been agreed on that the description 'Submit' for a button is not a
   good choice since it does not indicate what actually happens. While for
   example on node editing forms, 'Preview' and 'Delete' describe exactly what
   will happen when the user clicks on the button, 'Submit' only gives a vague
   idea. When labelling your buttons, make sure that it is clear what this
   button does when the user clicks on it. */

/* TODO Node previews and adding form fields to the node form.
   There is a subtle but important difference in the way node previews (and other
   such operations) are carried out when adding or editing a node. With the new
   Forms API, the node form is handled as a multi-step form. When the node form
   is previewed, all the form values are submitted, and the form is rebuilt with
   those form values put into $form['#node']. Thus, form elements that are added
   to the node form will lose any user input unless they set their '#default_value'
   elements using this embedded node object. */

/* TODO New user_mail_tokens() method may be useful.
   user.module now provides a user_mail_tokens() function to return an array
   of the tokens available for the email notification messages it sends when
   accounts are created, activated, blocked, etc. Contributed modules that
   wish to make use of the same tokens for their own needs are encouraged
   to use this function. */

/* TODO
   There is a new hook_watchdog in core. This means that contributed modules
   can implement hook_watchdog to log Drupal events to custom destinations.
   Two core modules are included, dblog.module (formerly known as watchdog.module),
   and syslog.module. Other modules in contrib include an emaillog.module,
   included in the logging_alerts module. See syslog or emaillog for an
   example on how to implement hook_watchdog.
function example_watchdog($log = array()) {
  if ($log['severity'] == WATCHDOG_ALERT) {
    mysms_send($log['user']->uid,
      $log['type'],
      $log['message'],
      $log['variables'],
      $log['severity'],
      $log['referer'],
      $log['ip'],
      format_date($log['timestamp']));
  }
} */

/* TODO Implement the hook_theme registry. Combine all theme registry entries
   into one hook_theme function in each corresponding module file.
function flvmediaplayer_io_theme() {
  return array(
  );
}; */


/* TODO
   An argument for replacements has been added to format_plural(),
   escaping and/or theming the values just as done with t().*/

/* ************************************************************** */
/* Import / Export  */
/* ************************************************************** */

/**
 * creates the form for the export process
 * @param int $pid
 * @return array $form drupal form array
 *
 */
function flvmediaplayer_export_form($drupal_form, $pid = null) {

  // if a $pid is incoming, display it
  if ($pid) {
    return flvmediaplayer_export_display($pid);
  }

  //get a list of all the profiles
  $profiles = flvmediaplayer_profile_get_configs();
  $form['flvmediaplayer_export'] = array(
    '#type' => 'radios',
    '#title' => t('Profile to export'),
    '#options' => $profiles,
    '#description' => t('Select the profile you wish to export'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Export'),
  );
  return $form;
}


/**
 * Send the user to the export display screen
 * @param string $form_id
 * @param array $form_values
 * return string drupal goto
 */
function flvmediaplayer_export_form_submit($form, &$form_state) {
  $form_state['redirect'] = 'admin/settings/flvmediaplayer/export/'. $form_state['values']['flvmediaplayer_export'];
}


/**
 * displays the profile in code form
 * @param int $pid
 */
function flvmediaplayer_export_display($pid) {
  // get the profile
  $profile = db_fetch_array(db_query('SELECT * FROM {flvmediaplayer_profiles} WHERE pid = %d', $pid));
  // unserialize the config
  $profile['config'] = unserialize($profile['config']);

  $profile = flvmediaplayer_export_display_helper($profile);
  $profile = '$profile = '. $profile .';';

  $form['output'] = array(
    '#type' => 'textarea',
    '#title' => t('Profile export'),
    '#default_value' => $profile,
    '#rows' => 20,
  );
  return $form;
}


/**
 * takes an array and converts it to text helper for the export function
 * @param array $data
 * @return string textual representation of array
 *
 */
function flvmediaplayer_export_display_helper($data) {
  if (is_array($data)) {
    $output = "array(\n";
    foreach ($data as $key => $value) {
      if (is_array($data[$key])) {
        $items[] = "'$key' => ". flvmediaplayer_export_display_helper($value);
      }
      else {
        $items[] = "'$key' => '$value'";
      }
    }
    $output .= implode(",\n  ", $items) ."\n)";
    return $output;
  }
  else {
    return "'$data'";
  }
}


/**
 * creates the import form
 */
function flvmediaplayer_import_form() {
  $form['import'] = array(
    '#title' => t('Enter the profile import code'),
    '#type' => 'textarea',
    '#description' => t('Paste the exported code here. This will create a new profile. If the imported profile already exists on the system, it will update the existing one.')
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import'),
  );
  return $form;
}

/**
 * accepts the import form submission
 * @param string $form_id
 * @param array $form_values
 * @return string
 */
function flvmediaplayer_import_form_submit($form, &$form_state) {
  // evaluate imported code
  ob_start();
  eval($form_state['values']['import']);
  ob_end_clean();

  // does this pid already exist? no, update the existing one
  $query = db_result(db_query('SELECT pid FROM {flvmediaplayer_profiles} WHERE pid = %d', $profile['pid']));
  if ($query) {
    db_query('UPDATE {flvmediaplayer_profiles} SET config = "%s" WHERE pid = %d', serialize($profile['config']), $profile['pid']);
    $pid = $profile['pid'];
    drupal_set_message(t('Successfully updated flvmediaplayer profile: !name .', array('!name' => $profile['name'])));
  }
  // create a new profile
  else {
    db_query('INSERT INTO {flvmediaplayer_profiles} (type, name, config) VALUES ("%s", "%s", "%s")', $profile['type'], $profile['name'], serialize($profile['config']));
    $pid = db_last_insert_id('flvmediaplayer_profiles', 'pid');
    drupal_set_message(t('Successfully imported flvmediaplayer profile: !name .', array('!name' => $profile['name'])));
  }
  $form_state['redirect'] = 'admin/settings/flvmediaplayer/profiles/'. $pid;
}


