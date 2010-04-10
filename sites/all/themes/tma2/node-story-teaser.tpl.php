<div class="node <?php print $node_classes; ?> clear-block" id="node-<?php print $node->nid; ?>"><div class="node-inner">

  <?php if ($teaser_pp_img): ?>
    <div class="teaser-image">
      <?php print $teaser_pp_img; ?>
    </div>
  <?php endif; ?>

  <?php if ($page == 0): ?>
    <div class="title">
      <a href="<?php print $node_url; ?>"><?php print $title; ?></a>
    </div>
  <?php endif; ?>

  <?php if ($unpublished): ?>
    <div class="unpublished"><?php print t('Unpublished'); ?></div>
  <?php endif; ?>

  <?php if ($picture) print $picture; ?>

  <?php if ($submitted): ?>
    <div class="submitted">
      <?php print $submitted; ?>
    </div>
  <?php endif; ?>

  <div class="content">
    <?php if ($field_subtitle_rendered): ?> 
      <?php print $field_subtitle_rendered; ?>
    <?php endif; ?>

    <?php if ($field_source_rendered): ?> 
      <?php print $field_source_rendered; ?>
    <?php endif; ?>

    <?php if ($field_date_rendered): ?> 
      <?php print $field_date_rendered; ?>
    <?php endif; ?>

    <?php if ($body_content): ?> 
      <?php print $body_content; ?>
    <?php endif; ?>
  </div>

  <?php if ($links): ?>
    <div class="links">
      <?php print $links; ?>
    </div>
  <?php endif; ?>

</div></div> <!-- /node-inner, /node -->
