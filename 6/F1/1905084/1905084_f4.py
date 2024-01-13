import random
from sympy import isprime
from ecdh import *
import time 

# a = 2
# b = 2
# p = 17
# print (scal_mul (17, 19, (5,1)))


print ("k", "comp. time for A", "comp. time for B", "comp. time for R (shared)", sep='\t')

for k in [128, 192, 256]:

    t_A, t_B, t_R = 0, 0, 0

    for i in range(5): 
        p, G, a, b = generate_pub (k)

        k_a = random.getrandbits(k) % p
        k_b = random.getrandbits(k) % p

        t_A -= time.time()
        A = scal_mul (p, a, k_a, G)
        t_A += time.time()
        t_B -= time.time()
        B = scal_mul (p, a, k_b, G)
        t_B += time.time()
        
        t_R -= time.time()
        R = scal_mul(p, a, k_a, B)
        R = scal_mul(p, a, k_b, A)
        R = scal_mul(p, a, k_a*k_b, G)
        t_R += time.time()

    print (k, 1000*t_A/5, 1000*t_B/5, 1000*t_R/15, sep='\t')