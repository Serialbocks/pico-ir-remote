import serial
import signal

ser = serial.Serial('COM5', 115200, timeout=1)
signal.signal(signal.SIGINT, signal.default_int_handler)

values = bytearray([0x0E])
values.extend((38000).to_bytes(4, byteorder='big'))
values.extend((5).to_bytes(4, byteorder='big'))
values.extend((9378).to_bytes(4, byteorder='big'))
values.extend((4513).to_bytes(4, byteorder='big'))
values.extend((585).to_bytes(4, byteorder='big'))
values.extend((573).to_bytes(4, byteorder='big'))
values.extend((584).to_bytes(4, byteorder='big'))

ser.write(values)

try:
    while True:
        line = ser.readline().decode('utf-8')
        if line:
            print(line[:-1])
except KeyboardInterrupt:
    ser.close()