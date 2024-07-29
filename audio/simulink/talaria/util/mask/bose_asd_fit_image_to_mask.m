function [ X Y W H ] = bose_asd_fit_image_to_mask( imageName, maxPixelHeight )
%FITIMAGETOMASK Calculate largest for image on block mask
%   Gets rectangular shape of current block and of given image file.
%   Calculates image size to fit on block without changing aspect ratio

%   Copyright 2018-2019 Bose Corporation
%Get block shape
pos = get(gcbh,'position');
width = pos(3) - pos(1);
height = pos(4) - pos(2);
Baspect = width / height;

%Get image shape
[w,h,c]  = size(imread(imageName));
Iaspect = w/h;

% work with normalized coordinates
% limit maximum pixel height
maxH = 1;
if (nargin >= 2 && height > maxPixelHeight)
   maxH = maxPixelHeight / height;
end

%Fit the image
W = 1;
H = Iaspect * Baspect;
if (H > maxH)
    W = maxH/H;
    H=maxH;
end
X=(1-W)/2;
Y=(1-H)/2;

end

