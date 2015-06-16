function imgList = readImages(root)
%%
% Function: 读取目录root下的图片
%
% Parameters:
% Input: 目录root
% Output: 所有图片的矩阵列表
%
% Author: Hypocrisy(虚伪)
% Date: 06/15/2015
%
%%
    subFolders = dir(root); %读取子文件夹
    imgList = cell(0);      %初始化列表为空
    imgListIndex = 0;
    subFoldersNumbers = length(subFolders);  %子文件夹个数
    for i = 1:subFoldersNumbers              %读取每个子文件夹下的图片
        if ~( strcmp(subFolders(i).name,'.')|| strcmp(subFolders(i).name,'..') )    %过滤'.'和'..'
            subFolder = strcat(root,'/',subFolders(i).name);        %子文件夹名
            files = dir(subFolder);          %所有子文件
            filesNumber = length(files);     %子文件数量
            
            for j = 1:filesNumber
                imgName = files(j).name;    %子文件名
                if length(imgName) < 4      %子文件名太短舍弃
                    continue
                end
                if strcmp(imgName(end-3:end), '.bmp')   %子文件名是bmp才读取，也可以换成其他
                    imgListIndex = imgListIndex+1;
                    imgList{imgListIndex} = imread( strcat(subFolder,'/',imgName) );    %每个图片矩阵都存入列表中
                end
            end
        end
    end
end