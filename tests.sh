#!/bin/sh

./tgrep data/small 06:52-8:55
#Feb  9 06:52:00 blah blah blah blah blah blah blah blah blah blah blah blah 
#Feb  9 07:54:56 blah blah blah blah blah blah blah blah blah blah blah 

echo --------------------------

./tgrep data/small 05:58-7:00
#Feb 10 05:58:57 blah blah blah 
#Feb 10 06:58:22 blah blah blah blah blah blah blah blah blah blah blah 

echo --------------------------

./tgrep data/small 06:50-6:51
#

echo --------------------------

./tgrep data/empty 05:58-7:00
#

echo --------------------------

./tgrep data/dateonly 06:52-06:53
# Feb  9 06:52:00

echo --------------------------

./tgrep data/dateonly 06:50
#

echo --------------------------

./tgrep data/broken-date 17:10-21:00
#Feb  9 17:10:31 blah blah blah blah blah blah blah blah 
#Feb  9 1 blah blah blah blah blah blah blah blah blah blah blah blah 
#Feb  9 19:05:07 blah blah blah blah blah blah blah blah blah blah 
#Feb  9 20:06:16 blah blah blah blah blah blah blah blah blah blah blah

echo --------------------------

./tgrep data/finish-mid-date 14:00-16:08
#Feb  9 14:02:22 blah blah blah blah blah blah blah blah blah blah 
#Feb  9 15:03:05 blah blah blah blah blah blah blah blah blah blah 
#Feb  9 16:07:20 blah blah blah blah blah blah 

echo --------------------------

# Failure
./tgrep data/finish-mid-date 18:00-19:00
#