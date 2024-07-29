[path,w,h] = IconIIR_Mask();

% work in pixels since the icon does not scale
pos = get(gcbh,'position');
bw = pos(3) - pos(1);
bh = pos(4) - pos(2);

% Draw centered icon
xAdj = bw/2 - w/2;
yAdj = bh/2 - h/2 + 4;
for i=1:length(path)
    %     patch(path(i).x+xAdj, path(i).y+yAdj, path(i).rgb);
    plot(path(i).x+xAdj, path(i).y+yAdj);
end

% Anchor the corners of the coordinate system
patch(0,0,[1 1 1]);
patch(bw,bh,[1 1 1]);

% Display some text
text(bw/2, 1,'Pool IIR', ...
    'horizontalAlignment','center', ...
    'VerticalAlignment','bottom');