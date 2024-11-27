from numba import jit, int32, float32
import numpy as np
import time

SIZE = 2000
x = np.random.random((SIZE, SIZE))

"""
给定n*n矩阵，对矩阵每个元素计算tanh值，然后求和。
因为要循环矩阵中的每个元素，计算复杂度为 n*n。
"""
@jit(nopython=True, nogil=True)
def jit_tan_sum(a):   # 函数在被调用时编译成机器语言
    tan_sum = 0
    for i in range(SIZE):   # Numba 支持循环
        for j in range(SIZE):
            tan_sum += np.tanh(a[i, j])   # Numba 支持绝大多数NumPy函数
    return tan_sum

# 总时间 = 编译时间 + 运行时间
start = time.perf_counter()
jit_tan_sum(x)
end = time.perf_counter()
print(f"Elapsed (with compilation) = { (end - start) * 1e+6} us" )

# Numba将加速的代码缓存下来
# 总时间 = 运行时间
start = time.perf_counter()
jit_tan_sum(x)
end = time.perf_counter()
print(f"Elapsed (with compilation) = { (end - start) * 1e+6} us" )
