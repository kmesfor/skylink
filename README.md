# **Skylink**

Skylink is an easy-to-use tool to help users plan the best airline routes based on over 23 million previous flights in the US.
It computes over 200 million data points into an adjacency list graph structure that can be easily searched with Dijkstra's and A* algorithms.

Skylink's UI is custom-built with SFML including custom components, graph interfaces, and more.
Skylink has persistent data storage, fast processing, and intuitive UI to provide a high quality user experience

<br>The full proposal can be found [here](https://docs.google.com/document/d/1c6Y-LKPQIOrkAKLpl_OfGjyv271B_L5RCtheeBo_95U/edit?tab=t.0).
<br>The final report can be found [here](https://docs.google.com/document/d/1ihv4TOMAcxtyh2yAFkQxaejjZ8zbmFpi2Z_L_UES6eo/edit?tab=t.0).
<br>The demo video can be found [here](https://www.youtube.com/watch?v=MLrn_zX0fxk).

## **How to run**

A copy of Skylink's generated graph JSON file necessary for execution can be downloaded at `${PROJECT_ROOT}/dist/data/generated/skylinkgraph.json`.
<br>This file can also be regenerated using the `GenerateGraphFiles` executable.
<br>All executables are available in `${PROJECT_ROOT}/dist` and available as a `release` on the GitHub release sidebar.

### Step 1 Option 1: Copy JSON File
If copying the JSON file, ensure to move it to the correct location before running `${PROJECT_ROOT}/dist/data/generated/skylinkgraph.json`

### Step 1 Option 2: Generate JSON File
If using the `GenerateGraphFiles` executable, place the executable in `${PROJECT_ROOT}/dist` and then execute. Make sure that you copy all the files in `${PROJECT_ROOT}/dist/data/raw` so the graph can be generated

### Step 2: Run Skylink
Run the `Skylink` executable to view the program. Ensure CMake can fetch remote repositories (Catch2, SFML). If this becomes an issue or no internet connection is available, download the repositories and place them manually in the `dist/` folder

### Raw data information (internal documentation):
* ORIGIN: Origin Airport
* DEST: Destination Airport
* ARR_DELAY: Difference in minutes between scheduled and actual arrival time. Early arrivals show negative numbers.
* ARR_DELAY_NEW: Difference in minutes between scheduled and actual arrival time. Early arrivals set to 0.	
* CANCELLED: Cancelled Flight Indicator (1=Yes, 0=No)
* CRS_ELAPSED_TIME: CRS Elapsed Time of Flight, in Minutes (scheduled time amount)
* ACTUAL_ELAPSED_TIME: Elapsed Time of Flight, in Minutes
* AIR_TIME: Flight Time, in Minutes	
* DISTANCE: Distance between airports (miles)

### Extensive Resource Links: 
A shortened version of this list with only major resources used can be found in the project report.
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
* https://stackoverflow.com/questions/16091616/c-sfml-drawing-scrollable-content-within-a-defined-rectangle
* https://www.sfml-dev.org/documentation/2.4.0-fr/classsf_1_1View.php
* https://github.com/SFML/SFML/blob/master/migration.md
* https://stackoverflow.com/questions/51222321/sfml-mouse-getposition-not-working-properly
* https://stackoverflow.com/questions/28256921/mouse-input-isnt-correct-to-world-coordinates
* https://stackoverflow.com/questions/23637765/does-c-provide-a-triple-template-comparable-to-pairt1-t2
* https://stackoverflow.com/questions/14505571/centering-text-on-the-screen-with-sfml
* https://data.opendatasoft.com/explore/dataset/airports-code%40public/export/?flg=en-us&refine.country_name=United+States
* https://stackoverflow.com/questions/54681508/how-can-i-add-a-sort-of-text-box-in-sfml-using-keyboard-input-and-sftext-to-di
* https://www.geeksforgeeks.org/dsa/a-search-algorithm/#
* https://www.transtats.bts.gov/DL_SelectFields.aspx?gnoyr_VQ=FGJ&QO_fu146_anzr=
* https://www.geeksforgeeks.org/dsa/haversine-formula-to-find-distance-between-two-points-on-a-sphere/
* https://theory.stanford.edu/~amitp/GameProgramming/AStarComparison.html
* https://cs.stackexchange.com/questions/71289/where-does-the-heuristic-come-from-in-the-a-star-algorithm-and-how-do-we-know-it
* https://theory.stanford.edu/~amitp/GameProgramming/Heuristics.html
* https://github.com/catchorg/Catch2

