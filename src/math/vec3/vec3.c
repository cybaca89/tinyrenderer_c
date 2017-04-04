/** @file  vec3.c
 *  @author Cy Baca
 */
#include <math.h>
#include <assert.h>

typedef struct { float x, y, z; } vec3f_t;

// threadsafe style
float
dot_productv3f(const vec3f_t a, const vec3f_t b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

float
lengthv3f(const vec3f_t a)
{
    return (float)sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
}

vec3f_t
normalizev3f(vec3f_t in)
{
    float rlen =
    1.0f / (float)sqrt(in.x * in.x + in.y * in.y + in.z * in.z);
    /*
    in.x *= rlen;
    in.y *= rlen;
    in.z *= rlen;
    return in;
    */
    return (vec3f_t) { in.x * rlen, in.y * rlen, in.z * rlen };
}

vec3f_t
crossv3f(const vec3f_t a, const vec3f_t b)
{
    return (vec3f_t) {
           a.y * b.z - a.z * b.y
        ,  a.z * b.x - a.x * b.z
        ,  a.x * b.y - a.y * b.x };
    /*
    vec3f_t out = {
           .x = a.y * b.z - a.z * b.y
        ,  .y = a.z * b.x - a.x * b.z
        ,  .z = a.x * b.y - a.y * b.x };
    return out;
    */
}

vec3f_t
sumv3f(const vec3f_t a, const vec3f_t b)
{
    return (vec3f_t){ a.x + b.x, a.y + b.y, a.z + b.z };
}
