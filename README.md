# Text-Analyser
This program takes in a large text file, scans and then displays the top 100 most frequent, stemmed words. An AVL tree is used to complete search and display in O(nlogn) time.

To start the program, run

```make```

then run the main executable, ```tw```, by specifying the text file you want to scan as command line argument e.g.

```./tw  data/0011.txt```

or if you want to specify the number of top words to display e.g.

```./tw  20  data/0011.txt```
