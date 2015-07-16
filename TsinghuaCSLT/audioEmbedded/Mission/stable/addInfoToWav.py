#!usr/bin/env python
#coding=utf-8

import wave
import numpy as np
import scipy.signal as signal
import matplotlib.pyplot as plt
import sys
import math
import disposeWav
import MCLT

sys.setrecursionlimit(1000000)  #手工设置递归调用深度

def getInfoOfBytes(strInfo):    #将字符串转为ascii码的二进制形式表示
    ascii = map(ord,strInfo)
    bytes = ''
    for byte in ascii:
        suffix_zero = 8-len(bin(byte))+2    #前导0个数
        bytes += (suffix_zero*'0' + bin(byte)[2:])  #增加前导0转为8位二进制

    # bytes = struct.pack('%ds'%len(strInfo),strInfo)
    return bytes

def setInfoWithLSB(audio,bytes):    #以LSB的方法将数据嵌入音频中,双通道对称嵌入相同信息
    synchronization = "00111100001111000011110000111100"
    bytes = synchronization+bytes
    
    for i in range(len(bytes)):
        if (audio[0][i]%2 == 0 and int(bytes[i]) == 1) or (audio[0][i]%2 == 1 and int(bytes[i]) == 0):
            audio[0][i] += 1
        if (audio[1][i]%2 == 0 and int(bytes[i]) == 1) or (audio[1][i]%2 == 1 and int(bytes[i]) == 0):
            audio[1][i] += 1
    
    return audio

def setInfoWithMCLT(audio,bytes):         #以MCLT的方法将数据嵌入音频中
    """
    Use this function, you can set information whose format is string into audio data.
    Args:
        audio: A list of 2*N. it is the carrier that carry the information.
        bytes: A string that store informatin that you want to set.
    Return:
        return a list that is same shape with audio, but this list has been set into information.
    """
    
    BlockLen = 4096
    
    B = len(audio[0]) / BlockLen          #将左通道的数据通过MCLT变换到复数域
    #######################################
    #以下为嵌入信息的方法
    synchronization = "00111100001111000011110000111100"
    bytes = synchronization + bytes
    L = 4                                 #一位扩展为L个频率
    s = [-1,1,-1,1]                       #1对应4位编码
    for i in range(B):
        if i % 2 == 1:
            continue
        X = MCLT.FastMCLT(audio[0][i*BlockLen:(i+1)*BlockLen])
        for k in range( len(bytes) ):
            for m in range(L):
                if bytes[k] == '1':
                    X[2*k*L+m] = abs(X[2*k*L+m])*s[m]       #2*k --> Every Other Frequency
                else:
                    X[2*k*L+m] = -abs(X[2*k*L+m])*s[m]
    #######################################
        y = MCLT.FastIMCLT(X)          #将嵌入信息后的复数域信息反变换到实数域
        audio[0][i*BlockLen:(i+1)*BlockLen] = y
    
    
    B = len(audio[1]) / BlockLen       #将右通道的数据通过MCLT变换到复数域
    #######################################
    #以下为嵌入信息的方法
    synchronization = "00111100001111000011110000111100"
    bytes = synchronization + bytes
    L = 4                                 #一位扩展为L个频率
    s = [-1,1,-1,1]                       #1对应4位编码
    
    for i in range(B):
        if i % 2 == 1:
            continue
        X = MCLT.FastMCLT(audio[1][i*BlockLen:(i+1)*BlockLen])
        for k in range( len(bytes) ):
            for m in range(L):
                if bytes[k] == '1':
                    X[2*k*L+m] = abs(X[2*k*L+m])*s[m]       #2*k --> Every Other Frequency
                else:
                    X[2*k*L+m] = -abs(X[2*k*L+m])*s[m]
        #######################################
        y = MCLT.FastIMCLT(X)             #将嵌入信息后的复数域信息反变换到实数域
        audio[1][i*BlockLen:(i+1)*BlockLen] = y
    
    return audio
    
def setInfoWithFFT(audio, bytes):
    BlockLen = 4096
    
    B = len(audio[0]) / BlockLen
    synchronization = "00111100001111000011110000111100"
    bytes = synchronization + bytes
    
    for i in range(B):
        FL = np.fft.rfft( audio[0][i*BlockLen:(i+1)*BlockLen] )
        FR = np.fft.rfft( audio[1][i*BlockLen:(i+1)*BlockLen] )
        for k in range( len(bytes) ):
            if bytes[k] == '1':
                FL[100+k] = abs(FL[100+k])
                FR[100+k] = abs(FR[100+k])
            else:
                FL[100+k] = -abs(FL[100+k])
                FR[100+k] = -abs(FR[100+k])
    
        outputLeft  = np.fft.irfft(FL)
        outputRight = np.fft.irfft(FR)
        audio[0][i*BlockLen:(i+1)*BlockLen] = outputLeft
        audio[1][i*BlockLen:(i+1)*BlockLen] = outputRight
    
    return audio

def test():
    strInfo = "NanaliCCC"
    bytes = getInfoOfBytes(strInfo)
    #print bytes
    
    nchannels, sampwidth, framerate, nframes, wave_data, time = disposeWav.read_wave_data("../wavFile/bird.wav")
    wave_data = setInfoWithLSB(wave_data, bytes)
    params = (nchannels, sampwidth, framerate, nframes,'NONE', 'not compressed')
    disposeWav.write_wave("../wavFile/result1.wav",params,wave_data)
    
    nchannels, sampwidth, framerate, nframes, wave_data, time = disposeWav.read_wave_data("../wavFile/bird.wav")
    wave_data = setInfoWithMCLT(wave_data,bytes)
    params = (nchannels, sampwidth, framerate, nframes,'NONE', 'not compressed')
    disposeWav.write_wave("../wavFile/result2.wav",params,wave_data)

    nchannels, sampwidth, framerate, nframes, wave_data, time = disposeWav.read_wave_data("../wavFile/bird.wav")
    wave_data = setInfoWithFFT(wave_data,bytes)
    params = (nchannels, sampwidth, framerate, nframes,'NONE', 'not compressed')
    disposeWav.write_wave("../wavFile/result3.wav",params,wave_data)


if __name__ == "__main__":
    #print setInfoWithMCLT.__doc__
    test()
