// COMP2521 20T2 Assignment 1
// Dict.c ... implementsation of Dictionary ADT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "Dict.h"
#include "WFreq.h"

typedef struct _DictNode *Link;

typedef struct  _DictNode {
   WFreq  data;
   int height;
   Link   left;
   Link   right;
} DictNode;

struct _DictRep {
   Link tree;
};

/*---------------------- Local Function Prototypes ---------------------*/

// Recursively finds and inserts the word in avl tree and returns pointer
// of (word, freq) pair
static Link doInsert(Link n, char *word, WFreq *new);
// Simple insertion function - implemented in doInsert
static Link insert(WFreq *new);
// Roof function - finds maximum value of two integers
static int roof(int a, int b);
// Calculates height of node
static int height(Link n);
// Rotates tree/subtree right
static Link rotateRight(Link n);
// Rotataes tree/subtree left
static Link rotateLeft(Link n);
// Recursively finds the word in dictionary and returns the
// (word, freq) pair - else returns NULL
static WFreq *doFind(Link n,char* w);
// Print words in tree using preoder traversal
static void preorder (Link n);

/*----------------------------------------------------------------------*/

// create new empty Dictionary
Dict newDict()
{
   Dict new = malloc(sizeof(*new));
   new->tree = NULL;
   return new;
}

// insert new word into Dictionary
// return pointer to the (word,freq) pair for that word
WFreq *DictInsert(Dict d, char *w)
{
   WFreq *new = malloc(sizeof(*new));
   new->freq = 1;
   new->word = malloc(strlen(w) + 1);
   strcpy(new->word, w);
   d->tree = doInsert(d->tree, w, new);
   WFreq *tmp = doFind(d->tree, w);
   return tmp; 
}

// find Word in Dictionary
// return pointer to (word,freq) pair, if found
// otherwise return NULL
WFreq *DictFind(Dict d, char *w)
{
   return doFind(d->tree, w);
}

// find top N frequently occurring words in Dict
// input: Dictionary, array of WFreqs, size of array
// returns: #WFreqs in array, modified array
int findTopN(Dict d, WFreq *wfs, int n)
{

}

// print a dictionary
void showDict(Dict d)
{
   preorder(d->tree);
}

/*-------------------------- Local Functions -------------------------*/

static void preorder (Link n) {
   if (n == NULL) return;
   printf("%d %s\n", n->data.freq, n->data.word);
   preorder(n->right);
   preorder(n->left);
}


static Link doInsert(Link n, char *word, WFreq *new) {
   // Base Case
   if (n == NULL) {
      Link n = insert(new);
      return n;
   }
   // Find position to insert recursively
   int cmp = strcmp(word, n->data.word);
   if (cmp < 0) {
      n->left = doInsert(n->left, word, new);
   } else if (cmp > 0) {
      n->right = doInsert(n->right, word, new);
   } else {
      n->data.freq++;
      free(new);
      return n;
   }
   // Insertion done
   n->height = 1 + roof(height(n->left), height(n->right));

   // Rebalance the tree 
   // Right Case
   if (height(n->left) - height(n->right) < -1) {
      if (n->right != NULL) {
         // Right-Left Case
         if (height(n->right->left) > height(n->right->right)) {
            n->right = rotateRight(n->right);
         } 
      }
      return rotateLeft(n);
   // Left Case
   } else if (height(n->left) - height(n->right) > 1) {
      if (n->left != NULL) {
         // Left-Right Case
         if (height(n->left->left) < height(n->left->right)) {
            n->left = rotateLeft(n->left);
         } 
      }
      return rotateRight(n);
   } else return n;
}

// Creates new node. Initialises the WFreq values 
static Link insert(WFreq *new) {
   Link n = malloc(sizeof(*n));
   if (n == NULL) {
      fprintf(stderr, "Insufficient Memory.\n");
      exit(EXIT_FAILURE);
   }
   // Creates new node with 0 height and modified WFreq values
   n->height = 0;
   n->left = NULL;
   n->right = NULL;
   n->data = *new;
   return n;
}

static int roof(int a, int b) {
   return (a > b) ? a : b;
}

static int height(Link n) {
   if (n == NULL) {
      return -1;
   } else {
      return n->height;
   }
}

static Link rotateRight(Link n) {
   // If root is NULL
   if (n == NULL) return n;
   // If the left of root is NULL
   if (n->left == NULL) return n;
   else {
      Link newRoot = n->left;
      n->left = newRoot->right;
      newRoot->right = n;
      n->height = roof(height(n->left), height(n->right)) + 1;
      newRoot->height = roof(height(newRoot->left), height(newRoot->right)) + 1;
      return newRoot;
    }
}

static Link rotateLeft(Link n) {
   // If root is NULL
   if (n == NULL) return n;
   // If the right of root is NULL
   if (n->right == NULL) return n;
   else {
      Link newRoot = n->right;
      n->right = newRoot->left;
      newRoot->left = n;
      n->height = roof(height(n->left), height(n->right)) + 1;
      newRoot->height = roof(height(newRoot->left), height(newRoot->right)) + 1;
      return newRoot;
   }
}

static WFreq *doFind(Link n, char* w){
   // Base Case - if word not found, return NULL
   if (n == NULL) return NULL;
   // Find word recursively
   int cmp = strcmp(w, n->data.word);
   if (cmp < 0) {
      return doFind(n->left, w);
   } else if (cmp > 0) {
      return doFind(n->right, w);
   } else {
      WFreq *ptr = &(n->data);
      return ptr;
   }
}

/*--------------------------------------------------------------------*/