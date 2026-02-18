# beats_simple.py
# Usage: python beats_simple.py path/to/track.wav

import sys
from pathlib import Path
import numpy as np
import pandas as pd
from scipy import signal
from scipy.signal import find_peaks

def read_wav_mono(path, target_sr=None):
    # Читает WAV и возвращает (y, sr) как float в диапазоне [-1,1]
    path = Path(path)
    if not path.exists():
        raise FileNotFoundError(f"{path} not found")
    else:
        from scipy.io import wavfile
        sr_int, data = wavfile.read(str(path))
        sr = int(sr_int)
        if data.dtype.kind == 'i':
            maxv = float(2**(8*data.dtype.itemsize - 1))
            y = data.astype('float32') / maxv
        else:
            y = data.astype('float32')
    if y.ndim > 1:
        y = np.mean(y, axis=1)
    if target_sr is not None and sr != target_sr:
        num = int(len(y) * float(target_sr) / sr)
        y = signal.resample(y, num)
        sr = target_sr
    if np.max(np.abs(y)) > 0:
        y = y / np.max(np.abs(y))
    return y.astype('float32'), int(sr)

def detect_beats_energy(y, sr, frame_ms=30, hop_ms=10,
                        smooth_win=5, threshold_factor=2.0, min_distance=0.5):
    frame_len = max(1, int(sr * frame_ms / 1000))
    hop = max(1, int(sr * hop_ms / 1000))
    n_frames = 1 + (len(y) - frame_len) // hop if len(y) >= frame_len else 0
    if n_frames <= 0:
        return np.array([]), None, None
    shape = (n_frames, frame_len)
    strides = (y.strides[0]*hop, y.strides[0])
    frames = np.lib.stride_tricks.as_strided(y, shape=shape, strides=strides).copy()
    energy = np.sum(frames**2, axis=1)
    if smooth_win > 1:
        energy_smooth = np.convolve(energy, np.ones(smooth_win)/smooth_win, mode='same')
    else:
        energy_smooth = energy
    med = np.median(energy_smooth) + 1e-12
    thr = med * threshold_factor
    min_dist_frames = max(1, int(min_distance * sr / hop))
    peaks, _ = find_peaks(energy_smooth, height=thr, distance=min_dist_frames)
    times = (peaks * hop + frame_len/2.0) / sr
    times_all = (np.arange(len(energy_smooth))*hop + frame_len/2.0) / sr
    return np.array(times), energy_smooth, times_all

def prune_peaks(times, values=None, min_interval=0.3):
    if times is None or len(times) == 0:
        return np.array([])
    keep = [0]
    for i in range(1, len(times)):
        if times[i] - times[keep[-1]] >= min_interval:
            keep.append(i)
        elif values is not None:
            if values is not None and values.size > i and values.size > keep[-1]:
                if values[i] > values[keep[-1]]:
                    keep[-1] = i
    return np.array(times[keep])

def run(wav_path):
    y, sr = read_wav_mono(wav_path)
    print(f"Loaded {wav_path}, sr={sr}, duration={len(y)/sr:.2f}s")

    # параметры как в ноутбуке
    times_e, energy_smooth, energy_times = detect_beats_energy(
        y, sr, frame_ms=30, hop_ms=10, smooth_win=3, threshold_factor=1.6, min_distance=0.28
    )

    times_e_prune = prune_peaks(times_e)


    pd.DataFrame({"timestamp_s": np.round(times_e_prune, 4)}).to_csv(r"C:\Users\HP\Documents\Unreal Projects\it-clinic-gamedev\MusicGame\Content\Python\beats_energy.csv", index=False)
    print("Saved beats_energy.csv")