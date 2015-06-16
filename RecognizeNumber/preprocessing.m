function [finalImageList] = preprocessing(inputImageList)
%%
% Function:预处理图片
%
% Parameters:
% Input Variables:  图片矩阵列表
% Output Variables: 预处理完成后的矩阵列表
%
% Example:
% imageList = readImages('root');
% finalList = preprocessing(imageList);
%
% Author: Hypocrisy(虚伪)
% Date:   06/14/2015
%
% Notice:
% 该程序只适用于包含一个数字的图片
%
% Log:
% 可以改进的地方有: 
%                 图像的细化用了自带的bwmorph，效果不是很好，有时还不如不加
%                 图像的中值滤波效果不好，未使用，可以尝试其他滤波方案
%%
    countImages = length(inputImageList);   %列表长度
    finalImageList = cell(0);     %初始化返回列表
    
    for imageIndex = 1:countImages
    %%
        inputImage = inputImageList{imageIndex};
        
        if ndims(inputImage) ~= 2
            grayImage = rgb2gray(inputImage);   %灰度化
        else
            grayImage = inputImage;
        end
        %medianFilteringImage = medfilt2(grayImage);   %中值去噪，若效果不好则去除
        medianFilteringImage= grayImage;
        binaryImage = im2bw(medianFilteringImage);     %二值化，二值化之后只有0和1
    %%
        %找到黑色像素最小行和最大行，最小列和最大列并截取
        %需要注意是1代表白色，0代表黑色，所以先反色
        binaryImage = ~binaryImage;
        rowMin = 1;
        while sum(binaryImage(rowMin,:)) == 0
            rowMin = rowMin+1;                          %找出最早出现的有效行
        end
        rowMax = size(binaryImage,1);
        while sum(binaryImage(rowMax,:)) == 0
            rowMax = rowMax-1;                          %找出最迟出现的有效行
        end

        columnMin = 1;
        while sum(binaryImage(:,columnMin)) == 0
            columnMin = columnMin+1;                    %找出最早的有效列
        end
        columnMax = size(binaryImage,2);
        while sum(binaryImage(:,columnMax)) == 0
            columnMax = columnMax-1;                    %找出最迟的有效列
        end
        %对于以上函数,可以改进，将其变为可以处理一行数字的图片，而不是单个数字的图片

        truncateImage = binaryImage(rowMin:rowMax,columnMin:columnMax);    %截取图片有效部分
        %找到黑色像素最小行和最大行，最小列和最大列并截取
    %%    
        resizeImage = imresize(truncateImage,[30,30]);  %将图片大小统一转换为为30*30像素
    %%    
        %注意必须先反色再细化效果才好
        inverseImage = resizeImage;   %反色处理，这里会将1变为0，0变为1，前面已经反色过，故不需要再次反色
        thinImage = bwmorph(inverseImage,'thin',Inf);   %细化反色处理后的图片
        
        finalImage = thinImage;                         %处理后的图片
        finalImageList{imageIndex} = finalImage;        %返回处理后的列表
    end
end