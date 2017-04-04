/** @file  vec3.h
 *  @author Cy Baca
 */

#ifndef _TINY_VEC3_H_
#define _TINY_VEC3_H_

// typedef struct { float x, y, z; } vec3f_t;
// typedef struct { double x, y, z; } vec3d_t;
// typedef struct { long unsigned int x, y, z; } vec3i64_t;

#include "vec3_type.h"

extern float
dot_productv3f(const vec3f_t a, const vec3f_t b),
lengthv3f(const vec3f_t a);

extern vec3f_t
normalizev3f(vec3f_t in),
crossv3f(const vec3f_t a, const vec3f_t b),
sumv3f(const vec3f_t a, const vec3f_t b);

#endif
