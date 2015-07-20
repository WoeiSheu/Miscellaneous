#!usr/bin/env python
#coding=utf-8

import wave
import matplotlib.pyplot as plt
import numpy as np
import math
import struct

def read_wave_data(file_path):      #读入wav文件
    #open a wave file, and return a Wave_read object
    f = wave.open(file_path,"rb")
    #read the wave's format infomation,and return a tuple
    params = f.getparams()
    #get the info
    nchannels, sampwidth, framerate, nframes = params[:4]
    #Reads and returns nframes of audio, as a string of bytes. 
    str_data = f.readframes(nframes)
    #close the stream
    f.close()
    #turn the wave's data to array
    wave_data = np.fromstring(str_data, dtype = np.short)
    #for the data is stereo,and format is LRLRLR...
    #shape the array to n*2(-1 means fit the y coordinate)
    wave_data.shape = -1, nchannels     #Modified at 13:17 at July 9, 2015, and the origin is "wave_data.shape = -1, 2"
    #transpose the data
    wave_data = wave_data.T
    #calculate the time bar
    time = np.arange(0, nframes) * (1.0/framerate)  #nframes or nframes/2, or maybe it's relative to samplewidth and nchannels.
    
    #print nchannels, sampwidth, framerate, nframes, wave_data, time
    return nchannels, sampwidth, framerate, nframes, wave_data, time

def wave_fft(time,wave_data,framerate):     #将读入的wav文件时域信号进行fft变换,得到频域信号
    fftSize = len(time)
    #fftSize = 8192  #采样个数
    
    xf = [[],[]]
    xf[0] = np.fft.rfft(wave_data[0])/fftSize
    xf[1] = np.fft.rfft(wave_data[1])/fftSize
    #freqs = np.fft.fftfreq(len(xf))
    #freqs = np.linspace(0, framerate/2, fftSize/2+1)   #如果sampwidth=2
    freqs = np.linspace(0, framerate, len(xf[0]))   #如果sampwidth=1
    
    xfp = [[],[]]
    xfp[0] = 20*np.log10(np.clip(np.abs(xf[0]), 1e-20, 1e100))
    xfp[1] = 20*np.log10(np.clip(np.abs(xf[1]), 1e-20, 1e100))
    
    phase = [[],[]]
    for i,j in zip(xf[0],xf[1]):
        phase[0].append( math.atan(i.imag/i.real) )
        phase[1].append( math.atan(j.imag/j.real) )
        
    #print xf[0],freqs,phase
    return xfp,freqs,phase

def write_wave(outputName,params,wave_data):        #输入参数和wave_data,生成对应wav文件
    result_wav = wave.open(outputName, 'w')
    #result_wav.setnchannels(nchannels)
    #result_wav.setsampwidth(sampwidth)
    #result_wav.setframerate(framerate)
    #result_wav.setnframes(nframes)
    result_wav.setparams(params)
    
    #output = wave_data * nframes * nchannels * sampwidth 
    
    if params[0] == 2:
        for channel0,channel1 in zip(wave_data[0],wave_data[1]):    #双通道音频才可以这样
            result_wav.writeframes(struct.pack('h',channel0))
            result_wav.writeframes(struct.pack('h',channel1))
    else:
        for channel in wava_data:
            result_wav.writeframes(struct.pack('h',channel))
    
    result_wav.close()


def test():
    nchannels, sampwidth, framerate, nframes, wave_data, time = read_wave_data("../wavFile/bird.wav")
    params = (nchannels, sampwidth, framerate, nframes,'NONE', 'not compressed')
    '''
    data0 = []
    data0.append(wave_data[0][0:3000])
    data0.append(wave_data[1][0:3000])
    nchannels, sampwidth, framerate, nframes, wave_data, time = read_wave_data("../wavFile/result2.wav")
    data = [[],[]]
    data[0] = data0[0].tolist() + wave_data[0].tolist()
    data[1] = data0[1].tolist() + wave_data[1].tolist()
    write_wave("../wavFile/disposedWav.wav",params,data)
    '''
    print nchannels, sampwidth, framerate, nframes
    #draw the wave
    #plt.subplot(211)
    #plt.plot(time, wave_data[0])
    #plt.subplot(212)
    #plt.plot(time, wave_data[1], c = "g")
    #plt.show()
    
    xfp, freqs, phase = wave_fft(time, wave_data, framerate)
    #print len(xfp),len(freqs), len(phase)
    #plt.subplot(211)
    #plt.title("Magnitude")
    #plt.plot(freqs, xfp[0])
    #plt.xlabel("Hz")
    #plt.subplots_adjust(hspace=0.4)
    
    #plt.subplot(212)
    #plt.title('Phase')
    #plt.plot(freqs,phase[0],c='g')
    #plt.xlabel('Hz')
    #plt.show()

if __name__ == "__main__":
    test()
