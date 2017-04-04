#ifndef LOCAL_WAVEFRONT_PARSER_H
#define LOCAL_WAVEFRONT_PARSER_H

// -----------------------------------------------------------------------------
#define BUF_LEN 128

#include <stdio.h>
#include <string.h>
#include <alloca.h>
#include "model_types.h"

#define TOKEN_SIZE 2
#define VERTEX_PATTERN  "v %lf %lf %lf"
#define TEXTURE_PATTERN "vt %lf %lf %lf"
#define NORMAL_PATTERN  "vn %lf %lf %lf"
#define FACE_PATTERN    "f %lu/%lu/%lu %lu/%lu/%lu %lu/%lu/%lu"

// struct Location { size_t v, t, n; };
// struct FaceLocation { struct Location x, y, z; };
// struct Model {
//     struct FaceLocation *face;
//     vec3d_t *vertex;
//     vec3f_t *texture;
//     vec3f_t *normal;
//     size_t face_count;
// };

const char *TOKENS[] = { "# ", "v ", "vt", "vn", "f " };
enum Token { T_COMMENT, T_VERTEX, T_TEXTURE, T_NORMAL, T_FACE, T_TOTAL };

/* vertex, texture, normal, face */
struct StreamPosByte_t { long   v, t, n, f; };
struct VectorCount_t   { size_t v, t, n, f; };

/** Compare the first two characters or a string to the two characters of a
 *  passed token.
 *
 *  @param buffer char string.
 *  @param token length 2 char string.
 *  @return 0 on negative, 1 on affirmative.
 */
static int
_tkneq(char *restrict buffer, const char *restrict token);

/** Allocate memory for the Model data structure. Exit on memory failure.
 *  "Model Heap Allocate"
 *
 *  @param count, data structure contains number of vertex, texture, normal, and
 *         face vectors.
 *  @return pointer to initialized Model structure.
 */
static void * // src/parser/model.c
_model_malloc(struct VectorCount_t *count);

/**
 *
 */
static void
_fcount_vectors_and_positions(
  FILE                   *fp
, struct StreamPosByte_t *pos
, struct VectorCount_t   *count);

/**
 *
 */
static void
_fget_vec3d_data(
  FILE *fp
, const long    pos
, const char   *pattern
, const size_t  count
, vec3d_t      *vector);

/**
 *
 */
static void
_fget_vec3f_data(
  FILE         *fp
, const long    pos
, const char   *pattern
, const size_t  count
, vec3f_t      *vector);

/**
 *
 */
static void
_fget_vface_data(
  FILE              *fp
, const long         pos
, const char        *pattern
, const size_t       count
, struct Vec3Face_t *vector);

#endif // LOCAL_WAVEFRONT_PARSER_H

