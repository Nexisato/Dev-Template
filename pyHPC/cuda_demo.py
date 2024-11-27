from numba import cuda
import random
from loguru import logger


SIZE = 8

def cpu_compute(N=SIZE):
    for i in range(N):
        print(f"cpu-{i}")

@cuda.jit
def gpu_compute(N):
    idx = cuda.blockDim.x * cuda.blockIdx.x + cuda.threadIdx.x
    if (idx < N):
        print(idx)



if __name__ == '__main__':
    logger.info(f"{cuda.gpus}")
    print("CPU:")
    cpu_compute()
    
    print("GPU:")
    gpu_compute[2, 4](8)
    cuda.synchronize()

