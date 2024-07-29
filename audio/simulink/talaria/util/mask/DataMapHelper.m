classdef DataMapHelper
%DATAMAPHELPER Common static methods for masks that use DataMapUtil.
%   Common static methods working with a mask that uses DataMapUtil to
%   store a value retrieved from an in-port which is then echoed
%   through the outport. These masks often support multple roles where
%   sometimes the stored value takes over when the inport is no longer
%   present.
% Note that derived masks must create a panel named 'AdvancedPanel'

%   Copyright 2018-2019 Bose Corporation
  methods (Static)
    %----------------------------------------------------------------------
    function CB_EditDialog(H,DataMap,controlName)
        % retrieve the value from storage
        theValue = DataMap.GetStoredValue(H);
        theValueStr = mat2str(theValue,'class');
        [newStrValue,cancelled] = MaskUtil.TextEditDialog(theValueStr, ...
            ['Edit ' controlName], [8 2]);
        if (false == cancelled)
            theMask = Simulink.Mask.get(H);
            try
                theValue = eval(newStrValue);
                DataMap.SetStoredValue(H,theValue);
                theValueStr = mat2str(theValue,'class');
                DataMapHelper.UpdateValueText(theMask,theValueStr,controlName);
            catch
                % Gracefully fail
                DataMapHelper.UpdateValueText(Simulink.Mask.get(H), ...
                    '<Unrecognized MATLAB expression>',controlName);
            end
            % Re-open the advanced panel. It tends to close here otherwise
            advancedPanel = theMask.getDialogControl('AdvancedPanel');
            advancedPanel.Expand = 'on';
        end
    end

    %--------------------------------------------------
    function UpdateValueText(theMask,theValueStr,strDescription)
        controlName = strrep(strDescription,' ','');
        valueText = theMask.getDialogControl([controlName 'Text']);
        % Only show the first part of very long values:
        if (numel(theValueStr) > 300)
            valueText.Prompt = [theValueStr(1:300) ' ...'];
        else
            valueText.Prompt = theValueStr;
        end
    end

    %--------------------------------------------------
    function castValue = CastValue(theValue,dataType)
        % accepts dataType as returned from evaluated datatype mask
        % parameter (i.e. unidt mask parameter)
        if (isempty(dataType) ||  ...
            strcmp(dataType,'Inherit: Same as input'))
            % Just return "empty" if no casting is to take place
            castValue = [];
        else
            if (ischar(dataType))
                % The dataType is a string datatype name.
                % Simulink and MATLAB have dug in their heels. One says
                % "logical", the other says "boolean" ... let's call the
                % whole thing off! Or... just play along:
                dataType = strrep(dataType,'boolean','logical');
                castValue = cast(theValue,dataType);
            else
                % The dataType is an object
                castValue = fi(theValue,dataType);
            end
        end
    end
    %--------------------------------------------------
    function SetOutputValue(H,theValue,dataType)
        % Cast the provided value into the desired datatype and stuff it
        % into UserData to be retrieved by the C MEX block responsible for
        % sending it.
        castValue = DataMapHelper.CastValue(theValue,dataType);
        if (isempty(castValue))
            % if no casting is desired, just use the original value
            castValue = theValue;
        end
        set_param(H,'UserData',castValue);
    end
    %--------------------------------------------------
    function [theValue] = GetOutputValue(H)
        % Called from our C MEX counterpart, this routine retrieves the
        % previously set output value which is already in the intended
        % output datatype and dimensions.
        theValue = get_param(H,'UserData');
    end
    %--------------------------------------------------
    function [theValue] = GetOutputSignalData(H)
        % Called from our C MEX counterpart, this routine retrieves the
        % data appropriate to copy into the output signal buffer. For
        % fixed-point types, this is different than the OutputValue above
        % because only the extracted data bytes are returned (i.e. no data
        % type information). This routine is also used to get intialization
        % values for code generation.
        theValue = get_param(H,'UserData');

        % If we're dealing with a fixed-point type, try to extract the
        % value as plain integer values instead of returning the whole
        % object oriented entity.
        if (isa(theValue,'embedded.fi'))
            theValue = theValue.storedInteger;
        end
    end

    function CreateDialogValueDisplay(theMask,containerName,strDescription)
        % Add stored value display to mask. strDescription uniquely
        % identifies what the value is (for use in display and mask names)
        controlName = strrep(strDescription,' ','');
        theMask.removeDialogControl('ValuePanel');
        vPanel = theMask.addDialogControl('Type','panel','Name',[controlName 'Panel'], ...
            'Container',containerName);
        theMask.addDialogControl('Type','text','Name',[controlName 'Prompt'], ...
            'Prompt',[strDescription ':'], ...
            'HorizontalStretch','off', ...
            'Container',vPanel.Name);
        theMask.removeDialogControl([controlName 'Group']);
        vGroup = theMask.addDialogControl('Type','group','Name',[controlName 'Group'], ...
            'Container',vPanel.Name, ...
            'Row','current');
        theMask.removeDialogControl([controlName 'Text']);
        theMask.addDialogControl('Type','text','Name',[controlName 'Text'], ...
            'Container',vGroup.Name, ...
            'Row','current');
    end

    function CreateDialogValueEditButton(theMask,strDataMap,strDescription)
        % creates mask button in container named 'AdvancedPanel' that uses
        % the DataMap object described by code expression strDataMap
        % e.g. 'TOP_Mask.DataMap'
        % strDescription uniquely identifies what the value is (for use in
        % display and mask names)
        controlName = strrep(strDescription,' ','');
        callbackCode = sprintf('DataMapHelper.CB_EditDialog(gcbh,%s,''%s'');', ...
            strDataMap,controlName);
        theMask.removeDialogControl(['ButtonEdit' controlName]);
        btn = theMask.addDialogControl('Type','pushbutton', ...
            'Name',['ButtonEdit' controlName], ...
            'Container','AdvancedPanel', ...
            'Visible','on', ...
            'Prompt',['Directly Edit ' strDescription], ...
            'Callback',callbackCode);
        btn.HorizontalStretch = 'off';
    end
  end
end

