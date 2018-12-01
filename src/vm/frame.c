#include "userprog/exception.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <list.h>
#include "userprog/gdt.h"
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "userprog/syscall.h"

static struct list frame_table;

struct frame {
	struct list_elem elem;
	void* page_addr;
	uintptr_t frame_addr;
	struct thread* t;


	/*not used*/
	int use_bit;
};


/*Frame_table size = 64MB*/
void
frame_table_init(){
	init_list(&frame_table);
}

void
frame_table_insert(void* kpage, void* upage) {
	struct frame* f= malloc(sizeof(struct frame));
	f->t = thread_current();
	f->frame_addr = vtop(kpage);
	f->page_addr = upage;
	f->use_bit = 1;
	list_push_back(&frame_table,&f->elem);
}

void
frame_table_delete(struct frame* f) {
	list_remove(&f->elem);
	free(f);
}






 





