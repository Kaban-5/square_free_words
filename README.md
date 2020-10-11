# square_free_words
This repository contains code for computing the number of ternary square-free words (OEIS A006156) quickly (several minutes to compute the answers up to 131 on a laptop; with exhaustive enumeration it takes at least several days to go up to 110 on a supercomputer). 
All the source files necessary for reproducing the experiment are included. The file with answers up to 131 is also included. An auxiliary file "dictionary.txt" is excluded, because of its enormous (several gigabytes) size (running "main.cpp" creates a dictionary anyway).

Warning (very important)! The code uses a lot of memory. Read the comment in the beginning of "main.cpp" for more details.

To compute the number of square-free words of length at most n, change the value of constant correct_answer_length in "main.cpp" to n. Also change the values of dictionary_address and answers_address appropriately (read the comment before their declaration in "main.cpp" for more details).

Then, compile and run "main.cpp". It will take some time and a lot of memory. It is advised to close all applications with high memory consumption before running "main.cpp". 

Also, it may be interesting to compile and run "test_overlay.cpp", which proves the main lemma for strings of lengths at most 200.
