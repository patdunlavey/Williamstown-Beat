Better formats is a module to add more flexibility to Drupal's core input format system.
Features

    * Set the default format per role.
    * Set the default format per content type.
    * Control allowed formats per content type.
    * Hide format tips.
    * Hide format selection, forcing the default to be used.
    * Expand the selection fieldset by default.
    * Disable the expand/collapse of the selection fieldset.
    * Set selection fieldset title.
    * Set default formats for nodes and comments separately.
    * Works with CCK textareas.
    * Panels comment support.
    * I18n module support.
    * and more.

-------------------------------------------------------------------

Installation:

1. Copy the module folder to your server.
2. Enable the module via the modules page.

-------------------------------------------------------------------

Simple 4-step usage:

1. Go to user permissions (/admin/user/permissions) and set your permissions.
2. Navigate to Site Configuration > Input formats (/admin/settings/filters)
3. There you will find 2 tabs where you can change your settings.
    Defaults (/admin/settings/filters/defauts)
    Settings (/admin/settings/filters/settings)
4. If you enable the "Control formats per node type" option. Go to your content
   type admin page to set those settings (example /admin/content/node-type/page).
   The settings are under the Input format settings fieldset.

-------------------------------------------------------------------

Extended usage and notes:

* The default format will only be set on NEW nodes and comments. The format
  selected when the form is submitted is used for future editing purposes.

* The module is designed to always fall back to default settings when needed.
  This means that when you enable the module before you change any settings,
  it will use your current Drupal settings. Also when you enable conrol per node
  type it will use your global settings until you save the content type with new
  settings.

* The permissions "collapse format fieldset by default" and
  "collapsible format selection" will only work if "Show format selection" is
  also given. This is because those 2 perms only have an effect when there is
  a format selection.

* The permission "collapse format fieldset by default" will only work if
  "collapsible format selection" is also given. This is because the
  fieldset can only be collapsed by default if it is collapsible.

* If you dis-allow a format that is already being used by content, the module
  will do its best to set the correct format. The precidence of the formats are:
  1. Existing format selected when last saved
  2. Content type default format
  3. Global default format
  4. First allowed format
  5. Drupal core site default format
