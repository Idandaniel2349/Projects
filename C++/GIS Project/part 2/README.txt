In this part we are adding a simulator that can run several Navigation 
algorithms with several GIS libraries, using multithreading.

This part is meant to run several navigation algorhitem with several
diffrent GIS and score them accroding to a consensus
(see more-info file for more information).

For that we added an absract interfence for the main classes we need 
to work with in the simulator ( For general use), the previous files 
are in "GIS" and "Navigation" folders, while the new files and classes 
needed for the simulator are in the "simulator" folder.

For the purpouse of using multiple diffrent GIS and navigations, we 
have a registration for each GIS and Navigation class

In the simulator folder:

- The main file: Is responsible for the main functionality of the simulator:
   Get the information(diffrent GIS,diffrent Navigation, requests).
   then we calculate the consensus of the diffrent GIS and Navigation
   files and see which ones gives us better results ( the scores accroding 
   to the consensus) and save the output to a file.

   We do our tasks through multithreading and a custom queue for
    multithreading that put and remove tasks from it(the tasks are finding
    shortest/fastest path from one point to another given in a file/see 
    more-info file for more information).

    To run: run the makefile and then run the command as seen in the more-info file
    with the "gis_sos" and "navigation_sos" folders, for a map and requests
    files you can use "complicatedMap"(as map) and "req" or take a 
    map JSON file from "Tests" folder and make a request file.

    More information documented in the code itself and more info file.

    Enjoy

