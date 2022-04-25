#include <stdio.h>
#include <stdlib.h>
#include "list.h"


/**
* See list.h for expected behavior of functions
**/

typedef struct node {
    ElemType val;
    struct node *next;
    struct node *prev;
} NODE;


struct list_struct {
    NODE *front;
    NODE *back;
    int list_length;
};


/*
* returns pointer to newly created empty list
*/
LIST *lst_create() {
LIST *l = malloc(sizeof(LIST));

  l->front = NULL;
  l->back = NULL;
  l->list_length = 0;
  return l;
}

void lst_free(LIST *l) {
NODE *p = l->front;
NODE *pnext;

  while(p != NULL) {
    pnext = p->next;   // keeps us from de-referencing a freed ptr
    free(p);
    p = pnext;
  }
  // now free the LIST 
  free(l);
}

void lst_print(LIST *l) {
NODE *p = l->front;

  printf("[");
  while(p != NULL) {
    printf(FORMAT, p->val);
    p = p->next;
  }
  printf("]\n");
}


/**
* TODO:  
*   function:  lst_are_equal
*   description:  returns true(1) if lst1 and lst2
*      contain exactly the same sequenc of values.
*      Returns 0 otherwise.
**/
int lst_are_equal(LIST *lst1, LIST *lst2) {
  NODE *p1, *p2;
  if (lst1->front->val != lst2->front->val || lst1->back->val != lst2->back->val) {
    return 0;
  }
  p1 = lst1->front->next;
  p2 = lst2->front->next;
  while (p1 != NULL) {
    printf("Checking %i against %i\n", p1->val, p2->val);
    if (p1->val != p2->val) {
      return 0;
    }
    p1 = p1->next;
    p2 = p2->next;
  }
  return 1;  // placeholder

}

void helper_print_rev(NODE* p) {
  if (p->next != NULL) {
    helper_print_rev(p->next);
  }
  printf(FORMAT, p->val);
}

/**
* TODO:  print in reverse order 
*
* Try to do without looking at notes!
* Hints:  recursive helper function
*/
void lst_print_rev(LIST *l) {
  printf("[");
  if (l->front != NULL) {
    helper_print_rev(l->front);
  }
  printf("]\n");
}

void lst_push_front(LIST *l, ElemType val) {
NODE *p = malloc(sizeof(NODE));

  p->val = val;
  p->next = l->front;
  l->list_length++;
  // printf("List length is now: %i\n", l->list_length);
  l->front = p;
  if(l->back == NULL)   // was empty, now one elem
      l->back = p;
}

void lst_push_back(LIST *l, ElemType val) {
NODE *p;

  if(l->back == NULL)   // list empty - same as push_front
	lst_push_front(l, val);
  else {  // at least one element before push
	p = malloc(sizeof(NODE));
	p->val = val;
	p->next = NULL;
	l->back->next = p;
  l->list_length++;
  // printf("List length is now: %i\n", l->list_length);
	l->back = p;  
  }
}

/** TODO 
*   modifications to enable this operation
*   to execute in O(1) time.  (You will almost
*   certainly modify other functions and the 
*   data structure itself -- see header).
**/
int lst_length(LIST *l) {
// NODE *p = l->front;
// int n=0;

//   while(p != NULL) {
//     n++;
//     p = p->next;
//   }
//   return n;
  return l->list_length;
}

int lst_is_empty(LIST *l) {
  return l->front == NULL;
}


/** TODO
 *    function:  lst_count
*     description:  Counts number of occurrences 
*     		of x in the list and returns 
*     		that value.
*/
int lst_count(LIST *l, ElemType x) {
  int count = 0;
  NODE *p = l->front;
  while(p != NULL) {
    if (x == p->val) {
      count++;
    }
    p = p->next;
  }
  return count; 
}


/* These are "sanity checker" functions that check to see
*     list invariants hold or not.
*/
int lst_sanity1(LIST *l) {
  if(l->front == NULL && l->back != NULL){
	fprintf(stderr, "lst_sanity1 error:  front NULL but back non-NULL\n");
	return 0;
  }
  if(l->back == NULL && l->front != NULL){
	fprintf(stderr, "lst_sanity1 error:  back NULL but front non-NULL\n");
	return 0;
  }
  return 1;
}

int lst_sanity2(LIST *l) {
  if(l->back != NULL && l->back->next != NULL) {
	fprintf(stderr, "lst_sanity2 error:  back elem has a non-NULL next?\n");
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
static NODE * find_back(NODE *p) {

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
int lst_sanity3(LIST *l) {
NODE *real_back;

  real_back = find_back(l->front);

  return (real_back == l->back);
}



ElemType lst_pop_front(LIST *l) {
  ElemType ret;
  NODE *p;
 

  if(lst_is_empty(l))
	return DEFAULT;   // no-op

  ret = l->front->val;
  
  if(l->front == l->back) {  // one element
	free(l->front);
	l->front = NULL;
	l->back = NULL;
  }
  else {
	p = l->front;  // don't lose node being deleted
	l->front = l->front->next;  // hop over
	free(p);
  }
  l->list_length--;
  // printf("List length is now: %i\n", l->list_length);
  return ret;
}

     
  


/* TODO 
*    
*    if list is empty, we do nothing and return arbitrary value
*    otherwise, the last element in the list is removed and its
*      value is returned.
*
*/
ElemType lst_pop_back(LIST *l) {
  ElemType ret;
  NODE *p;
  NODE *temp;

  if(lst_is_empty(l))
    return DEFAULT;   

  ret = l->front->val;
  
  if(l->front == l->back) {  // one element
    free(l->front);
    l->front = NULL;
    l->back = NULL;
  }
  else {
    p = l->front;
    while(p->next != NULL) {
      temp = p; //holds previous node while iterating
      p = p->next;
    }
    ret = p->val;
    free(p);
    temp->next = NULL;
    l->back = temp;
  }
  l->list_length--;
  // printf("List length is now: %i\n", l->list_length);
  return ret;
} 


/* TODO
*  For full credit, you cannot allocate any new memory!
*
*  description:  self-evident 
*/
void lst_reverse(LIST *l) {
  NODE* cur = l->front;
  NODE* prev = NULL;
  NODE* next = NULL;
  l->back = cur;
  while (cur != NULL) {
    next = cur->next;
    cur->next = prev;
    prev = cur;
    cur = next; //iterate to next node in list
  }
  l->front = prev;
}


/*
* removes first occurrence of x (if any).  Returns
*   0 or 1 depending on whether x was found
*/
int lst_remove_first(LIST *l, ElemType x) {
NODE *p;
NODE *tmp;

  if(l->front == NULL) return 0;
  if(l->front->val == x) {
    lst_pop_front(l);
    l->list_length--;
    return 1;
  }
  // lst non-empty; no match on 1st elem
  p = l->front;

  while(p->next != NULL) {
    if(x == p->next->val) {
      tmp = p->next;
      p->next = tmp->next;
      if(tmp == l->back) 
        l->back = p;
      free(tmp);
      l->list_length--;
      return 1;
     }
     p = p->next;
  }
  return 0;
}


/**
* function: lst_remove_all_slow
* description:  removes all occurrences of x
*   from given list (if any).
*   Returns number of occurrences (i.e., deletions).
* 
* notes:  it is called "slow" because in the worst
*   case, it takes quadratic time.
*   (Discussion of why and when this happens is a
*   lecture topic).
*/
int lst_remove_all_slow(LIST *l, ElemType x) {
  int n=0;
  while(lst_remove_first(l, x))
    n++;
  return n;
}

/** 
 * function: lst_sra_bad_case (sra:  slow_remove_all)
 *
 * description: constructs a list of length n such that 
 * the above function takes quadratic time to remove
 * all occurrences of a specified value. 
 *
 * By convention, the specified value will be 0
 */
LIST *lst_sra_bad_case(int n) {
LIST *l = lst_create();
int i;
int k=n/2;

  for(i=0; i<k; i++) {
	lst_push_front(l, 0);
  }
  for(i=0; i<n-k; i++) {
	lst_push_front(l, 1);
  }
  return l;
}

/** TODO
*   function:  lst_remove_all_fast
*   description:  same behavior/semantics as
*      lst_remove_all_slow.  However, this function
*      must guarantee linear time worst case 
*      runtime (hence, "fast").
*
*   REQUIREMENT:  linear worst-case runtime.
*
*   Note:  your solution may be either recursive or 
*   iteratieve.
**/
int lst_remove_all_fast(LIST *l, ElemType x) {
  printf("removing fast\n");
  NODE* cur = l->front;
  int n = 0;
  int del = 0;
  NODE* temp = NULL;
  NODE* prev = cur;
  while (cur != NULL) {
    printf("test\n");
    if (cur->val == x) {
      temp = cur;
    } else {
      prev = cur;
    }
    cur = cur->next;
    if(temp != NULL) {
      printf("Removing\n");
      printf("Prev: %i\n", prev->val);
      printf("n: %i\n", n);
      printf("temp: %i\n", temp->val);
      if (n == 0) {
        l->front = cur;
      }
      del++;
      free(temp);
      temp = NULL;
      prev->next = cur;
    }
    n++;
  }
  l->list_length = n-del;
  return del;
}

int lst_is_sorted(LIST *l){
NODE *p = l->front;

  while(p!=NULL && p->next != NULL) {
	if(p->val > p->next->val)
	  return 0;
	p = p->next;
  }
  return 1;
}



/** TODO
* function:  lst_insert_sorted
*
* description:  assumes given list is already in sorted order
*	   and inserts x into the appropriate position
* 	   retaining sorted-ness.
* Note 1:  duplicates are allowed.
*
* Note 2:  if given list not sorted, behavior is undefined/implementation
* 		dependent.  We blame the caller.
* 		So... you don't need to check ahead of time if it is sorted.
*/
void lst_insert_sorted(LIST *l, ElemType x) {
  NODE* cur = l->front;
  NODE* prev = NULL;
  int i = 0;
  l->list_length++;
  if (cur == NULL || cur->val > x) {
    lst_push_front(l, x);
    return;
  }
  if (l->back->val < x) {
    lst_push_back(l, x);
    return;
  }
  while (cur->val < x && cur != NULL) {
    prev = cur;
    cur = cur->next;
    i++;
  }
  NODE* new = malloc(sizeof(NODE));
  new->val = x;
  prev->next = new;
  new->next = cur;
}

/** TODO
 * function:  lst_merge_sorted
 *
 * description:  assumes both list a and b are in
 * 	sorted (non-descending) order and merges them
 * 	into a single sorted list with the same
 * 	elements.  
 *
 * 	This single sorted list is stored in a while
 * 	b becomes empty.
 *
 * 	if either of given lists are not sorted, 
 * 	we blame the caller and the behavior is
 * 	implementation dependent -- i.e., don't worry
 * 	about it!
 *
 * 	Example:
 *
 * 		a:  [2 3 4 9 10 30]
 * 		b:  [5 8 8 11 20 40]
 *
 * 		after call on (a,b)
 *
 * 		a:  [2 3 4 5 8 8 9 10 11 20 30 40]
 * 		b:  []
 * 
 * implementation:  should not allocate ANY new list
 * 	nodes -- it should just re-link existing
 * 	nodes.
 *
 * 	Must be linear time in the |a|+|b| -- i.e.,
 * 	the total number of elements being processed.
 */
void lst_merge_sorted(LIST *a, LIST *b){
  NODE* pointer_a = a->front;
  NODE* pointer_b = b->front;
  NODE* temp;
  int length_a = lst_length(a);
  int length_b = lst_length(b);
  int i = 0;
  int j = 0;
  a->list_length = length_a+length_b;
  if (length_b == 0) {
    return;
  }
  if (length_a == 0) {
    a->front = b->front;
    a->back = b->back;
    a->list_length = b->list_length;
    b->front = NULL;
    b->back = NULL;
    b->list_length = 0;
    return;
  }
  b->front = NULL;
  b->back = NULL;
  b->list_length = 0;
  if (pointer_a->val < pointer_b->val) {
    temp = pointer_a;
    pointer_a = pointer_a->next;
    temp->next = pointer_b;
    pointer_b = pointer_b->next;
  } else {
    temp = pointer_b;
    a->front = temp;
    pointer_b = pointer_b->next;
    temp->next = pointer_a;
    pointer_a = pointer_a->next;
  }
  i++;
  j++;
  temp = temp->next;
  while ( i < length_a && j < length_b) {
    // printf("Vals compared\n a: %i b:%i\n", pointer_a->val, pointer_b->val);
    if (pointer_a->val < pointer_b->val) {
      temp->next = pointer_a;
      pointer_a = pointer_a->next;
      i++;
    } else {
      temp->next = pointer_b;
      pointer_b = pointer_b->next;

      j++;
    }
    temp = temp->next;
  }
  if (pointer_a == NULL) {
    temp->next = pointer_b;
    a->back = pointer_b;
  } else {
    temp->next = pointer_a;
  }
}

/**
* TODO
* function:  lst_clone
*
* description:  makes a "deep copy" of the given list a
*   and returns it (as a LIST pointer).
*
*/
LIST * lst_clone(LIST *a) {
  LIST * l = lst_create();

  NODE * cur = a->front;
  if (a->list_length == 0) {
    return l;
  }
  while (cur != NULL) {
    lst_push_back(l, cur->val);
    cur = cur->next;
  }
  return l;
}



/**
* TODO
* function:  lst_from_array 
*
* description:  creates a new list populated with the
*      elements of array a[] in the same order as 
*      they appear in a[] (element at a[0] will be the
*      first element in the list and so-on).  List is
*      returned as a LIST pointer.
*
*      Parameter n indicates the length of the given array.
*
* runtime requirement:  THETA(n)
*/
LIST * lst_from_array(ElemType a[], int n){
  LIST * lst = lst_create();
  for (int i = 0; i<n; i++) {
    lst_push_back(lst, a[i]);
  } 
  return lst;
}


/**
* TODO
* function:  lst_to_array 
*
* description:  allocates an array of ElemType and populates
*         it with the elements in the given list (as with
*         lst_from_array elements are in the same order in
*         both data structures).
*
* runtime requirement:  THETA(n)
*
*/
ElemType * lst_to_array(LIST *lst) {

  ElemType * newarray = malloc(lst_length(lst)*sizeof(ElemType));
  NODE* cur = lst->front;
  int i = 0;
  while (cur != NULL) {
    newarray[i] = cur->val;
    i++;
    cur = cur->next;
  }
  return newarray;
}


/**
* TODO
* function:  lst_prefix
*
* description:  removes the first k elements from the
*               given list which are used to form a new list
*		which is then returned.
*
*		if n is the length of the given list, we have the
*		following boundary conditions:
*
*		  if k==0:
*			lst unchanged and an empty list returned
*		  if k>=n:
*			lst becomes empty and a list containing
*			all elements previously in lst is returned.
*
*		examples:
*
*		  EX1:  lst:  [2, 3, 9, 7, 8]
*			k:    3
*
*			after call:
*			   lst:  [7, 8]
*			   returned list:  [2, 3, 9]
*
*		  EX2  lst:  [2, 3, 9, 7, 8]
*			k:    0
*
*			after call:
*			   lst:  [2, 3, 9, 7, 8]  (unchanged)
*			   returned list:  []
*
*		  EX3  lst:  [2, 3, 9, 7, 8]
*			k:    5
*
*			after call:
*			   lst:  []
*			   returned list:  [2, 3, 9, 7, 8]
*
* REQUIREMENTS:
*
*	RUNTIME:  THETA(n) worst case where n is the length of the given list
*
*       ORDERING:  the ordering of the returned prefix should be the same as
*                  in the given list
*
*       MEMORY:    for full credit, no new nodes should be allocated or deallocated;
*		   you should just "re-use" the existing nodes.  HOWEVER, you will
*		   need to allocate a LIST structure itself (i.e., for the returned
*		   list).
*/
LIST * lst_prefix(LIST *lst, unsigned int k) {

  LIST * newlst = lst_create();
  NODE* temp = lst->front;
  if (lst_length(lst) == 0 || k == 0) {
    return newlst;
  }
  newlst->front = temp;
  newlst->list_length = k;
  lst->list_length = lst_length(lst)-k;
  for (int i = 0; i<k; i++) {
    newlst->back = temp;
    temp = temp->next;
    lst->front = temp;
  }
  // printf("%i", newlst->back->next->val);
  temp = newlst->back;
  temp->next = NULL;
  return newlst;
}




/**
* TODO
* function:  lst_filter_leq
*
* description:  removes all elements of the given list (lst) which
*		are less than or equal to a given value (cutoff)
*		
*		A list containing the removed elements is returned.
*
* examples:
*
*	EX1: 	lst:  [4, 9, 2, 4, 8, 12, 7, 3]
*		cutoff:  4
*
*		after call:
*			lst:  [9, 8, 12, 7]
*			returned list:  [4, 2, 4, 3]
*
*       -----------------------------------------
*	EX2: 	lst:  [6, 5, 2, 1]
*		cutoff:  6
*
*		after call:
*			lst:  []
*	 		returned list:  [6, 5, 2, 1]
*
* REQUIREMENTS:
*
*	RUNTIME:  THETA(n) where n is the length of the given list
*
*       ORDERING:  the ordering of the returned list should be the same as
*                  in the given list
*
*       MEMORY:    for full credit, no new nodes should be allocated or deallocated;
*		   you should just "re-use" the existing nodes.  HOWEVER, you will
*		   need to allocate a LIST structure itself (i.e., for the returned
*		   list).
*			
*/
LIST * lst_filter_leq(LIST *lst, ElemType cutoff) {
  LIST * filteredlist = lst_create();
  if (lst_length(lst) == 0) {
    return filteredlist;
  }
  int first = 0;
  int firstfiltered = 0;
  NODE* cur = lst->front;
  NODE* listtrack;
  NODE* filteredtrack;

  if (lst_length(lst) == 1) {
    if (cur->val <= cutoff) {
      lst->front = NULL;
      lst->back = NULL;
      lst->list_length--;
      listtrack = cur;
      filteredlist->front = listtrack;
      filteredlist->back = listtrack;
    }
    return filteredlist;
  }

  while (cur != NULL) {
    if (cur->val <= cutoff) {
      if (first == 0) {
        listtrack = cur;
        filteredlist->front = listtrack;
        first = 1;
      } else {
        listtrack->next = cur;
        listtrack = listtrack->next;
      }
      lst->list_length--;
      filteredlist->list_length++;
    } else {
      if (firstfiltered == 0) {
        filteredtrack = cur;
        lst->front = filteredtrack;
        firstfiltered = 1;
      } else {
        filteredtrack->next = cur;
        filteredtrack = filteredtrack->next;
      }
    }
    cur = cur->next;
  }
  
  filteredtrack->next = NULL;
  lst->back = filteredtrack;
  listtrack->next = NULL;
  filteredlist->back = listtrack;
  return filteredlist;
}

/**
* TODO
* function:  lst_concat
*
* description:  concatenates lists a and b; resulting
*    concatenation is reflected in list a; list b becomes
*    empty.
*
*    example:
*
*	EX1:  a: [2, 9, 1]
*	      b: [5, 1, 2]
*
*	after call:
*
*		a: [2, 9, 1, 5, 1, 2]
*		b: []
*
* REQUIREMENTS:  
*
*     runtime:  O(1)
*
*     sanity:  this operation makes sense when a and b
*		are distinct lists.  For example, we don't
*		want/allow the caller to do something like
*		this:
*
*			LIST *my_list;
*
*			lst_push_front(my_lst, 4);
*			lst_push_front(my_lst, 2);
*
*			lst_concat(my_lst, my_lst);
*
*		your implementation must detect if it is being
*		called this way.  If so the function does nothing
*		and (optionally) prints an error message to
*		stderr.
*	
*/
void lst_concat(LIST *a, LIST *b) {
  if (a == b) {
    fprintf(stderr, "Cannot concatenate a list with itself!\n");
    return;
  }
  if (lst_length(a) == 0 && lst_length(b) == 0) {
    return;
  }
  if (lst_length(a) == 0) {
    a->front = b->front;
    a->back = b->back;
    a->list_length = lst_length(b);
    b->front = NULL;
    b->back = NULL;
    b->list_length = 0;
    return;
  }
  a->back->next = b->front;
  a->back = b->back;
  a->list_length = lst_length(a)+lst_length(b);
  b->front = NULL;
  b->back = NULL;
  b->list_length = 0;
}