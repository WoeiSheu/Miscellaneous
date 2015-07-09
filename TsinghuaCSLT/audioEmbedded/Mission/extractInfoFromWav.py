#!usr/bin/env python
#coding=utf-8

import wave  
import numpy as np  
import scipy.signal as signal  
import matplotlib.pyplot as plt  
import sys
import disposeWav

def getInt(byte):   #得到8位二进制字符串对应的整数
    x = 0           #待修改,不明白为何少1
    count = 7
    for i in byte:
        x += int(i)*(2**count)
        count -= 1

    return x

def extractInfoWithLSB(audio):  #提取以LSB嵌入的信息
    bytes = ''
    length = 8*9    #8位乘以n字节
    for i in range(length):
        if audio[0][i] % 2 == 0:
            bytes += '0'
        else:
            bytes += '1'
        
    info = ''        
    for i in range( length/8 ):
        byte = bytes[i*8:i*8+8]
        info += chr( getInt(byte) )
    
    return info

def extractInfoWithMCLT(audio):
    bytes = ''
    
    info = ''
    return info
    
def test():
    nchannels, sampwidth, framerate, nframes, wave_data, time = disposeWav.read_wave_data("result1.wav")
    info = extractInfoWithLSB(wave_data)
    print info
    
    info = extractInfoWithMCLT(wave_data)
    print info
    
if __name__ == "__main__":
    test()
