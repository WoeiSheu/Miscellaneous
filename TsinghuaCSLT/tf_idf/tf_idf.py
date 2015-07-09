# coding=utf-8
from __future__ import division
import math

fileInput = open("gselect.10.dubm",'r')     #输入文件
lines = fileInput.readlines()

document = []                   #每个文档占用数组一个位置
key = []                        #每个文档的第一个word作为key

for line in lines:              #对于每个文档(即行),将其word作为key,出现的次数作为value存入一个字典,再将该字典作为document数组的一个元素
    tf_idfDict = {}
    wordOfLine = line.split()
    key.append(wordOfLine[0])
    
    wordOfLine = wordOfLine[1:]
    
    for word in wordOfLine:
        if not tf_idfDict.has_key(word):
            tf_idfDict[word] = 1
        else:
            tf_idfDict[word] += 1
    
    tf_idfDict['sum'] = len(wordOfLine)     #每个文档字典的最后一个关键字为'sum',用于记录每个文档总的word数量
    document.append(tf_idfDict)


outputFile = open('./result.txt','w')       #存入结果的文档
for i in range(len(document)):              #对于每个文档进行遍历
    tf = 0.0
    idf = 0.0
    tf_idf = 0.0
    tf_idf_sum = 0.0
    
    outputFile.write(key[i]+"\n")
    
    dic = document[i]                       #取出每个文档字典
    for j in dic.keys():                    #取出字典中的每个key,计算其tf值与idf值
        if j is not 'sum':
            tf = dic[j]/dic['sum']          #tf值即文档中该word出现次数与总word数比值
            
        for k in range(len(document)):      #计算有多少个文档出现了该word
            if document[k].has_key(j):
                idf += 1
        
        # print j,idf,len(document)
        # print j, dic[j], dic['sum'], tf   #测试用
        
        idf = math.log( len(document)/idf ) #计算idf
        tf_idf = tf*idf                     #对于每个word计算其tf_idf
                
        outputFile.write(str(j)+": "+str(tf_idf)+"\n")
        
        tf_idf_sum += tf_idf
        
    outputFile.write("\n\n")
    
    # print key[i], ' ', tf_idf_sum
