import socket
import time

TCP_IP = 'localhost'
TCP_PORT = 15007
BUFFER_SIZE = 1024

# Open a TCP socket
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((TCP_IP, TCP_PORT))

# Write the value of the coeff
s.send(b'set_value,TestControl1.coeff[2]\n')
data = s.recv(BUFFER_SIZE)
print (data)

'''
# Attenuate channel 1
s.send(b'set_value,TestControl.Coeff,2\n')
data = s.recv(BUFFER_SIZE)
print (data)
'''
# Clsoe a TCP coonection
s.close()
