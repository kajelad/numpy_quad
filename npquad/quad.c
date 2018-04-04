/*
 * Quad math implementation 
 * Copyright (c) 2011 Martin Ling
 *
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the NumPy Developers nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTERS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "quad.h"
#include "quadmath.h"

#define _QUAT_EPS 1e-6

int
quad_isnonzero(quad q)
{
    return q != 0;
}

int
quad_isnan(quad q)
{
    return isnanq(q);
}

int
quad_isinf(quad q)
{
    return isinfq(q);
}

int
quad_isfinite(quad q)
{
    return finiteq(q);
}

quad
quad_absolute(quad q)
{
   return fabsq(q);
}

quad
quad_add(quad q1, quad q2)
{
   return q1 + q2;
}

quad
quad_subtract(quad q1, quad q2)
{
   return q1 - q2;
}

quad
quad_multiply(quad q1, quad q2)
{
   return q1 * q2;
}

quad
quad_divide(quad q1, quad q2)
{
   return q1 / q2;
}
/*
quad
quad_multiply_scalar(quad q, double s)
{
   return (quad) {s*q.w, s*q.x, s*q.y, s*q.z};
}

quad
quad_divide_scalar(quad q, double s)
{
   return (quad) {q.w/s, q.x/s, q.y/s, q.z/s};
}
*/
quad
quad_log(quad q)
{
    return logq(q);
}

quad
quad_exp(quad q)
{
    return expq(q);
}

quad
quad_power(quad q, quad p)
{
   return powq(q, p);
}
/*
quad
quad_power_scalar(quad q, double p)
{
   return quad_exp(quad_multiply_scalar(quad_log(q), p));
}
*/
quad
quad_negative(quad q)
{
   return -q;
}
/*
quad
quad_conjugate(quad q)
{
   return (quad) {q.w, -q.x, -q.y, -q.z};
}
*/
quad
quad_copysign(quad q1, quad q2)
{
    return copysignq(q1, q2);
}

int
quad_equal(quad q1, quad q2)
{
    return !quad_isnan(q1) && !quad_isnan(q2) && q1 == q2;
}

int
quad_not_equal(quad q1, quad q2)
{
    return !quad_equal(q1, q2);
}

int
quad_less(quad q1, quad q2)
{
    return (!quad_isnan(q1) && !quad_isnan(q2)) && (q1 < q2);
}

int
quad_less_equal(quad q1, quad q2)
{
   return (!quad_isnan(q1) && !quad_isnan(q2)) && (q1 <= q2);
}
