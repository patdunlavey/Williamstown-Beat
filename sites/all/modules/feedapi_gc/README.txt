
FeedAPI Garbage Collector
=========================

This FeedAPI processor unpublishes feed item nodes that are not present on the 
feed.

Installation
============

Install module, go to feed content type, enable FeedAPI Garbage Collector as
processor on the feed content type. Use only in conjunction with FeedAPI Node
processor. Make sure that FeedAPI Garbage Collector has a higher weight than
FeedAPI Node processor.