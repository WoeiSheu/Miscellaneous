#!usr/bin/env python
#coding=utf-8

import wave  
import numpy as np  
import scipy.signal as signal  
import matplotlib.pyplot as plt  
import sys
import disposeWav

sys.setrecursionlimit(1000000)  #手工设置递归调用深度

def getInfoOfBytes(strInfo):    #将字符串转为ascii码的二进制形式表示
    ascii = map(ord,strInfo)
    bytes = ''
    for i in ascii:
        bytes += ('0' + bin(i)[2:])
    #bytes = struct.pack('%ds'%len(strInfo),strInfo)
    return bytes

def setInfoWithLSB(audio,bytes):    #以LSB的方法将数据嵌入音频中,双通道对称嵌入
    for i in range(len(bytes)):
        if (audio[0][i]%2 == 0 and int(bytes[i]) == 1) or (audio[0][i]%2 == 1 and int(bytes[i]) == 0):
            audio[0][i] += 1
        if (audio[1][i]%2 == 0 and int(bytes[i]) == 1) or (audio[1][i]%2 == 1 and int(bytes[i]) == 0):
            audio[1][i] += 1
    
    return audio

def setInfoWithMCLT(audio):         #以MCLT的方法将数据嵌入音频中
    
    return audio
    
def test():
    strInfo = "testMe,OK"
    bytes = getInfoOfBytes(strInfo)
    print bytes
    
    nchannels, sampwidth, framerate, nframes, wave_data, time = disposeWav.read_wave_data("test1.wav")
    
    wave_data = setInfoWithLSB(wave_data, bytes)
    params = (nchannels, sampwidth, framerate, nframes,'NONE', 'not compressed')
    disposeWav.write_wave("result1.wav",params,wave_data)


if __name__ == "__main__":
    test()
