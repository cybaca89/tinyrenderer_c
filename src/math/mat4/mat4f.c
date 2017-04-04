/** @file mat4f.c
 *  @author Cy Baca
 */
#include <math.h>
#include <assert.h>
#include <stdlib.h>

#define MAT4ARRAY_LEN 16
#define MAT4VEC_LEN 4
enum {
      MAT4ARRAY_U_ERROR
    , MAT4ARRAY_IDENTITY
    , MAT4ARRAY_ZERO
    , MAT4ARRAY_DEBUG
/*  , MAT4ARRAY_MODEL
    , MAT4ARRAY_VIEW
    , MAT4ARRAY_CLIP */
    , MAT4ARRAY_O_ERROR
};

enum {
      MAT4ARRAY_EULER_U_ERROR = 0x0
    , MAT4ARRAY_ROLL =        0x1
    , MAT4ARRAY_PITCH =       0x2
    , MAT4ARRAY_YAW =         0x4
    , MAT4ARRAY_EULER_O_ERROR = 0x8
};
void _about_roll(float *, double);
void _about_pitch(float *, double);
void _about_yaw(float *, double);

/** utility ------------------------------------------------------------------*/
void
mat4array_copy(float *restrict out, const float *restrict in)
{
    out[ 0] = in[ 0]; out[ 1] = in[ 1]; out[ 2] = in[ 2]; out[ 3] = in[ 3];
    out[ 4] = in[ 4]; out[ 5] = in[ 5]; out[ 6] = in[ 6]; out[ 7] = in[ 7];
    out[ 8] = in[ 8]; out[ 9] = in[ 9]; out[10] = in[10]; out[11] = in[11];
    out[12] = in[12]; out[13] = in[13]; out[14] = in[14]; out[15] = in[15];
}

void
mat4array_set(float *restrict out, int style)
{
    assert(out != NULL);
    switch (style) {
        case MAT4ARRAY_IDENTITY:
            out[ 0] = 1.0f; out[ 1] = 0.0f; out[ 2] = 0.0f; out[ 3] = 0.0f;
            out[ 4] = 0.0f; out[ 5] = 1.0f; out[ 6] = 0.0f; out[ 7] = 0.0f;
            out[ 8] = 0.0f; out[ 9] = 0.0f; out[10] = 1.0f; out[11] = 0.0f;
            out[12] = 0.0f; out[13] = 0.0f; out[14] = 0.0f; out[15] = 1.0f;
            break;
        case MAT4ARRAY_ZERO:
            out[ 0] = 0.0f; out[ 1] = 0.0f; out[ 2] = 0.0f; out[ 3] = 0.0f;
            out[ 4] = 0.0f; out[ 5] = 0.0f; out[ 6] = 0.0f; out[ 7] = 0.0f;
            out[ 8] = 0.0f; out[ 9] = 0.0f; out[10] = 0.0f; out[11] = 0.0f;
            out[12] = 0.0f; out[13] = 0.0f; out[14] = 0.0f; out[15] = 0.0f;
            break;
        case MAT4ARRAY_DEBUG:
            out[ 0] = 0.0f; out[ 1] = 1.0f; out[ 2] = 2.0f; out[ 3] = 3.0f;
            out[ 4] = 4.0f; out[ 5] = 5.0f; out[ 6] = 6.0f; out[ 7] = 7.0f;
            out[ 8] = 8.0f; out[ 9] = 9.0f; out[10] = 10.0f; out[11] = 11.0f;
            out[12] = 12.0f; out[13] = 13.0f; out[14] = 14.0f; out[15] = 15.0f;
            break;
        default:
            assert(style < MAT4ARRAY_O_ERROR && style > MAT4ARRAY_U_ERROR);
    }
}

/** translation --------------------------------------------------------------*/
/*   これが典型的なモデル・マトリクスです。
 *   1,  0,  0, T_x [0][0], [0][1], [0][2], [0][3] @ ────> i0, i1, i2, i3 >──╮
 *   --------------                                  ╭───<────────────────<──╯
 * , 0,  1,  0, T_y [1][0], [1][1], [1][2], [1][3] @ ╰──> i4, i5, i6, i7 ─>──╮
 *   --------------
 * , 0,  0,  1, T_z
 *   --------------
 * , 0,  0,  0,  1
 *
 *
 *
 *   1, | 0, | 0, | 0 [0][0], [1][0], [2][0], [3][0] @  ───> i0, i1, i2, i3 >──╮
 *      |    |    |                                    ╭───<────────────────<──╯
 * , 0, | 1, | 0, | 0 [0][1], [1][1], [2][1], [3][1] @ ╰──> i4, i5, i6, i7 ─>──╮
 *      |    |    |
 * , 0, | 0, | 1, | 0
 *      |    |    |
 * ,T_x,|T_y,|T_z,| 1
 */
#define X 0
#define Y 1
#define Z 2
#define W 3
void
mat4array_get_translationv3(float *restrict out, float *restrict in)
{
    out[ 0] =  1.0f; out[ 1] =  0.0f; out[ 2] =  0.0f; out[ 3] = 0.0f;
    out[ 4] =  0.0f; out[ 5] =  1.0f; out[ 6] =  0.0f; out[ 7] = 0.0f;
    out[ 8] =  0.0f; out[ 9] =  0.0f; out[10] =  1.0f; out[11] = 0.0f;
    out[12] = in[X]; out[13] = in[Y]; out[14] = in[Z]; out[15] = 1.0f;
}

void
mat4array_get_translation(float *restrict out, float x, float y, float z)
{
    out[ 0] = 1.0f; out[ 1] = 0.0f; out[ 2] = 0.0f; out[ 3] = 0.0f;
    out[ 4] = 0.0f; out[ 5] = 1.0f; out[ 6] = 0.0f; out[ 7] = 0.0f;
    out[ 8] = 0.0f; out[ 9] = 0.0f; out[10] = 1.0f; out[11] = 0.0f;
    out[12] = x;    out[13] = y;    out[14] = z;    out[15] = 1.0f;
}

void
mat4array_translate(float *restrict out, float x, float y, float z)
{
    vecMat4_t mat = vecMat4_row_get(out);
    mat.w[X] += mat.x[X] * x + mat.y[X] * y + mat.z[X] * z;
    mat.w[Y] += mat.x[Y] * x + mat.y[Y] * y + mat.z[Y] * z;
    mat.w[Z] += mat.x[Z] * x + mat.y[Z] * y + mat.z[Z] * z;
    mat.w[W] += mat.x[W] * x + mat.y[W] * y + mat.z[W] * z;
}

void
mat4array_translatev3(float *restrict out, float *restrict in)
{
    vecMat4_t mat = vecMat4_row_get(out);
    mat.w[X] += mat.x[X] * in[X] + mat.y[X] * in[Y] + mat.z[X] * in[Z];
    mat.w[Y] += mat.x[Y] * in[X] + mat.y[Y] * in[Y] + mat.z[Y] * in[Z];
    mat.w[Z] += mat.x[Z] * in[X] + mat.y[Z] * in[Y] + mat.z[Z] * in[Z];
    mat.w[W] += mat.x[W] * in[X] + mat.y[W] * in[Y] + mat.z[W] * in[Z];
}
#undef X
#undef Y
#undef Z
#undef W

/** multiply -----------------------------------------------------------------*/
void
mat4array_get_product(float *restrict out, const float *restrict inp)
{
    float A[16];

    A[ 0] = out[ 0]; A[ 1] = out[ 1]; A[ 2] = out[ 2]; A[ 3] = out[ 3];
    A[ 4] = out[ 4]; A[ 5] = out[ 5]; A[ 6] = out[ 6]; A[ 7] = out[ 7];
    A[ 8] = out[ 8]; A[ 9] = out[ 9]; A[10] = out[10]; A[11] = out[11];
    A[12] = out[12]; A[13] = out[13]; A[14] = out[14]; A[15] = out[15];

    vecMat4_t ot = vecMat4_row_get(out);
    cvecMat4_t in = cvecMat4_row_get(inp);
    int i = 0;
    for (; i < 4; ++i) {
        ot.x[i] = A[ 0] * in.x[i]
                + A[ 1] * in.y[i]
                + A[ 2] * in.z[i]
                + A[ 3] * in.w[i];

        ot.y[i] = A[ 4] * in.x[i]
                + A[ 5] * in.y[i]
                + A[ 6] * in.z[i]
                + A[ 7] * in.w[i];

        ot.z[i] = A[ 8] * in.x[i]
                + A[ 9] * in.y[i]
                + A[10] * in.z[i]
                + A[11] * in.w[i];

        ot.w[i] = A[12] * in.x[i]
                + A[13] * in.y[i]
                + A[14] * in.z[i]
                + A[15] * in.w[i];
    }
}

/** rotation -----------------------------------------------------------------*/
void
mat4array_rotatev3(float *restrict out, double rad, float *restrict in)
{
    float cop[16];
    float rot[16];
    vecMat4_t ot;
    vecMat4_t cp;
    ot = vecMat4_row_get(out);
    cp = vecMat4_row_get(cop);

    float c = (float)cos(rad);
    float s = (float)sin(rad);

    /* copy input matrix */
    cop[ 0] = out[ 0]; cop[ 1] = out[ 1]; cop[ 2] = out[ 2]; cop[ 3] = out[ 3];
    cop[ 4] = out[ 4]; cop[ 5] = out[ 5]; cop[ 6] = out[ 6]; cop[ 7] = out[ 7];
    cop[ 8] = out[ 8]; cop[ 9] = out[ 9]; cop[10] = out[10]; cop[11] = out[11];
    cop[12] = out[12]; cop[13] = out[13]; cop[14] = out[14]; cop[15] = out[15];


#   define x 0
#   define y 1
#   define z 2
    /* normalize */
    float len = (float)sqrt(
        in[x] * in[x]
      + in[y] * in[y]
      + in[z] * in[z]);

    float norm[3] = { in[x] / len, in[y] / len, in[z] / len };

    float frac[3] = {
          (1.0f - c) * norm[x]
        , (1.0f - c) * norm[y]
        , (1.0f - c) * norm[z] };

    /* make rotation matrix, thank you glm header lib */
    rot[ 0] = c + frac[x] * norm[x];
    rot[ 1] =     frac[x] * norm[y] + s * norm[z];
    rot[ 2] =     frac[x] * norm[z] - s * norm[y];

    rot[ 4] =     frac[y] * norm[x] - s * norm[z];
    rot[ 5] = c + frac[y] * norm[y];
    rot[ 6] =     frac[y] * norm[z] + s * norm[x];

    rot[ 8] =     frac[z] * norm[x] + s * norm[y];
    rot[ 9] =     frac[z] * norm[y] - s * norm[x];
    rot[10] = c + frac[z] * norm[z];
#   undef x
#   undef y
#   undef z

    ot.x[0] = cp.x[0] * rot[0] + cp.y[0] * rot[1] + cp.z[0] * rot[ 2];
    ot.x[1] = cp.x[1] * rot[0] + cp.y[1] * rot[1] + cp.z[1] * rot[ 2];
    ot.x[2] = cp.x[2] * rot[0] + cp.y[2] * rot[1] + cp.z[2] * rot[ 2];
    ot.x[3] = cp.x[3] * rot[0] + cp.y[3] * rot[1] + cp.z[3] * rot[ 2];

    ot.y[0] = cp.x[0] * rot[4] + cp.y[0] * rot[5] + cp.z[0] * rot[ 6];
    ot.y[1] = cp.x[1] * rot[4] + cp.y[1] * rot[5] + cp.z[1] * rot[ 6];
    ot.y[2] = cp.x[2] * rot[4] + cp.y[2] * rot[5] + cp.z[2] * rot[ 6];
    ot.y[3] = cp.x[3] * rot[4] + cp.y[3] * rot[5] + cp.z[3] * rot[ 6];

    ot.z[0] = cp.x[0] * rot[8] + cp.y[0] * rot[9] + cp.z[0] * rot[10];
    ot.z[1] = cp.x[1] * rot[8] + cp.y[1] * rot[9] + cp.z[1] * rot[10];
    ot.z[2] = cp.x[2] * rot[8] + cp.y[2] * rot[9] + cp.z[2] * rot[10];
    ot.z[3] = cp.x[3] * rot[8] + cp.y[3] * rot[9] + cp.z[3] * rot[10];
}

void
mat4array_rotate(float *restrict out, double rad, float x, float y, float z)
{
    float cop[16];
    float rot[16];
    vecMat4_t ret;
    vecMat4_t mat;

    /* copy input matrix */
    cop[ 0] = out[ 0]; cop[ 1] = out[ 1]; cop[ 2] = out[ 2]; cop[ 3] = out[ 3];
    cop[ 4] = out[ 4]; cop[ 5] = out[ 5]; cop[ 6] = out[ 6]; cop[ 7] = out[ 7];
    cop[ 8] = out[ 8]; cop[ 9] = out[ 9]; cop[10] = out[10]; cop[11] = out[11];
    cop[12] = out[12]; cop[13] = out[13]; cop[14] = out[14]; cop[15] = out[15];

    float c = (float)cos(rad);
    float s = (float)sin(rad);

    /* normalize http://www.fundza.com/vectors/normalize/ */
    float len = (float)sqrt((x * x) + (y * y) + (z * z));
    float nx = x / len;
    float ny = y / len;
    float nz = z / len;

    /* make rotation matrix, thank you glm header lib */
    float tx = (1.0f - c) * nx;
    float ty = (1.0f - c) * ny;
    float tz = (1.0f - c) * nz;
    rot[ 0] = c + tx * nx;
    rot[ 1] =     tx * ny + s * nz;
    rot[ 2] =     tx * nz - s * ny;

    rot[ 4] =     ty * nx - s * nz;
    rot[ 5] = c + ty * ny;
    rot[ 6] =     ty * nz + s * nx;

    rot[ 8] =     tz * nx + s * ny;
    rot[ 9] =     tz * ny - s * nx;
    rot[10] = c + tz * nz;

    /* multiply by original */
    ret = vecMat4_row_get(out);
    mat = vecMat4_row_get(cop);
    ret.x[0] = mat.x[0] * rot[0] + mat.y[0] * rot[1] + mat.z[0] * rot[ 2];
    ret.x[1] = mat.x[1] * rot[0] + mat.y[1] * rot[1] + mat.z[1] * rot[ 2];
    ret.x[2] = mat.x[2] * rot[0] + mat.y[2] * rot[1] + mat.z[2] * rot[ 2];
    ret.x[3] = mat.x[3] * rot[0] + mat.y[3] * rot[1] + mat.z[3] * rot[ 2];

    ret.y[0] = mat.x[0] * rot[4] + mat.y[0] * rot[5] + mat.z[0] * rot[ 6];
    ret.y[1] = mat.x[1] * rot[4] + mat.y[1] * rot[5] + mat.z[1] * rot[ 6];
    ret.y[2] = mat.x[2] * rot[4] + mat.y[2] * rot[5] + mat.z[2] * rot[ 6];
    ret.y[3] = mat.x[3] * rot[4] + mat.y[3] * rot[5] + mat.z[3] * rot[ 6];

    ret.z[0] = mat.x[0] * rot[8] + mat.y[0] * rot[9] + mat.z[0] * rot[10];
    ret.z[1] = mat.x[1] * rot[8] + mat.y[1] * rot[9] + mat.z[1] * rot[10];
    ret.z[2] = mat.x[2] * rot[8] + mat.y[2] * rot[9] + mat.z[2] * rot[10];
    ret.z[3] = mat.x[3] * rot[8] + mat.y[3] * rot[9] + mat.z[3] * rot[10];
}

void
mat4array_get_rotation(float *restrict out, double rad, int axis)
{
    switch (axis) {
        case MAT4ARRAY_ROLL | MAT4ARRAY_PITCH | MAT4ARRAY_YAW:
            _about_roll( out, rad);
            _about_pitch(out, rad);
            _about_yaw(  out, rad);
            break;
        case MAT4ARRAY_ROLL | MAT4ARRAY_PITCH:
            _about_roll( out, rad);
            _about_pitch(out, rad);
            break;
        case MAT4ARRAY_ROLL | MAT4ARRAY_YAW:
            _about_roll(out, rad);
            _about_yaw( out, rad);
            break;
        case MAT4ARRAY_PITCH | MAT4ARRAY_YAW:
            _about_pitch(out, rad);
            _about_yaw(  out, rad);
            break;
        case MAT4ARRAY_ROLL:  _about_roll( out, rad); break;
        case MAT4ARRAY_PITCH: _about_pitch(out, rad); break;
        case MAT4ARRAY_YAW:   _about_yaw(  out, rad); break;
        default:
            assert(axis > MAT4ARRAY_EULER_U_ERROR
                && axis < MAT4ARRAY_EULER_O_ERROR);
    }
}

void _about_roll(float *out, double rad)
{
    float c = (float)cos(rad);
    float s = (float)sin(rad);
    out[ 0] =    c; out[ 1] =    s; out[ 2] = 0.0f; out[ 3] = 0.0f;
    out[ 4] =   -s; out[ 5] =    c; out[ 6] = 0.0f; out[ 7] = 0.0f;
    out[ 8] = 0.0f; out[ 9] = 0.0f; out[10] = 1.0f; out[11] = 0.0f;
    out[12] = 0.0f; out[13] = 0.0f; out[14] = 0.0f; out[15] = 1.0f;
}

void _about_pitch(float *out, double rad)
{
    float c = (float)cos(rad);
    float s = (float)sin(rad);
    out[ 0] = 1.0f; out[ 1] = 0.0f; out[ 2] = 0.0f; out[ 3] = 0.0f;
    out[ 4] = 0.0f; out[ 5] =    c; out[ 6] =   -s; out[ 7] = 0.0f;
    out[ 8] = 0.0f; out[ 9] =   -s; out[10] =    c; out[11] = 0.0f;
    out[12] = 0.0f; out[13] = 0.0f; out[14] = 0.0f; out[15] = 1.0f;
}

void _about_yaw(float *out, double rad)
{
    float c = (float)cos(rad);
    float s = (float)sin(rad);
    out[ 0] =    c; out[ 1] = 0.0f; out[ 2] =   -s; out[ 3] = 0.0f;
    out[ 4] = 0.0f; out[ 5] = 1.0f; out[ 6] = 0.0f; out[ 7] = 0.0f;
    out[ 8] =    s; out[ 9] = 0.0f; out[10] =    c; out[11] = 0.0f;
    out[12] = 0.0f; out[13] = 0.0f; out[14] = 0.0f; out[15] = 1.0f;
}

/** scale --------------------------------------------------------------------*/
/** mat4array_scale.c */
/*   S_x  0   0   0
 *    0  S_y  0   0
 *    0   0  S_z  0
 *    0   0   0   1
 */

void
mat4array_get_scaled(float *restrict out, float x, float y, float z)
{
    out[ 0] = x;    out[ 1] = 0.0f; out[ 2] = 0.0f; out[ 3] = 0.0f;
    out[ 4] = 0.0f; out[ 5] = y;    out[ 6] = 0.0f; out[ 7] = 0.0f;
    out[ 8] = 0.0f; out[ 9] = 0.0f; out[10] = z;    out[11] = 0.0f;
    out[12] = 0.0f; out[13] = 0.0f; out[14] = 0.0f; out[15] = 1.0f;
}

void
mat4array_scale(float *restrict out, float x, float y, float z)
{
    vecMat4_t ot = vecMat4_row_get(out);
    /*
    float cop[16];
    out[ 0] = cop[ 0]; out[ 1] = cop[ 1]; out[ 2] = cop[ 2]; out[ 3] = cop[ 3];
    out[ 4] = cop[ 4]; out[ 5] = cop[ 5]; out[ 6] = cop[ 6]; out[ 7] = cop[ 7];
    out[ 8] = cop[ 8]; out[ 9] = cop[ 9]; out[10] = cop[10]; out[11] = cop[11];
    out[12] = cop[12]; out[13] = cop[13]; out[14] = cop[14]; out[15] = cop[15];
    vecMat_t cp = vecMat4_row_get(cop);
    float scl[16];
    scl[ 0] = x;    scl[ 1] = 0.0f; scl[ 2] = 0.0f; scl[ 3] = 0.0f;
    scl[ 4] = 0.0f; scl[ 5] = y;    scl[ 6] = 0.0f; scl[ 7] = 0.0f;
    scl[ 8] = 0.0f; scl[ 9] = 0.0f; scl[10] = z;    scl[11] = 0.0f;
    scl[12] = 0.0f; scl[13] = 0.0f; scl[14] = 0.0f; scl[15] = 1.0f;
    vecMat_t sc = vecMat4_row_get(scl);
    int i = 0;
    for (; i < 4; ++i){
        ot.a[i] = cp.a[i] * x;
        ot.b[i] = cp.b[i] * y;
        ot.c[i] = cp.c[i] * z;
    }
    */
    int i = 0;
    for (; i < 4; ++i) {
        ot.x[i] *= x;
        ot.y[i] *= y;
        ot.z[i] *= z;
    }
}

void
mat4array_scalev3(float *restrict out, float *restrict in)
{
    vecMat4_t ot = vecMat4_row_get(out);
    int i = 0;
    for (; i < 4; ++i) {
        ot.x[i] *= in[0];
        ot.y[i] *= in[1];
        ot.z[i] *= in[2];
    }
}
/** transpose ----------------------------------------------------------------*/
void
mat4array_transpose(float *restrict out)
{
    assert(out != (void *)0);

    float tmp[16];
    tmp[ 0] = out[ 0]; tmp[ 1] = out[ 1]; tmp[ 2] = out[ 2]; tmp[ 3] = out[ 3];
    tmp[ 4] = out[ 4]; tmp[ 5] = out[ 5]; tmp[ 6] = out[ 6]; tmp[ 7] = out[ 7];
    tmp[ 8] = out[ 8]; tmp[ 9] = out[ 9]; tmp[10] = out[10]; tmp[11] = out[11];
    tmp[12] = out[12]; tmp[13] = out[13]; tmp[14] = out[14]; tmp[15] = out[15];

    int i = 0;
    for (; i < 4; out += 4, ++i) {
        float *restrict itr = tmp;
        int j = 0;
        for (; j < 4; itr += 4, ++j)
            out[j] = itr[i];
    }
}

/** compound -----------------------------------------------------------------*/
void
mat4array_get_look_at(
      float *restrict out
    , float *restrict eye
    , float *restrict mid
    , float *restrict up)
{
#   define x 0
#   define y 1
#   define z 2
    float roll[3] = { mid[x] - eye[x],  mid[y] - eye[y], mid[z] - eye[z] };
    float len_recip = 1.0f / (float)sqrt(
        roll[x] * roll[x]
      + roll[y] * roll[y]
      + roll[z] * roll[z]);
    roll[x] *= len_recip;
    roll[y] *= len_recip;
    roll[z] *= len_recip;

    float pitch[3] = {
          roll[y] * up[z] - roll[z] * up[y]
        , roll[z] * up[x] - roll[x] * up[z]
        , roll[x] * up[y] - roll[y] * up[x] };
    len_recip = 1.0f / (float)sqrt(
        pitch[x] * pitch[x]
      + pitch[y] * pitch[y]
      + pitch[z] * pitch[z]);
    pitch[x] *= len_recip;
    pitch[y] *= len_recip;
    pitch[z] *= len_recip;

    float yaw[3] = {
          pitch[y] * roll[z] - pitch[z] * roll[y]
        , pitch[z] * roll[x] - pitch[x] * roll[z]
        , pitch[x] * roll[y] - pitch[y] * roll[x]
    };

    out[ 0] = pitch[x];
    out[ 4] = pitch[y];
    out[ 8] = pitch[z];

    out[ 1] = yaw[x];
    out[ 5] = yaw[y];
    out[ 9] = yaw[z];

    out[ 2] = -roll[x];
    out[ 6] = -roll[y];
    out[10] = -roll[z];

    out[12] = -(pitch[x] * eye[x] + pitch[y] * eye[y] + pitch[z] * eye[z]);
    out[13] = -(yaw[x] * eye[x] + yaw[y] * eye[y] + yaw[z] * eye[z]);
    out[14] = roll[x] * eye[x] + roll[y] * eye[y] + roll[z] * eye[z];
#   undef x
#   undef y
#   undef z
}

void
mat4array_get_frustrum(
      float *restrict out
    , float left
    , float right
    , float top
    , float bottom
    , float near
    , float far)
{
    float dz = right - left;
    float dy = top - bottom;
    float dx = far - near;

    out[ 0] =  2.0f * near / dz;
    out[ 5] =  2.0f * near / dy;
    out[ 8] =  (right + left) / dz;
    out[ 9] =  (top + bottom) / dy;
    out[10] = -(far + near) / dx;
    out[11] = -1;
    out[14] = -(2 * far * near) / dx;
}
void
mat4array_get_perspective( /* glm/gtc/matrix_transform.inl */
      float *restrict out
    , const float field_of_view
    , const float aspect_ratio
    , const float near_plane
    , const float far_plane)
{
    assert(aspect_ratio > 0.0f);

    float loc = (float)tan(field_of_view / 2);

    out[ 0] = 1.0f / (aspect_ratio * loc);
    out[ 5] = 1.0f / loc;
    out[10] = -(far_plane + near_plane) / far_plane - near_plane;
    out[11] = -1.0f;
    out[14] = -(2.0f * far_plane * near_plane) / far_plane - near_plane;
}

/** eof */

