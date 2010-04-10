;$Id: README.txt,v 1.2 2009/05/04 21:35:56 cvsadmin Exp $
README.txt for outline module.


==================
== Permissions  ==
==================

______________________________________
1) Permissions defined by book.module:
______________________________________

add content to books
--------------------
The users see the link "Add child page" below a node, allowing them to create a new node which will automatically be added to the book outline.



administer book outlines
------------------------
The users see the link "Add child page" below a node, allowing them to create a new node which will automatically be added to the book outline.
They see the "Outline tab" in a node.
They can add any node into any outline, even nodes that are not regularly allowed in book outlines.


create new books 
----------------
The users can create new books, e.g. new top level nodes acting as book covers of new outlines.


access printer-friendly version
-------------------------------
The users see the link "Show a printer-friendly version of this book page and its sub-pages." below a node.


_________________________________________
2) Permissions defined by outline.module:
_________________________________________

The following book.module permissions are not modified in any way by outline.module:
  - administer book outlines.
  - create new books
  - access printer-friendly version 

By default, outline.module does not modify the permission 'add content to books' set by book.module. 
If you have been using book.module for a while, when you enable outline.module for the first time, none of the current permissions will be affected.
Only when you edit the permissions for a specific book, will the outline.module affect the default permissions.

With outline module, you can edit the 'add content to books'  permissions or a per-book basis.
You can set which roles can add content to which books.
You can also set which individual users can add content to which books.
Thus users who didn't have the 'add content to books' can add content to specific books.
Inversely, users who had that permission, can be denied adding content to specific books.



Note, the link "Create child page" at the bottom of an outlined node will show up if the two following conditions are met:
- the user must be allowed to add content to the current book.
- the user is allowed to create content of the default type set for that book.


==============================
== Your support is welcome  ==
==============================

The module outline is charity-ware.
--------------------------------------

Please contribute back by supporting the charity work of the following web sites. 
None of the web sites listed here are for profit, and none of them carry advertising.
They are all web sites dedicated to creating a better tomorrow for the whole society.

 * http://activistsolutions.org/ Activist Solutions: harvesting grassroots power.
 * htpp://www.reuniting.info/ Reuniting: healing with sexual relationships.
 * http://overshoot.tv/ Overshoot TV: making high quality videos and documentaries promoting environmental and economical sustainability.
 * http://minguo.info/ Minguo.info: promotting better voting systems, and an experiment in direct democracy.
 * http://www.wechange.org/ We Change: because we live in a world of solutions...


You can support those web sites in the following ways:

 * Blog about them.
 * Put links in a block in a sidebar.
 * Put links in any other logical place in your web site, where your visitors will find the information useful.
 * Register and participate if they match your own interest!
 * We also appreciate if, on behalf of this maintainer, you help any charity of your choice, or/and make a donation to them.

Please let the maintainer know about the options you chose. 

Thank you for your support and cooperation.




