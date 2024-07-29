function addMfbFuncTrigger(block)
% addMfbFuncTrigger Adds a function-trigger input to specified MATLAB
% Function Block.
% Usage:
%    addMfbFuncTrigger(block)

%   Copyright 2022 Bose Corporation

    if (nargin < 1)
        block = gcb;
    end

    blockPath = getfullname(block);

    % Use Diagnostic Viewer for status:
    myStage = sldiagviewer.createStage('TalariaAutomation','ModelName',bdroot(block));
    sldiagviewer.reportInfo(sprintf('Adjusting block [%s]\n',block));

    % First attempt to retrieve the MFB's chart object
    sf = sfroot();
    theMfb = sf.find('-isa','Stateflow.EMChart','-regexp','Path',block);
    if (isempty(theMfb))
        error('Block [%s] does not appear to be a MATLAB Function Block', ...
            block);
    end

    % Souce #2 Discover MFB Parameters already registered to Ports&Data
    args = theMfb.getChildren();
    mfbParams = findobj(args,'Trigger','Function call');
    if (false == isempty(mfbParams))
        MaskUtil.ReportWarning('Function trigger already present for ''%s''. No action taken', ...
            blockPath);
        return;
    end

    % Existing trigger is not found... add one
    newElem = Stateflow.Trigger(theMfb);
    newElem.Trigger = "Function call";

    MaskUtil.ReportInfo('Added function call trigger to ''%s''', ...
        blockPath)

end