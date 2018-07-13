/*
 * Quad precision math implementation
 * 2018 Jack Dale
 *
 * Derived from numpy quaternion, copyright information retained below.
 *
 * Quaternion Math Implementation
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
#ifndef __QUAD_H__
#define __QUAD_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef __float128 quad;

int quad_isnonzero(quad q);
int quad_isnan(quad q);
int quad_isinf(quad q);
int quad_isfinite(quad q);
quad quad_absolute(quad q);
quad quad_add(quad q1, quad q2);
quad quad_subtract(quad q1, quad q2);
quad quad_multiply(quad q1, quad q2);
quad quad_divide(quad q1, quad q2);
quad quad_floor_divide(quad q1, quad q2);
quad quad_remainder(quad q1, quad q2);
quad quad_log(quad q);
quad quad_exp(quad q);
quad quad_power(quad q, quad p);
quad quad_negative(quad q);
quad quad_copysign(quad q1, quad q2);
int quad_equal(quad q1, quad q2);
int quad_not_equal(quad q1, quad q2);
int quad_less(quad q1, quad q2);
int quad_less_equal(quad q1, quad q2);
int quad_greater(quad q1, quad q2);
int quad_greater_equal(quad q1, quad q2);
quad quad_minimum(quad q1, quad q2);
quad quad_maximum(quad q1, quad q2);
quad quad_floor(quad q);
quad quad_ceil(quad q);
quad quad_rint(quad q);
quad quad_square(quad q);
quad quad_sqrt(quad q);
quad quad_sin(quad q);
quad quad_cos(quad q);

#ifdef __cplusplus
}
#endif

#endif
