import socket
import time
import sys
from AresTsf import AresTsf

TCP_IP = 'localhost'
TCP_PORT = 15007
BUFFER_SIZE = 1024
gain_val = 0

tsf_obj = AresTsf(argv=sys.argv[1:])

dc = tsf_obj.get_symbol('DC1')
Source = tsf_obj.get_symbol('Source1')
Audio = tsf_obj.get_symbol('TestAudio1')

# Open a TCP socket
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((TCP_IP, TCP_PORT))

print(dc.symbol_name)
print(Source.symbol_name)
print(Audio.symbol_name)

# Write the inital value of the gain
mystring = f'set_value,DC1.value,0\n'.encode()
s.send(mystring)
data = s.recv(BUFFER_SIZE)

idx = 0
value = 1;

while idx < 10:
    mystring = f'write_float_array,Source1.value[0],{value},0,1,0,1,0,1,0,1,0,1,0\n'.encode()
    # Write the value of the coeff
    idx = idx + 1
    s.send(mystring)
    data = s.recv(BUFFER_SIZE)
    time.sleep(2)
    if value == 1:
        value = 0
    else:
        value = 1
    print (data)

myGain = 0

while myGain < 10:
    mystring = f'set_value,DC1.value,{myGain}\n'.encode()
    # Write the value of the gain
    myGain = myGain + 1
    s.send(mystring)
    data = s.recv(BUFFER_SIZE)
    time.sleep(2)
    print (data)

# Clsoe a TCP coonection
s.close()
