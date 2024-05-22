import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

# Custom FIR filter weights generation
def fir_lowpass_weights(num_taps, cutoff, window='hamming'):
    n = np.arange(num_taps)
    h = np.sinc(2 * cutoff * (n - (num_taps - 1) / 2))
    window = np.hamming(num_taps)
    h = h * window
    h = h / np.sum(h)
    return h

# FIR filter application
def fir_filter(data, weights):
    return np.convolve(data, weights, mode='same')

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
def plot_signals(time, original, filtered, og_frq, og_fft, fil_frq, fil_fft, filter_info, signal_name):
    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(12, 8))
    
    ax1.plot(time, original, 'k', label='Original')
    ax1.plot(time, filtered, 'r', label='Filtered')
    ax1.set_xlabel('Time')
    ax1.set_ylabel('Amplitude')
    ax1.set_title(f'{signal_name} with FIR Filter ({filter_info})')
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

# FIR filter parameters
cutoff_frequencies = [0.1, 0.2, 0.3]  # Example cutoff frequencies (normalized 0-0.5)
num_taps = 101  # Number of filter coefficients (taps)
window_types = ['hamming', 'hann', 'blackman']  # Different window types

for name, df in signals.items():
    time = df['time'].values
    original = df['value'].values
    sample_rate = calculate_sample_rate(time)
    
    for cutoff in cutoff_frequencies:
        for window in window_types:
            weights = fir_lowpass_weights(num_taps, cutoff, window=window)
            filtered = fir_filter(original, weights)
            
            og_frq, og_fft = fft(original, sample_rate)
            fil_frq, fil_fft = fft(filtered, sample_rate)
            
            filter_info = f'cutoff={cutoff}, window={window}, taps={num_taps}'
            plot_signals(time, original, filtered, og_frq, og_fft, fil_frq, fil_fft, filter_info, name)
