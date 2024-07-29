function HTML_to_Icon(filename)
% This script helps to create lovely mask icons. Specify the name of an
% HTML 5 canvas file created by Inkscape. The script will output a file
% (in the current directly) with the same name prepended with "Icon".
% Use the M code in the Icon file to perform patch operations in the mask
% icon drawing code. For best results, ensure the source file uses only
% integer coordinates (whole numbers only!). In the mask definition, use
% "Pixels" for Icon units, paste in the code that defines path, w, and h
% and follow it with something like this:
%             % work in pixels since the icon does not scale
%             pos = get(gcbh,'position');
%             bw = pos(3) - pos(1);
%             bh = pos(4) - pos(2);
% 
%             % Draw centered icon
%             xAdj = bw/2 - w/2;
%             yAdj = bh/2 - h/2;
%             for i=1:length(path)
%                 patch(path(i).x+xAdj, path(i).y+yAdj, path(i).rgb);
%             end
% 
%             % Anchor the corners of the coordinate system
%             patch(0,0,[1 1 1]);
%             patch(bw,bh,[1 1 1]);

%   Copyright 2018-2019 Bose Corporation
    fid = fopen(filename);
    
    beginPathTag = '.beginPath';
    transformTag = '.transform';
    saveTag = '.save';
    restoreTag = '.restore';
    canvasTag = '<canvas';

    ctx = [0 0 0 0 0 0];
    ctxStack = {};
    ctxPtr = 1;
    paths = [];
    pathCounter = 0;
    
    width = 0;
    height = 0;
    
    % search for fillstyle (or lineWidth)
    while(~feof(fid))
        s = fgetl(fid);
        if (contains(s,canvasTag))
            width = str2num( char( extractBetween(s, 'width=''','''') ) );
            height = str2num( char( extractBetween(s, 'height=''','''') ) );
        end
        if (contains(s,saveTag))
            ctxStack{ctxPtr} = ctx;
            ctxPtr = ctxPtr + 1;
        end
        if (contains(s,restoreTag) && ctxPtr > 1)
           ctxPtr = ctxPtr - 1;
           ctx = ctxStack{ctxPtr};
        end
        if (contains(s,transformTag))
            colorStr = char( extractBetween(s,'(',')') );
            ctx = ctx + sscanf(colorStr,'%f,');
        end        
        if (contains(s,beginPathTag))
            [x,y,stroke,fill] = ParseObj(fid);
            x = x + ctx(5);
            y = y + ctx(6);
            pathCounter = pathCounter + 1;
            paths(pathCounter).x = x;
            paths(pathCounter).y = y;
            paths(pathCounter).rgb = fill / 255;
            paths(pathCounter).stroke = stroke / 255;
        end
    end
    
    % Now that the entire image is in, flip it vertically and output it
    [pathstr,name,ext] = fileparts(filename);
    funcName = ['Icon' name];
    outName = [funcName '.m'];
    outF = fopen(outName,'w+');
    fprintf(outF,'function [path w h] = %s()\n', funcName);
    maxY = max([paths.y]);
    minY = min([paths.y]);
    maxX = max([paths.x]);
    minX = min([paths.x]);
    clf % clear current figure
    hold on % keep multiple plots all on the current figure
    for i=1:length(paths)
        paths(i).y = height - paths(i).y;
        if (sum(paths(i).rgb >= 0) == 3)
            patch(paths(i).x,paths(i).y,paths(i).rgb,'LineStyle','none');
        end
        if (sum(paths(i).stroke >= 0) == 3)
            plot(paths(i).x,paths(i).y,'color',paths(i).stroke);
        end
        xName = sprintf('  path(%i).x',i);
        yName = sprintf('  path(%i).y',i);
        WriteVar(outF,xName,paths(i).x);
        WriteVar(outF,yName,paths(i).y);
        fprintf(outF,'  path(%i).rgb = [%f %f %f];\n',i,paths(i).rgb(1),paths(i).rgb(2),paths(i).rgb(3));
        fprintf(outF,'  path(%i).stroke = [%f %f %f];\n',i,paths(i).stroke(1),paths(i).stroke(2),paths(i).stroke(3));
        %fprintf(outF,'patch(x,y,[%f %f %f]);\n',paths(i).rgb(1),paths(i).rgb(2),paths(i).rgb(3));
    end
    hold off % set the figure hold back to default where it resets each time
    fprintf(outF,'  w = %i;\n',round(width));
    fprintf(outF,'  h = %i;\n',round(height));
    fprintf(outF,'end');
    fclose(fid);
    fclose(outF);
end

function [x,y,stroke,fill] = ParseObj(fid)
    strokeTag = '.strokeStyle';
    fillTag = '.fillStyle';
    moveTo = '.moveTo';
    lineTo = '.lineTo';

    fill = [-255 -255 -255];
    stroke = [-255 -255 -255];
    
    x = [];
    y = [];
    while(~feof(fid))
        s = fgetl(fid);
        
        if (contains(s,fillTag))
            colorStr = char( extractBetween(s,'(',')') );
            colorVec = sscanf(colorStr,'%i,');
            fill = transpose(colorVec);
        end
        if (contains(s,strokeTag))
            colorStr = char( extractBetween(s,'(',')') );
            colorVec = sscanf(colorStr,'%i,');
            stroke = transpose(colorVec);
        end
        if (contains(s,moveTo) || contains(s,lineTo))
            pointStr = char( extractBetween(s,'(',')') );
            if (isempty(pointStr))
                break;
            end
            coords = sscanf(pointStr,'%f,');
            if (isempty(coords))
                break;
            end
            x = [x coords(1)];
            y = [y coords(2)];
        end
        if (contains(s,'.stroke();') || contains(s,'.fill();'))
            break; % signals end of path for us
        end
    end
end

function WriteVar(outF,varname,var)
    nbytes = 0;
    fprintf(outF,'%s=[',varname);
    for v=var
        nbytes = nbytes + fprintf(outF,'%i ',round(v));
        if (nbytes > 100)
            fprintf(outF,'...\n   ');
            nbytes = 0;
        end
    end
    fprintf(outF,'];\n');
end
