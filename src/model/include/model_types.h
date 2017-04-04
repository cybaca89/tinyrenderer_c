#ifndef MODEL_TYPE_H_
#define MODEL_TYPE_H_
#include <stdint.h>
#include "vec3_type.h"

struct Segment_t { int16_t x0, y0, x1, y1; };
struct Frame_t   { struct Segment_t *segs; uint32_t size; };

struct FaceCoordLocation_t { size_t v, t, n; };
struct Vec3Face_t          { struct FaceCoordLocation_t p0, p1, p2; };

struct Model_t {
    struct Vec3Face_t *face;
    vec3d_t           *vertex;
    vec3f_t           *texture;
    vec3f_t           *normal;
    size_t             face_count
                     , vertex_count
                     , texture_count
                     , normal_count;
};
#endif /* MODEL_TYPE_H_ */

