classdef SignalOverrideMask
%SignalOverrideMask Calibrated Signal Mask
%   This block illustrates how one might create a re-useable mechanism to
%   define streaming scalar signals such as vehicle engine speed.
%   Internally the block creates a high priority RTC signal receiver as
%   well as two tuning variables and TOPS to coordinate calibration of
%   the signal.

%   Copyright 2018-2019 Bose Corporation
  properties (Constant = true)
    MaskType = 'Bose Signal Override';
  end

  methods (Static)
    %--------------------------------------------------
    function [ok] = Init(H)
        
        ok = true;
        
        % ... Show or hide error box
        MaskUtil.ErrorBoxDisplay(H,{});
    end

    %--------------------------------------------------
    function CB_TriggerInit(H)

        % If the location has changed, force the block to run its init by
        % changing a parameter. This in turn causes the icon to refresh.
        pn = 'TriggerInit';
        loc = get_param(H,'Position');
        locStr = sprintf('[%d %d %d %d]',loc);
        MaskUtil.safe_setparam(H,pn,locStr);
    end


    %-------------------------------------------------------------------------
    function CreateMaskParametersAndDialog(H)
        theMask = Simulink.Mask.get(H);
        
        % clear any existing display code to avoid errors. Below we destroy
        % all the mask parameters and recreate them. In the meantime,
        % display code might try to access a parameter that has not be
        % recreated yet.
        theMask.Display = '';

        % ----- Introductory information
        theMask.removeAllParameters;
        theMask.removeDialogControl('DescGroupVar');
        descGroupVar = theMask.addDialogControl('Type','group','Name','DescGroupVar', ...
            'Prompt','%<MaskType>');
        theMask.removeDialogControl('DescTextVar');
        descGroupVar.addDialogControl('Type','text','Name','DescTextVar', ...
            'Prompt','%<MaskDescription>');
        
        % ----- Mask Parameters
        theMask.removeDialogControl('ParameterGroupVar');
        paramContainer = theMask.addDialogControl('Type','group','Name','ParameterGroupVar', ...
            'Prompt','Parameters');

        theMask.addParameter('Type','promote','Name','MaskSignalName', ...
            'TypeOptions',{'FeaturePath/FeatureName'}, ...
            'Value','', ...
            'Container',paramContainer.Name, ...
            'Tunable','off', ...
            'Evaluate', 'off', ...
            'Prompt','Signal Name');
        MaskUtil.locate_prompt(theMask,'SignalName','left');
        
        theMask.addParameter('Type','promote','Name','MaskDataTypeStr', ...
            'TypeOptions',{'RTC Override/REQElemType2'}, ...
            'Value','single', ...
            'Container',paramContainer.Name, ...
            'Prompt','Signal Type');        
        MaskUtil.locate_prompt(theMask,'MaskDataTypeStr','top');

        theMask.addParameter('Type','promote','Name','MaskSignalWidth', ...
            'TypeOptions',{'RTC Override/REQElemWidth2'}, ...
            'Value','1', ...
            'Container',paramContainer.Name, ...
            'Prompt','Signal Width');
        MaskUtil.locate_prompt(theMask,'MaskSignalWidth','top');
        MaskUtil.same_row(theMask,'MaskSignalWidth','on');
        
        % ----- Hidden Parameters
        theMask.addParameter('Type','edit','Name','LastName', ...
            'Value','', ...
            'Evaluate','off', ...
            'Tunable','off', ...
            'Visible','off', ...
            'Container',paramContainer.Name, ...
            'Prompt','non-visible helper to detect name change');
        
        theMask.addParameter('Type','edit','Name','TriggerInit', ...
            'Value','[]', ...
            'Tunable','off', ...
            'Evaluate', 'off', ...
            'Hidden','on', ...
            'Callback','SignalOverrideMask.CB_TriggerInit(gcbh);', ...
            'Prompt','Hidden: MoveFcn redraw');
        
        % ----- Error messages
        MaskUtil.AddErrorBoxToMaskDialog(theMask);

        % ----- Other mask properties
        theMask.Type = SignalOverrideMask.MaskType;
        theMask.Description = ['This creates the ability to override a signal ' ...
            'at runtime. Only scalar and vector signals are currently supported ' ...
            '(too lazy to put a reshaper block in there).'];
        theMask.Initialization = '[ok] = SignalOverrideMask.Init(gcbh);';
        theMask.RunInitForIconRedraw = 'on';
        theMask.IconUnits = 'pixels';
        iconDrawCode = {

            '[path,w,h] = IconSignalOverrideMask();'
            ''
            '% work in pixels since the icon does not scale'
            'pos = get(gcbh,''position'');'
            'bw = pos(3) - pos(1);'
            'bh = pos(4) - pos(2);'
            ''
            '% Draw centered icon'
            'xAdj = bw/2 - w/2;'
            'yAdj = 2; %bh/2 - h/2 + 4;'
            'for i=1:length(path)'
            '    if (path(i).rgb(1) >= 0)'
            '        patch(path(i).x+xAdj, path(i).y+yAdj, path(i).rgb);'
            '    end'
            '    if (path(i).stroke(1) >= 0)'
            '        plot(path(i).x+xAdj, path(i).y+yAdj);'
            '    end'
            'end'
            ''
            '% Display some text'
            'text(bw/2, bh-2,[MaskSignalName ''\nOverride''], ...'
            '    ''horizontalAlignment'',''center'', ...'
            '    ''VerticalAlignment'',''top'');'
            ''
            '% show a big red X when trouble is present'
            'if (exist(''ok'',''var'') && ok)'
            '    % Anchor the corners of the coordinate system'
            '    patch(0,0,[1 1 1]);'
            '    patch(bw,bh,[1 1 1]);'
            'else'
            '        tx=bw*0.03;'
            '        ty=bh*0.03;'
            '        patch([0 tx bw bw-tx], [ty 0 bh-ty bh], [1 0 0]);'
            '        patch([0 bw-tx bw tx], [bh-ty 0 ty bh], [1 0 0]);'
            'end'            };
        theMask.Display = strjoin(iconDrawCode,char(10));

        % ----- Block callbacks
        set_param(H,'MoveFcn','SignalOverrideMask.CB_TriggerInit(gcbh);');
        
    end

  
  end % methods (Static)
   
end

