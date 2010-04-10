<?
// Make channel subblocks look like channel blocks by adding classes: block block-pp_ch
?>
<div id="sub-block-<?php print $subblock['tid']; ?>" class="sub-block block block-pp_ch <?php print $subblock_classes; ?>"><div class="sub-block-inner">

  <?php if ($subblock['subject']): ?>
    <h2 class="title"><?php print $subblock['subject']; ?></h2>
  <?php endif; ?>

  <div class="content">
    <?php print $subblock['content']; ?>
  </div>

  <?php print $edit_links; ?>

</div></div> <!-- /sub-block-inner, /sub-block -->

