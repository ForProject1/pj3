#include "userprog/exception.h"
#include <inttypes.h>
#include <stdio.h>
#include "userprog/gdt.h"
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "userprog/syscall.h"
#include "threads/synch.h"
#include <hash.h>
#include "page.h"


static unsigned
hash_func(const struct hash_elem *e, void *aux) {
	return (unsigned)hash_entry(e, struct spt_entry, elem)->upage;
}

static bool
hash_less(const struct hash_elem *a, const struct hash_elem *b,	void *aux) {
	return (unsigned)hash_entry(a, struct spt_entry, elem)->upage < (unsigned)hash_entry(b, struct spt_entry, elem)->upage;
}

void
spt_init (struct hash* spt) {
	hash_init(&spt, &hash_func, &hash_less, NULL);
}

bool
spt_insert(struct hash* spt, struct file *file, off_t ofs, uint8_t *upage,
	uint32_t read_bytes, uint32_t zero_bytes, bool writable) {
	struct spt_entry e = malloc(sizeof(struct spt_entry));
	e->file = file;
	e->file_page = ofs;
	e->upage = upage;
	e->read_bytes = read_bytes;
	e->zero_bytes = zero_bytes;
	e->writable = writable;
	lock_acquire(&thread_current()->spt_lock);
	hash_insert(&spt, &e->elem);
	lock_release(&thread_current()->spt_lock);
	return true;
}

struct spt_entry*
spt_get_page(struct hash* spt, void* upage) {
	struct spt_entry temp;
	struct hash_elem e;
	temp.upage = upage; 

	if (e = hash_find(&spt, &temp->elem) != NULL)
		return hash_entry(e, struct spt_entry, elem);
	else
		return NULL;

}

//lazy load
bool
spt_load(struct hash* spt, void* upage) {
	struct spt_entry* e;
	struct Elf32_Phdr phdr;

	e = spt_get_page(upage);
	if (e == NULL)
		return false;
	
	return load_segment(e->file, e->file_page, e->upage,
		e->read_bytes, e->zero_bytes, e->writable);
}

//
void
spt_delete(struct hash* spt,void* upage) {

}

void
spt_destroy(struct hash* spt) {

}


bool



