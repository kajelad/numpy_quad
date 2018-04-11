import numpy as np
import npquad

print("declaring quad from string '1.7'")
q1 = np.quad('1.7')
print("result: {0}".format(str(q1)))

f = float("1.7")
f32 = np.float32("1.7")
f64 = np.float64("1.7")

print("coercing floating point values 1.7 to quad")
q3 = np.quad(f)
print("python float: {0}".format(str(q3)))
q4 = np.quad(f32)
print("numpy float32: {0}".format(str(q4)))
q5 = np.quad(f64)
print("numpy float64: {0}".format(str(q5)))

i = int(2)
i8 = np.int8(2)
i16 = np.int16(2)
i32 = np.int32(2)
i64 = np.int64(2)

print("coercing floating point values 1.7 to quad")
q6 = np.quad(i)
print("python int: {0}".format(str(q6)))
q7 = np.quad(i8)
print("numpy int8: {0}".format(str(q7)))
q8 = np.quad(i16)
print("numpy int16: {0}".format(str(q8)))
q9 = np.quad(i32)
print("numpy int32: {0}".format(str(q9)))
q10 = np.quad(i64)
print("numpy int64: {0}".format(str(q10)))



print("declaring empty array")
ar = np.empty(4, dtype=np.quad)
print(ar)

print("filling it with 1.7")
ar.fill(np.quad("1.7"))
print(ar)

print("assigning value to single elements:")
print("np.float64 2.3 at position 0")
ar[0] = np.float64(2.3)
print(ar)
print("np.int32 4 at position 1")
ar[1] = np.int32(4)
print(ar)


print("declaring zero array")
ar = np.empty(4, dtype=np.quad)
print(ar)


print("creating np.float64 array")
far = np.empty(4, dtype=np.float64)
far.fill(1.7)
print(far)
print("coercing to quad")
far_toq = far.astype(np.quad)
print(far_toq)
print("dtype:", far_toq.dtype)
print("size:", far_toq.nbytes)

print("creating random 2x2 quad matrix")
mat = np.random.rand(2, 2).astype(np.quad)
print(mat)

print("matrix multiplying by itself")
print(np.dot(mat, mat, out=mat))
