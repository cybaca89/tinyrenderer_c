#include <stdlib.h>
#include <stdio.h>
#include "model_types.h"
#include <assert.h>

void
model_scale_vertices(
  struct Model_t *model
, const double w_coef
, const double h_coef)
{
    size_t i, count = model->vertex_count;
    for (i = 0; i < count; ++i)
        model->vertex[i].x = (model->vertex[i].x + 1.0) * w_coef,
        model->vertex[i].y = (model->vertex[i].y + 1.0) * h_coef;
}

void
model_destroy(struct Model_t *model)
{
    free(model->face);
    free(model->vertex);
    free(model->texture);
    free(model->normal);
    free(model);
}

void *
model_frame_create(struct Model_t *model)
#define VERTICES_PER_FACE 3
{
    struct Frame_t *frame = malloc(sizeof(struct Frame_t));
    if (!frame) goto frame_create_fail;

    frame->segs = malloc(
          sizeof(struct Segment_t)
        * model->face_count
        * VERTICES_PER_FACE);
    if (!frame->segs) goto frame_create_fail;

    frame->size = (uint32_t)model->face_count * VERTICES_PER_FACE;

    return frame;

    frame_create_fail:
    fprintf(stderr, "Failed to allocate frame\n");
    exit(EXIT_FAILURE);
}

void
model_frame_destroy(struct Frame_t *frame)
{
    assert(frame && frame->segs);
    free(frame->segs);
    free(frame);
}

void *
model_to_frame(struct Model_t *model)
{
    struct Vec3Face_t *fptr = model->face;
    vec3d_t *vptr = model->vertex;

    size_t count = model->face_count;
    /*
    struct Segment_t *seg = malloc(sizeof(struct Segment_t) * count * 3);
    if (!seg) return NULL;
    */
    struct Frame_t *frame = model_frame_create(model);

    struct Segment_t *segptr = frame->segs;

    size_t i;
    for (i = 0; i < count; ++i) {

        segptr->x0 = (int16_t)(vptr[fptr->p0.v].x);
        segptr->y0 = (int16_t)(vptr[fptr->p0.v].y);
        segptr->x1 = (int16_t)(vptr[fptr->p1.v].x);
        segptr->y1 = (int16_t)(vptr[fptr->p1.v].y);
        ++segptr;

        segptr->x0 = (int16_t)(vptr[fptr->p1.v].x);
        segptr->y0 = (int16_t)(vptr[fptr->p1.v].y);
        segptr->x1 = (int16_t)(vptr[fptr->p2.v].x);
        segptr->y1 = (int16_t)(vptr[fptr->p2.v].y);
        ++segptr;

        segptr->x0 = (int16_t)(vptr[fptr->p2.v].x);
        segptr->y0 = (int16_t)(vptr[fptr->p2.v].y);
        segptr->x1 = (int16_t)(vptr[fptr->p0.v].x);
        segptr->y1 = (int16_t)(vptr[fptr->p0.v].y);
        ++segptr;
        ++fptr;
    }

    return frame;
}

uint32_t model_get_frame_size(struct Model_t *model)
{
    return (uint32_t)(model->face_count * 3);
}
/* EOF */

