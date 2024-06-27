import socket
import time
import sys
from AresTsf import AresTsf

TCP_IP = 'localhost'
TCP_PORT = 15007
BUFFER_SIZE = 1024
gain_val = 0

tsf_obj = AresTsf(argv=sys.argv[1:])

DC = tsf_obj.get_symbol('DC1')
Audio = tsf_obj.get_symbol('TestAudio1')

# Open a TCP socket
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((TCP_IP, TCP_PORT))

print(DC.symbol_name)
print(Audio.symbol_name)

#while gain_val<11:
#    mystring = f'set_value,DC1.value,{gain_val}\n'.encode()
#    # Write the value of the coeff
#    s.send(mystring)
#    gain_val = gain_val + 1
#    data = s.recv(BUFFER_SIZE)
#    time.sleep(2)
#    print (data)

# Write harmonic order
s.send(f',write_float_array,TestAudio1.coeff[0],1.0\n'.encode())
print("Write coeff data")
data = s.recv(BUFFER_SIZE)
print(data.decode())

gain_val = 10
while ((gain_val >= 0) & (gain_val <= 11)):
    mystring = f'set_value,TestAudio1.gain,{gain_val}\n'.encode()
    # Write the value of the coeff
    s.send(mystring)
    gain_val = gain_val - 1
    data = s.recv(BUFFER_SIZE)
    time.sleep(2)
    print (data)

# Clsoe a TCP coonection
s.close()
