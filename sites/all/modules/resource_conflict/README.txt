= Resource Conflict =

This module allows for users to book resources for use during events. For
example, a student can book a microscope for use within their lab.

== Requirements ==

CCK and it's nodereference module: http://drupal.org/project/cck
Event: http://drupal.org/project/event

== How does Resource Conflict work? ==

Resource Conflict requires three things to be set up within a content type.  The
content type must have at least one nodereference field.  The content type must
be event-enabled.  The resource conflict option must be enabled, and at least
one nodereference field must be selected. These will be the resources which will
be checked for conflicts.

If there is a pre-existing event which conflicts in both time and resource, a
form error is set and the user is forced to either book a different resource or
change the event times.

== Contact ==
This module was developed by Andrew Berry (andrewberry@sentex.net) for use at
the Protein Dynamics lab at the University of Guelph.
Project Page: http://drupal.org/project/resource_conflict