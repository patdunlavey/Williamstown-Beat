<?php // $Id: page.tpl.php,v 1.3 2008/10/14 08:45:39 bengtan Exp $ ?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="<?php print $language->language; ?>" xml:lang="<?php print $language->language; ?>">

<head>
  <title><?php print $head_title; ?></title>
  <?php print $head; ?>
  <?php print $styles; ?>
  <!--[if IE]>
    <?php if (file_exists($directory . '/ie.css')): ?>
      <link rel="stylesheet" href="<?php print $base_path . $directory; ?>/ie.css" type="text/css">
    <?php else: ?>
      <link rel="stylesheet" href="<?php print $base_path . $zentheme_directory; ?>/ie.css" type="text/css">
    <?php endif; ?>
  <![endif]-->
  <?php print $scripts; ?>
</head>

<body class="<?php print $body_classes; ?>">

  <div id="page"><div id="page-inner">

    <div id="header"><div id="header-inner" class="clear-block">

      <?php if ($logo || $site_name || $site_slogan): ?>
        <div id="logo-title">

          <?php if ($logo): ?>
            <div id="logo"><a href="<?php print $base_path; ?>" title="<?php print t('Home'); ?>" rel="home"><img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" id="logo-image" /></a></div>
          <?php endif; ?>

          <?php if ($site_name): ?>
            <div id='site-name'>
              <a href="<?php print $base_path; ?>" title="<?php print t('Home'); ?>" rel="home">
                <strong><?php print $site_name; ?></strong>
              </a>
            </div>
          <?php endif; ?>

          <?php if ($site_slogan): ?>
            <div id='site-slogan'><?php print $site_slogan; ?></div>
          <?php endif; ?>

        </div> <!-- /#logo-title -->

        <?php if ($search_box || $header_right): ?>
          <div id="header-bar"><div id="header-bar-inner" class="region region-header-bar">

            <?php if ($search_box): ?>
              <h3 id="search-title"><?php print t('Search'); ?></h3>
              <div id="search-box">
                <?php print $search_box; ?>
              </div> <!-- /#search-box -->
            <?php endif; ?>

            <?php if ($header_right): ?>
              <div id="header-right-blocks" class="region region-header-right">
                <?php print $header_right; ?>
              </div> <!-- /#header-right-blocks -->
            <?php endif; ?>
            
          </div></div> <!-- /#header-bar-inner, /#header-bar -->
        <?php endif; ?>

      <?php endif; ?>

      <?php if ($header): ?>
        <div id="header-blocks" class="region region-header">
          <?php print $header; ?>
        </div> <!-- /#header-blocks -->
      <?php endif; ?>

    </div></div> <!-- /#header-inner, /#header -->

    <div id="main"><div id="main-inner" class="clear-block<?php if ($search_box || $primary_links || $secondary_links || $navbar) { print ' with-navbar'; } ?>">

      <?php if ($primary_links || $secondary_links || $navbar): ?>
        <div id="navbar"><div id="navbar-inner" class="region region-navbar">
          <?php if ($primary_links): ?>
            <div id="primary">
              <?php print theme('links', $primary_links); ?>
            </div> <!-- /#primary -->
          <?php endif; ?>

          <?php if ($secondary_links): ?>
            <div id="secondary">
              <?php print theme('links', $secondary_links); ?>
            </div> <!-- /#secondary -->
          <?php endif; ?>

          <?php print $navbar; ?>
        </div></div> <!-- /#navbar-inner, /#navbar -->
      <?php endif; ?>

      <div id="topbanner">
        <?php if ($breadcrumb): ?>
          <?php print $breadcrumb; ?>
        <?php endif; ?>      
      </div> <!-- /#topbanner -->

      <div id="content"><div id="content-inner">

        <?php if ($content_top): ?>
          <div id="content-top" class="region region-content_top">
            <?php print $content_top; ?>
          </div> <!-- /#content-top -->
        <?php endif; ?>

        <?php if ($title or $tabs or $help or $messages): ?>
          <div id="content-header">
            <?php if ($title): ?>
              <h1 class="title"><?php print $title; ?></h1>
            <?php endif; ?>
            <?php print $messages; ?>
            <?php if ($tabs): ?>
              <div class="tabs"><?php print $tabs; ?></div>
            <?php endif; ?>
            <?php print $help; ?>
          </div> <!-- /#content-header -->
        <?php endif; ?>

        <div id="content-area">
          <?php print $content; ?>
        </div>

        <?php if ($feed_icons): ?>
          <div class="feed-icons"><?php print $feed_icons; ?></div>
        <?php endif; ?>

        <?php if ($content_bottom_left): ?>
          <div id="content-bottom-left" class="region region-content_bottom_left">
            <?php print $content_bottom_left; ?>
          </div> <!-- /#content-bottom-left -->
        <?php endif; ?>

        <?php if ($content_bottom_right): ?>
          <div id="content-bottom-right" class="region region-content_bottom_right">
            <?php print $content_bottom_right; ?>
          </div> <!-- /#content-bottom-right -->
        <?php endif; ?>

        <?php if ($content_bottom): ?>
          <div id="content-bottom" class="region region-content_bottom">
            <?php print $content_bottom; ?>
          </div> <!-- /#content-bottom -->
        <?php endif; ?>

      </div></div> <!-- /#content-inner, /#content -->

      <div id="sidebar-wrapper"><div id="sidebar-wrapper-inner">

        <?php if ($mission): ?>
          <div id="mission"><div id="mission-inner">
            <?php print $mission; ?>
          </div></div>
        <?php endif; ?>

        <?php if ($sidebar_top): ?>
          <div id="sidebar-top"><div id="sidebar-top-inner" class="region region-top">
            <?php print $sidebar_top; ?>
          </div></div> <!-- /#sidebar-top-inner, /#sidebar-top -->
        <?php endif; ?>

        <?php if ($left): ?>
          <div id="sidebar-left" class="clear-block"><div id="sidebar-left-inner" class="region region-left">
            <?php print $left; ?>
          </div></div> <!-- /#sidebar-left-inner, /#sidebar-left -->
        <?php endif; ?>

        <?php if ($right): ?>
          <div id="sidebar-right"><div id="sidebar-right-inner" class="region region-right">
            <?php print $right; ?>
          </div></div> <!-- /#sidebar-right-inner, /#sidebar-right -->
        <?php endif; ?>

      </div></div> <!-- /#sidebar-wrapper-inner, /#sidebar-wrapper -->


    </div></div> <!-- /#main-inner, /#main -->

    <div id="footer"><div id="footer-inner" class="region region-footer">

      <div id="footer-message"><?php print $footer_message; ?>
      </div>

      <?php print $footer; ?>
      <div class="pp_icon"><a href="http://www.prosepoint.org" title="Open source online newspaper and magazine software"><img src="<?php print $base_path . $directory; ?>/prosepoint_icon.png" title="Theme courtesy of ProsePoint: Open source online newspaper and magazine software" alt="Open source online newspaper and magazine software"></a></div>
    
    </div></div> <!-- /#footer-inner, /#footer -->

  </div></div> <!-- /#page-inner, /#page -->

  <?php if ($closure_region): ?>
    <div id="closure-blocks" class="region region-closure"><?php print $closure_region; ?></div>
  <?php endif; ?>

  <?php print $closure; ?>

</body>
</html>
