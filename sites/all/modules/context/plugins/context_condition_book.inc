<?php
// $Id: context_condition_book.inc,v 1.1.2.1 2009/12/14 22:34:05 yhahn Exp $

/**
 * Expose book properties as a context condition.
 */
class context_condition_book extends context_condition {
  function condition_values() {
    $values = array();
    foreach (book_get_books() as $book) {
      $values[$book['menu_name']] = $book['title'];
    }
    return $values;
  }

  function execute($node, $op) {
    $this->values[] = $node;
    if (isset($node->book, $node->book['menu_name'])) {
      $contexts = $this->get_contexts($node->book['menu_name']);
      foreach ($contexts as $context) {
        context_set('context', $context->name, $context);
      }
    }
  }
}
