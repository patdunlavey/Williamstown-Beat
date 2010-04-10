OG Expire - module allowing memberships in organic groups to expire.

Whenever you add member to the group, you can set an expiration date.
You can set a default expiration offset - default form value for expiration dates.
Users and administrators can receive emails prior to expiration.
Provides Views integration.
Provides API to set/get expiration date.

Installation
------------

1) Download og_expire from http://drupal.org/project/og_expire and untar to
   sites/all/modules or sites/yourmultisite.com/modules
2) Enable Organic Groups Membership Expiration module at example.com/admin/build/modules
3) Essential step! Go to example.com/admin/build/views and find og_members
   view. Click 'Edit' and click "+" sign in Fields section. Find "OG: Membership
   expiration date" field in "Organic groups" group and add it to the view. Check
   "If viewer is group administrator, include link to change expiration date.",
   click "Update" and then "Save" the view.
4) Use the module.
5) Don't forget to run cron.


API
---

Set expiration date, pass $time as NULL if you want to remove the expiration. You must be group administrator.
og_expire_set($nid, $uid, $time = NULL)

Get expiration date for organic group ID $nid and user $uid.
og_expire_get($nid, $uid)


Author
------
Jakub Suchy: jakub antispamdot suchy antispamAT technoergonomics nospamDOT com

Sponsored by Technoergonomics: http://technoergonomics.com
Sponsored by Drupal.cz: http://www.drupal.cz
