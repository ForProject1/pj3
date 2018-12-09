#include "userprog/exception.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <list.h>
#include "userprog/gdt.h"
#include "threads/interrupt.h"
#include "threads/synch.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include "userprog/syscall.h"

static struct list frame_table;

static struct lock frame_table_lock;

static struct list_elem* clock_hand;

struct frame_entry {
	struct list_elem elem;
	void* page_addr;
	void* physical_addr;
	struct thread* t;
};


/*Frame_table size = 64MB*/
void
frame_table_init(){
	list_init(&frame_table);
	lock_init(&frame_table_lock);
	clock_hand = list_head(&frame_table);
}

void
frame_table_insert(void* kpage, void* upage) {
	struct frame_entry* f= malloc(sizeof(struct frame_entry));
	struct thread* t = thread_current();
	f->t = t;
	f->physical_addr = vtop(kpage);
	f->page_addr = upage;
	pagedir_set_accessed(t->pd,upage,true);
	list_push_back(&frame_table,&f->elem);

	if (list_size(&frame_table) == 1)
		clock_hand = list_begin(&frame_table);
}

void
frame_table_delete(struct frame_entry* f) {
	list_remove(&f->elem);
	free(f);
}

//frame allocate
void*
frame_allocate(void* kapge, void* upage) {
	if (kpage == NULL) {
		//clock alogrithm
		frame_find_to_evict();
		kpage = palloc_get_page(PAL_USER);
	}
	//Add to frame table & return
	frame_table_insert(kpage, upage);
	return kpage;
}
//evict argument frame_entry
void
frame_evict(struct frame_entry* frame_entry) {
	palloc_free( ptov(frame_entry->physical_addr) );
	list_remove(&frame_entry->list_elem);
}


//upage(virtual address) 위치를 줬을때 그게 pa어디ㅔ ㅣㅆ는지 리턴해줘!
void
frame_find_to_evict(){
	struct frame_entry* f = list_entry(clock_hand, struct frame_entry, elem);
		
	//evict 할거 찾기
	while (pagedir_is_accessed(f->t->pd, f->page_addr) == true;){
		f = list_entry(clock_hand, struct frame_entry, elem);

		pagedir_set_accessed(f->t->pd, f->page_addr, false);

		if(clock_hand = list_next(clock_hand) == list_tail(&frame_table) )
			clock_hand = list_begin(&frame_table);
	}
	frame_evict(f);
}

struct frame_entry*
frame_table_search(void* upage) {
	struct list_elem* e;

	for (e = list_begin(&frame_table); e != list_end(&frame_table);	e = list_next(e)){

		struct frame_entry *f = list_entry(e, struct frame_entry, elem);
		if (f->page_addr == upage) {
			return f;
		}
	}
	return NULL;
}

void frame_lock_acquire() {
	lock_acquire(&frame_table_lock);
}

void frame_lock_release() {
	lock_release(&frame_table_lock);
}

 





