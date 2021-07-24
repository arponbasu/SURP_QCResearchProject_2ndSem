# The TARANG Code Generator
The main software for this project is in the folder "Intermediates". The instructions for using that are given here (below) as well as in a README within that folder itself.
1. Before running this code, run "sudo apt-get install libboost-all-dev" on your terminal (open the terminal in your current directory). You need to do this only once, the first time you use this software.
2. After that, suppose you want to generate the intermediate code for a certain text file (note that the text files have to be of a very specific format. Refer the folder "Test Text files" to see some standard templates), say t2.txt (BTW if your file name is t2, 
rename it to t2.txt). 
Copy that file to this current directory.
3. After this compile all the cpp codes together as (note that the name of the executable necessarily has to be ffg) "g++ -o ffg *.cpp *.h -std=c++11".
4. Now, run "sh ifg.sh t2.txt 1" on your terminal if you want the timer to be activated, run "sh ifg.sh t2.txt 0" otherwise.
5. The code you desire will be written in the file "IntermediatesEvaluation_t2.py" created in this directory.

In addition to this, we also have 3 files : Factorization.cpp, fullexpressionparsing.py and singleexpressionconversion.py. Although the upgraded/"advanced" version of all three are
already present in the Intermediates folder, one may occasionally need to use these in a separate manner. Thus, instructions on how to use these are also given below :
  # Factorization.cpp
1. First, compile this file as "g++ -o Fac Factorization.cpp -std=c++11" (you're free to name the executable as you wish provided you remain consistent throughout).
2. Then, suppose you want to see all the cheapest possible contractions of a tensor expression like r^{}\_{j} t^{a}\_{k} t^{b}\_{i} v^{jk}\_{ab} : To do this, "collect"
all the indices in the expression as j, ak, bi, jkab (basically lower all the upper indices in the tensors and collate them with the lower ones), and then call
the executable as "./Fac j ak bi jkab".
3. You'll see an output which looks something like this : 

"""

O(O^2V^2) : [[j, ak, bi, jkab], [ak, bi, abk], [abk, abik], [i]]

O(O^2V^2) : [[j, ak, bi, jkab], [ak, bi, abk], [ak, aik], [i]]

...

O(O^2V^2) : [[j, ak, bi, jkab], [j, bi, bj], [j, ij], [i]]

O(O^2V^2) : [[j, ak, bi, jkab], [j, jkab, abik], [abik, abk], [i]]

The program took 831 microseconds to execute.

"""

This basically shows all the (cheapest) possible ways in which the tensor expression could be contracted by forming intermediates from 2 terms at a time, to reach the common
final expression (which in this case will only contain the index 'i'). 'O' and 'V' here denote the number of Occupied (represented by indices like i, j, k, l) and Virtual (represented by indices like a, b, c, d)
Orbitals, and terms like O(O^2V^2) denote the time it would take, in the big-O notation, to perform the contraction shown. The cost of two contractions is compared under the
assumption V > O, as recommended by the Guide Achintya Kumar Dutta.

Even though tampering with the code is generally not advisable, if you're curious and want to see _*all*_ possible contractions that an index list could have (this would be
a large number, and would include within itself the cheapest contractions too), open the file, go to the main function, replace the line "display(keepMinimum(traverse(processArgument(argc,argv))));"
by "display(traverse(processArgument(argc,argv)));", save the file and run the program as above.

  # fullexpressionparsing.py
For using this python code, open the terminal and run the command "python3 fullexpressionparsing.py t2.txt". While doing so ensure that the file singleexpressionconversion.py
is in the same directory. This creates a file named "GeneratedCodeFor_t2.py", as in the case of the code generator in the Intermediates folder. This python file is literally
just the translation of your text files (in LateX) to raw python code (while in the actual code generator above intermediates are generated, timers are set up and a lot
more extra processing is done).
