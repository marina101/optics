/* optics.h
   Rémi Attab (remi.attab@gmail.com), 25 Feb 2016
   FreeBSD-style copyright and disclaimer apply
*/

#pragma once

#include "utils/compiler.h"
#include "utils/log.h"
#include "utils/errors.h"

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>


// -----------------------------------------------------------------------------
// config
// -----------------------------------------------------------------------------

enum { optics_name_max_len = 128 };

typedef uint64_t optics_ts_t;

// -----------------------------------------------------------------------------
// optics
// -----------------------------------------------------------------------------

struct optics;

struct optics * optics_open(const char *name);
struct optics * optics_create(const char *name);
struct optics * optics_create_at(const char *name, optics_ts_t now);
void optics_close(struct optics *);
bool optics_unlink(const char *name);
bool optics_unlink_all();

const char *optics_get_prefix(struct optics *);
bool optics_set_prefix(struct optics *, const char *prefix);

typedef size_t optics_epoch_t;
optics_epoch_t optics_epoch(struct optics *optics);
optics_epoch_t optics_epoch_inc(struct optics *optics);
optics_epoch_t optics_epoch_inc_at(
        struct optics *optics, optics_ts_t now, optics_ts_t *last_inc);


// -----------------------------------------------------------------------------
// lens
// -----------------------------------------------------------------------------

struct optics_lens;

enum optics_lens_type
{
    optics_counter,
    optics_gauge,
    optics_dist,
};

enum optics_ret
{
    optics_ok = 0,
    optics_err = -1,
    optics_busy = 1,
    optics_break = 2,
};

struct optics_lens * optics_lens_get(struct optics *, const char *name);
enum optics_lens_type optics_lens_type(struct optics_lens *);
const char * optics_lens_name(struct optics_lens *);
void optics_lens_close(struct optics_lens *);
bool optics_lens_free(struct optics_lens *);

typedef enum optics_ret (*optics_foreach_t) (void *ctx, struct optics_lens *lens);
enum optics_ret optics_foreach_lens(struct optics *, void *ctx, optics_foreach_t cb);


// -----------------------------------------------------------------------------
// counter
// -----------------------------------------------------------------------------

struct optics_lens * optics_counter_alloc(struct optics *, const char *name);
bool optics_counter_inc(struct optics_lens *, int64_t value);
enum optics_ret
optics_counter_read(struct optics_lens *, optics_epoch_t epoch, int64_t *value);


// -----------------------------------------------------------------------------
// gauge
// -----------------------------------------------------------------------------

struct optics_lens * optics_gauge_alloc(struct optics *, const char *name);
bool optics_gauge_set(struct optics_lens *, double value);
enum optics_ret
optics_gauge_read(struct optics_lens *, optics_epoch_t epoch, double *value);


// -----------------------------------------------------------------------------
// dist
// -----------------------------------------------------------------------------

struct optics_dist
{
    size_t n;
    double p50;
    double p90;
    double p99;
    double max;
};

struct optics_lens * optics_dist_alloc(struct optics *, const char *name);
bool optics_dist_record(struct optics_lens *, double value);
enum optics_ret
optics_dist_read(struct optics_lens *, optics_epoch_t epoch, struct optics_dist *value);
