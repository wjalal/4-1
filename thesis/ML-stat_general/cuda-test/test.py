from torch import cuda
assert cuda.is_available()
assert cuda.device_count() > 0
print(cuda.get_device_name(cuda.current_device()))

from numba import jit, cuda
import numpy as np
# to measure exec time
from timeit import default_timer as timer   
 
# normal function to run on cpu
def func(a):                                
    for i in range(300000000):
        a = (a + i) % 476573     
    return a
 
# function optimized to run on gpu 
@jit(target_backend='cuda')                         
def func2(a):
    for i in range(300000000):
        a = (a + i) % 476573
    return a


if __name__=="__main__":
   
    a = 0
    start = timer()
    b = func2(a)
    print("with GPU:", timer()-start, b)

    a = 0
    start = timer()
    b = func(a)
    print("without GPU:", timer()-start, b)    
     
