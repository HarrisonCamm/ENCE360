/*
 * linkedList.c - An exercise in function pointers and lists.
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>


// We prefer to use typedefs with our structs, even in messy cases like linked lists. 
// Note that quite often low-level or OS code won't use them, so you need to be able to use struct tags everywhere. 
typedef struct Link_s Link;
struct Link_s {
	Link *next;
	int32_t value;
};


/**
 * Print all the values in a linked list structure
 */
void list_print(Link *list) {
	Link *l = list;
	while (l != NULL) {
		printf("%d", l->value);

		if (l->next) {
			printf(", ");
		}

		l = l->next;
	}

	printf("\n");
}


//
// Append a value to the front of a linked list
// the returned list now looks like:  head->rest of list 
//

Link *list_append(int32_t x, Link *head) {
	Link *head_ = (Link*)malloc(sizeof(Link));
	head_->next = head;
	head_->value = x;

	return head_;
}





/**
 * Reverse a linked list in place (modifies original list)
 */
Link *list_reverse(Link *list) {
	Link *head = NULL;

	Link *l = list;
	while (l != NULL) {
		Link *next = l->next;
		l->next = head;
		head = l;

		l = next;
	}

	return head;
}

//
// Iteratively compute the fibonacci sequence and store the results
// in a Linked list structure. Note the first 'head' of a list should be a NULL pointer.
//  
// fib (0) = 1
// fib (1) = 1
// fib (n) = fib(n - 1) + fib(n - 2)
//
// fib = 1, 1, 3, 5, 8, 13...
//

Link *fibonacci(int32_t n) {
	Link *head = NULL;
	int32_t prev = 1;
	int32_t latest = 1;

	if (n > 0) {
		head = list_append(1, head);
	}

	if (n > 1) {
		head = list_append(1, head);
	}

	for (int32_t i = 2; i < n; i++) {
		head = list_append(latest + prev, head);

		prev = latest;
		latest = head->value;
	}

	return list_reverse(head);
}




//
// Write a function called "list_map" which takes a linked list, 
// and a function pointer, and return a new list with values transformed using the function given.
//
// There's a question on the Quiz about this - so do this first!
//

// Link *list_map(...) {
// TODO: Implement me!
// }



/**
 * Free the linked list structure for this you'll need to
 * traverse the list.
 */
void list_free(Link *list) {
	// TODO: Implement me
}



// Our function to transform the elements of a list
int32_t square(int32_t x) {
	return x * x;
}



int main(void) {
	Link *fib = fibonacci(10);
	// Link *fib_sq = list_map(fib, square);  

	// print out our list of fibonacci^2 (in reverse)
	// 1, 1, 4, 9, 25, 64, 169, 441, 1156, 3025
	// list_print(fib_sq);
	list_print(fib);

	// list_free(fib_sq);
	list_free(fib);

	return 0;
}
