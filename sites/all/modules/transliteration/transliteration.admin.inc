<?php
// $Id: transliteration.admin.inc,v 1.1.2.1 2009/11/28 12:48:53 smk Exp $

/**
 * @file
 * Retroactive transliteration and admin settings UI.
 */

/**
 * Form builder function; generate retroactive transliteration confirm form.
 *
 * @see transliteration_retroactive_submit()
 * @ingroup forms
 */
function transliteration_retroactive() {
  if (!$query = transliteration_file_query(TRUE)) {
    drupal_set_message(t('Database not supported.'), 'error');
    $form['description']['#value'] = t('Retroactive transliteration is currently not supported on the database system this Drupal installation uses. If you think this should be fixed please <a href="@issues-url">file an issue</a> in the project issue queue.', array('@issues-url' => 'http://drupal.org/project/issues/search/transliteration'));
    return $form;
  }

  $count = db_result(db_query($query));
  if (!$count) {
    drupal_set_message(t('Transliteration not required.'), 'status');
    $form['description']['#value'] = t('There are currently no files containing non-ASCII characters in their name.');
    return $form;
  }

  $form['#redirect'] = 'admin/settings/file-system/transliteration';
  $question = t('Are you sure you want to transliterate existing file names?');
  $description = t('The database currently lists @filenames containing non-ASCII characters. This count might be inaccurate, though, since it is possible that not all of them actually need to be renamed. Only local files will be processed, while off-site files will never be changed. Note that changing the file name might render references to these files void. Since there is currently no way to fix manually inserted links in contents, it is a good idea to create a database backup beforehand. This action cannot be undone.', array('@filenames' => format_plural($count, '1 file name', '@count file names')));
  return confirm_form($form, $question, '<front>', $description, t('Transliterate'));
}

/**
 * Form submit function; retroactively transliterate existing file names.
 *
 * @see transliteration_retroactive()
 */
function transliteration_retroactive_submit($form, &$form_state) {
  $count = 0;
  $errors = array();
  $result = db_query(transliteration_file_query());

  while ($file = db_fetch_object($result)) {
    if (!file_exists('./'. $file->filepath)) {
      // Missing file.
      $errors[] = $file->filepath;
      continue;
    }

    // Sanitize file name.
    $filename = transliteration_clean_filename(basename($file->filepath));
    // Build destination path.
    $destination = dirname($file->filepath) . '/' . $filename;

    // Rename and update the file record accordingly.
    if (file_move($file->filepath, $destination, FILE_EXISTS_RENAME)) {
      db_query("UPDATE {files} SET filepath = '%s' WHERE fid = %d", $file->filepath, $file->fid);
      $count++;
    }
    else {
      $errors[] = $file->filepath;
    }
  }

  if ($errors) {
    $message = t('Not all file names could be converted. The following files could not be accessed and have been ignored:');
    $message .= theme('item_list', $errors);
    drupal_set_message($message, 'error');
  }
  else {
    drupal_set_message(t('@filenames have been successfully transliterated.', array('@filenames' => format_plural($count, '1 file name', '@count file names'))));
  }
}

/**
 * Build a query that returns all file names from the database containing non-ASCII characters.
 *
 * @param $count
 *   Set to TRUE to return a count query.
 */
function transliteration_file_query($count = FALSE) {
  // Regular expressions are not supported by Drupal's database layer. Operators
  // differ between manufacturers.
  switch ($GLOBALS['db_type']) {
    case 'mysql':
    case 'mysqli':
      $operator = 'REGEXP';
      $regex = '[^a-z0-9_./-]';
      break;

    case 'pgsql':
      $operator = '~*';
      $regex = '[^a-z0-9_./-]';
      break;

    case 'mssql':
      // LIKE is case-sensitive.
      $operator = 'LIKE';
      $regex = '%[^A-Za-z0-9_./-]%';
      break;

    default:
      return FALSE;
  }

  $fields = ($count ? 'COUNT(*)' : '*');
  return "SELECT $fields FROM {files} WHERE filepath $operator '$regex'";
}

