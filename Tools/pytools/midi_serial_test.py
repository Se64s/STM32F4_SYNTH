import serial

midi_test_cmd0 = b'\x90\x10\x10'
midi_test_cmd1 = b'\x80\x10\x10'
midi_cmd = {
    midi_test_cmd0,
    midi_test_cmd1
}

ser = serial.Serial("COM3", 31250)

for a in range(10000):
    for cmd in midi_cmd:
        ser.write(cmd)

ser.close()
