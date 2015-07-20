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
    synchronization = "1111111100000000111111110000000011111111000000001111111100000000"
    #bytes = synchronization + bytes
    L = 6                                 #一位扩展为L个频率
    s = [-1,1,-1,1,-1,1]                       #1对应4位编码
    
    #将左通道的数据通过MCLT变换到复数域
    B = len(audio[0])*2 / BlockLen - 1          #Block数量
    bytes_segment = []
    segment_length = 64
    for i in range( B-1 ):
        if (i+1)*segment_length <= len(bytes):
            bytes_segment.append(synchronization+bytes[segment_length*i:segment_length*i+segment_length])
        else:
            break
    bytes_segment.append(synchronization+bytes[segment_length*i:])
    #######################################
    #以下为嵌入信息的方法
    
    for i in range( B-1 ):
        if i % 2 == 0:                    #Every Other Block
            continue
        X_prev = MCLT.FastMCLT(audio[0][(i-1)*BlockLen/2:(i+1)*BlockLen/2])
        X_curr = MCLT.FastMCLT(audio[0][i*BlockLen/2:(i+2)*BlockLen/2])
        X_next = MCLT.FastMCLT(audio[0][(i+1)*BlockLen/2:(i+3)*BlockLen/2])
        #X = MCLT.FastMCLT(audio[0][i*BlockLen:(i+1)*BlockLen])
        X = X_curr
        for k in range( len(bytes_segment[(i/2)%len(bytes_segment)]) ):
            #Calculate z1 and z2
            z1 = []
            z2 = []
            for l in range(BlockLen/2):
                if abs(l-k) < 2*L and abs(l-k) % 2 == 0:
                    temp = pow(-1,l)/( 2.0*math.pi*(l-k-1)*(l-k+1) )
                    z1.append(temp)
                    z2.append(temp)
                elif abs(l-k) == 1:
                    temp = pow(-1,l)/8.0
                    z1.append(temp)
                    z2.append(-temp)
                else:
                    temp = 0
                    z1.append(0)
                    z2.append(0)
            
            ###
            for m in range(L):
                if bytes_segment[(i/2)%len(bytes_segment)][k] == '1':
                    X[(2*k+1)*L+m] = abs(X[(2*k+1)*L+m])*s[m]       #2*k --> Every Other Frequency
                else:
                    X[(2*k+1)*L+m] = -abs(X[(2*k+1)*L+m])*s[m]
            #The following is for compensating the interference
            for m in range(L):
                if k < len(synchronization):        #同步序列嵌入方式不同,为防止干扰
                    temp = np.inner(z1,X_prev)+np.inner(z2,X_next)+1.0/4.0*X[(2*k+1)*L+m-1]-1.0/4.0*X[(2*k+1)*L+m+1]
                    X[(2*k+1)*L+m] = X[(2*k+1)*L+m] - 2j*temp
    #######################################
        y = MCLT.FastIMCLT(X)          #将嵌入信息后的复数域信息反变换到实数域
        y_prev = MCLT.FastIMCLT(X_prev).tolist()[BlockLen/2:]
        #y_prev = audio[0][i*BlockLen/2:(i+1)*BlockLen/2]       #IMCLT变换结果与原始序列不同,故不能如此
        y_next = MCLT.FastIMCLT(X_next).tolist()[:BlockLen/2]
        #y_next = audio[0][(i+1)*BlockLen/2:(i+2)*BlockLen/2]   #IMCLT变换结果与原始序列不同,故不能如此
        y = np.array(y_prev + y_next) + y
        #The following is according to experience
        for yi in range(len(y)):
            if y[yi] > 30000:
                y[yi] = 30000
            if y[yi] < -30000:
                y[yi] = -30000
        audio[0][i*BlockLen/2:(i+2)*BlockLen/2] = y
    
    '''
    #将右通道的数据通过MCLT变换到复数域
    B = len(audio[1])*2 / BlockLen - 1      #Block数量
    #######################################
    #以下为嵌入信息的方法
    
    for i in range(B-1):
        if i % 2 == 0:                    #Every Other Block
            continue
        X_prev = MCLT.FastMCLT(audio[1][(i-1)*BlockLen/2:(i+1)*BlockLen/2])
        X_curr = MCLT.FastMCLT(audio[1][(i)*BlockLen/2:(i+2)*BlockLen/2])
        X_next = MCLT.FastMCLT(audio[1][(i+1)*BlockLen/2:(i+3)*BlockLen/2])
        #X = MCLT.FastMCLT(audio[0][i*BlockLen:(i+1)*BlockLen])
        X = X_curr
        for k in range( len(bytes) ):
            #Calculate z1 and z2
            z1 = []
            z2 = []
            for l in range(BlockLen/2):
                if abs(l-k) < 2*L and abs(l-k) % 2 == 0:
                    temp = pow(-1,l)/( 2.0*math.pi*(l-k-1)*(l-k+1) )
                    z1.append(temp)
                    z2.append(temp)
                elif abs(l-k) == 1:
                    temp = pow(-1,l)/8.0
                    z1.append(temp)
                    z2.append(-temp)
                else:
                    temp = 0
                    z1.append(0)
                    z2.append(0)
            ###
            for m in range(L):
                if bytes[k] == '1':
                    X[(2*k+1)*L+m] = abs(X[(2*k+1)*L+m])*s[m]       #2*k --> Every Other Frequency
                else:
                    X[(2*k+1)*L+m] = -abs(X[(2*k+1)*L+m])*s[m]
            #The following is for compensating the interference
            for m in range(L):
                temp = np.inner(z1,X_prev)+np.inner(z2,X_next)+1.0/4.0*X[(2*k+1)*L+m-1]-1.0/4.0*X[(2*k+1)*L+m+1]
                X[(2*k+1)*L+m] = X[(2*k+1)*L+m] - 2j*temp
        #######################################
        y = MCLT.FastIMCLT(X)             #将嵌入信息后的复数域信息反变换到实数域
        y_prev = MCLT.FastIMCLT(X_prev).tolist()[BlockLen/2:]
        y_next = MCLT.FastIMCLT(X_next).tolist()[:BlockLen/2]
        y = np.array(y_prev + y_next) + y
        audio[1][i*BlockLen/2:(i+2)*BlockLen/2] = y
    '''
    return audio
    
def setInfoWithFFT(audio, bytes):           #fft 变换为频域嵌入信息
    BlockLen = 4096
    
    B = len(audio[0]) / BlockLen
    synchronization = "00111100001111000011110000111100"
    bytes = synchronization + bytes
    
    for i in range(B):
        FL = np.fft.rfft( audio[0][i*BlockLen:(i+1)*BlockLen] )
        FR = np.fft.rfft( audio[1][i*BlockLen:(i+1)*BlockLen] )
        for k in range( len(bytes) ):
            if bytes[k] == '1':
                FL[1000+k] = abs(FL[1000+k])        #1kHz以上,因为分段导致每段之间最高只有2kHz左右
                FR[1000+k] = abs(FR[1000+k])
            else:
                FL[1000+k] = -abs(FL[1000+k])
                FR[1000+k] = -abs(FR[1000+k])
    
        outputLeft  = np.fft.irfft(FL)
        outputRight = np.fft.irfft(FR)
        audio[0][i*BlockLen:(i+1)*BlockLen] = outputLeft
        audio[1][i*BlockLen:(i+1)*BlockLen] = outputRight
    
    return audio

def test():
    strInfo = "http://cslt.riit.tsinghua.edu.cn/"
    bytes = getInfoOfBytes(strInfo)
    #print bytes
    
    nchannels, sampwidth, framerate, nframes, wave_data, time = disposeWav.read_wave_data("../wavFile/outputfile.wav")
    wave_data = setInfoWithLSB(wave_data, bytes)
    params = (nchannels, sampwidth, framerate, nframes,'NONE', 'not compressed')
    disposeWav.write_wave("../wavFile/result1.wav",params,wave_data)
    
    nchannels, sampwidth, framerate, nframes, wave_data, time = disposeWav.read_wave_data("../wavFile/outputfile.wav")
    wave_data = setInfoWithMCLT(wave_data,bytes)
    params = (nchannels, sampwidth, framerate, nframes,'NONE', 'not compressed')
    disposeWav.write_wave("../wavFile/result2.wav",params,wave_data)

    nchannels, sampwidth, framerate, nframes, wave_data, time = disposeWav.read_wave_data("../wavFile/outputfile.wav")
    wave_data = setInfoWithFFT(wave_data,bytes)
    params = (nchannels, sampwidth, framerate, nframes,'NONE', 'not compressed')
    disposeWav.write_wave("../wavFile/result3.wav",params,wave_data)


if __name__ == "__main__":
    #print setInfoWithMCLT.__doc__
    test()
