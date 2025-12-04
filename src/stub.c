#include <sys/types.h>

// TODO: Try to replace this function
// Stub implementation for _sbrk to satisfy linker
// Used by malloc/free indirectly - required for embedded systems
extern char _end;
static char *heap_end = 0;

caddr_t _sbrk(int incr)
{
    char *prev_heap_end;

    if (heap_end == 0)
    {
        heap_end = &_end;
    }

    prev_heap_end = heap_end;
    heap_end += incr;

    return (caddr_t)prev_heap_end;
}
