TODO: update this page

Resources:
* Switched from CORGIS database to BTS database (https://www.transtats.bts.gov/DL_SelectFields.aspx?gnoyr_VQ=FGJ&QO_fu146_anzr=) because we need information on flight expected vs actual time, distance, cancelled, and specific flight paths
* Also using a lookup table to convert airport codes into location information

Raw data information (internal documentation):
* ORIGIN: Origin Airport
* DEST: Destination Airport
* ARR_DELAY: Difference in minutes between scheduled and actual arrival time. Early arrivals show negative numbers.
* ARR_DELAY_NEW: Difference in minutes between scheduled and actual arrival time. Early arrivals set to 0.	
* CANCELLED: Cancelled Flight Indicator (1=Yes, 0=No)
* CRS_ELAPSED_TIME: CRS Elapsed Time of Flight, in Minutes (scheduled time amount)
* ACTUAL_ELAPSED_TIME: Elapsed Time of Flight, in Minutes
* AIR_TIME: Flight Time, in Minutes	
* DISTANCE: Distance between airports (miles)

Resource Links (update as used):
* https://stackoverflow.com/questions/44990068/using-c-typedef-using-type-alias
* https://github.com/nlohmann/json (download link: https://github.com/nlohmann/json/releases/tag/v3.12.0)
* https://stackoverflow.com/questions/76489630/explanation-of-nodiscard-in-c17
* https://json.nlohmann.me/api/basic_json/
* https://www.w3schools.com/cpp/cpp_exceptions.asp
* https://en.cppreference.com/w/cpp/language/throw.html
* https://stackoverflow.com/questions/71658440/c17-create-directories-automatically-given-a-file-path
* https://www.geeksforgeeks.org/cpp/file-system-library-in-cpp-17/
* https://stackoverflow.com/questions/2390912/checking-for-an-empty-file-in-c
* https://www.geeksforgeeks.org/cpp/friend-class-function-cpp/
* https://www.geeksforgeeks.org/cpp/csv-file-management-using-c/
* https://stackoverflow.com/questions/5674374/remove-double-quotes-from-a-string-in-c
* https://www.geeksforgeeks.org/cpp/measure-execution-time-function-cpp/
* https://takeuforward.org/data-structure/dijkstras-algorithm-using-priority-queue-g-32/
* https://forum.processing.org/one/topic/dijkstra-need-multiple-paths.html
* https://www.geeksforgeeks.org/computer-graphics/sfml-graphics-library-quick-tutorial/
* https://www.sfml-dev.org/documentation/3.0.1/
* https://stackoverflow.com/questions/34458791/making-custom-types-drawable-with-sfml
* https://www.sfml-dev.org/documentation/3.0.0/classsf_1_1RenderTexture.html
* https://stackoverflow.com/questions/42228164/how-to-color-a-line-in-sfml