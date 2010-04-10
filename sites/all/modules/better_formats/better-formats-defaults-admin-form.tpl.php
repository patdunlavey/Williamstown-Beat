<?php
// $Id: better-formats-defaults-admin-form.tpl.php,v 1.3.2.2 2009/03/17 08:16:52 dragonwize Exp $

/**
 * @file
 * Default theme implementation to configure format manager filter admin page.
 *
 * Available variables:
 * - $form_submit: Form submit button.
 *
 * Each $node_default_rows contains a row
 *
 * Each $data in $node_default_rows contains:
 * - $data->role: Role name.
 * - $data->format_select: Drop-down menu for setting format.
 * - $data->weight_select: Drop-down menu for setting weights.
 */
?>
<?php
  // Add table javascript
  drupal_add_tabledrag('node-format-defaults', 'order', 'sibling', 'better-formats-role-node-weight');
  drupal_add_tabledrag('comment-format-defaults', 'order', 'sibling', 'better-formats-role-comment-weight');
?>
<div class="description"><?php print t('Only formats that a role has permission to use are shown.'); ?></div>
<fieldset>
  <legend><strong><?php print t('Node Defaults'); ?></strong></legend>
  <table id="node-format-defaults">
    <thead>
      <tr>
        <th><?php print t('Role'); ?></th>
        <th><?php print t('Default Format'); ?></th>
        <th><?php print t('Weight'); ?></th>
      </tr>
    </thead>
    <tbody>
      <?php $row = 0; ?>
      <?php foreach ($node_default_rows as $rid => $data): ?>
      <tr class="draggable <?php print $row % 2 ? 'odd' : 'even'; ?>">
        <td class=""><?php print $data->role; ?></td>
        <td><?php print $data->format_select; ?></td>
        <td><?php print $data->weight_select; ?></td>
      </tr>
      <?php $row++; ?>
      <?php endforeach; ?>
    </tbody>
  </table>
</fieldset>

<fieldset>
  <legend><strong><?php print t('Comment Defaults'); ?></strong></legend>
  <table id="comment-format-defaults">
    <thead>
      <tr>
        <th><?php print t('Role'); ?></th>
        <th><?php print t('Default Format'); ?></th>
        <th><?php print t('Weight'); ?></th>
      </tr>
    </thead>
    <tbody>
      <?php $row = 0; ?>
      <?php foreach ($comment_default_rows as $rid => $data): ?>
      <tr class="draggable <?php print $row % 2 ? 'odd' : 'even'; ?>">
        <td class=""><?php print $data->role; ?></td>
        <td><?php print $data->format_select; ?></td>
        <td><?php print $data->weight_select; ?></td>
      </tr>
      <?php $row++; ?>
      <?php endforeach; ?>
    </tbody>
  </table>
</fieldset>

<?php print $form_submit; ?>
