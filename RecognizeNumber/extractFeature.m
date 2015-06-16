function featureLattice = extractFeature(handledImages)
%%
% Function: 提取图像特征，将图像划分为6*6=36个区域
% 
% Parameters:
% Input: 经过预处理的图片列表
% Output: 特征矩阵，每一列代表图片列表中一个图片的特征
%
% Author: Hypocrisy(虚伪)
% Date: 06/15/2015
%
%%
    countImages = length(handledImages);   %列表长度
    for k = 1:countImages
    %%
        handledImage = handledImages{k};
        rowInterval = floor(size(handledImage,1)/6);    %将行分为6块
        columnInterval = floor(size(handledImage,2)/6); %将列分为6块
        singleFeatureLattice = zeros(1,rowInterval*columnInterval); %每块的和为一个数，一个图片的特征值为一维数组
        %以下循环求出一个图片的特征值
        for i = 1:6
            for j = 1:6
                singleFeatureLattice( 1, 6*(i-1)+j ) = sum(sum(handledImage(rowInterval*(i-1)+1:rowInterval*i,columnInterval*(j-1)+1:columnInterval*j)));
            end
        end
        %singleFeatureLattice = singleFeatureLattice/36;    %归一化为0-1之间的数字
        featureLattice(:,k) = singleFeatureLattice';        %所有图片的特征值，每列为一个图片的
    end
end