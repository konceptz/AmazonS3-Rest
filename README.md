S3 Pre Signed Request
=====================

If you would like to implement a signed S3 request where a token has already been generated, this code will fetch the data and place it in memory, returning a pointer to it's location.


Use case
========

The fetching of data is done on an insecure machine, one where you would not like to share your private amazon keys.


Depricated (**To use this code please have libcURL installed.
http://curl.haxx.se/libcurl)

We now use hand crafted gets.  


Author
======

Arthur Hinds<br>
July 2013
