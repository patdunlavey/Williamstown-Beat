*******************************************************
    README.txt for userplus.module for Drupal
*******************************************************

This module was developed by the friendly primates at FunnyMonkey.com
to make the user administration process more efficient for administrators.

The userplus module supplements Drupal's user administration with the
following features:

   1. "add multiple users" on a single form
   2. "assign user roles" to multiple users on a single form
   3. "delete multiple users" on a single form

These features can be found alongside Drupal's user administration on the
"Userplus administration" tab at admin/user/userplus.

INSTALLATION:

Put the module in your modules directory.
Enable it via admin/modules.
[optional] Copy the contents of userplus.css and paste it into your theme's
style.css.

CONFIGURATION:

It is not necessary to do any configuration to use userplus.  Users with
'administer users' are able to access all userplus functionality.  In
addition, you may assign 'add users via userplus', 'assign roles via userplus',
and 'assign groups via userplus' permissions to roles in order to give them
selective access to userplus functionality.

There's also an 'administer userplus settings' permission that allows users
to configure userplus.  This permission should not be given out without some
thought.

Userplus can be customized via the admin/settings/userplus form, which allows
you to configure the number of rows that will appear on each of the userplus
administration screens by visiting admin/settings/userplus.  You can also
configure the roles that will appear on each of the 'add multiple users'
and 'assign roles' forms.  This, combined with selective assignment of
permissions to 'assign roles via userplus', should allow you to delegate
some portion of user administration without compromising full integrity of
your site.

The 'show required profile fields' configuration option controls whether
profile fields (provided by the core profile module) appear on the 'add
multiple users' form.  This feature works best with text fields right
now, so you should expect to see some problems with other field types
and should feel free to report them in the userplus issue queue.

TECHNICAL NOTES:

During "add multiple users" user validation bypasses user.module because
errors that occur are reported through form_set_error().  Since our form
contains different fields than the standard "add user" form, it doesn't make
sense to raise errors on the standard fields.  Also, and this is the main
reason, if multiple errors occur in either 'name' or 'mail' when using the
standard user validation process, we will lose errors past the first one
because they get ignored in form_set_error().  To get around this, we do
our own validation in _userplus_validate_user(), however, since we don't
call hook_user('validate', ...) there is a good chance that this module
will not play nicely with modules that implement and depend on
hook_user('validate', ...).

TO DO:

1. Think about how to separate out core functionality from UI in user.module
so we can reuse more code there.

2. Consider merging some or all of userplus with the user_import and
role_delegation modules.

3. Put together a handbook page for advanced user administration on
drupal.org -- users of this module are urged to take this on!
