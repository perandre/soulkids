// $Id: README.txt,v 1.1.4.2 2009/06/22 01:10:09 weitzman Exp $

OVERVIEW
--------

The submitagain module is a simple module to reroute users back to node/add/xxx
pages after they have submitted a node. It can be turned on for each content
type and is off by default.

This module is useful if you or your users repeatedly submit content, and would
like to speed up the process.

INSTALL
-------

No addons required. Drop the entire submit again folder in your modules directory,
usually sites/all/modules on your Drupal filesystem, activate it on the modules
page, and turn it on for each content type it should be attached to at

  Administer -> Content management -> Content types -> (edit each content type)