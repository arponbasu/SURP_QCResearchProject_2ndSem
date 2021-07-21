1. Before running this code, run "sudo apt-get install libboost-all-dev" on your terminal (open the terminal in your current directory). You need to do this only once, the first time you use this code.
2. After that, suppose you want to generate the intermediate code for a certain text file, say t2.txt (BTW if your file is t2, rename it to t2.txt). Bring that file in the current directory.
3. After this compile all the cpp codes together as (note that the name of the executable necessarily has to be ffg) "g++ -o ffg *.cpp *.h -std=c++11". 
4. Now, run "sh ifg.sh t2.txt 1" on your terminal if you want the timer to be activated, run "sh ifg.sh t2.txt 0" otherwise.
5. The code you desire will be written in the file "IntermediatesEvaluation_t2.py" created in the same directory.

