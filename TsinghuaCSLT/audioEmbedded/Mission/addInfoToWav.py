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
    #B = len(audio[0]) / 4096              #将左通道的数据通过MCLT变换到复数域
    #X = []
    #for i in range(B):
    X = MCLT.FastMCLT(audio[0])
    #######################################
    #以下为嵌入信息的方法
    synchronization = []
    L = 4                                 #一位扩展为L个频率
    s = [-1,1,-1,1]                       #1对应4位编码
    for k in range( len(bytes) ):
        for m in range(L):
            if bytes[k] == '1':
                X[k*L+m] = abs(X[k*L+m])*s[m]
            else:
                X[k*L+m] = -abs(X[k*L+m])*s[m]
    #######################################
    y = MCLT.FastIMCLT(X)                 #将嵌入信息后的复数域信息反变换到实数域
    
    endLoc = len(y) - len(audio[0])
    audio[0][:endLoc] = y
    
    
    X = MCLT.FastMCLT(audio[1])           #将右通道的数据通过MCLT变换到复数域
    #######################################
    #以下为嵌入信息的方法
    for k in range( len(bytes) ):
        for m in range(L):
            if bytes[k] == '1':
                X[k*L+m] = abs(X[k*L+m])*s[m]
            else:
                X[k*L+m] = -abs(X[k*L+m])*s[m]
    #######################################
    y = MCLT.FastIMCLT(X)                 #将嵌入信息后的复数域信息反变换到实数域
    
    endLoc = len(y) - len(audio[1])
    audio[1][:endLoc] = y
    
    return audio
    
def test():
    strInfo = "testMe,OK"
    bytes = getInfoOfBytes(strInfo)
    #print bytes
    
    nchannels, sampwidth, framerate, nframes, wave_data, time = disposeWav.read_wave_data("ambulancehorn.wav")
    #print len(wave_data[0])
    
    wave_data = setInfoWithLSB(wave_data, bytes)
    params = (nchannels, sampwidth, framerate, nframes,'NONE', 'not compressed')
    disposeWav.write_wave("result1.wav",params,wave_data)
    
    wave_data = setInfoWithMCLT(wave_data,bytes)
    params = (nchannels, sampwidth, framerate, nframes,'NONE', 'not compressed')
    disposeWav.write_wave("result2.wav",params,wave_data)

if __name__ == "__main__":
    #print setInfoWithMCLT.__doc__
    test()
