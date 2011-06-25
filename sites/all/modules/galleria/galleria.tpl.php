<?php
// $Id: galleria.tpl.php,v 1.1.2.4 2009/06/08 08:52:17 marktheunissen Exp $

/**
 * @file
 * Default output for a galleria node.
 */
?>

<?php if ($image_count > 0): ?>
  <div id="galleria-content">
    <div id="main-image"></div>
    <?php print $thumbnails; ?>
    <script type="text/javascript">
      <!--//--><![CDATA[//><!--
      //Hide images while Galleria is loading
      $('ul.gallery img').css('display', 'none');
      //--><!]]>
    </script>

    <?php if ($image_count > 1): ?>
      <div class="galleria-nav">
        <a onclick="$.galleria.prev(); return false;" href="#">&laquo; <?php print $prev; ?></a> |
        <a onclick="$.galleria.next(); return false;" href="#"><?php print $next; ?> &raquo;</a>
      </div>
    <?php endif; ?>
  </div>
<?php endif; ?>
