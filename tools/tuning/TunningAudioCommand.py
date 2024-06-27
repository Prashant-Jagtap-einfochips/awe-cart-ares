import socket
import time

TCP_IP = 'localhost'
TCP_PORT = 15007
BUFFER_SIZE = 1024
gain_val = 0

# Open a TCP socket
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((TCP_IP, TCP_PORT))

# Write the value of the coeff
s.send(b'set_value,Control1.coeff[2], 0.25\n')
data = s.recv(BUFFER_SIZE)
print (data)

# Write the value of the coeff
s.send(b'set_value,TestAudio1.coeff[2], 0\n')
data = s.recv(BUFFER_SIZE)
print (data)

# Write the value of the gain
while gain_val<11:
    mystring = f'set_value,TestAudio1.gain, {gain_val}\n'.encode()
    gain_val = gain_val + 1
    s.send(mystring)
    time.sleep(1)
    data = s.recv(BUFFER_SIZE)
    print (data)

# Write the value of the coeff
s.send(b'set_value,TestAudio1.gain, 1\n')
data = s.recv(BUFFER_SIZE)
print (data)

# Clsoe a TCP coonection
s.close()
