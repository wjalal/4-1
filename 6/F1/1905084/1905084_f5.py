import socket             
from aes128 import *
from ecdh import *
import pickle

s = socket.socket()         
print ("Socket successfully created on Alice's end")
 
port = 12345               
s.bind(('', port))         
print ("socket binded to %s" %(port)) 
 
s.listen(55)     
print ("Alice is listening")            
 
while True: 
 
  c, addr = s.accept()     
  print ('Got connection from', addr, "(Bob)")
  print ('Generating EC parameters')
  p, G, a, b = generate_pub(128)
  print ('Generating A')
  k_a = random.getrandbits(128) % p
  A = scal_mul (p, a, k_a, G)
  print ('Sending EC parameters, and A')
  c.sendall(pickle.dumps([p, G, a, b, A])) 
  print ('Sent EC parameters and A')
  B = pickle.loads(c.recv(102400))
  print ('B received from Bob: ', B, sep=' ')
  print ("Calculating R")
  R = scal_mul(p, a, k_a, B)
  print(R)
  x,y = R
  key = [byte for byte in x.to_bytes(16)]
  print ("Shared secret key:  ", " ".join([format(byte, '02x') for byte in key]))
  
  print("Write a message for bob: ")
  msg_str = input()
  ptext = [ord(char) for char in msg_str]
  if (len(ptext) % 16):
    for i in range (16 - len(ptext)%16):      
        ptext.append(0)
  iv = [random.randint(0, 0xFF) for i in range(16)]

  ctext = aes_cbc_encrypt(ptext, iv, key)
  print ("Seding this to Bob: ", "".join([chr(x) for x in ctext]))
  c.sendall(pickle.dumps(ctext)) 

  c.close()
   
  break