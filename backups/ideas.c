/*--------------------------- Queue ADT----------------------------*/

static Queue *newQueue(void) {
   Queue *new = malloc(sizeof(*new));
   new->head = NULL;
   new->tail = NULL;
   return new;
}

static void QueueJoin(Queue *Q, Link n) {
   assert(Q != NULL);
   if (Q->head == NULL) 
      Q->head = n;
   if (Q->tail != NULL) 
      Q->tail->left = n;
   Q->tail = n;
}

static Link QueueLeave(Queue *Q) { // dont forget to free in main
   assert (Q != NULL);
	assert (Q->head != NULL);
   Link old = Q->head;
	Q->head = old->left;
	if (Q->head == NULL)
		Q->tail = NULL;
	return old;
}

static bool QueueIsEmpty(Queue *Q) {
   return Q->head == NULL;
}

static void dropQueue(Queue *Q);




//--------------------------------------------------------------

static void orderDict(Link n, List a) {
   assert(n != NULL);
   Queue *line = newQueue();
   QueueJoin(line, n);
   while (QueueIsEmpty(line) != true) {
      Link leave = QueueLeave(line);
      WFreq *ptr = &leave->data;
      joinList(a, ptr);
      if (n->left != NULL) 
         QueueJoin(line, n->left);
      if (n->right != NULL) 
         QueueJoin(line, n->right);
   }
}


static List joinList (List n, WFreq *joiner) {
   assert(n != NULL);
   // NULL case
   if (n->data == NULL) {
      n->data = joiner;
      return n;
   // Bigger than head case   
   } else if (joiner->freq >= n->data->freq) {
      List new = malloc(sizeof(*new));
      new->data = joiner;
      new->next = n;
      return new;
   // Less than head case
   } else {
      List behind = n;
      List front = n->next;
      while (front != NULL && joiner->freq <= front->data->freq) {
         behind = behind->next;
         front = front->next;
      }
      List new = malloc(sizeof(*new));
      new->data = joiner;
      new->next = front;
      behind->next = new;
      return n;
   }
}

//========================================

/*--------------------------- Queue ADT----------------------------*/

static Queue *newQueue(void);
static void QueueJoin(Queue *Q, Link n);
static Link QueueLeave(Queue *Q);
static bool QueueIsEmpty(Queue *Q);
static void dropQueue(Queue *Q);


//============

// Inserts (w,f) pair in order
static List joinList (List n, WFreq *joiner);
// Traverse through the dictionary and add to singly linked list
static void orderDict(Link n, List a);

//=====================


/*---------------------- Created Structs --------------------------*/

typedef struct LinkedList *List;

typedef struct _DictNode *Link;

struct LinkedList {
   WFreq *data;
   List next;
};

typedef struct Queue {
	Link head; // ptr to first node
	Link tail; // ptr to last node
} Queue;

typedef struct  _DictNode {
   WFreq  data;
   Link   left;
   Link   right;
} DictNode;

struct _DictRep {
   Link tree;
};