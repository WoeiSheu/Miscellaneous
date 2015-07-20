#!usr/bin/env python
#coding=utf-8

import wave  
import numpy as np  
import scipy.signal as signal  
import matplotlib.pyplot as plt  
import math
import disposeWav
import MCLT

def getInt(byte):   #得到8位二进制字符串对应的整数
    x = 0           #待修改,不明白为何少1
    count = 7
    for i in byte:
        x += int(i)*(2**count)
        count -= 1

    return x

def extractInfoWithLSB(audio, BlockLen, infoLen):  #提取以LSB嵌入的信息
    bytes = ''
    synchronization = "00111100001111000011110000111100"
    #length = 8*infoLen+len(synchronization)    #8位乘以n字节 #对于需要广播的音频无法使用
    length = BlockLen
    
    for i in range(length):
        if audio[0][i] % 2 == 0:
            bytes += '0'
        else:
            bytes += '1'
    
    while bytes.find(synchronization) == -1:
        synchronization = synchronization[0:len(synchronization)-8]
        if synchronization == "":
            break
    
    start = bytes.find(synchronization) + len(synchronization)
    bytes = bytes[start:]
    info = ''        
    for i in range( infoLen ):
        byte = bytes[i*8:i*8+8]
        info += chr( getInt(byte) )
    
    return info

def extractInfoWithMCLT(audio, BlockLen, infoLen):
    L = 6
    r = [-1,1,-1,1,-1,1]                  #扩展后的数组
    synchronization = "1111111100000000111111110000000011111111000000001111111100000000"
    #length = 8*infoLen + 8*len(synchronization)    #对于需要广播的音频无法使用
    length = BlockLen/2/L                           #Subbands 数量
    
    info = ''
    
    maxSameBits = 0
    Loc = {}
    for n in range( 4096 ):
        smax = 0
        X = MCLT.FastMCLT(audio[0][n+BlockLen/2:n+3*BlockLen/2])
        for s in range( 2*len(synchronization) ):
            if s % 2 == 0:                          #Every Other Frequency
                continue
            for m in range(L):
                if synchronization[s/2] == '1':
                    smax += (X[s*L+m]/abs(X[s*L+m]) * r[m])
                else:
                    smax += -(X[s*L+m]/abs(X[s*L+m]) * r[m])
        if maxSameBits < smax.real:
            maxSameBits = smax.real
            Loc[maxSameBits] = n
        
    offset = Loc[maxSameBits]
    print maxSameBits,offset
    segment = audio[0][offset:]
    
    B = len(segment)*2 / BlockLen - 1
    for i in range(B-1):                            #range(B) or range(B-1) is all right, because you cannot get to the last block
        if i % 2 == 0:                              #Every Other Block
            continue
        bytes = ''
        X = MCLT.FastMCLT(segment[i*BlockLen/2:(i+2)*BlockLen/2])
        for k in range(length):
            if k % 2 == 0:                          #Every Other Frequency
                continue
            d = 0.0
            for m in range(L):
                if X[k*L+m].real > 0:
                    d += -r[m]
                if X[k*L+m].real < 0:
                    d += r[m]
            d = d/L
            if d <= 0:         #if sameLen <= 1:
                bytes += '1'
            elif d > 0:        #if sameLen >= 3:
                bytes += '0'

        #print bytes
        start = len(synchronization)
        bytes = bytes[start:]
        
        for j in range( 8 ):
            byte = bytes[j*8:j*8+8]
            info += chr( getInt(byte) )
    
    result = ''
    interval = (infoLen / 8 + 1) * 8
    for i in range(len(info)/interval):
        temp = info[i*interval:i*interval+infoLen]
        result += temp
    if len(info[(i+1)*interval:]) >= infoLen:
        temp = info[(i+1)*interval:(i+1)*interval+infoLen]
        result += temp
    info = result
        
    return info
    
def extractInfoWithFFT(audio, BlockLen, infoLen):
    synchronization = "00111100001111000011110000111100"
    bytes = ''
    info = ''
    B = len(audio[0]) / BlockLen
    length = 8*infoLen + 8*len(synchronization)      #对于需要广播的音频无法使用
    #length = BlockLen
    
    for i in range(B):
        FL = np.fft.rfft( audio[0][i*BlockLen:(i+1)*BlockLen] )
        #FR = np.fft.rfft( audio[1][i*BlockLen:(i+1)*BlockLen] )
        for k in range( length ):
            if FL[1000+k].real > 0:
                bytes += '1'
            else:
                bytes += '0'
        
        start = bytes.find(synchronization) + len(synchronization)
        bytes = bytes[start:]
        for j in range( infoLen ):
            byte = bytes[j*8:j*8+8]
            info += chr( getInt(byte) )
            
    return info
    

def test():
    strInfo = "http://cslt.riit.tsinghua.edu.cn/"
    
    nchannels, sampwidth, framerate, nframes, wave_data, time = disposeWav.read_wave_data("../wavFile/result1.wav")
    BlockLen = 4096
    infoLen = len(strInfo)
    info = extractInfoWithLSB(wave_data, BlockLen, infoLen)
    print info,'\n\n'
    
    nchannels, sampwidth, framerate, nframes, wave_data, time = disposeWav.read_wave_data("../wavFile/result2.wav")
    #nchannels, sampwidth, framerate, nframes, wave_data, time = disposeWav.read_wave_data("../wavFile/disposedWav.wav")
    BlockLen = 4096
    infoLen = len(strInfo)
    info = extractInfoWithMCLT(wave_data, BlockLen, infoLen)
    print info,'\n\n'
    
    nchannels, sampwidth, framerate, nframes, wave_data, time = disposeWav.read_wave_data("../wavFile/result3.wav")
    BlockLen = 4096
    infoLen = len(strInfo)
    info = extractInfoWithFFT(wave_data, BlockLen, infoLen)
    print info,'\n\n'
    
    
    #print nchannels, sampwidth, framerate, nframes
    
if __name__ == "__main__":
    test()
