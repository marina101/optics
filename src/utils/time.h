/* time.h
   Rémi Attab (remi.attab@gmail.com), 03 Mar 2016
   FreeBSD-style copyright and disclaimer apply
*/

#pragma once

#include <time.h>

// -----------------------------------------------------------------------------
// time
// -----------------------------------------------------------------------------

enum { clock_type = CLOCK_REALTIME };

#define optics_now(ts)                                                  \
    do {                                                                \
        if (optics_unlikely(clock_gettime(clock_type, (ts)) < 0)) {     \
            optics_fail_errno("unable to read realtime clock");         \
            optics_abort();                                             \
        }                                                               \
    } while (false)


bool optics_nsleep(uint64_t nanos);
void optics_yield();
