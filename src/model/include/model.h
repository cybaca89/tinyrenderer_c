#ifndef TR_MODEL_H_
#define TR_MODEL_H_
#include <stdint.h>
#include "model_types.h"
/** model.c */
extern void model_scale_vertices(struct Model_t *model, const double w_coef
, const double h_coef);
extern void model_destroy(struct Model_t *model);
extern void *model_to_frame(struct Model_t *model);
extern uint32_t model_get_frame_size(struct Model_t *model);

/** parser.c */
extern void *model_create_from_wavefront(const char *filepath);

/** transform.c */
extern void model_frame_translate(struct Frame_t *frame, double x, double y);
extern void model_frame_rotate(struct Frame_t *frame, double rad);

typedef struct Model_t model_t;
typedef struct Frame_t frame_t;
typedef struct Segment_t segment_t;
typedef struct Vec3Face_t vec3face_t;
#endif /* TR_MODEL_H_ */
/* EOF */

