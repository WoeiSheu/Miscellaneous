function net = bpTrain(featureLattice)
%% Function: 训练神经网络
%% Parameters:
% Input Variables: 预处理后的图片所整合的矩阵
% Output Variables: None
%
%% Author: Hypocrisy(虚伪)
%% Date: 06/15/2015
%
%% 
    interval = floor(size(featureLattice,2)/10);    %每个数字的间隔，也相当于每个数字的训练数量有多少
    desiredOutput= zeros(10,size(featureLattice,2));
    for i = 1:10
        desiredOutput(i,interval*(i-1)+1:interval*i) = 1; %对于每个输入，其期望的输出
    end
    
    %PR = minmax(featureLattice);    %minmax(featureLattice):对神经网络输入的最大最小值的限制
    %Si = [16,10];                    %神经网络层结构，隐层9个神经元，输出层10个
    %以上两句是newff的旧使用方法，弃之
    net = newff(featureLattice,desiredOutput,12,{'tansig','tansig','tansig'},'trainlm');  %输入输出隐层的传递函数均为S型的正切函数,使用Levenberg-Marquard算法进行训练
    %隐层12个单元
%%
    net = init(net);
    [m1,n1]=size(net.IW{1,1});  %初始化当前输入层权值
    net.IW{1,1}=0.3*ones(m1,n1);
    [m2,n2]=size(net.LW{2,1});  %初始化隐层与输出层的连接权值
    net.LW{2,1}=0.3*ones(m2,n2);
%% 网络初始化
    net.trainParam.show=100;    %显示的间隔次数
    net.trainParam.lr=0.01;     %网络学习速率
    net.trainParam.mc=0.9;      %动量因子
    net.trainParam.epochs=5000; %最大训练次数
    net.trainParam.goal=0.001;  %性能目标值
    net.trainFcn='trainrp';
%%
    [net,tr] = train(net,featureLattice,desiredOutput);  %静态批处理方式进行网络训练.net：更新了权值的神经网络，tr：训练次数和每次训练的误差
    plotperf(tr)      %
end