/** @file  vec3.h
 *  @author Cy Baca
 */

#ifndef _TINY_VEC3_TYPE_H_
#define _TINY_VEC3_TYPE_H_
#include <stddef.h>
typedef struct { float  x, y, z; } vec3f_t;
typedef struct { double x, y, z; } vec3d_t;
typedef struct { int    x, y, z; } vec3i_t;
typedef struct { size_t x, y, z; } vec3size_t;

typedef struct { float  x, y, z, w; } vec4f_t;
typedef struct { double x, y, z, w; } vec4d_t;
typedef struct { int    x, y, z, w; } vec4i_t;
#endif // _TINY_VEC3_TYPE_H_
