/**
* $Id: editor_plugin_src.js,v 1.2 2007/10/08 15:20:16 stborchert Exp $
*/

/* Import plugin specific language pack */
tinyMCE.importPluginLanguagePack('linktonode', 'en,de,fr');
var TinyMCE_LinkToNodePlugin = {
  getInfo : function() {
    return {
      longname : 'Link to node'
    }
  },

  getControlHTML : function(cn) {
    switch (cn) {
      case "linktonode":
        return tinyMCE.getButtonHTML(cn,
          'lang_linktonode_image_desc',
          '{$pluginurl}/images/linktonode.gif',
          'mceLinktonode');
    }
    return "";
  },

  execCommand : function(editor_id, element, command, user_interface, value) {
    switch (command) {
      case "mceLinktonode":
        var anySelection = false;
        var inst = tinyMCE.getInstanceById(editor_id);
        var focusElm = inst.getFocusElement();
        var selectedText = inst.selection.getSelectedText();

        var template = new Array();

        template['file']   = '../../plugins/linktonode/popup.php';
        template['width']  = 400;
        template['height'] = 295;

        // Language specific width and height addons
        template['width']  += tinyMCE.getLang('lang_linktonode_delta_width', 0);
        template['height'] += tinyMCE.getLang('lang_linktonode_delta_height', 0);

        tinyMCE.openWindow(template, {editor_id: editor_id, inline: "no", resizable: "no"});

        return true;
    } // switch
    return false;
  },

  handleNodeChange : function(editor_id, node, undo_index, undo_levels, visual_aid, any_selection) {
    if (node == null) {
      return;
    }

    do {
      if (node.nodeName == "A" && tinyMCE.getAttrib(node, 'href') != "") {
        tinyMCE.switchClass(editor_id + '_linktonode', 'mceButtonSelected');
        return true;
      }
    } while ((node = node.parentNode));

    if (any_selection) {
      tinyMCE.switchClass(editor_id + '_linktonode', 'mceButtonNormal');
      return true;
    }

    return true;
  }

};
// Adds the plugin class to the list of available TinyMCE plugins
tinyMCE.addPlugin("linktonode", TinyMCE_LinkToNodePlugin);
