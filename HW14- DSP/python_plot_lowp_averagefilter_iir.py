import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

# IIR filter
def iir_filter(data, A, B):
    filtered_data = np.zeros_like(data)
    filtered_data[0] = data[0]
    for i in range(1, len(data)):
        filtered_data[i] = A * filtered_data[i-1] + B * data[i]
    return filtered_data

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
def plot_signals(time, original, filtered, og_frq, og_fft, fil_frq, fil_fft, A, B, signal_name):
    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(12, 8))
    
    ax1.plot(time, original, 'k', label='Original')
    ax1.plot(time, filtered, 'r', label='Filtered')
    ax1.set_xlabel('Time')
    ax1.set_ylabel('Amplitude')
    ax1.set_title(f'{signal_name} with IIR Filter (A={A}, B={B})')
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

# Define A and B values
AB_values = [(0.9, 0.1), (0.7, 0.3), (0.5, 0.5)]  # Different A and B values to try

for name, df in signals.items():
    time = df['time'].values
    original = df['value'].values
    sample_rate = calculate_sample_rate(time)
    
    for A, B in AB_values:
        filtered = iir_filter(original, A, B)
        
        og_frq, og_fft = fft(original, sample_rate)
        fil_frq, fil_fft = fft(filtered, sample_rate)
        
        plot_signals(time, original, filtered, og_frq, og_fft, fil_frq, fil_fft, A, B, name)
