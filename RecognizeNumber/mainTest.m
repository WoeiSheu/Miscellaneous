%% 训练神经网络
trainImages = readImages('./train-sample');     %读取训练图片
handledImages = preprocessing(trainImages);     %预处理后的图片列表
featureLattice = extractFeature(handledImages); %特征提取，每一列代表一张图片
net = bpTrain(featureLattice);                        %训练
%% 测试神经网络
testImages = readImages('./test-sample');        %读取测试图片
handledImages = preprocessing(testImages);       %预处理后的图片列表
featureLattice = extractFeature(handledImages);  %特征提取
result = recognition(featureLattice,net);            %识别图片