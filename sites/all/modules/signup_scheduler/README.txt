/* $Id: README.txt,v 1.1.2.1.2.1 2008/11/19 23:32:20 jrbeeman Exp $ */

The Signup Scheduler module allows users of the Signup module to define a 
schedule for node signups to be opened and closed.  Views support is included
through providing several fields and filters based on schedule data.


Requirements
------------------------
* Signup module must be installed (http://drupal.org/project/signup)
* Date API module must be installed (http://drupal.org/projects/date)
* Signups will be opened and closed during cron, so your site must have a cron
  job configured.  See http://drupal.org/cron for details.


Setup
------------------------
Once the module is enabled, any signup-enabled node will have an additional
fieldset in the "Signup settings" rollout that will allow configuring the
open / close dates for signups.


Support
------------------------
Please submit any feature requests or bug reports to this project's issue
queue at http://drupal.org/project/issues/signup_scheduler


Author / Maintainer
------------------------
Jeff Beeman: http://drupal.org/user/16734
