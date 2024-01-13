import random
from sympy import isprime

def generate_pub (length):
    while True:
        while True:
            r = random.getrandbits(length)
            r |= (1 << (length - 1)) | 1
            if isprime(r):
                p = r
                break
        a = random.getrandbits(length) % p
        G = (random.getrandbits(length) % p, random.getrandbits(length) % p)
        x,y = G
        b = (y**2 - x**3 - a*x) % p
        if (4*a**3 + 27*b**2) % p != 0:
            break
    return p, G, a, b

def add_mod (p, a, A, B):
    s = 0
    x1, y1 = A
    x2, y2 = B
    if x1 == x2 and y1 == y2:
        s = ((3*x1**2 + a) * pow(2*y1,-1,p)) % p
    else:
        s = ((y2-y1) * pow((x2-x1),-1,p)) % p
    x3 = (s**2 - x1 - x2) % p
    y3 = (s * (x1 - x3) - y1) % p
    return (x3, y3)

def scal_mul (p, a, d, P):
    T = P
    for i in reversed(range(0, d.bit_length()-1)):
        T = add_mod(p, a, T, T)
        if (d >> i) & 1 :
            T = add_mod(p, a, T, P)
    return T