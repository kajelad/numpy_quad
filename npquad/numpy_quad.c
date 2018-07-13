/*
 * IEEE 128 bit floating point type for NumPy
 * 2018 Jack Dale
 *
 * Derived from numpy quaternion, copyright information retained below:
 *
 * Quaternion type for numpy
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

#define NPY_NO_DEPRECATED_API NPY_API_VERSION
//#define VERBOSE

#include "stdio.h"
#include <Python.h>
#include <numpy/arrayobject.h>
#include <numpy/npy_math.h>
#include <numpy/ufuncobject.h>
#include "structmember.h"
#include "numpy/npy_3kcompat.h"
#include "quadmath.h"

#include "quad.h"

typedef struct {
        PyObject_HEAD
        quad obval;
} PyQuadScalarObject;
/*
PyMemberDef PyQuadArrType_members[] = {
    {"real", T_DOUBLE, offsetof(PyQuadScalarObject, obval.w), READONLY,
        "The real component of the quad"},
    {"w", T_DOUBLE, offsetof(PyQuadScalarObject, obval.w), READONLY,
        "The real component of the quad"},
    {"x", T_DOUBLE, offsetof(PyQuadScalarObject, obval.x), READONLY,
        "The first imaginary component of the quad"},
    {"y", T_DOUBLE, offsetof(PyQuadScalarObject, obval.y), READONLY,
        "The second imaginary component of the quad"},
    {"z", T_DOUBLE, offsetof(PyQuadScalarObject, obval.z), READONLY,
        "The third imaginary component of the quad"},
    {NULL}
};
*/
/*
static PyObject *
PyQuadArrType_get_components(PyObject *self, void *closure)
{
    quad *q = &((PyQuadScalarObject *)self)->obval;
    PyObject *tuple = PyTuple_New(4);
    PyTuple_SET_ITEM(tuple, 0, PyFloat_FromDouble(q->w));
    PyTuple_SET_ITEM(tuple, 1, PyFloat_FromDouble(q->x));
    PyTuple_SET_ITEM(tuple, 2, PyFloat_FromDouble(q->y));
    PyTuple_SET_ITEM(tuple, 3, PyFloat_FromDouble(q->z));
    return tuple;
}
*/
/*
static PyObject *
PyQuadArrType_get_imag(PyObject *self, void *closure)
{
    quad *q = &((PyQuadScalarObject *)self)->obval;
    PyObject *tuple = PyTuple_New(3);
    PyTuple_SET_ITEM(tuple, 0, PyFloat_FromDouble(q->x));
    PyTuple_SET_ITEM(tuple, 1, PyFloat_FromDouble(q->y));
    PyTuple_SET_ITEM(tuple, 2, PyFloat_FromDouble(q->z));
    return tuple;
}
*/
/*
PyGetSetDef PyQuadArrType_getset[] = {
    {"components", PyQuadArrType_get_components, NULL,
        "The components of the quad as a (w,x,y,z) tuple", NULL},
    {"imag", PyQuadArrType_get_imag, NULL,
        "The imaginary part of the quad as an (x,y,z) tuple", NULL},
    {NULL}
};
*/
PyTypeObject PyQuadArrType_Type = {
#if defined(NPY_PY3K)
    PyVarObject_HEAD_INIT(NULL, 0)
#else
    PyObject_HEAD_INIT(NULL)
    0,                                          /* ob_size */
#endif
    "npquad.quad",                              /* tp_name*/
    sizeof(PyQuadScalarObject),                 /* tp_basicsize*/
    0,                                          /* tp_itemsize */
    0,                                          /* tp_dealloc */
    0,                                          /* tp_print */
    0,                                          /* tp_getattr */
    0,                                          /* tp_setattr */
#if defined(NPY_PY3K)
    0,                                          /* tp_reserved */
#else
    0,                                          /* tp_compare */
#endif
    0,                                          /* tp_repr */
    0,                                          /* tp_as_number */
    0,                                          /* tp_as_sequence */
    0,                                          /* tp_as_mapping */
    0,                                          /* tp_hash */
    0,                                          /* tp_call */
    0,                                          /* tp_str */
    0,                                          /* tp_getattro */
    0,                                          /* tp_setattro */
    0,                                          /* tp_as_buffer */
    0,                                          /* tp_flags */
    0,                                          /* tp_doc */
    0,                                          /* tp_traverse */
    0,                                          /* tp_clear */
    0,                                          /* tp_richcompare */
    0,                                          /* tp_weaklistoffset */
    0,                                          /* tp_iter */
    0,                                          /* tp_iternext */
    0,                                          /* tp_methods */
    0,                                          /* tp_members */
    0,                                          /* tp_getset */
    0,                                          /* tp_base */
    0,                                          /* tp_dict */
    0,                                          /* tp_descr_get */
    0,                                          /* tp_descr_set */
    0,                                          /* tp_dictoffset */
    0,                                          /* tp_init */
    0,                                          /* tp_alloc */
    0,                                          /* tp_new */
    0,                                          /* tp_free */
    0,                                          /* tp_is_gc */
    0,                                          /* tp_bases */
    0,                                          /* tp_mro */
    0,                                          /* tp_cache */
    0,                                          /* tp_subclasses */
    0,                                          /* tp_weaklist */
    0,                                          /* tp_del */
#if PY_VERSION_HEX >= 0x02060000
    0,                                          /* tp_version_tag */
#endif
};

static PyArray_ArrFuncs _PyQuad_ArrFuncs;
PyArray_Descr *quad_descr;


static quad
MyPyFloat_AsQuad(PyObject *obj)
{
    double d;
    quad q;
    PyObject *num;

    if( obj == Py_None) {
        d = NPY_NAN;
    } else {
        num = PyNumber_Float(obj);
        if (num == NULL) {
            d = NPY_NAN;
        } else {
            d = PyFloat_AsDouble(num);
            Py_DECREF(num);
        }
    }

    q = (quad) d;

    return q;
}

static PyObject *
QUAD_getitem(char *ip, PyArrayObject *ap)
{
    quad q;
    PyObject *op;
    PyArray_Descr *descr;
    descr = PyArray_DESCR(ap);

    if ((ap == NULL) || PyArray_ISBEHAVED_RO(ap)) {
        #ifdef VERBOSE
        printf("getitem: well behaved\n");
        #endif
        q = *((quad *)ip);
    }
    else {
        #ifdef VERBOSE
        printf("getitem: swapping\n");
        #endif
        descr->f->copyswap(&q, ip, !PyArray_ISNOTSWAPPED(ap), NULL);
    }

    op = PyFloat_FromDouble((double) q);
    Py_DECREF(descr);
    return op;
}

static int QUAD_setitem(PyObject *op, char *ov, PyArrayObject *ap)
{
    quad q;

    if (PyArray_IsScalar(op, Quad)) {
        q = ((PyQuadScalarObject *)op)->obval;
    }
    else {
        q = PyFloat_AsDouble(op);
        //q = MyPyFloat_AsQuad(op);
    }
    if (PyErr_Occurred()) {
        if (PySequence_Check(op)) {
            PyErr_Clear();
            PyErr_SetString(PyExc_ValueError,
                    "setting an array element with a sequence.");
        }
        return -1;
    }
    if (ap == NULL || PyArray_ISBEHAVED(ap))
        *((quad *)ov)=q;
    else {
        PyArray_Descr *descr;

        descr = PyArray_DESCR(ap);
        descr->f->copyswap(ov, &q, !PyArray_ISNOTSWAPPED(ap), NULL);

        //descr = PyArray_DescrFromType(NPY_DOUBLE);
        //descr->f->copyswap(ov, &q.w, !PyArray_ISNOTSWAPPED(ap), NULL);
        //descr->f->copyswap(ov+8, &q.x, !PyArray_ISNOTSWAPPED(ap), NULL);

        Py_DECREF(descr);
    }

    return 0;
}
/*
static void
QUAD_copyswap(quad *dst, quad *src,
        int swap, void *NPY_UNUSED(arr))
{
    PyArray_Descr *descr;
    descr = PyArray_DescrFromType(NPY_DOUBLE);
    descr->f->copyswapn(dst, sizeof(double), src, sizeof(double), 4, swap, NULL);
    Py_DECREF(descr);
}

static void
QUAD_copyswapn(quad *dst, npy_intp dstride,
        quad *src, npy_intp sstride,
        npy_intp n, int swap, void *NPY_UNUSED(arr))
{
    PyArray_Descr *descr;
    descr = PyArray_DescrFromType(NPY_DOUBLE);
    descr->f->copyswapn(&dst->w, dstride, &src->w, sstride, n, swap, NULL);
    descr->f->copyswapn(&dst->x, dstride, &src->x, sstride, n, swap, NULL);
    descr->f->copyswapn(&dst->y, dstride, &src->y, sstride, n, swap, NULL);
    descr->f->copyswapn(&dst->z, dstride, &src->z, sstride, n, swap, NULL);
    Py_DECREF(descr);    
}
*/
static int
QUAD_compare (quad *pa, quad *pb, PyArrayObject *NPY_UNUSED(ap))
{
    quad a = *pa, b = *pb;
    npy_bool anan, bnan;
    int ret;

    anan = quad_isnan(a);
    bnan = quad_isnan(b);

    if (anan) {
        ret = bnan ? 0 : -1;
    } else if (bnan) {
        ret = 1;
    } else if(quad_less(a, b)) {
        ret = -1;
    } else if(quad_less(b, a)) {
        ret = 1;
    } else {
        ret = 0;
    }

    return ret;
}

static int
QUAD_argmax(quad *ip, npy_intp n, npy_intp *max_ind, PyArrayObject *NPY_UNUSED(aip))
{
    npy_intp i;
    quad mp = *ip;

    *max_ind = 0;

    if (quad_isnan(mp)) {
        /* nan encountered; it's maximal */
        return 0;
    }

    for (i = 1; i < n; i++) {
        ip++;
        /*
         * Propagate nans, similarly as max() and min()
         */
        if (!(quad_less_equal(*ip, mp))) {  /* negated, for correct nan handling */
            mp = *ip;
            *max_ind = i;
            if (quad_isnan(mp)) {
                /* nan encountered, it's maximal */
                break;
            }
        }
    }
    return 0;
}

static npy_bool
QUAD_nonzero (char *ip, PyArrayObject *ap)
{
    quad q;
    if (ap == NULL || PyArray_ISBEHAVED_RO(ap)) {
        q = *(quad *)ip;
    }
    else {
        PyArray_Descr *descr;
        descr = PyArray_DESCR(ap);
        descr->f->copyswap(&q, ip, !PyArray_ISNOTSWAPPED(ap), NULL);
        Py_DECREF(descr);
    }
    return (npy_bool) !quad_equal(q, (quad) 0);
}

static void
QUAD_fillwithscalar(quad *buffer, npy_intp length, quad *value, void *NPY_UNUSED(ignored))
{
    npy_intp i;
    quad val = *value;

    for (i = 0; i < length; ++i) {
        buffer[i] = val;
    }
}

static void
QUAD_dot(char *ip1, npy_intp is1, char *ip2, npy_intp is2, char *op, npy_intp n,
           void *NPY_UNUSED(ignore))
{
    quad tmp = 0;
    npy_intp i;

    for (i = 0; i < n; i++, ip1 += is1, ip2 += is2) {
        tmp += (*((quad *)ip1)) * (*((quad *)ip2));
    }

    *((quad *) op) = tmp;
}

#ifdef VERBOSE
#define MAKE_T_TO_QUAD(TYPE, type)                                             \
static void                                                                    \
TYPE ## _to_quad(type *ip, quad *op, npy_intp n,                               \
               PyArrayObject *NPY_UNUSED(aip), PyArrayObject *NPY_UNUSED(aop)) \
{                                                                              \
    printf("converting %s to quad\n", #type);                                  \
    double temp;                                                               \
    while (n--) {                                                              \
        temp = (double)  (*ip++);                                              \
        *op++ = (__float128) temp;                                             \
    }                                                                          \
}
#else
#define MAKE_T_TO_QUAD(TYPE, type)                                             \
static void                                                                    \
TYPE ## _to_quad(type *ip, quad *op, npy_intp n,                               \
               PyArrayObject *NPY_UNUSED(aip), PyArrayObject *NPY_UNUSED(aop)) \
{                                                                              \
    double temp;                                                               \
    while (n--) {                                                              \
        temp = (double)  (*ip++);                                              \
        *op++ = (__float128) temp;                                             \
    }                                                                          \
}
#endif

MAKE_T_TO_QUAD(FLOAT, npy_float);
MAKE_T_TO_QUAD(DOUBLE, npy_double);
//MAKE_T_TO_QUAD(LONGDOUBLE, npy_longdouble);
MAKE_T_TO_QUAD(BOOL, npy_bool);
MAKE_T_TO_QUAD(BYTE, npy_byte);
MAKE_T_TO_QUAD(UBYTE, npy_ubyte);
MAKE_T_TO_QUAD(SHORT, npy_short);
MAKE_T_TO_QUAD(USHORT, npy_ushort);
MAKE_T_TO_QUAD(INT, npy_int);
MAKE_T_TO_QUAD(UINT, npy_uint);
MAKE_T_TO_QUAD(LONG, npy_long);
MAKE_T_TO_QUAD(ULONG, npy_ulong);
MAKE_T_TO_QUAD(LONGLONG, npy_longlong);
MAKE_T_TO_QUAD(ULONGLONG, npy_ulonglong);

#define MAKE_CT_TO_QUAD(TYPE, type)                                            \
static void                                                                    \
quad_to_ ## TYPE(type *ip, quad *op, npy_intp n,                               \
               PyArrayObject *NPY_UNUSED(aip), PyArrayObject *NPY_UNUSED(aop)) \
{                                                                              \
    while (n--) {                                                              \
        op->w = (double)(*ip++);                                               \
        op->x = (double)(*ip++);                                               \
        op->y = 0;                                                             \
        op->z = 0;                                                             \
    }                                                                          \
}

//MAKE_CT_TO_QUAD(CFLOAT, npy_uint32);
//MAKE_CT_TO_QUAD(CDOUBLE, npy_uint64);
//MAKE_CT_TO_QUAD(CLONGDOUBLE, npy_longdouble);

static void register_cast_function(int sourceType, int destType, PyArray_VectorUnaryFunc *castfunc)
{
    PyArray_Descr *descr = PyArray_DescrFromType(sourceType);
    PyArray_RegisterCastFunc(descr, destType, castfunc);
    PyArray_RegisterCanCast(descr, destType, NPY_NOSCALAR);
    Py_DECREF(descr);
}

#ifdef VERBOSE
#define MAKE_QUAD_TO_T(TYPE, type)                                             \
static void                                                                    \
quad_to_ ## TYPE(quad *ip, type *op, npy_intp n,                               \
               PyArrayObject *NPY_UNUSED(aip), PyArrayObject *NPY_UNUSED(aop)) \
{                                                                              \
    printf("converting quad to %s\n", #type);                                  \
    while (n--) {                                                              \
        *op++ =  (type) *ip++;                                                 \
    }                                                                          \
}
#else
#define MAKE_QUAD_TO_T(TYPE, type)                                             \
static void                                                                    \
quad_to_ ## TYPE(quad *ip, type *op, npy_intp n,                               \
               PyArrayObject *NPY_UNUSED(aip), PyArrayObject *NPY_UNUSED(aop)) \
{                                                                              \
    while (n--) {                                                              \
        *op++ =  (type) *ip++;                                                 \
    }                                                                          \
}
#endif

MAKE_QUAD_TO_T(FLOAT, npy_float64);
MAKE_QUAD_TO_T(DOUBLE, npy_double);
//MAKE_QUAD_TO_T(LONGDOUBLE, npy_longdouble);
MAKE_QUAD_TO_T(BOOL, npy_bool);
MAKE_QUAD_TO_T(BYTE, npy_byte);
MAKE_QUAD_TO_T(UBYTE, npy_ubyte);
MAKE_QUAD_TO_T(SHORT, npy_short);
MAKE_QUAD_TO_T(USHORT, npy_ushort);
MAKE_QUAD_TO_T(INT, npy_int);
MAKE_QUAD_TO_T(UINT, npy_uint);
MAKE_QUAD_TO_T(LONG, npy_long);
MAKE_QUAD_TO_T(ULONG, npy_ulong);
MAKE_QUAD_TO_T(LONGLONG, npy_longlong);
MAKE_QUAD_TO_T(ULONGLONG, npy_ulonglong);

static PyObject *
quad_arrtype_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    #ifdef VERBOSE
    printf("new quad\n");
    #endif
    PyObject *obj = NULL;
    quad q = 0;

    if (!PyArg_ParseTuple(args, "|O", &obj)) {
        return NULL;
    }
    if (PyObject_IsInstance(obj,(PyObject*)&PyQuadArrType_Type)) {
        Py_INCREF(obj);
        return obj;
    } else if (PyUnicode_Check(obj)) {
        char* s = PyUnicode_AsUTF8(obj);
        q = strtoflt128(s, NULL);
    } else if (obj != NULL) {
        q = MyPyFloat_AsQuad(obj);
    }
    //PyArray_Descr *descr = PyArray_DescrFromType(NPY_COMPLEX128);
    return PyArray_Scalar(&q, quad_descr, NULL);
}

static PyObject *
gentype_richcompare(PyObject *self, PyObject *other, int cmp_op)
{
    PyObject *arr, *ret;

    arr = PyArray_FromScalar(self, NULL);
    if (arr == NULL) {
        return NULL;
    }
    ret = Py_TYPE(arr)->tp_richcompare(arr, other, cmp_op);
    Py_DECREF(arr);
    return ret;
}

static long
quad_arrtype_hash(PyObject *o)
{
    quad q = ((PyQuadScalarObject *)o)->obval;
    union u_type
    {
        quad t_quad;
        double t_doubles[2];
    }
    temp;
    temp.t_quad = q;
    long value = 0x456789;
    value = (10000004 * value) ^ _Py_HashDouble(temp.t_doubles[0]);
    value = (10000004 * value) ^ _Py_HashDouble(temp.t_doubles[1]);
    //value = (10000004 * value) ^ _Py_HashDouble(q.y);
    //value = (10000004 * value) ^ _Py_HashDouble(q.z);
    if (value == -1)
        value = -2;
    return value;
}

static PyObject *
quad_arrtype_repr(PyObject *o)
{
    #ifdef VERBOSE
    printf("getting quad string representation\n");
    #endif
    char str[128];
    quad q = ((PyQuadScalarObject *)o)->obval;
    quadmath_snprintf(str, sizeof(str), "%.36Qg", q);
    return PyUnicode_FromString(str);
}

static PyObject *
quad_arrtype_str(PyObject *o)
{
    #ifdef VERBOSE
    printf("casting quad to string\n");
    #endif
    char str[128];
    quad q = ((PyQuadScalarObject *)o)->obval;
    quadmath_snprintf(str, sizeof(str), "%.36Qg", q);
    return PyUnicode_FromString(str);
}

static PyMethodDef QuadMethods[] = {
    {NULL, NULL, 0, NULL}
};

#define UNARY_UFUNC(name, ret_type)\
static void \
quad_##name##_ufunc(char** args, npy_intp* dimensions,\
    npy_intp* steps, void* data) {\
    char *ip1 = args[0], *op1 = args[1];\
    npy_intp is1 = steps[0], os1 = steps[1];\
    npy_intp n = dimensions[0];\
    npy_intp i;\
    for(i = 0; i < n; i++, ip1 += is1, op1 += os1){\
        const quad in1 = *(quad *)ip1;\
        *((ret_type *)op1) = quad_##name(in1);};}

UNARY_UFUNC(isnan, npy_bool)
UNARY_UFUNC(isinf, npy_bool)
UNARY_UFUNC(isfinite, npy_bool)
UNARY_UFUNC(absolute, quad)
UNARY_UFUNC(log, quad)
UNARY_UFUNC(exp, quad)
UNARY_UFUNC(negative, quad)
UNARY_UFUNC(floor, quad)
UNARY_UFUNC(ceil, quad)
UNARY_UFUNC(rint, quad)
UNARY_UFUNC(square, quad)
UNARY_UFUNC(sqrt, quad)
UNARY_UFUNC(sin, quad)
UNARY_UFUNC(cos, quad)

#define BINARY_GEN_UFUNC(name, func_name, arg_type, ret_type)   \
static void                                                     \
quad_##func_name##_ufunc(char** args, npy_intp* dimensions,     \
    npy_intp* steps, void* data) {                              \
    char *ip1 = args[0], *ip2 = args[1], *op1 = args[2];        \
    npy_intp is1 = steps[0], is2 = steps[1], os1 = steps[2];    \
    npy_intp n = dimensions[0];                                 \
    npy_intp i;                                                 \
    for(i = 0; i < n; i++, ip1 += is1, ip2 += is2, op1 += os1){ \
        const quad in1 = *(quad *)ip1;                          \
        const arg_type in2 = *(arg_type *)ip2;                  \
        *((ret_type *)op1) = quad_##func_name(in1, in2);};};

#define BINARY_UFUNC(name, ret_type)\
    BINARY_GEN_UFUNC(name, name, quad, ret_type)
#define BINARY_SCALAR_UFUNC(name, ret_type)\
    BINARY_GEN_UFUNC(name, name##_scalar, npy_double, ret_type)

BINARY_UFUNC(add, quad)
BINARY_UFUNC(subtract, quad)
BINARY_UFUNC(multiply, quad)
BINARY_UFUNC(divide, quad)
BINARY_UFUNC(floor_divide, quad)
BINARY_UFUNC(remainder, quad)
BINARY_UFUNC(power, quad)
BINARY_UFUNC(copysign, quad)
BINARY_UFUNC(equal, npy_bool)
BINARY_UFUNC(not_equal, npy_bool)
BINARY_UFUNC(less, npy_bool)
BINARY_UFUNC(less_equal, npy_bool)
BINARY_UFUNC(greater, npy_bool)
BINARY_UFUNC(greater_equal, npy_bool)
BINARY_UFUNC(minimum, quad)
BINARY_UFUNC(maximum, quad)


//BINARY_SCALAR_UFUNC(multiply, quad)
//BINARY_SCALAR_UFUNC(divide, quad)
//BINARY_SCALAR_UFUNC(power, quad)

#if defined(NPY_PY3K)
static struct PyModuleDef moduledef = {
    PyModuleDef_HEAD_INIT,
    "numpy_quad",
    NULL,
    -1,
    QuadMethods,
    NULL,
    NULL,
    NULL,
    NULL
};
#endif

#if defined(NPY_PY3K)
PyMODINIT_FUNC PyInit_numpy_quad(void) {
#else
PyMODINIT_FUNC initnumpy_quad(void) {
#endif

    PyObject *m;
    int quadNum;
    PyObject* numpy = PyImport_ImportModule("numpy");
    PyObject* numpy_dict = PyModule_GetDict(numpy);
    int arg_types[3];
    PyArray_Descr *descr;

#if defined(NPY_PY3K)
    m = PyModule_Create(&moduledef);
#else
    m = Py_InitModule("numpy_quad", QuadMethods);
#endif

    if (!m) {
        return NULL;
    }

    /* Make sure NumPy is initialized */
    import_array();
    import_umath();

    /* Register the quad array scalar type */
#if defined(NPY_PY3K)
    PyQuadArrType_Type.tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE;
#else
    PyQuadArrType_Type.tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE | Py_TPFLAGS_CHECKTYPES;
#endif
    PyQuadArrType_Type.tp_new = quad_arrtype_new;
    PyQuadArrType_Type.tp_richcompare = gentype_richcompare;
    PyQuadArrType_Type.tp_hash = quad_arrtype_hash;
    PyQuadArrType_Type.tp_repr = quad_arrtype_repr;
    PyQuadArrType_Type.tp_str = quad_arrtype_str;
    PyQuadArrType_Type.tp_base = &PyGenericArrType_Type;
    if (PyType_Ready(&PyQuadArrType_Type) < 0) {
        PyErr_Print();
        PyErr_SetString(PyExc_SystemError, "could not initialize PyQuadArrType_Type");
        return NULL;
    }

    /* The array functions */
    PyArray_InitArrFuncs(&_PyQuad_ArrFuncs);
    _PyQuad_ArrFuncs.getitem = (PyArray_GetItemFunc*)QUAD_getitem;
    _PyQuad_ArrFuncs.setitem = (PyArray_SetItemFunc*)QUAD_setitem;
    /* copy copyswap and copyswapn from complex128 */
    descr = PyArray_DescrFromType(NPY_COMPLEX128);
    _PyQuad_ArrFuncs.copyswap = descr->f->copyswap;
    _PyQuad_ArrFuncs.copyswapn = descr->f->copyswapn;
    Py_DECREF(descr);
    _PyQuad_ArrFuncs.compare = (PyArray_CompareFunc*)QUAD_compare;
    _PyQuad_ArrFuncs.argmax = (PyArray_ArgFunc*)QUAD_argmax;
    _PyQuad_ArrFuncs.nonzero = (PyArray_NonzeroFunc*)QUAD_nonzero;
    _PyQuad_ArrFuncs.fillwithscalar = (PyArray_FillWithScalarFunc*)QUAD_fillwithscalar;
    _PyQuad_ArrFuncs.dotfunc = (PyArray_DotFunc*)QUAD_dot;

    _PyQuad_ArrFuncs.cast[NPY_BOOL] = (PyArray_VectorUnaryFunc*)quad_to_BOOL;
    _PyQuad_ArrFuncs.cast[NPY_BYTE] = (PyArray_VectorUnaryFunc*)quad_to_BYTE;
    _PyQuad_ArrFuncs.cast[NPY_UBYTE] = (PyArray_VectorUnaryFunc*)quad_to_UBYTE;
    _PyQuad_ArrFuncs.cast[NPY_SHORT] = (PyArray_VectorUnaryFunc*)quad_to_SHORT;
    _PyQuad_ArrFuncs.cast[NPY_USHORT] = (PyArray_VectorUnaryFunc*)quad_to_USHORT;
    _PyQuad_ArrFuncs.cast[NPY_INT] = (PyArray_VectorUnaryFunc*)quad_to_INT;
    _PyQuad_ArrFuncs.cast[NPY_UINT] = (PyArray_VectorUnaryFunc*)quad_to_UINT;
    _PyQuad_ArrFuncs.cast[NPY_LONG] = (PyArray_VectorUnaryFunc*)quad_to_LONG;
    _PyQuad_ArrFuncs.cast[NPY_ULONG] = (PyArray_VectorUnaryFunc*)quad_to_ULONG;
    _PyQuad_ArrFuncs.cast[NPY_LONGLONG] = (PyArray_VectorUnaryFunc*)quad_to_LONGLONG;
    _PyQuad_ArrFuncs.cast[NPY_ULONGLONG] = (PyArray_VectorUnaryFunc*)quad_to_ULONGLONG;
    _PyQuad_ArrFuncs.cast[NPY_FLOAT] = (PyArray_VectorUnaryFunc*)quad_to_FLOAT;
    _PyQuad_ArrFuncs.cast[NPY_DOUBLE] = (PyArray_VectorUnaryFunc*)quad_to_DOUBLE;
    //_PyQuad_ArrFuncs.cast[NPY_LONGDOUBLE] = (PyArray_VectorUnaryFunc*)quad_to_LONGDOUBLE;
    //_PyQuad_ArrFuncs.cast[NPY_CFLOAT] = (PyArray_VectorUnaryFunc*)quad_to_CFLOAT;
    //_PyQuad_ArrFuncs.cast[NPY_CDOUBLE] = (PyArray_VectorUnaryFunc*)quad_to_CDOUBLE;
    //_PyQuad_ArrFuncs.cast[NPY_CLONGDOUBLE] = (PyArray_VectorUnaryFunc*)quad_to_CLONGDOUBLE;

    /* The quad array descr */
    //quad_descr = PyObject_New(PyArray_Descr, &PyArrayDescr_Type);
    /* Cannabilize  the gcc longdouble. I have no idea why this fixes the segfaults,
       or how it affects the performance of np.float128m but it works.
       Change at your own risk. */
    quad_descr = PyArray_DescrFromType(NPY_LONGDOUBLE);
    quad_descr->typeobj = &PyQuadArrType_Type;
    quad_descr->kind = 'f';
    quad_descr->type = 'k';
    quad_descr->byteorder = '=';
    quad_descr->type_num = 0; /* assigned at registration */
    quad_descr->elsize = 16;
    quad_descr->alignment = 16;
    quad_descr->subarray = NULL;
    quad_descr->fields = NULL;
    quad_descr->names = NULL;
    quad_descr->f = &_PyQuad_ArrFuncs;

    Py_INCREF(&PyQuadArrType_Type);
    quadNum = PyArray_RegisterDataType(quad_descr);

    if (quadNum < 0)
        return NULL;

    register_cast_function(NPY_BOOL, quadNum, (PyArray_VectorUnaryFunc*)BOOL_to_quad);
    register_cast_function(NPY_BYTE, quadNum, (PyArray_VectorUnaryFunc*)BYTE_to_quad);
    register_cast_function(NPY_UBYTE, quadNum, (PyArray_VectorUnaryFunc*)UBYTE_to_quad);
    register_cast_function(NPY_SHORT, quadNum, (PyArray_VectorUnaryFunc*)SHORT_to_quad);
    register_cast_function(NPY_USHORT, quadNum, (PyArray_VectorUnaryFunc*)USHORT_to_quad);
    register_cast_function(NPY_INT, quadNum, (PyArray_VectorUnaryFunc*)INT_to_quad);
    register_cast_function(NPY_UINT, quadNum, (PyArray_VectorUnaryFunc*)UINT_to_quad);
    register_cast_function(NPY_LONG, quadNum, (PyArray_VectorUnaryFunc*)LONG_to_quad);
    register_cast_function(NPY_ULONG, quadNum, (PyArray_VectorUnaryFunc*)ULONG_to_quad);
    register_cast_function(NPY_LONGLONG, quadNum, (PyArray_VectorUnaryFunc*)LONGLONG_to_quad);
    register_cast_function(NPY_ULONGLONG, quadNum, (PyArray_VectorUnaryFunc*)ULONGLONG_to_quad);
    register_cast_function(NPY_FLOAT, quadNum, (PyArray_VectorUnaryFunc*)FLOAT_to_quad);
    register_cast_function(NPY_DOUBLE, quadNum, (PyArray_VectorUnaryFunc*)DOUBLE_to_quad);
    //register_cast_function(NPY_LONGDOUBLE, quadNum, (PyArray_VectorUnaryFunc*)LONGDOUBLE_to_quad);
    //register_cast_function(NPY_CFLOAT, quadNum, (PyArray_VectorUnaryFunc*)CFLOAT_to_quad);
    //register_cast_function(NPY_CDOUBLE, quadNum, (PyArray_VectorUnaryFunc*)CDOUBLE_to_quad);
    //register_cast_function(NPY_CLONGDOUBLE, quadNum, (PyArray_VectorUnaryFunc*)CLONGDOUBLE_to_quad);

#define REGISTER_UFUNC(name)\
    PyUFunc_RegisterLoopForType((PyUFuncObject *)PyObject_GetAttrString(numpy, #name),\
            quad_descr->type_num, quad_##name##_ufunc, arg_types, NULL)

#define REGISTER_SCALAR_UFUNC(name)\
    PyUFunc_RegisterLoopForType((PyUFuncObject *)PyDict_GetItemString(numpy_dict, #name),\
            quad_descr->type_num, quad_##name##_scalar_ufunc, arg_types, NULL)

    /* quad -> bool */
    arg_types[0] = quad_descr->type_num;
    arg_types[1] = NPY_BOOL;

    REGISTER_UFUNC(isnan);
    REGISTER_UFUNC(isinf);
    REGISTER_UFUNC(isfinite);
    /* quat -> double */
    arg_types[1] = NPY_DOUBLE;

    //REGISTER_UFUNC(absolute);

    /* quad -> quad */
    arg_types[1] = quad_descr->type_num;

    REGISTER_UFUNC(log);
    REGISTER_UFUNC(exp);
    REGISTER_UFUNC(negative);
    REGISTER_UFUNC(absolute);
    REGISTER_UFUNC(floor);
    REGISTER_UFUNC(ceil);
    REGISTER_UFUNC(rint);
    REGISTER_UFUNC(square);
    REGISTER_UFUNC(sqrt);
    REGISTER_UFUNC(sin);
    REGISTER_UFUNC(cos);

    /* quad, quad -> bool */

    arg_types[2] = NPY_BOOL;

    REGISTER_UFUNC(equal);
    REGISTER_UFUNC(not_equal);
    REGISTER_UFUNC(less);
    REGISTER_UFUNC(less_equal);
    REGISTER_UFUNC(greater);
    REGISTER_UFUNC(greater_equal);

    /* quad, double -> quad */

    arg_types[1] = NPY_DOUBLE;
    arg_types[2] = quad_descr->type_num;

    //REGISTER_SCALAR_UFUNC(multiply);
    //REGISTER_SCALAR_UFUNC(divide);
    //REGISTER_SCALAR_UFUNC(power);

    /* quad, quad -> quad */

    arg_types[1] = quad_descr->type_num;

    REGISTER_UFUNC(add);
    REGISTER_UFUNC(subtract);
    REGISTER_UFUNC(multiply);
    REGISTER_UFUNC(divide);
    REGISTER_UFUNC(floor_divide);
    REGISTER_UFUNC(remainder);
    REGISTER_UFUNC(power);
    REGISTER_UFUNC(copysign);
    REGISTER_UFUNC(minimum);
    //This causes a segfault if uncommented
    //REGISTER_UFUNC(maximum);

    PyModule_AddObject(m, "quad", (PyObject *)&PyQuadArrType_Type);

    return m;
}
