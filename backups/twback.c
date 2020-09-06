// COMP2521 20T2 Assignment 1
// tw.c ... compute top N most frequent words in file F
// Usage: ./tw [Nwords] File

// Notes : change static memeory allocation of string [20] if you can
// Maybe fscanf
// Dict.c line 122 too many return statements-> change to n = rotateleft etc
// Free malloced new->word (sterlen etc one)
// Reduce space complexity if needed
// when cleaning, apostrophy was removed. fix it
// macro is not working for me in the cleanword function. change later if u can
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
#define TRUE 1
#define FALSE 0

#define STARTING "*** START OF"
#define ENDING   "*** END OF"

#define isWordChar(c) (isalnum(c) || (c) == '\'' || (c) == '-')

/*--------------------- Function Protypes -------------------------*/

// Check if "*** END OF" is reached
static bool verifyEnd (char *line);
// Returns Porter stemmed word
static char *stemmed(char *word);
// Convert to lower case
static void lowercase (char *word);
// Remove any 'non-word' characters wuch as , and .
static void cleanWord (char *word);
// Performs stemming and checks if given word is a stopword before inserting
// into dictionary if required
static WFreq *operate (Dict a, Dict b, char *word);

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
   Dict stopwordDict = newDict();
   in = fopen("stopwords", "r");
   if (in == NULL) {
      fprintf(stderr, "Can't open stopwords\n");
      exit(EXIT_FAILURE);
   }
   while (fscanf(in, "%s[^\n]%*c", word) != EOF) {
      DictInsert(stopwordDict, word);
   }
   fclose(in);

   // Open File for reading
   in = fopen(fileName, "r");
   if (in == NULL) {
      fprintf(stderr, "Can't open stopwords\n");
      exit(EXIT_FAILURE);
   }

   // Skip to line containing "*** START OF"
   while (fgets(word, sizeof("*** START OF"), in) != NULL) {
      if (strcmp(word, "*** START OF") == 0) break;
   }

   // Go to next line
   if (fgets(line, sizeof(line), in) == NULL) {
      fprintf(stderr, "Not a Project Gutenberg book\n");
   }

   // While not EOF and not reached line containing "*** END OF"
   Dict wordDict = newDict();
   while (fscanf(in, "%s[^\n || ^ ]%*c", word) != EOF) {
      if (strcmp(word, "***") == 0) {
         fscanf(in, "%s[^\n || ^ ]%*c", word);
         if (strcmp(word, "END") == 0) break;
      }
      cleanWord(word);
      lowercase(word);
      operate(stopwordDict, wordDict, word);
   }
   // List top = malloc(sizeof(*top));
   // top->data = NULL;
   // top->next = NULL;
   // orderDict(wordDict->tree, top);
   // showDict(wordDict);

   // compute and display the top N words
   
   // for (int i = 0; i < nWords; i++) {
   //    printf("%d %s\n", top->data->freq, top->data->word);
   //    top = top->next;
   // }
   fclose(in);

   return EXIT_SUCCESS;
}

/*-------------------------- Functions ----------------------------*/

static bool verifyEnd (char *line) {
   char a[11];
   strncpy(a, line, sizeof(a));
   if (strcmp(a, "*** END OF") == 0) return TRUE; 
   else return FALSE;
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
   int j = 0;
   for (int i = 0; i < strlen(word) + 1; i++) {
      if (isWordChar(word[i]) != FALSE || word[i] == '\0') {
         tmp[j] = word[i];
         j++;
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
      if (strcmp(word, "") == 0) return NULL;
      return DictInsert(b, word);
   } else {
      return NULL;
   }
}

/*-----------------------------------------------------------------*/
