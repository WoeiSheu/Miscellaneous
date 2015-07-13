import wave
import matplotlib.pyplot as plt
import numpy as np
import math
import disposeWav
import addInfoToWav
import extractInfoFromWav
import MCLT

nchannels, sampwidth, framerate, nframes, wave_data, time = disposeWav.read_wave_data("test1.wav")
xfp, freqs, phase = disposeWav.wave_fft(time, wave_data, framerate)
strInfo = "testMe,OK"
bytes = addInfoToWav.getInfoOfBytes(strInfo)
