/** @file parser.c
 *  @author Cy Baca
 *
 *  Parses wavefront .obj files and stores data
 */
// -----------------------------------------------------------------------------

#include "local_parser.h"
#include <stdlib.h>

void *
model_create_from_wavefront(const char *filepath)
{
    FILE *fp = fopen(filepath, "r");
    if (!fp) goto file_open_error;

    struct StreamPosByte_t *pos   = alloca(sizeof (struct StreamPosByte_t));
    struct VectorCount_t   *count = alloca(sizeof (struct VectorCount_t));
    *count = (struct VectorCount_t){ 0, 0, 0, 0 };

    // get sizes ---------------------------------------------------------------
    _fcount_vectors_and_positions(fp, pos, count);

    // data memory -------------------------------------------------------------
    struct Model_t *model = _model_malloc(count);

    // get data ----------------------------------------------------------------
    _fget_vec3d_data(fp, pos->v, VERTEX_PATTERN,  count->v, model->vertex );
    _fget_vec3f_data(fp, pos->t, TEXTURE_PATTERN, count->t, model->texture);
    _fget_vec3f_data(fp, pos->n, NORMAL_PATTERN,  count->n, model->normal );
    _fget_vface_data(fp, pos->f, FACE_PATTERN,    count->f, model->face   );
    model->face_count        = count->f;
    model->vertex_count      = count->v;
    model->texture_count     = count->t;
    model->normal_count      = count->n;

    return model;

    file_open_error:
    fprintf(stderr, "Unable to open file \"%s\"\n", filepath);
    exit(EXIT_FAILURE);
}

static void
_fcount_vectors_and_positions(
  FILE                   *fp
, struct StreamPosByte_t *pos
, struct VectorCount_t   *count)
{
    char buffer[BUF_LEN] = { 0 };

    while (!_tkneq(buffer, TOKENS[T_VERTEX]))
        pos->v = ftell(fp), fgets(buffer, BUF_LEN, fp);

    while (_tkneq(buffer, TOKENS[T_VERTEX]))
        count->v++, fgets(buffer, BUF_LEN, fp);

    while (!_tkneq(buffer, TOKENS[T_TEXTURE]))
        pos->t = ftell(fp), fgets(buffer, BUF_LEN, fp);
    while (_tkneq(buffer, TOKENS[T_TEXTURE]))
        count->t++, fgets(buffer, BUF_LEN, fp);

    while (!_tkneq(buffer, TOKENS[T_NORMAL]))
        pos->n = ftell(fp), fgets(buffer, BUF_LEN, fp);
    while (_tkneq(buffer, TOKENS[T_NORMAL]))
        count->n++, fgets(buffer, BUF_LEN, fp);

    char *eof;
    while (!_tkneq(buffer, TOKENS[T_FACE]))
        pos->f = ftell(fp), fgets(buffer, BUF_LEN, fp);
    while (_tkneq(buffer, TOKENS[T_FACE]) && eof)
        count->f++, eof = fgets(buffer, BUF_LEN, fp);
}

static int
_tkneq(char *restrict buffer, const char *restrict token)
{
    if (token[2] != 0) {
        fprintf(stderr, "token size fucked up\n");
        exit(EXIT_FAILURE);
    }

    if (*buffer++ != *token++) return 0;

    if (*buffer != *token) return 0;

    return 1;
}

static void
_fget_vec3d_data(
  FILE *fp
, const long   pos
, const char  *pattern
, const size_t count
, vec3d_t     *vector)
{
    char buffer[BUF_LEN];
    fseek(fp, pos, SEEK_SET);

    size_t i;
    for (i = 0; i < count; ++i)
        fgets(buffer, BUF_LEN, fp),
        sscanf(buffer, pattern, &vector->x, &vector->y, &vector->z),
        ++vector;

    printf("\n");
}

static void
_fget_vec3f_data(
  FILE *fp
, const long pos
, const char *pattern
, const size_t count
, vec3f_t *vector)
{
    char buffer[BUF_LEN];
    fseek(fp, pos, SEEK_SET);

    size_t i;
    for (i = 0; i < count; ++i) {
        fgets(buffer, BUF_LEN, fp);
        double x, y, z;
        sscanf(buffer, pattern, &x, &y, &z);
        vector->x = (float)x;
        vector->y = (float)y;
        vector->z = (float)z;
        ++vector;
    }
}

static void
_fget_vface_data(
  FILE *fp
, const long pos
, const char *pattern
, const size_t count
, struct Vec3Face_t *vector)
{
    char buffer[BUF_LEN];
    fseek(fp, pos, SEEK_SET);

    size_t i;
    for (i = 0; i < count; ++i) {
        fgets(buffer, BUF_LEN, fp),
        sscanf(buffer, pattern
        , &vector->p0.v, &vector->p0.t, &vector->p0.n
        , &vector->p1.v, &vector->p1.t, &vector->p1.n
        , &vector->p2.v, &vector->p2.t, &vector->p2.n);

        // wavefront obj indices start at 1, so subtract 1 from each element
        --vector->p0.v; --vector->p0.t; --vector->p0.n;
        --vector->p1.v; --vector->p1.t; --vector->p1.n;
        --vector->p2.v; --vector->p2.t; --vector->p2.n;
        ++vector;
    }
}

static void *
_model_malloc(struct VectorCount_t *count)
{
    struct Model_t *model = malloc(sizeof(struct Model_t));
    if (!model) goto virtual_memory_exhausted;

    model->face    = malloc(sizeof(struct Vec3Face_t) * count->f);
    model->vertex  = malloc(sizeof(vec3d_t)           * count->v);
    model->texture = malloc(sizeof(vec3d_t)           * count->t);
    model->normal  = malloc(sizeof(vec3d_t)           * count->n);
    if (!model->face || !model->vertex || !model->texture || !model->normal)
        goto virtual_memory_exhausted;

    return model;
    virtual_memory_exhausted:
    fprintf(stderr, "unable to allocate model memory\n");
    exit(EXIT_FAILURE);
}

