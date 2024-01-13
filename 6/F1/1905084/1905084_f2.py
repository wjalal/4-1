import numpy as np
import random
from BitVector import *
import time
from aes128 import *

print("Key:")
print("In ASCII:", end=' ')
# key_str = input()
key_str = "BUET CSE19 Batch"
key = [ord(char) for char in key_str]
if (len(key) % 16):
    for i in range (16 - len(key)%16):      
        key.append(0)
print("In Hex:", " ".join([format(byte, '02x') for byte in key]), sep=' ')

print("\nPlain Text:")
print("In ASCII:", end=' ')
# ptext_str = input()
ptext_str = "Never Gonna Give you up"
ptext = [ord(char) for char in ptext_str]
if (len(ptext) % 16):
    for i in range (16 - len(ptext)%16):      
        ptext.append(0)
print("In Hex:", " ".join([format(byte, '02x') for byte in ptext]), sep=' ')

iv = [random.randint(0, 0xFF) for i in range(16)]



enc_st = time.time()
ctext = aes_cbc_encrypt(ptext, iv, key)
enc_et = time.time()
# print ("".join([chr(x) for x in ctext]))

# np.set_printoptions(formatter={'int':hex})

#-------------
# ctext = aes_cbc_encrypt(ptext)
print ("\nCiphered Text:")
print ("In Hex: ", " ".join([format(byte, '02x') for byte in ctext]), sep=' ')
print ("In ASCII: ", "".join([chr(x) for x in ctext]), sep=' ')

dec_st = time.time()
dtext = aes_cbc_decrypt(ctext, key)
dec_et = time.time()
print ("\nDeciphered Text:")
print ("In Hex: ", " ".join([format(byte, '02x') for byte in dtext]), sep=' ')
print ("In ASCII: ", "".join([chr(x) for x in dtext]), sep=' ')

#-------------

print ("\nExecution Time Details:")
print("Key Schedule Time: ", 1000*(key_et-key_st), " ms")
print("Encryption Time: ", 1000*(enc_et-enc_st), " ms")
print("Decryption Time: ", 1000*(dec_et-dec_st), " ms")


# state_mat = shift_rows(sub_bytes(add_rkey(ptext, 0)))
# print(mix_cols(state_mat))
# aes_cbc_encrypt(ptext, [0x1e, 0x2, 0x9d, 0xd6, 0xdf, 0xb5, 0x1d, 0x5a, 0x7a, 0xaa, 0x3, 0x43, 0x7d, 0x42, 0x92, 0xb5])