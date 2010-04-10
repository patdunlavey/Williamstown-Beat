<?php
// $Id: visitors-settings.tpl.php,v 1.1.2.7 2009/06/28 19:46:51 hass Exp $
?>
<h2><?php print t('Browser families') ?></h2>
<div class="content">
  <object width="100%" height="250" type="application/x-shockwave-flash" data="<?php print $piwik_url ?>/libs/open-flash-chart/open-flash-chart.swf?v2i" id="UserSettingsgetBrowserTypeChart">
    <param name="allowScriptAccess" value="always"/>
    <param name="wmode" value="opaque"/>
    <param name="flashvars" value="data-file=<?php print $data1_url ?>"/>
  </object>
</div>

<h2><?php print t('Configurations') ?></h2>
<div class="content">
  <object width="100%" height="250" type="application/x-shockwave-flash" data="<?php print $piwik_url ?>/libs/open-flash-chart/open-flash-chart.swf?v2i" id="UserSettingsgetConfigurationChart">
    <param name="allowScriptAccess" value="always"/>
    <param name="wmode" value="opaque"/>
    <param name="flashvars" value="data-file=<?php print $data2_url ?>"/>
  </object>
</div>

<h2><?php print t('Operating systems') ?></h2>
<div class="content">
  <object width="100%" height="250" type="application/x-shockwave-flash" data="<?php print $piwik_url ?>/libs/open-flash-chart/open-flash-chart.swf?v2i" id="UserSettingsgetOSChart">
    <param name="allowScriptAccess" value="always"/>
    <param name="wmode" value="opaque"/>
    <param name="flashvars" value="data-file=<?php print $data3_url ?>"/>
  </object>
</div>

<h2><?php print t('Screen resolutions') ?></h2>
<div class="content">
  <object width="100%" height="250" type="application/x-shockwave-flash" data="<?php print $piwik_url ?>/libs/open-flash-chart/open-flash-chart.swf?v2i" id="UserSettingsgetResolutionChart">
    <param name="allowScriptAccess" value="always"/>
    <param name="wmode" value="opaque"/>
    <param name="flashvars" value="data-file=<?php print $data4_url ?>"/>
  </object>
</div>
