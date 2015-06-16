function recognitionResult = recognition(featureLattice,net)
%%
% Function: 利用测试数据测试训练后的网络
%
% Parameters:
% Input: 测试数据
% Output: 实际得到的结果
%
% Author: Hypocrisy(虚伪)
% Date: 06/15/2015
%
%% 测试网络
    desiredResult = zeros(10,size(featureLattice,2));   %初始化测试样本希望得到的输出
    interval = floor(size(featureLattice,2)/10);        %测试样本每个数字数量
    for i = 1:10
        desiredResult(i,interval*(i-1)+1:interval*i) = 1;   %测试样本希望得到的输出
    end
    
    Y = sim(net,featureLattice);
    %recognitionResult = zeros(10,size(featureLattice,2));   %初始化利用实际网络得到的输出
    for i=1:length(featureLattice)
        recognitionResult(i)=find(Y(:,i)==max(Y(:,i)));     %利用训练过的网络识别手写体得到的实际结果
    end
    
%% 计算准确率
    [u,v]=find(desiredResult==1);
    label=u';
    error=label-recognitionResult;
    accuracy=size(find(error==0),2)/size(label,2)
end