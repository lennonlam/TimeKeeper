Time Keeper
=========

Some user-friendly timing objects allow you to time your C++ codes easily.

Object "SimpleTimer", "PreciseTimer" provided a very simple way to start/stop counter, and get the counted time in the middle way.

Object "Stopwatch" has same functions as a stopwatch we use in daily life. User can start / stop / pause / resume the counter. 

Object "TimeKeeper" is a collection of stopwatches. User can manipulate different stopwatches with the name of the stopwatch. This allows you to use different stopwatches to count your processes separately.

Personally, I like to use TimeKeeper with singleton pattern and send it to the global scope, so I can time wherever I wanted and get all the timing results at the end of the program