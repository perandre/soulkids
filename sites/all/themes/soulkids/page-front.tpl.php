<?php
// $Id: page.tpl.php,v 1.1.2.1 2009/02/24 15:34:45 dvessel Exp $
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language ?>" lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">

<head>
  <title><?php print $head_title; ?></title>
  <?php print $head; ?>
  <?php print $styles; ?>
  <?php print $scripts; ?>
</head>

<body class="<?php print $body_classes; ?>">
  <div id="top-nav">
      <?php print $top; ?>
  </div>
  <div id="page" class="container-16 clear-block">

    <div id="header" class="clear-block">
      <h1 id="logo" class="grid-3 clear-block"><a href="#">Soulkids barnekor i Fredrikstad</a></h1>

      <div id="nav" class="grid-13">
        <?php print $header; ?>
      </div>
      <div id="new-pics" class="grid-13">
        <h2>NYESTE BILDER</h2>
        <div id="pics"><?php print $header2; ?></div>
      </div>
    </div>
    <div id="main" class="column <?php print ns('grid-16', $left, 4, $right, 4) . ' ' . ns('push-4', !$left, 4); ?>">
      <?php print $breadcrumb; ?>
      <?php if ($title): ?>
        <h1 class="title" id="page-title"><?php print $title; ?></h1>
      <?php endif; ?>
      <?php if ($tabs): ?>
        <div class="tabs"><?php print $tabs; ?></div>
      <?php endif; ?>
      <?php print $messages; ?>
      <?php print $help; ?>
      <div id="featured" class="region clear-block">
        <?php print $content_top; ?>
      </div>
      <div class="hr"></div>
      <div id="about" class="grid-4 alpha">
        <h2>HVEM HVA HVOR!</h2>
        <?php print $under1; ?> 
      </div>
      <div id="video" class="grid-8 omega">
        <div class="video-inner">
          <?php print $under2; ?>
        </div>
      </div>
    </div>

  <?php if ($left): ?>
    <div id="sidebar-left" class="column sidebar region grid-4 <?php print ns('pull-12', $right, 3); ?>">
      <?php print $left; ?>
    </div>
  <?php endif; ?>

  <?php if ($right): ?>
    <div id="sidebar-right" class="column sidebar region grid-4">
      <div id="activity">
        <?php print $right; ?>
      </div>
    </div>
  <?php endif; ?>


  <div id="footer" class="prefix-1 suffix-1">
    <?php if ($footer): ?>
      <div id="footer-region" class="region grid-14 clear-block">
        <?php print $footer; ?>
      </div>
    <?php endif; ?>

    <?php if ($footer_message): ?>
      <div id="footer-message" class="grid-14">
        <?php print $footer_message; ?>
      </div>
    <?php endif; ?>
  </div>


  </div>
  <?php print $closure; ?>
</body>
</html>
