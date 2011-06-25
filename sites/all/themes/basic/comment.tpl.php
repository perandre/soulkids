<?php
// $Id: comment.tpl.php,v 1.1.4.7 2009/03/19 23:49:01 couzinhub Exp $

/**
 * @file comment.tpl.php
 * Default theme implementation for comments.
 *
 * Available variables:
 * - $author: Comment author. Can be link or plain text.
 * - $content: Body of the post.
 * - $date: Date and time of posting.
 * - $links: Various operational links.
 * - $new: New comment marker.
 * - $picture: Authors picture.
 * - $signature: Authors signature.
 * - $status: Comment status. Possible values are:
 *   comment-unpublished, comment-published or comment-preview.
 * - $submitted: By line with date and time.
 * - $title: Linked title.
 *
 * These two variables are provided for context.
 * - $comment: Full comment object.
 * - $node: Node object the comments are attached to.
 *
 * @see template_preprocess_comment()
 * @see theme_comment()
 */
?>

<div class="comment <?php echo comment_classes($comment) .' '. $zebra; if ($unpublished) { echo 'comment-unpublished'; } ?> clear-block">
	<div class="comment-inner">
		
    <?php if ($title): ?>
      <h3 class="title"><?php echo $title; if (!empty($new)): ?> <span class="new"><?php echo $new; ?></span><?php endif; ?></h3>
    <?php elseif (!empty($new)): ?>
      <div class="new"><?php echo $new; ?></div>
    <?php endif; ?>
    
    <?php if ($unpublished): ?>
      <div class="unpublished"><?php echo t('Unpublished'); ?></div>
    <?php endif; ?>
    
    <?php if ($picture): ?>
	    <div class="picture"><?php echo $picture; ?></div>
	  <?php endif; ?>
	    
    <div class="submitted">
      <?php echo $submitted; ?>
    </div>
    
    <div class="content">
      <?php echo $content ?>
      <?php if ($signature): ?>
      <div class="user-signature clear-block">
        <?php echo $signature; ?>
      </div>
      <?php endif; ?>
    </div>
    
    <?php if ($links): ?>
      <div class="links">
        <?php echo $links; ?>
      </div>
    <?php endif; ?>  

  </div> <!-- /comment-inner -->
</div> <!-- /comment -->