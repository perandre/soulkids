<?php
// $Id: nd.tpl.php,v 1.1.2.1 2010/01/15 14:33:14 swentel Exp $
/**
 * This template is optimized for use with the Node displays module.
 *
 * Differences with the default node.tpl.php
 *   - Uses only the $content variable.
 *   - Extra check on $sticky because this node might be build with another build mode.
 */
?>
<div class="node <?php if (isset($node_classes)) print $node_classes; ?><?php if ($is_front) { print ' front-node'; } ?><?php if ($sticky && $node->build_mode == 'sticky') { print ' sticky'; } ?><?php if (!$status) { print ' node-unpublished'; } ?> clear-block" id="node-<?php print $node->nid; ?>">
  <div class="node-inner">
    <div class="buildmode-<?php print $node->build_mode; ?>">
      <?php print $content; ?>
    </div>
  </div> <!-- /node-inner -->
</div> <!-- /node -->