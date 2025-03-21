//===----------------------------------------------------------------------===//
//                         GrainDB
//
// third_party/hyperloglog/hyperloglog.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>

/* Error codes */
#define C_OK                    0
#define C_ERR                   -1

typedef struct {
    void *ptr;
} robj;

//! Create a new empty HyperLogLog object
robj *hll_create(void);
//! Destroy the specified HyperLogLog object
void hll_destroy(robj *obj);
//! Add an element with the specified amount of bytes to the HyperLogLog. Returns C_ERR on failure, otherwise returns 0 if the cardinality did not change, and 1 otherwise.
int hll_add(robj *o, unsigned char *ele, size_t elesize);
//! Returns the estimated amount of unique elements seen by the HyperLogLog. Returns C_OK on success, or C_ERR on failure.
int hll_count(robj *o, size_t *result);
//! Merge hll_count HyperLogLog objects into a single one. Returns NULL on failure, or the new HLL object on success.
robj *hll_merge(robj **hlls, size_t hll_count);

#ifdef __cplusplus
}
#endif
