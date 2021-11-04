
#!/usr/bin/python
import math
import matplotlib.pyplot as plt

f = open('wave_out.c','w')

dac_data_wide = 24
dac_max_range = 2**dac_data_wide - 1
signal_amplitude = 1.0
num_steps = 256

# Saw waveform
x=[]
y=[]
signal_amplitude = 2.0
step_val = signal_amplitude/num_steps
data_val = -1.0
f.write( "const float saw_wave[%d] = {" % (num_steps) )
for step in range(num_steps):
    data_val = data_val + step_val
    f.write( "%fF," % data_val )
    x.append(step)
    y.append(data_val)
f.write( "};\n" )
fig_saw = plt.plot(x, y, label='Saw')

# Reverse saw waveform
x=[]
y=[]
signal_amplitude = 2.0
step_val = signal_amplitude/num_steps
data_val = 1.0
f.write( "const float rsaw_wave[%d] = {" % (num_steps) )
for step in range(num_steps):
    data_val = data_val - step_val
    f.write( "%fF" % data_val )
    if step < num_steps - 1:
        f.write( "," )
    x.append(step)
    y.append(data_val)
f.write( "};\n" )
fig_saw = plt.plot(x, y, label='Reverse saw')

# Square waveform
x=[]
y=[]
signal_amplitude = 1.0
step_val = signal_amplitude/num_steps
data_val = 0.0
f.write( "const float square_wave[%d] = {" % (num_steps) )
for step in range(num_steps):
    if step < ( num_steps / 2 ):
        data_val = signal_amplitude
    else:
        data_val = -1.0 * signal_amplitude
    f.write( "%fF" % data_val )
    if step < num_steps - 1:
        f.write( "," )
    x.append(step)
    y.append(data_val)
f.write( "};\n" )
fig_saw = plt.plot(x, y, label='Square')

# Sine waveform
x=[]
y=[]
signal_amplitude = 1.0
step_val = 2*math.pi/num_steps
f.write( "const float sin_wave[%d] = {" % (num_steps) )
for step in range(num_steps):
    data_val = signal_amplitude * math.sin(step_val*step)
    f.write( "%fF" % data_val )
    if step < num_steps - 1:
        f.write( "," )
    x.append(step)
    y.append(data_val)
f.write( "};\n" )
fig_sin = plt.plot(x, y, label='Sin')

# Triagle waveform
x=[]
y=[]
signal_amplitude = 4.0
delta_step = signal_amplitude / num_steps
data_val = -1.0
f.write( "const float tri_wave[%d] = {" % (num_steps) )
for step in range(num_steps):
    if step < ( num_steps / 2 ):
        data_val = data_val + delta_step
    else:
        data_val = data_val - delta_step
    f.write( "%fF" % data_val )
    if step < num_steps - 1:
        f.write( "," )
    x.append(step)
    y.append(data_val)
f.write( "};\n" )
fig_tri = plt.plot(x, y, label='Tri')

f.close()

plt.grid()
plt.show()
