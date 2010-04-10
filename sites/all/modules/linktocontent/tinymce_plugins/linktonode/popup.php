<?php
// $Id: popup.php,v 1.7 2007/10/08 15:18:09 stborchert Exp $

header('Expires: Mon, 26 Jul 1997 05:00:00 GMT');
header('Last-Modified: ' . gmdate('D, d M Y H:i:s') . ' GMT');
header('Cache-Control: no-store, no-cache, must-revalidate');
header('Cache-Control: post-check=0, pre-check=0', false);
header('Pragma: no-cache');
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html lang="de" xml:lang="de" xmlns="http://www.w3.org/1999/xhtml">
 <head>
  <meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
  <title>{$lang_linktonode_title}</title>
  <script type="text/javascript" src="../../tiny_mce_popup.js"></script>
  <script type="text/javascript" src="../../utils/mctabs.js"></script>
  <script type="text/javascript" src="../../utils/form_utils.js"></script>
  <script type="text/javascript">
   function appendScripts() {
     var jquery = document.createElement("script");
     var functions = document.createElement("script");
     var url = tinyMCE.baseURL.substring(0, tinyMCE.baseURL.indexOf('modules/'));
     if (url.indexOf('sites/') > -1) {
       url = url.substring(0, url.indexOf('sites/'));
     }
     jquery.type = functions.type = "text/javascript";
     jquery.src = url + "misc/jquery.js";
     functions.src = "jscripts/functions.js";
     document.getElementsByTagName("head")[0].appendChild(jquery);
     document.getElementsByTagName("head")[0].appendChild(functions);
   }
   appendScripts();
  </script>
  <link rel="stylesheet" href="css/linktocontent.css" type="text/css" />
  <style type="text/css">
   th { border-bottom: 1px solid #999; }
   td { text-align: left; padding: .05em .5em .05em 0; white-space: nowrap; }

   tr { cursor: pointer; }
   tr.head { cursor: auto; }
   td.nid { display: none; }
  </style>
  <base target="_self" />
 </head>
 <body onload="tinyMCEPopup.executeOnLoad('init();');" style="display: none;">
  <div id="mcBodyWrapper">
   <!-- insert tabs -->
   <div class="tabs">
    <ul>
     <li id="browse_tab" class="current"><span><a href="javascript:mcTabs.displayTab('browse_tab','browse_panel');" onmousedown="return false;">{$lang_linktonode_browse_tab}</a></span></li>
    </ul>
   </div>
   <!-- insert panels -->
   <div class="panel_wrap">
    <div id="browse_panel" class="panel current">
     <form id="form_browse" action="#" onsubmit="return false;">
      <label>{$lang_linktonode_browse_title}</label>
     </form>
    </div>
   </div>
   <form action="#" onsubmit="return false;" name="elemlist">
    <!-- node list -->
    <div class="panel_wrap" id="list">
     <div class="nodes">
      <label>{$lang_linktonode_label_documents}</label>
      <div id="nodelist" class="scrollable accessible">
       <table cellspacing="0" summary="nodelist">
        <thead>
         <tr>
          <th>{$lang_linktonode_thead_title}</th>
          <th>{$lang_linktonode_thead_date}</th>
          <th>{$lang_linktonode_thead_author}</th>
         </tr>
        </thead>
        <tbody>
        </tbody>
       </table>
      </div>
     </div>
    </div>
    <!-- form buttons -->
    <div class="mceActionPanel">
     <div style="float: left">
      <input type="button" id="insert" name="insert" value="{$lang_insert}" onclick="insertAction();" />
     </div>
     <div style="float: right">
      <input type="button" id="cancel" name="cancel" value="{$lang_cancel}" onclick="tinyMCEPopup.close();" />
     </div>
    </div>
   </form>
   <div id="statusImg" style="display: block; position: absolute; left: 125px; background: #ffffff; border: 1px solid #dadfe9; font-size: 14px; color: #333333; width: auto; padding-right: .5em; line-height: 42px; vertical-align: middle;"><img alt="loading" border="0" style="margin: 5px 10px; float: left;" src="images/loading.gif" />{$lang_linktonode_loading}</div>
  </div>
 </body>
</html>
