
#include "../include/libdsaa.h"

#include <stdlib.h>

/*****************************************************************************/

void list_init(struct list* l, struct list_function* f) {
  l->head = NULL;
  l->tail = NULL;
  l->function = f;
  l->size = 0;
}

/*****************************************************************************/

int list_add(struct list *l, void* data) {
	struct list_item *item =  malloc(sizeof(struct list_item));
	if(item == NULL) {
		return -1;
	}
	item->data = data;
	item->next = NULL;
	item->previous = NULL;

	if(l->head == NULL) {
	   l->head = l->tail = item;
	} else {
	   l->tail->next = item;
	   item->previous = l->tail;
	   l->tail = item;
	}
	l->size++;

	return 0;
}

/*****************************************************************************/

int list_add_sort(struct list *l, void* data) {
	struct list_item *item =  malloc(sizeof(struct list_item));

	if(item == NULL) {
		return -1;
	}

	item->data = data;
	item->next = NULL;
	item->previous = NULL;

	if(l->head == NULL) {

		l->head = l->tail = item;

	} else {

		struct list_item* before = NULL;
		struct list_item* after = l->head;

		while(after != NULL) {
	   	    if(l->function->compare(after->data, item->data) >= 0) {
	   	    	break;
	   	    } else {
	   	      before = after;
	   	      after = after->next;
	   	    }
	   	}

		/* before head */
	   	if(before == NULL) {
	   		after->previous = item;
	   		l->head = item;
	   		item->next = after;

	   	} else {
	   		/* after tail */
	   		if(after == NULL) {
	   			l->tail->next = item;
	   			item->previous = l->tail;
	   			l->tail = item;
	   		} else {
	   			/* between head and tail */
	   			before->next = item;
	   			after->previous = item;
	   			item->previous = before;
	   			item->next = after;
	   		}
	   	}
	}

	l->size++;

	return 0;
}

/*****************************************************************************/

int list_find(struct list *l, void* f) {

	if(l->function->find == NULL) {
	  return -1;
	}

	struct list_item* item = l->head;
	while(item != NULL) {
		if(l->function->find(item->data, f) == 1) {
	        return 1;
	    }
	    item = item->next;
	}
	return 0;
}


/*****************************************************************************/

int list_move(struct list *l, void* i, list_item_position p, void* v, void* d) {

    if(l->function->find == NULL) {
	  return -1;
	}

    struct list_item* item = l->head;
    while(item != NULL) {
        if(l->function->find(item->data, i) == 1) {

        	if(item == l->head) {
               if(p == list_item_last) {
        	       l->head = item->next;
        	       l->head->previous = NULL;
               }
        	} else if(item == l->tail) {
        		if(p == list_item_first) {
        		    l->tail = item->previous;
        		    l->tail->next = NULL;
        		}
        	} else {
        		item->previous->next = item->next;
        		item->next->previous = item->previous;
        	}

        	if(p == list_item_last && item != l->tail) {
               l->tail->next = item;
               item->previous = l->tail;
  	           l->tail = item;
  	           l->tail->next = NULL;
        	} else if (p == list_item_first && item != l->head) {
        		l->head->previous = item;
        		item->next = l->head;
       		    l->head = item;
       		    l->head->previous = NULL;
        	}

    	    if(l->function->update != NULL) {
    	    	l->function->update(item->data, v, d);
    	    }

    	    return 1;
    	}
    	item = item->next;
    }
    return 0;
}

/*****************************************************************************/

int list_remove(struct list *l, void *p) {

	if(l->head == NULL) {
		return -1;
	}

	if( *(list_item_position*)p == list_item_first) {

		if(l->function->release != NULL) {
	        l->function->release(l->head->data);
	    }

		if(l->head->next == NULL) {
		  free(l->head);
		  l->head = l->tail = NULL;
		} else {
		  l->head = l->head->next;
	      free(l->head->previous);
	      l->head->previous = NULL;
		}

	    l->size--;
	    return 1;

	} else if(*(list_item_position*)p == list_item_last) {

		if(l->function->release != NULL) {
	        l->function->release(l->tail->data);
	    }

		if(l->tail->previous == NULL) {
		    free(l->tail);
			l->head = l->tail = NULL;
		} else {
			l->tail = l->tail->previous;
			free(l->tail->next);
			l->tail->next = NULL;
		}

		l->size--;
		return 1;

	} else {

	    if(l->function->find == NULL) {
		  return -1;
		}

		struct list_item* item = l->head;
		while(item != NULL) {
		    if(l->function->find(item->data, p) == 1) {

			   if(l->function->release != NULL) {
				   l->function->release(item->data);
			   }

			   if(item->next != NULL && item->previous != NULL ) {
			       item->previous->next = item->next;
				   item->next->previous = item->previous;
			   } else {
				   l->head = NULL;
				   l->tail = NULL;
			   }
			   free(item);

			   l->size--;
		       return 1;
			 }
			 item = item->next;
		}
	}

    return 0;
}

/*****************************************************************************/

int list_print(struct list *l, list_item_position p) {

	if(l->function->print == NULL) {
	  return -1;
	}

	struct list_item* item = (p == list_item_last ? l->tail : l->head);

	int i = 1;
	while(item != NULL) {
	   l->function->print(i++, item->data);
	   item = (p == list_item_last ? item->previous : item->next);
	}

	return 0;
}

/*****************************************************************************/

int list_release(struct list *l) {
	struct list_item* item;

	if(l->head == NULL) {
	    return -1;
	}

	while(l->head != NULL) {
		item = l->head;
	    l->head = l->head->next;
	    l->size--;
		if(l->function->release != NULL) {
	        l->function->release(item->data);
	    }
	    free(item);
	}
	return 0;
}

/*****************************************************************************/

int list_get(struct list *l, void* f, void** list_data) {

	if(l->function->find == NULL) {
	  return -1;
	}

	struct list_item* item = l->head;
	while(item != NULL) {
		if(l->function->find(item->data, f) == 1) {
	        *list_data = item->data;
	        return 1;
	    }
	    item = item->next;
	}
	return 0;
}

/*****************************************************************************/

int list_update(struct list *l, void* i, void* v, void* d) {

	if(l->function->find == NULL) {
	  return -1;
	}

	if(l->function->compare == NULL) {
	  return -1;
	}

	if(l->function->update == NULL) {
	  return -1;
	}

	struct list_item* found = NULL;
    struct list_item* item = l->head;
    while(item != NULL) {
        if(l->function->find(item->data, i) == 1) {
        	l->function->update(item->data, v, d);
        	found = item;
        	break;
        }
        item = item->next;
    }

    if(found == NULL) {
    	return 0;
    }

    if(found == l->head) {
    	item = l->head->next;
    	while(item != NULL ) {
    		if(l->function->compare(item->data, found->data) >= 0) {
    			break;
    		}
    		item = item->next;
    	}

    	/* after tail */
    	if(item == NULL) {
        	l->head = found->next;
        	l->head->previous = NULL;

        	l->tail->next = found;
    		found->next = NULL;
    		found->previous = l->tail;
    		l->tail = found;
    	} else {
    		/* after one element after head */
    		if(item != l->head->next) {
    	    	l->head = found->next;
    	    	l->head->previous = NULL;

    	    	item->previous->next = found;
    	    	item->previous = found;
    	    	found->previous = item->previous->next;
    	    	found->next = item;
    		}
    	}
    } else if (found == l->tail) {
    	item = l->tail->previous;
    	while(item != NULL ) {
    	    if(l->function->compare(item->data, found->data) <= 0) {
    	       break;
    	    }
    	    item = item->previous;
    	}

    	/* before head */
    	if(item == NULL) {
        	l->tail = found->previous;
        	l->tail->next = NULL;

        	l->head->previous = found;
        	found->previous = NULL;
    	    found->next = l->head;
    	    l->head = found;
    	} else {
    		/* before one element before tail */
    	    if(item != l->tail->previous) {
    	    	l->tail = found->previous;
    	    	l->tail->next = NULL;

    	    	found->previous = item;
    	    	found->next = item->next;
    	    	item->next->previous = found;
    	    	item->next = found;
    	    }
    	}
    } else {

    	/* up */
    	if(l->function->compare(found->data, found->next->data) > 0) {
    		item = found->next;
    		while(item != NULL ) {
    			if(l->function->compare(item->data, found->data) >= 0) {
    		        break;
    		    }
    		    item = item->next;
    		}

    		/* after tail */
    		if(item == NULL) {

    			found->previous->next = found->next;
    			found->next->previous = found->previous;

    			l->tail->next = found;
				found->next = NULL;
				found->previous = l->tail;
				l->tail = found;

			} else {
				/* after one element after found */

				found->previous->next = found->next;
    			found->next->previous = found->previous;

				item->previous->next = found;
				item->previous = found;
				found->previous = item->previous->next;
				found->next = item;
			}

    	/* down */
    	} else if (l->function->compare(found->data, found->previous->data) < 0) {
    		item = found->previous;
    		while(item != NULL ) {
    		    if(l->function->compare(item->data, found->data) <= 0) {
    		        break;
    		    }
    		    item = item->previous;
    		}

    		/* before head */
			if(item == NULL) {
    			found->previous->next = found->next;
    			found->next->previous = found->previous;

				l->head->previous = found;
				found->previous = NULL;
				found->next = l->head;
				l->head = found;
			} else {
	    		/* before one element before found */

				found->previous->next = found->next;
	    		found->next->previous = found->previous;

	    	    found->previous = item;
	    	    found->next = item->next;
	    	    item->next->previous = found;
	    	    item->next = found;
	    	}
    	}
    }

    return 1;
}

/*****************************************************************************/
