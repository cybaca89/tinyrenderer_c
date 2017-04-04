/** mat4array.h */
#ifndef TEST_______WINDMILL_MAT4ARRAY_H_
#define TEST_______WINDMILL_MAT4ARRAY_H_
/** these may come in handy
*/
#define WINDMILL_PI  3.14159265358979323846264338327950288419716939937510582097f
#define WINDMILL_PI2 1.57079632679489661923132169163975144209858469968755291048f
#define WINDMILL_PI3 1.04719755119659774615421446109316762806572313312503527365f
#define WINDMILL_PI4 0.78539816339744830961566084581987572104929234984377645524f
#define WINDMILL_PI6 0.52359877559829887307710723054658381403286156656251763682f
#define WINDMILL_TAU 6.28318530717958647692528676655900576839433879875021164194f

//////////////////////////////////////////////////////////////////////////
//      COPY
//
extern void mat4array_copy(float *restrict out, const float *restrict in);

//////////////////////////////////////////////////////////////////////////
//      FRUSTRUM
//
/* @param float 4x4matrix[16]
 * @param float left
 * @param float right
 * @param float top
 * @param float bottom
 * @param float near
 * @param float far
 */
extern void mat4array_get_frustrum(float *restrict, float, float, float, float, float, float);


//////////////////////////////////////////////////////////////////////////
//      LOOK AT
// 
void mat4array_get_look_at(float *restrict, float *restrict, float *restrict, float *restrict);

//////////////////////////////////////////////////////////////////////////
//      MULTIPLY
// 
extern void mat4array_get_product(float *restrict, const float *restrict);

//////////////////////////////////////////////////////////////////////////
//      PERSPECTIVE
// 
extern void mat4array_get_perspective(
    float *restrict, const float, const float, const float, const float);

//////////////////////////////////////////////////////////////////////////
//      PRINT
// 
extern void mat4array_print(const float *);

//////////////////////////////////////////////////////////////////////////
//      ROTATION
// 
/* @params
 *  4x4 matrix (float[16])
 *  angle in radians
 *  MAT4ARRAY_ROLL | MAT4ARRAY_PITCH | MAT4ARRAY_YAW
 */
extern void mat4array_get_rotation(float *restrict, double, int);
/* @params
 *  4x4matrix (float[16])
 *  angle in radians
 *  x coordinate
 *  y coordinate
 *  z coordinate
 */
extern void mat4array_rotate(float *restrict, double, float, float, float);
extern void mat4array_rotatev3(float *restrict, double, float *restrict);

//////////////////////////////////////////////////////////////////////////
//      SCALE
// 
/* 4x4 matrix[16], x, y, z pos */
extern void mat4array_get_scaled(float *restrict, float, float, float);
extern void mat4array_scale(float *restrict, float, float, float);
/* 4x4 matrix[16], float[3] */
extern void mat4array_scalev3(float *restrict, float *restrict);

//////////////////////////////////////////////////////////////////////////
//      SET
// 
extern void mat4array_set(float *restrict, int);

//////////////////////////////////////////////////////////////////////////
//      TRANSLATE
// 
/* @params
 *  4x4 matrix(float[16])
 *  x coordinate
 *  y coordinate
 *  z coordinate
 */
extern void mat4array_get_translation(float *restrict, float, float, float);
/* @params
 *  4x4 matrix(float[16])
 *  x coordinate
 *  y coordinate
 *  z coordinate
 */
extern void mat4array_translate(float *restrict, float, float, float);
/* @params
 *  4x4 matrix(float[16])
 *  len 3 vector
 */
extern void mat4array_translatev3(float *restrict, float *restrict);

//////////////////////////////////////////////////////////////////////////
//      TRANSPOSE
// 
extern void mat4array_transpose(float *restrict);
#include "../header/mat4array_defs.h"
#include "../header/mat4array_euler_defs.h"
#endif
