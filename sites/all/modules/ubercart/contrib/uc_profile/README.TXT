INTRO
=====
This module adds a checkout pane to Ubercart's checkout page.  The pane
pulls from the "ubercart" category of the profile.  The fields will be saved
against the users profile.

INSTALLATION
============

1) Place this module directory in your "modules" folder (this will usually be
   "sites/all/modules/"). Don't install your module in Drupal core's "modules"
   folder, since that will cause problems and is bad practice in general. If
   "sites/all/modules" doesn't exist yet, just create it.

2) Enable the UC Profile module in Drupal at:
   administration -> site configuration -> modules (admin/build/modules)

3) Enable the pane at 'admin/store/settings/checkout/edit/panes'.

   
4) Set the pane's title under the Profile Panes settings.
  This will also change the title of the pane on the profile page
  and anywhere else the profile is displayed.
