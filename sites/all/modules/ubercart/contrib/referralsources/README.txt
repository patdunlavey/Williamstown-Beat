$Id

-- LICENSE --

This module is licensed under version 2 of the Gnu General Public License (GPLv2).
http://www.gnu.org/licenses/gpl-2.0.html


-- SUMMARY --

The Referral Sources module allows you to ask your users "Where did you hear about us?" 
anywhere on your site, and track that data in a central location.  It provides a admin
interface for creating referral sources that users can select from a list, and a page
where you can view statistics on referral source selections.  Referral source submissions
are stored in a single table, no matter where they came from, making it easy to track
submissions and create reports based on that data.

The core Referral Sources module allows you to easily embed a referral source selection
to the user registration page, or your own custom forms and modules.

The Referral Sources (Ubercart) module adds a checkout pane prompting users to specify
a referral source, and a order pane so you can view referral sources selected on 
when viewing individual orders.

The Referral Sources (Webform) module adds support for embedding the referral sources
selection in any webform.

The Referral Sources (Example) module demonstrates how you can add the referral source
selection to your own forms and modules.
  

-- DEPENDENCIES --

The Referral Sources (Ubercart) module requires Ubercart.
The Referral Sources (Webform) module requires the Webform module.


-- INSTALLATION --

* Install like any other module.
* If you are using the Referral Sources (Webform) module, you need to copy the 
  Webform component file (referralsources.inc) in 'referralsources/webform/webform_component'
  to the 'components' folder of the Webform module.


-- CONFIGURATION --

* Configure user permissions in Administer >> User management >> Permissions >>
  referralsources module:

  - administer referral source settings

    Users in roles with the "administer referral source settings" permission will have
    access to the Referral Sources settings page in Site Configuration.   

  - edit referral sources

    Users in roles with the "edit referral sources" permission will be able to create,
    edit, and delete Referral Sources in Content Management.
    
  - view referral source statistics
  
    Users in roles with the "view referral source statistics" permission will be able to
    view the Referral Sources Statistics page in Content Management.
  

* Customize the settings in Administer >> Site configuration >> Referral sources.

* Add your referral sources in Administer >> Content Management >> Referral sources.


-- CUSTOMIZATION --

See the included Referral Sources (Example) module for details on how you can include the
referral sources prompt in your own forms and modules.


-- CONTACT --

Current maintainers:
* Adam Overlock (adamo)
  http://drupal.org/user/220401
  http://adamo.org

This project has been sponsored by:
* Lie-Nielsen Toolworks
  Makers of heirloom quality tools.
  http://www.lie-nielsen.com


