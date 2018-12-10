#ifndef VM_FRAME_H
#define VM_FRAME_H

#include "threads/thread.h"
#include <list.h>


struct frame_entry {
	struct list_elem elem;
	void* page_addr;
	void* physical_addr;
	struct thread* t;
};

void frame_table_init();
void frame_table_insert(void* kpage, void* upage);
void frame_table_delete(frame_entry * f);
void * frame_allocate(void * kapge, void * upage);
void frame_evict(frame_entry * frame_entry);
void frame_find_to_evict();
struct frame_entry * frame_table_search(void * upage);
void frame_lock_acquire();
void frame_lock_release();


#endif /* userprog/exception.h */