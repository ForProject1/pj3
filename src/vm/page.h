#ifndef VM_PAGE_H
#define VM_PAGE_H

/* Page fault error code bits that describe the cause of the exception.  */
#define PF_P 0x1    /* 0: not-present page. 1: access rights violation. */
#define PF_W 0x2    /* 0: read, 1: write. */
#define PF_U 0x4    /* 0: kernel, 1: user process. */

void exception_init (void);
void exception_print_stats (void);

#endif /* userprog/exception.h */


struct spt_entry {
	struct file* file;
	uint32_t file_page;
	uint32_t upage;
	uint32_t read_bytes;
	uint32_t zero_bytes;
	bool writable;
	struct hash_elem elem;
};



