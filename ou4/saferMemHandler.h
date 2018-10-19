#ifndef __SAFERMEMHANDLER__
#define __SAFERMEMHANDLER__

void *smalloc (size_t size);
void *scalloc (size_t nmeb, size_t size);
void *srealloc (void *ptr, size_t size);
void sfree (void *mem);

#endif //__SAFERMEMHANDLER__
