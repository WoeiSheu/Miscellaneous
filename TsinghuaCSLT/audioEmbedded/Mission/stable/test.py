#!usr/bin/env python
#coding=utf-8

import wave
import matplotlib.pyplot as plt
import numpy as np
import math
import disposeWav
import addInfoToWav
import extractInfoFromWav
import MCLT

BlockLen = 4096
strInfo = "baidu.com"
bytes = addInfoToWav.getInfoOfBytes(strInfo)
infoLen = len(strInfo)

nchannels, sampwidth, framerate, nframes, wave_data, time = disposeWav.read_wave_data("../wavFile/duck.wav")
plt.subplot(211)
plt.plot(time, wave_data[0])

wave_data = addInfoToWav.setInfoWithMCLT(wave_data,bytes)
plt.subplot(212)
plt.plot(time, wave_data[0], c='g')

params = (nchannels, sampwidth, framerate, nframes,'NONE', 'not compressed')
disposeWav.write_wave("../wavFile/result2.wav",params,wave_data)

info = extractInfoFromWav.extractInfoWithMCLT(wave_data, BlockLen, infoLen)
print info

#The following test the record audios which have been added watermarking.
#加密音频录音后得到的音频
'''
nchannels, sampwidth, framerate, nframes, wave_data, time = disposeWav.read_wave_data("../wavFile/record1.wav")
plt.subplot(313)
plt.plot(time, wave_data[0], c='b')

info = extractInfoFromWav.extractInfoWithMCLT(wave_data, BlockLen, infoLen)
print info
'''
'''
nchannels, sampwidth, framerate, nframes, wave_data, time = disposeWav.read_wave_data("../wavFile/record2.wav")
info = extractInfoFromWav.extractInfoWithFFT(wave_data, BlockLen, infoLen)
print info
'''

plt.show()
