import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

# Moving average filter
def average(data, X):
    return np.convolve(data, np.ones(X)/X, mode='same')

# FFT computation
def fft(signal, sample_rate):
    n = len(signal)
    k = np.arange(n)
    T = n / sample_rate
    frq = k / T
    frq = frq[range(int(n / 2))]
    Y = np.fft.fft(signal) / n
    Y = Y[range(int(n / 2))]
    return frq, abs(Y)

# Plotting function
def plot_signals(time, original, filtered, og_frq, og_fft, fil_frq, fil_fft, X, signal_name):
    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(12, 8))
    
    ax1.plot(time, original, 'k', label='Original')
    ax1.plot(time, filtered, 'r', label='Filtered')
    ax1.set_xlabel('Time')
    ax1.set_ylabel('Amplitude')
    ax1.set_title(f'{signal_name} with Moving Average Filter with X = {X}')
    ax1.legend()
    
    ax2.plot(og_frq, og_fft, 'k', label='Original FFT')
    ax2.plot(fil_frq, fil_fft, 'r', label='Filtered FFT')
    ax2.set_xlabel('Frequency')
    ax2.set_ylabel('Magnitude')
    ax2.set_title('FFT of Signal')
    ax2.legend()
    
    plt.tight_layout()
    plt.show()

# Calculate sample rate
def calculate_sample_rate(time_data):
    total_time = time_data[-1] - time_data[0]
    total_samples = len(time_data)
    return total_samples / total_time

# File paths and reading CSV files
file_paths = {
    'sigA': '/Users/markchauhan/Desktop/Pico/HW14/sigA.csv',
    'sigB': '/Users/markchauhan/Desktop/Pico/HW14/sigB.csv',
    'sigC': '/Users/markchauhan/Desktop/Pico/HW14/sigC.csv',
    'sigD': '/Users/markchauhan/Desktop/Pico/HW14/sigD.csv'
}

signals = {name: pd.read_csv(path, header=None, names=['time', 'value']) for name, path in file_paths.items()}

X_values = [10, 50, 100]  # Different X values to try

for name, df in signals.items():
    time = df['time'].values
    original = df['value'].values
    sample_rate = calculate_sample_rate(time)
    
    for X in X_values:
        filtered = average(original, X)
        
        og_frq, og_fft = fft(original, sample_rate)
        fil_frq, fil_fft = fft(filtered, sample_rate)
        
        plot_signals(time, original, filtered, og_frq, og_fft, fil_frq, fil_fft, X, name)
