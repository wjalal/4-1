import socket             
import pickle
from aes128 import *
from ecdh import *

s = socket.socket()         
port = 12345               
s.connect(('127.0.0.1', port)) 

# receive data from the server and decoding to get the string.
p, G, a, b, A = pickle.loads(s.recv(102400))
print ("Received EC parameters (p, G, a, b) and A from Alice:", p, G, a, b, A, sep='\n')
print ("Calculating B")
k_b = random.getrandbits(128) % p
B = scal_mul (p, a, k_b, G)
print ("Sending B")
s.sendall(pickle.dumps(B))
print ("Sent B")
print ("Calculating R")
R = scal_mul(p, a, k_b, A)
print(R)
x,y = R
key = [byte for byte in x.to_bytes(16)]
print ("Shared secret key:  ", " ".join([format(byte, '02x') for byte in key]))
ctext = pickle.loads(s.recv(102400))
print ("Received ciphertext from Alice: ", "".join([chr(x) for x in ctext]), "\nDecrypting...")
dtext = aes_cbc_decrypt(ctext, key)
print ("Alice says: ", "".join([chr(x) for x in dtext]))
# close the connection 
s.close()   