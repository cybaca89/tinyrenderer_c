/* vec3array.c */
#include <math.h>
#include <assert.h>

#   define x 0
#   define y 1
#   define z 2
float
dot_productv3(const float *a, const float *b)
{
    return a[x] * b[x] + a[y] * b[y] + a[z] * b[z];
}

float
lengthv3(const float *a)
{
    return (float)sqrt(a[x] * a[x] + a[y] * a[y] + a[z] * a[z]);
}

void
normalizev3(float *out)
{
    float rlen =
    1.0f / (float)sqrt(out[x] * out[x] + out[y] * out[y] + out[z] * out[z]);
    out[x] *= rlen;
    out[y] *= rlen;
    out[z] *= rlen;
}

void
crossv3fst(
      float *restrict out
    , const float *restrict a
    , const float *restrict b)
{
    out[x] = a[y] * b[z] - a[z] * b[y];
    out[y] = a[z] * b[x] - a[x] * b[z];
    out[z] = a[x] * b[y] - a[y] * b[x];
}

void
crossv3(float *out, const float *a, const float *b)
{
    out[x] = a[y] * b[z] - a[z] * b[y];
    out[y] = a[z] * b[x] - a[x] * b[z];
    out[z] = a[x] * b[y] - a[y] * b[x];
}
