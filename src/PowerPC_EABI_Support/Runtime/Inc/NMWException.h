#ifndef NMWEXCEPTION_H
#define NMWEXCEPTION_H

#include "__ppc_eabi_linker.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DTORCALL(dtor, objptr) (((void (*)(void *, int))dtor)(objptr, -1))

typedef struct DestructorChain {
    struct DestructorChain *next;
    void *destructor;
    void *object;
} DestructorChain;

void __unregister_fragment(int fragmentID);
int __register_fragment(struct __eti_init_info *info, char *TOC);
void *__register_global_object(void *object, void *destructor, void *regmem);
void __destroy_global_chain(void);

#ifdef __cplusplus
}
#endif

#endif /* NMWEXCEPTION_H */
