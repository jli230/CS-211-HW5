#include <stdio.h>
#include <stdlib.h>

#include "sq.h"

#define INITCAP 1000000

typedef int ElemType;
#define FORMAT " %i "


//from deq.c

typedef struct dnode {
    ElemType val;
    struct dnode *next;
    struct dnode *prev;
} DNODE;


typedef struct deq_struct {
    DNODE *front;
    DNODE *back;
} DEQ;


/*
* returns pointer to newly created empty list
*/
DEQ *deq_create() {
DEQ *q = malloc(sizeof(DEQ));

  q->front = NULL;
  q->back = NULL;
  return q;
}

void deq_free(DEQ *q) {
DNODE *p = q->front;
DNODE *pnext;

  while(p != NULL) {
    pnext = p->next;   // keeps us from de-referencing a freed ptr
    free(p);
    p = pnext;
  }
  // now free the DEQ 
  free(q);
}

void deq_print(DEQ *q) {
DNODE *p = q->front;

  printf("[");
  while(p != NULL) {
    printf(FORMAT, p->val);
    p = p->next;
  }
  printf("]\n");
}


// TODO: non-recursive print-reverse
void deq_print_rev(DEQ *q) {
  DNODE *p = q->back;
  printf("[");
  while(p != NULL) {
    printf(FORMAT, p->val);
    p = p->prev;
  }
  printf("]\n");
}

void deq_push_front(DEQ *l, ElemType val) {
DNODE *p = malloc(sizeof(DNODE));

  p->val = val;
  p->next = l->front;
  p->prev = NULL;
  
  l->front = p;
  if(l->back == NULL)   // was empty, now one elem
      l->back = p;
  else{
    p->next->prev = p;
  }
}

/* TODO */
void deq_push_back(DEQ *l, ElemType val) {
  DNODE *p = malloc(sizeof(DNODE));
  p->val = val;
  p->next = NULL;
  p->prev = l->back;
  if (l->front == NULL) {
    l->front = p;
  } else {
    l->back->next = p;
  }
  l->back = p;
}

/* TODO */
int deq_pop_front(DEQ *l, ElemType *val) {
  if (l->front == NULL) {
    return 0;
  }
  DNODE *p = l->front;
  *val = p->val;
  if(l->front == l->back) {
    free(l->front);
    l->front = NULL;
    l->back = NULL;
    return 1;
  }
  DNODE *q = p->next;
  l->front = q;
  q->prev = NULL;
  free(p);
  return 1;
}

/* TODO */
int deq_pop_back(DEQ *q, ElemType *val) {
  if (q->front == NULL) {
    return 0;
  }
  DNODE *p = q->back;
  *val = p->val;
  if(q->front == q->back) {
    free(q->front);
    q->front = NULL;
    q->back = NULL;
    return 1;
  }
  DNODE *prev = p->prev;
  q->back = prev;
  prev->next = NULL;
  free(p);
  return 1;
} 


int deq_is_empty(DEQ *q) {
  return q->front == NULL;
}




/* These are "sanity checker" functions that check to see
*     list invariants hold or not.
*/
int deq_sanity1(DEQ *q) {
  if(q->front == NULL && q->back != NULL){
    fprintf(stderr, "lst_sanity1 error:  front NULL but back non-NULL\n");
    return 0;
  }
  if(q->back == NULL && q->front != NULL){
    fprintf(stderr, "lst_sanity1 error:  back NULL but front non-NULL\n");
    return 0;
  }
  return 1;
}

int deq_sanity2(DEQ *q) {
  if(q->back != NULL && q->back->next != NULL) {
    fprintf(stderr, "lst_sanity2 error:  back elem has a non-NULL next?\n");
    return 0;
  }
  return 1;
}
int deq_sanity3(DEQ *q) {
  if(q->front!= NULL && q->front->prev != NULL) {
    fprintf(stderr, "lst_sanity3 error:  front elem has a non-NULL prev?\n");
    return 0;
  }
  return 1;
}


/*
* function:  find_back()
* description:  returns a pointer to the last
*               node of the given list.
*		Note that we are operating at
*		the "node level".
*
*		if p is NULL, NULL is returned.
*
* purpose:  mostly for debugging -- enables sanity3
*/
static DNODE * find_back(DNODE *p) {

  if(p ==  NULL)
    return NULL;

  while(p->next != NULL) {
    p = p->next;
  }
  return p;
}

/*
*   makes sure that the back pointer is also the last reachable
*    node when you start walking from front.
*    HINT:  use pointer comparison
*/
int deq_sanity4(DEQ *q) {
DNODE *real_back;

  real_back = find_back(q->front);

  return (real_back == q->back);
}



/**
* file:  sq.c
*
* description:  simple implementation of the 
*		service queue ADT as specified
*		in sq.h
*
*		(Believed to be) functionally correct,
*		but doe not meet all of the runtime
*		requirements specified in sq.h
*
*		Acts as a client of the linked list
*		ADT previously developed.
*/

struct service_queue {
    DEQ * the_queue;
    DEQ * buzzer_bucket;
    DNODE ** buzzer_id2pointer_map;
    int total;
    int capacity;
    int queue_length;
};

/**
* Function: sq_create()
* Description: creates and intializes an empty service queue.
* 	It is returned as an SQ pointer.
*/
SQ * sq_create() {
SQ *q;

  q = malloc(sizeof(SQ));

  q->the_queue = deq_create();
  q->queue_length = 0;
  q->buzzer_bucket = deq_create();
  q->buzzer_id2pointer_map = malloc(INITCAP*sizeof(DNODE *));
  q->total = 0;
  q->capacity = INITCAP;
  return q;
}

/**
* Function: sq_free()
* Description:  see sq.h
*
* RUNTIME REQUIREMENT:  O(N_t)  where N_t is the number of buzzer 
*	IDs that have been used during the lifetime of the
*	service queue; in general, at any particular instant
*	the actual queue length may be less than N_t.
*
*	[See discussion of in header file]
*
* RUNTIME ACHIEVED:  ???
*
*/
void sq_free(SQ *q) {

  deq_free(q->the_queue);
  deq_free(q->buzzer_bucket);
  free(q->buzzer_id2pointer_map);
  free(q);
}

/**
* Function: sq_display()
* Description:  see sq.h
*
* REQUIRED RUNTIME:  O(N)  (N is the current queue length).
* ACHIEVED RUNTIME:  O(N)  YAY!!
*/
void sq_display(SQ *q) {

  printf("current-queue contents:\n    ");
  deq_print(q->the_queue);
  printf("\n");
}

/**
* Function: sq_length()
* Description:  see sq.h
*
* REQUIRED RUNTIME:  O(1)
* ACHIEVED RUNTIME:  ???
*/
int  sq_length(SQ *q) {
  return q->queue_length;
}

/**
* Function: sq_give_buzzer()
* Description:  see sq.h
*
* REQUIRED RUNTIME:  O(1)  ON AVERAGE or "AMORTIZED":  in other words, if 
*		there have been M calls to sq_give_buzzer, the total
*		time taken for those M calls is O(M).
*
*		An individual call may therefore not be O(1) so long
*		as when taken as a whole they average constant time.
*
*		(Hopefully this reminds you of an idea we employed in
*		the array-based implementation of the stack ADT).
*
* ACHIEVED RUNTIME:  ???
*/
int  sq_give_buzzer(SQ *q) {
  int buzzer;

  if(!deq_is_empty(q->buzzer_bucket)) {
    deq_pop_front(q->buzzer_bucket, &buzzer);
    deq_push_back(q->the_queue, buzzer);
    q->buzzer_id2pointer_map[buzzer] = q->the_queue->back;
    q->queue_length++;
    return buzzer;
  }
  else {
    /*  invariant:  
        if no re-useable buzzers, the buzzers 
        in the queue are {0,1,2,...,N-1} where
        N is the queue length.

        Thus, the smallest available new buzzer 
        is N
        */
    q->total++;
    if (q->total > q->capacity-1) {
      DNODE ** new_items;
      int i;

      q->capacity = 2 * q->capacity;
      new_items = malloc(q->capacity * sizeof(DNODE *));
      for(i=0; i<=q->total-1; i++) 
        new_items[i]= q->buzzer_id2pointer_map[i];

      free(q->buzzer_id2pointer_map);
      q->buzzer_id2pointer_map= new_items;
    }
    buzzer = sq_length(q);
    deq_push_back(q->the_queue, buzzer);
    q->buzzer_id2pointer_map[buzzer] = q->the_queue->back;
    q->queue_length++;
    return buzzer;
  }
}

/**
* function: sq_seat()
* description:  see sq.h
*
* REQUIRED RUNTIME:  O(1)
* ACHIEVED RUNTIME:  ???
*/
int sq_seat(SQ *q) {
int buzzer;

	if(deq_is_empty(q->the_queue))
	  return -1;
	else{
	  deq_pop_front(q->the_queue, &buzzer);
    q->buzzer_id2pointer_map[buzzer] = NULL;
	  deq_push_front(q->buzzer_bucket, buzzer);
    q->queue_length--;
	  return buzzer;
	}
} 



/**
* function: sq_kick_out()
*
* description:  see sq.h
*
* REQUIRED RUNTIME:  O(1)
* ACHIEVED RUNTIME:  ???
*/
int sq_kick_out(SQ *q, int buzzer) {
  DNODE * p = q->buzzer_id2pointer_map[buzzer];
  if(p != NULL) {
    DNODE * prev = p->prev;
    DNODE * next = p->next;
    free(p);
    q->buzzer_id2pointer_map[buzzer] = NULL;
    q->queue_length--;
    deq_push_front(q->buzzer_bucket, buzzer);
    if (q->the_queue->front == p) {
      deq_pop_front(q->the_queue, &buzzer);
      return 1;
    } else if (q->the_queue->back == p) {
      deq_pop_back(q->the_queue, &buzzer);
      return 1;
    }
    prev->next = next;
    next->prev = prev;
    return 1;
  }
  else {
    return 0;
  }
}

/**
* function:  sq_take_bribe()
* description:  see sq.h
*
* REQUIRED RUNTIME:  O(1)
* ACHIEVED RUNTIME:  ???
*/
int sq_take_bribe(SQ *q, int buzzer) {
  DNODE * p = q->buzzer_id2pointer_map[buzzer];
  /* remove buzzer then push it on front */
  if(p != NULL) {
    DNODE * prev = p->prev;
    DNODE * next = p->next;
    if (p == q->the_queue->front) {
      return 1;
    }
    if (next != NULL && prev != NULL) {
      prev->next = next;
      next->prev = prev;
    } else if (next == NULL && prev == NULL) {
      return 1;
    } else if (p == q->the_queue->back) {
      deq_pop_back(q->the_queue, &buzzer);
    }
    q->buzzer_id2pointer_map[buzzer] = NULL;
    deq_push_front(q->the_queue, buzzer);
    q->buzzer_id2pointer_map[buzzer] = q->the_queue->front;
    return 1;
  }
  else {
    /* person has to be in line to offer a bribe */
    return 0;
  }
}





