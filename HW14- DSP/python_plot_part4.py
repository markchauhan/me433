import matplotlib.pyplot as plt # for plotting
import numpy as np # for sine function
import pandas as pd

##dt = 1.0/100.0 # 100Hz
##t = np.arange(0.0, 5.0, dt) # for 5s
##
##s = 2.0 * np.sin(2 * np.pi * 2.3 * t) + 2.5 # 2.3Hz
##
##plt.plot(t,s,'b-*')
##plt.xlabel('Time [s]')
##plt.ylabel('Signal')
##plt.title('Signal vs Time')
##plt.show()
column = ['time', 'value']
sigA = pd.read_csv('/Users/markchauhan/Desktop/Pico/HW14/sigA.csv', names=column)
sigB = pd.read_csv('/Users/markchauhan/Desktop/Pico/HW14/sigB.csv', names=column)
sigC = pd.read_csv('/Users/markchauhan/Desktop/Pico/HW14/sigC.csv', names=column)
sigD = pd.read_csv('/Users/markchauhan/Desktop/Pico/HW14/sigD.csv', names=column)

def fft(signal, sample_rate, title):
    t = signal['time']
    y = signal['value']
    n = len(y)
    k = np.arange(n)
    T = n / sample_rate
    frq = k/T
    frq = frq[range(int(n / 2))]
    Y = np.fft.fft(y)
    Y = Y[range(int(n /2))]

    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10,8))
    ax1.plot(t,y,'b')
    ax1.set_xlabel('Time [s]')
    ax1.set_ylabel('Amplitude')
    ax1.set_title(f'Signal vs. Time: {title}')
    ax2.plot(frq,abs(Y),'b') # plotting the fft
    ax2.set_xlabel('Freq (Hz)')
    ax2.set_ylabel('|Y(freq)|')
    ax2.set_title(f'FFT {title}')
    plt.tight_layout
    plt.show()
    


def find_sample_rate(d):
    total_data = len(d)
    total_time = d['time'].iloc[-1] - d['time'].iloc[0]
    sample_rate = total_data / total_time
    return sample_rate

sample_rateA = find_sample_rate(sigA)
sample_rateB = find_sample_rate(sigB)
sample_rateC = find_sample_rate(sigC)
sample_rateD = find_sample_rate(sigD)

fft(sigA, sample_rateA, 'Signal A')
fft(sigB, sample_rateB, 'Signal B')
fft(sigC, sample_rateC, 'Signal C')
fft(sigD, sample_rateD, 'Signal D')
