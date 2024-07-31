import socket
import time
import sys
from AresTsf import AresTsf

TCP_IP = 'localhost'
TCP_PORT = 15007
BUFFER_SIZE = 1024
gain_val = 0

tsf_obj = AresTsf(argv=sys.argv[1:])
TestAudio = tsf_obj.get_symbol('TestAudio1')
   
# Open a TCP socket
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((TCP_IP, TCP_PORT))
coreId = 16 * tsf_obj.get_core_id('TestAudio1')

print("Connected")
print("Core ID = %s" % coreId)

#cmd = str(coreId) + ',' + f'add_symbol_id,TestAudio1,{TestAudio.class_name},{TestAudio.object_id}\n'
#s.send(cmd.encode())

# Get version
#cmd = str(coreId) + ',' + 'get_value,TestAudio1.version\n'
#s.send(cmd.encode())
#data = s.recv(BUFFER_SIZE)
#print(data)
#if 'success' in data.decode():
#   v = int(data.decode().split(",")[3])
#   print(f"Version: {v:08x}\n")

cmd = f'set_value,DC1.value,1\n'
s.send(cmd.encode())
data = s.recv(BUFFER_SIZE)
print(data)

time.sleep(5)

cmd = f'write_int_array,TestAudio1.CONTROL_REQUEST[0], 0, 24, 268435460, 3000, 240, 0, 30, 0\n'
s.send(cmd.encode())
data = s.recv(BUFFER_SIZE)
print(data)

time.sleep(5)

cmd = f'write_int_array,TestAudio1.CONTROL_REQUEST[0], 0, 24, 268435460, 3000, 0, 0, 30, 0\n'
s.send(cmd.encode())
data = s.recv(BUFFER_SIZE)
print(data)

time.sleep(5)

s.send(b'set_value,TestAudio1.coeff[0], 0.1\n')
data = s.recv(BUFFER_SIZE)
print (data)

s.send(b'set_value,TestAudio1.gain, 0\n')
data = s.recv(BUFFER_SIZE)
print (data)

cmd = f'set_value,DC1.value,0\n'
s.send(cmd.encode())
data = s.recv(BUFFER_SIZE)
print(data)

time.sleep(5)

cmd = f'write_int_array,TestAudio1.CONTROL_REQUEST[0], 0, 24, 268435460, 3000, 240, 0, 30, 0\n'
s.send(cmd.encode())
data = s.recv(BUFFER_SIZE)
print(data)

time.sleep(5)

cmd = f'set_value,DC1.value,1\n'
s.send(cmd.encode())
data = s.recv(BUFFER_SIZE)
print(data)

# Clsoe a TCP coonection
s.close()
