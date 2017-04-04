/** @file mat4f.c
 *  @author Cy Baca
 */
#include <math.h>
#include <assert.h>
#include <stdlib.h>

#define MAT4D_LEN    16
#define MAT4DVEC_LEN 4
enum {
      M4EULER_U_ERROR = 0x0
    , M4ROLL =          0x1
    , M4PITCH =         0x2
    , M4YAW =           0x4
    , M4EULER_O_ERROR = 0x8
};

static void _about_roll (double *, double);
static void _about_pitch(double *, double);
static void _about_yaw  (double *, double);

struct Mat4RowVectors_t      { double *x, *y, *z, *w; };
struct Mat4RowVectorsConst_t { const double *x, *y, *z, *w; };

/** utility ------------------------------------------------------------------*/
static struct Mat4RowVectors_t
_mat4_to_row_vector(double *mat)
{ return (struct Mat4RowVectors_t){ mat, mat + 4, mat + 8, mat + 12 }; }

static struct Mat4RowVectorsConst_t
_mat4_to_row_vector_const(const double *mat)
{ return (struct Mat4RowVectorsConst_t){ mat, mat + 4, mat + 8, mat + 12 }; }


void
mat4d_copy(double *restrict out, const double *restrict in)
{
    out[ 0] = in[ 0]; out[ 1] = in[ 1]; out[ 2] = in[ 2]; out[ 3] = in[ 3];
    out[ 4] = in[ 4]; out[ 5] = in[ 5]; out[ 6] = in[ 6]; out[ 7] = in[ 7];
    out[ 8] = in[ 8]; out[ 9] = in[ 9]; out[10] = in[10]; out[11] = in[11];
    out[12] = in[12]; out[13] = in[13]; out[14] = in[14]; out[15] = in[15];
}

void
mat4d_set_identity(double *out)
{
    *out++ = 1.0; // out[0]
    *out++ = 0.0; // out[1]
    *out++ = 0.0; // out[2]
    *out++ = 0.0; // out[3]

    *out++ = 0.0; // out[4]
    *out++ = 1.0; // out[5]
    *out++ = 0.0; // out[6]
    *out++ = 0.0; // out[7]

    *out++ = 0.0; // out[8]
    *out++ = 0.0; // out[9]
    *out++ = 1.0; // out[10]
    *out++ = 0.0; // out[11]

    *out++ = 0.0; // out[12]
    *out++ = 0.0; // out[13]
    *out++ = 0.0; // out[14]
    *out++ = 1.0; // out[15]
}

void
mat4d_set_zero(double *restrict out)
{
    *out++ = 0.0; *out++ = 0.0; *out++ = 0.0; *out++ = 0.0;
    *out++ = 0.0; *out++ = 0.0; *out++ = 0.0; *out++ = 0.0;
    *out++ = 0.0; *out++ = 0.0; *out++ = 0.0; *out++ = 0.0;
    *out++ = 0.0; *out++ = 0.0; *out++ = 0.0; *out++ = 0.0;
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
 *   1, | 0, | 0, | 0 [0][0], [1][0], [2][0], [3][0] @  ───> i0, i1, i2, i3 >──╮
 *      |    |    |                                    ╭───<────────────────<──╯
 * , 0, | 1, | 0, | 0 [0][1], [1][1], [2][1], [3][1] @ ╰──> i4, i5, i6, i7 ─>──╮
 *      |    |    |
 * , 0, | 0, | 1, | 0
 *      |    |    |
 * ,T_x,|T_y,|T_z,| 1
 */
void
mat4d_get_translationv3(double *restrict out, double *restrict in)
{
    *out++ = 1.0; *out++ = 0.0; *out++ = 0.0; *out++ = 0.0;
    *out++ = 0.0; *out++ = 1.0; *out++ = 0.0; *out++ = 0.0;
    *out++ = 0.0; *out++ = 0.0; *out++ = 1.0; *out++ = 0.0;
    *out++ = *in++; // out[12] = in[0]
    *out++ = *in++; // out[13] = in[1]
    *out++ = *in++; // out[14] = in[2]
    *out++ = 1.0;
}

void
mat4d_get_translation(double *out, double x, double y, double z)
{
    *out++ = 1.0; *out++ = 0.0; *out++ = 0.0; *out++ = 0.0;
    *out++ = 0.0; *out++ = 1.0; *out++ = 0.0; *out++ = 0.0;
    *out++ = 0.0; *out++ = 0.0; *out++ = 1.0; *out++ = 0.0;
    *out++ = x;   *out++ = y;   *out++ = z;   *out++ = 1.0;
}

void
mat4d_translate(double *out, double x, double y, double z)
    #define X 0
    #define Y 1
    #define Z 2
    #define W 3
{
    struct Mat4RowVectors_t mat = mat4_to_row_vector(out);
    mat.w[X] += mat.x[X] * x + mat.y[X] * y + mat.z[X] * z;
    mat.w[Y] += mat.x[Y] * x + mat.y[Y] * y + mat.z[Y] * z;
    mat.w[Z] += mat.x[Z] * x + mat.y[Z] * y + mat.z[Z] * z;
    mat.w[W] += mat.x[W] * x + mat.y[W] * y + mat.z[W] * z;
}

void
mat4d_translatev3(double *restrict out, double *restrict in)
{
    struct Mat4RowVectors_t mat = mat4_to_row_vector(out);
    mat.w[X] += mat.x[X] * in[X] + mat.y[X] * in[Y] + mat.z[X] * in[Z];
    mat.w[Y] += mat.x[Y] * in[X] + mat.y[Y] * in[Y] + mat.z[Y] * in[Z];
    mat.w[Z] += mat.x[Z] * in[X] + mat.y[Z] * in[Y] + mat.z[Z] * in[Z];
    mat.w[W] += mat.x[W] * in[X] + mat.y[W] * in[Y] + mat.z[W] * in[Z];
    #undef X
    #undef Y
    #undef Z
    #undef W
}

/** multiply -----------------------------------------------------------------*/
void
mat4d_get_product(double *restrict out, const double *restrict inp)
{
    double A[16];

    A[ 0] = out[ 0]; A[ 1] = out[ 1]; A[ 2] = out[ 2]; A[ 3] = out[ 3];
    A[ 4] = out[ 4]; A[ 5] = out[ 5]; A[ 6] = out[ 6]; A[ 7] = out[ 7];
    A[ 8] = out[ 8]; A[ 9] = out[ 9]; A[10] = out[10]; A[11] = out[11];
    A[12] = out[12]; A[13] = out[13]; A[14] = out[14]; A[15] = out[15];

    struct Mat4RowVectors_t      ot = mat4_to_row_vector(out);
    struct Mat4RowVectorsConst_t in = mat4_to_row_vector_const(inp);
    int i = 0;
    for (; i < 4; ++i) {
        ot.x[i] = A[ 0] * in.x[i] + A[ 1] * in.y[i] + A[ 2] * in.z[i]
                + A[ 3] * in.w[i];

        ot.y[i] = A[ 4] * in.x[i] + A[ 5] * in.y[i] + A[ 6] * in.z[i]
                + A[ 7] * in.w[i];

        ot.z[i] = A[ 8] * in.x[i] + A[ 9] * in.y[i] + A[10] * in.z[i]
                + A[11] * in.w[i];

        ot.w[i] = A[12] * in.x[i] + A[13] * in.y[i] + A[14] * in.z[i]
                + A[15] * in.w[i];
    }
}

/** rotation -----------------------------------------------------------------*/
void
mat4d_rotatev3(double *restrict out, double rad, double *restrict in)
{
    double cop[16];
    double rot[16];
    struct Mat4RowVectors_t ot = mat4_to_row_vector(out);
    struct Mat4RowVectors_t cp = mat4_to_row_vector(cop);

    double c = (double)cos(rad);
    double s = (double)sin(rad);

    /* copy input matrix */
    cop[ 0] = out[ 0]; cop[ 1] = out[ 1]; cop[ 2] = out[ 2]; cop[ 3] = out[ 3];
    cop[ 4] = out[ 4]; cop[ 5] = out[ 5]; cop[ 6] = out[ 6]; cop[ 7] = out[ 7];
    cop[ 8] = out[ 8]; cop[ 9] = out[ 9]; cop[10] = out[10]; cop[11] = out[11];
    cop[12] = out[12]; cop[13] = out[13]; cop[14] = out[14]; cop[15] = out[15];


#   define x 0
#   define y 1
#   define z 2
    /* normalize */
    double len = (double)sqrt(
        in[x] * in[x]
      + in[y] * in[y]
      + in[z] * in[z]);

    double norm[3] = { in[x] / len, in[y] / len, in[z] / len };

    double frac[3] = {
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
mat4d_rotate(double *restrict out, double rad, double x, double y, double z)
{
    double cop[16];
    double rot[16];
    struct Mat4RowVectors_t ret, mat;

    /* copy input matrix */
    cop[ 0] = out[ 0]; cop[ 1] = out[ 1]; cop[ 2] = out[ 2]; cop[ 3] = out[ 3];
    cop[ 4] = out[ 4]; cop[ 5] = out[ 5]; cop[ 6] = out[ 6]; cop[ 7] = out[ 7];
    cop[ 8] = out[ 8]; cop[ 9] = out[ 9]; cop[10] = out[10]; cop[11] = out[11];
    cop[12] = out[12]; cop[13] = out[13]; cop[14] = out[14]; cop[15] = out[15];

    double c = (double)cos(rad);
    double s = (double)sin(rad);

    /* normalize http://www.fundza.com/vectors/normalize/ */
    double len = (double)sqrt((x * x) + (y * y) + (z * z));
    double nx = x / len;
    double ny = y / len;
    double nz = z / len;

    /* make rotation matrix, thank you glm header lib */
    double tx = (1.0f - c) * nx;
    double ty = (1.0f - c) * ny;
    double tz = (1.0f - c) * nz;
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
    ret = mat4_to_row_vector(out);
    mat = mat4_to_row_vector(cop);
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
mat4d_get_rotation(double *restrict out, double rad, int axis)
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
            assert(axis > MAT4EULER_U_ERROR
                && axis < MAT4EULER_O_ERROR);
    }
}

void _about_roll(double *out, double rad)
{
    double c = (double)cos(rad), s = (double)sin(rad);
    *out++ =   c; *out++ =   s; *out++ = 0.0; *out++ = 0.0;
    *out++ =  -s; *out++ =   c; *out++ = 0.0; *out++ = 0.0;
    *out++ = 0.0; *out++ = 0.0; *out++ = 1.0; *out++ = 0.0;
    *out++ = 0.0; *out++ = 0.0; *out++ = 0.0; *out++ = 1.0;
}

void _about_pitch(double *out, double rad)
{
    double c = (double)cos(rad), s = (double)sin(rad);
    *out++ = 1.0; *out++ = 0.0; *out++ = 0.0; *out++ = 0.0;
    *out++ = 0.0; *out++ =   c; *out++ =  -s; *out++ = 0.0;
    *out++ = 0.0; *out++ =  -s; *out++ =   c; *out++ = 0.0;
    *out++ = 0.0; *out++ = 0.0; *out++ = 0.0; *out++ = 1.0;
}

void _about_yaw(double *out, double rad)
{
    double c = (double)cos(rad), s = (double)sin(rad);
    *out++ =   c; *out++ = 0.0; *out++ =  -s; *out++ = 0.0;
    *out++ = 0.0; *out++ = 1.0; *out++ = 0.0; *out++ = 0.0;
    *out++ =   s; *out++ = 0.0; *out++ =   c; *out++ = 0.0;
    *out++ = 0.0; *out++ = 0.0; *out++ = 0.0; *out++ = 1.0;
}

/** scale --------------------------------------------------------------------*/
/** mat4d_scale.c */
/*   S_x  0   0   0
 *    0  S_y  0   0
 *    0   0  S_z  0
 *    0   0   0   1
 */

void
mat4d_get_scaled(double *restrict out, double x, double y, double z)
{
    *out++ =   x; *out++ = 0.0; *out++ = 0.0; *out++ = 0.0;
    *out++ = 0.0; *out++ =   y; *out++ = 0.0; *out++ = 0.0;
    *out++ = 0.0; *out++ = 0.0; *out++ =   z; *out++ = 0.0;
    *out++ = 0.0; *out++ = 0.0; *out++ = 0.0; *out++ = 1.0;
}

void
mat4d_scale(double *restrict out, double x, double y, double z)
{
    struct Mat4RowVectors_t ot = _mat4_to_row_vector(out);
    /*
    double cop[16];
    out[ 0] = cop[ 0]; out[ 1] = cop[ 1]; out[ 2] = cop[ 2]; out[ 3] = cop[ 3];
    out[ 4] = cop[ 4]; out[ 5] = cop[ 5]; out[ 6] = cop[ 6]; out[ 7] = cop[ 7];
    out[ 8] = cop[ 8]; out[ 9] = cop[ 9]; out[10] = cop[10]; out[11] = cop[11];
    out[12] = cop[12]; out[13] = cop[13]; out[14] = cop[14]; out[15] = cop[15];
    vecMat_t cp = vecMat4_row_get(cop);
    double scl[16];
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
    int i;
    for (i = 0; i < 4; ++i) {
        ot.x[i] *= x;
        ot.y[i] *= y;
        ot.z[i] *= z;
    }
}

void
mat4d_scalev3(double *restrict out, double *restrict in)
{
    struct Mat4RowVectors_t ot = _mat4_to_row_vector(out);
    int i;
    for (i = 0; i < 4; ++i) {
        ot.x[i] *= in[0];
        ot.y[i] *= in[1];
        ot.z[i] *= in[2];
    }
}

/** transpose ----------------------------------------------------------------*/
void
mat4d_transpose(double *out)
{
    assert(out);
    double tmp[16];
    tmp[ 0] = out[ 0]; tmp[ 1] = out[ 1]; tmp[ 2] = out[ 2]; tmp[ 3] = out[ 3];
    tmp[ 4] = out[ 4]; tmp[ 5] = out[ 5]; tmp[ 6] = out[ 6]; tmp[ 7] = out[ 7];
    tmp[ 8] = out[ 8]; tmp[ 9] = out[ 9]; tmp[10] = out[10]; tmp[11] = out[11];
    tmp[12] = out[12]; tmp[13] = out[13]; tmp[14] = out[14]; tmp[15] = out[15];

    int i;
    for (i = 0; i < 4; out += 4, ++i) {
        double *restrict itr = tmp;
        int j;
        for (j = 0; j < 4; itr += 4, ++j)
            out[j] = itr[i];
    }
}

/** compound -----------------------------------------------------------------*/
void
mat4d_get_look_at(
      double *restrict out
    , double *restrict eye
    , double *restrict mid
    , double *restrict up)
{
#   define x 0
#   define y 1
#   define z 2
    double roll[3] = { mid[x] - eye[x],  mid[y] - eye[y], mid[z] - eye[z] };
    double len_recip = 1.0 / sqrt(
        roll[x] * roll[x]
      + roll[y] * roll[y]
      + roll[z] * roll[z]);
    roll[x] *= len_recip;
    roll[y] *= len_recip;
    roll[z] *= len_recip;

    double pitch[3] = {
          roll[y] * up[z] - roll[z] * up[y]
        , roll[z] * up[x] - roll[x] * up[z]
        , roll[x] * up[y] - roll[y] * up[x] };
    len_recip = 1.0 / sqrt(
        pitch[x] * pitch[x]
      + pitch[y] * pitch[y]
      + pitch[z] * pitch[z]);
    pitch[x] *= len_recip;
    pitch[y] *= len_recip;
    pitch[z] *= len_recip;

    double yaw[3] = {
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
mat4d_get_frustrum(
      double *out
    , double left
    , double right
    , double top
    , double bottom
    , double near
    , double far)
{
    double dz = right - left;
    double dy = top - bottom;
    double dx = far - near;

    out[ 0] =  2.0 * near / dz;
    out[ 5] =  2.0 * near / dy;
    out[ 8] =  (right + left) / dz;
    out[ 9] =  (top + bottom) / dy;
    out[10] = -(far + near) / dx;
    out[11] = -1;
    out[14] = -(2 * far * near) / dx;
}
void
mat4d_get_perspective( /* glm/gtc/matrix_transform.inl */
      double *out
    , const double field_of_view
    , const double aspect_ratio
    , const double near_plane
    , const double far_plane)
{
    assert(aspect_ratio > 0.0);

    double loc = (double)tan(field_of_view / 2);

    out[ 0] = 1.0 / (aspect_ratio * loc);
    out[ 5] = 1.0 / loc;
    out[10] = -(far_plane + near_plane) / far_plane - near_plane;
    out[11] = -1.0;
    out[14] = -(2.0 * far_plane * near_plane) / far_plane - near_plane;
}
/** eof */

