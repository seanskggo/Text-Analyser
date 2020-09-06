// COMP2521 20T2 Assignment 1
// tw.c ... compute top N most frequent words in file F
// Usage: ./tw [Nwords] File

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include "Dict.h"
#include "WFreq.h"
#include "stemmer.h"

#define STOPWORDS "stopwords"
#define MAXLINE 1000
#define MAXWORD 100

#define STARTING "*** START OF"
#define ENDING   "*** END OF"

#define isWordChar(c) (isalnum(c) || (c) == '\'' || (c) == '-')

/*--------------------- Function Protypes -------------------------*/

// Returns Porter stemmed word
static char *stemmed(char *word);
// Convert to lower case
static void lowercase (char *word);
// Remove any 'non-word' characters wuch as , and .
static void cleanWord (char *word);
// Performs stemming and checks if given word is a stopword before inserting
// into dictionary if required
static WFreq *operate (Dict a, Dict b, char *word);
// Confirms if *** END OF is reached
static bool check(FILE *file, char *word);

/*-----------------------------------------------------------------*/

int main( int argc, char *argv[])
{
   // TODO ... add any variables you need

   FILE  *in;         // currently open file
   Dict   stopwords;  // dictionary of stopwords
   Dict   wfreqs;     // dictionary of words from book
   WFreq *results;    // array of top N (word,freq) pairs
                      // (dynamically allocated)

   char *fileName;    // name of file containing book text
   int   nWords;      // number of top frequency words to show

   char   line[MAXLINE];  // current input line
   char   word[MAXWORD];  // current word

   // process command-line args
   switch (argc) {
   case 2:
      nWords = 10;
      fileName = argv[1];
      break;
   case 3:
      nWords = atoi(argv[1]);
      if (nWords < 10) nWords = 10;
      fileName = argv[2];
      break;
   default:
      fprintf(stderr,"Usage: %s [Nwords] File\n", argv[0]);
      exit(EXIT_FAILURE);
   }

   // build stopword dictionary
   stopwords = newDict();
   in = fopen(STOPWORDS, "r");
   if (in == NULL) {
      fprintf(stderr, "Can't open stopwords\n");
      exit(EXIT_FAILURE);
   }
   while (fscanf(in, "%s[^\n]%*c", word) != EOF) {
      DictInsert(stopwords, word);
   }
   fclose(in);

   // Open File for reading
   in = fopen(fileName, "r");
   if (in == NULL) {
      fprintf(stderr, "Can't open %s\n", fileName);
      exit(EXIT_FAILURE);
   }

   // Skip to line containing "*** START OF"
   while (fgets(word, sizeof(STARTING), in) != NULL) {
      if (strcmp(word, STARTING) == 0) break;
   }

   // Go to next line
   if (fgets(line, sizeof(line), in) == NULL) {
      fprintf(stderr, "Not a Project Gutenberg book\n");
      exit(EXIT_FAILURE);
   }

   // If next line is EOF, exit
   char temp[MAXLINE];
   FILE *test = in;
   if (fgets(temp, sizeof(temp), test) == NULL) {
      fprintf(stderr, "Book is empty\n");
      exit(EXIT_SUCCESS);
   }

   // While not EOF and not reached line containing "*** END OF"
   wfreqs = newDict();
   char ch; int count = 0;
   while ((ch = fgetc(in)) != EOF) {
      if (isWordChar(ch) == true || ch == '*') {
         word[count] = ch;
         count++;
      } else {
         word[count] = '\0';
         count = 0;
         if (check(in, word) == true) break;
         if (word[0] == '*') {
            cleanWord(word);
         }
         if (strlen(word) != 1 && strlen(word) != 0) {
            lowercase(word);
            operate(stopwords, wfreqs, word);
         }
      }
   }

   // compute and display the top N words
   nWords++;
   results = malloc(nWords * sizeof(*results));
   findTopN(wfreqs, results, nWords);  
   for (int i = 0; i < nWords - 1; i++) {
      printf("%7d %s\n", results[i].freq, results[i].word);
   }
   fclose(in);
   return EXIT_SUCCESS;
}

/*-------------------------- Functions ----------------------------*/

static bool check(FILE *file, char *word) {
   if (strcmp(word, "***") == 0) {
      char tmp[MAXWORD];
      FILE *ptr = file;
      fgets(tmp, sizeof("END OF"), ptr);
      if (strcmp(tmp, "END OF") == 0) return true;
   } 
   return false;
}

static void lowercase (char *word) {
   int i = 0;
   while (word[i] != EOF) {
      if (word[i] >= 'A' && word[i] <= 'Z') {
         word[i] = word[i] + ('a' - 'A');
      }
      i++;
   }
}

static char *stemmed(char *word) {
   int last = stem(word, 0, strlen(word) - 1);
   word[last + 1] = '\0';
   return word;
}

static void cleanWord(char *word) {
   char tmp[strlen(word) + 1];
   int i = 0, j = 0;
   while (isWordChar(word[i]) == false && i < strlen(word)) {
      i++;
   }
   if (i == strlen(word)) tmp[0] = '\0';
   for (int k = i ; k < strlen(word) + 1; k++) {
      if (isWordChar(word[k]) != false || word[k] == '\0') {
         tmp[j] = word[k];
         j++;
      } else {
         tmp[j] = '\0';
         break;
      }
   }
   for (int i = 0; i < strlen(tmp) + 1; i++) {
      word[i] = tmp[i];
   }
}

static WFreq *operate (Dict a, Dict b, char *word) {
   WFreq *textWord = DictFind(a, word);
   if (textWord == NULL) {
      stemmed(word);
      return DictInsert(b, word);
   } else {
      return NULL;
   }
}

/*-----------------------------------------------------------------*/
