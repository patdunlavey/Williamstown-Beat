<?php
// $Id: visitors-overview.tpl.php,v 1.1.2.6 2009/06/28 19:47:10 hass Exp $
?>
<h2><?php
  switch ($period) {
    case 'week':
      print t('Visitors in time period by week');
      break;

    default:
      print t('Visitors in time period by day');
  }
?></h2>
<div class="content">
  <object width="100%" height="300" type="application/x-shockwave-flash" data="<?php print $piwik_url ?>/libs/open-flash-chart/open-flash-chart.swf?v2i" id="VisitsSummarygetLastVisitsGraphChart">
    <param name="allowScriptAccess" value="always"/>
    <param name="wmode" value="opaque"/>
    <param name="flashvars" value="data-file=<?php print $data1_url ?>"/>
  </object>
</div>
