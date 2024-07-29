classdef DsmCheatReadConfig < BtcBaseClass
% DsmCheatReadConfig Build-time configuration class for the DsmCheatRead block
% Usage:
%    aConfig = DsmCheatReadConfig
%

%   Copyright 2021 Bose Corporation

  properties
      MakeDsmCodeIdentifierUnique logical = false
  end
  methods
    %----------------------------------------------------------------------
    function DoMaskInit(this, blockHandle)
        % Invoked by mask initialization, interact with objects on the
        % Simulink canvas to setup code generation and other properties

        % First ensure the DSM read output signal is a pointer in codegen
        this.SetupCheatPointerCsc(blockHandle);
        
        % Make sure the DSM is setup with CSC of Simulink.Export
        dsmIdentifier = this.SetupDsmCodeGen(blockHandle, ...
            this.MakeDsmCodeIdentifierUnique);
        assignin("caller","dsmIdentifier",dsmIdentifier);
    end
  end
  methods (Static)
    %----------------------------------------------------------------------
    function CbButtonRegister(blockHandle)
        % Trigger block to re-initialize. Cannot directly call the code
        % from this callback as only the mask initialization routine is
        % allowed to make changes inside the block
        
        % Reset the text control that records the latest registered name
        theMask = Simulink.Mask.get(blockHandle);
        registeredName = theMask.getDialogControl('RegisteredDsmName');
        registeredName.Prompt = '';

        pn = 'TriggerInit';
        MaskUtil.safe_setparam(blockHandle,pn,'');
        DsmCheatReadConfig.CbTriggerInit(blockHandle)
    end
    %----------------------------------------------------------------------
    function CbTriggerInit(blockHandle)
        pn = 'TriggerInit';
        loc = get_param(blockHandle,'Position');
        locStr = sprintf('[%d %d %d %d]',loc);
        MaskUtil.safe_setparam(blockHandle,pn,locStr);
    end
    %----------------------------------------------------------------------
    function SetupCheatPointerCsc(blockHandle)
        % Setup the storage class of the cheat pointer signal line
        dsmReadBlock = MaskUtil.SubBlockName(blockHandle,'DsmAccess');
        dsmReadBlockHandle = getSimulinkBlockHandle(dsmReadBlock);
        if (ishandle(dsmReadBlockHandle))
            % The block is there! Setup its storage class!
            % Use the name of the overall block as a base
            baseName = get_param(blockHandle,'Name');
            pointerName = sprintf('Cheat%s%s', ...
                baseName,dec2hex(crc32MEX(uint8(dsmReadBlock))));
            CscUtil.SetOutportCsc(dsmReadBlockHandle,1, ...
                'Simulink', 'ImportedExternPointer', ...
                'Identifier',pointerName);
        end
    end
    %----------------------------------------------------------------------
    function dsmIdentifier = SetupDsmCodeGen(blockHandle, ...
            makeDsmCodeIdentifierUnique)
        % Find the DataStoreMemory block and ensure good CSC

        dsmName = get_param(blockHandle,'DataStoreName');
        theMask = Simulink.Mask.get(blockHandle);
        registeredName = theMask.getDialogControl('RegisteredDsmName');
        if (isempty(registeredName))
            error('Could not find text dialog control');
        end

        if (makeDsmCodeIdentifierUnique)    
            pathInfo = FeaturePathUtil.GetPathInfo(blockHandle,dsmName);
            dsmIdentifier = sprintf('%s%s',dsmName, ...
                dec2hex(crc32MEX(uint8(pathInfo.featurePath))));
        else
            dsmIdentifier = dsmName;
        end
        
        % Abort now if this is an Engine model
        coreIndexRole = CoreIndexMask.GetCoreIndex(blockHandle);
        dsmBlockIsRelevant = (coreIndexRole <= CoreIndexMask.SourceModel);
        if (false == dsmBlockIsRelevant)            
            return;
        end
        
        % The mask text control records the previously registered DSM name.
        % If name does not match, time to update to the new name.
        doUpdate = (false == strcmp(registeredName.Prompt,dsmIdentifier));
        if (doUpdate)
            [theDsmPath, foundIt] = DsmCheatReadConfig.FindDsm(blockHandle,dsmName);
            if (false == foundIt)
                error("Could not find Data Store Memory with DataStoreName of '%s'", ...
                    dsmName);                
            end
            % Found the DSM. Setup the code generation Custom Storage Class
            CscUtil.SetDatastoreCsc(theDsmPath, ...
                'Simulink','ExportToFile (Simulink)', ...
                'Identifier',dsmIdentifier);

            % Update the mask's text control to indicate that this name
            % has now been registered. Text controls are not block
            % properties, so this update will not trigger parameterized
            % library links. It's just mask text. ;-)
            registeredName.Prompt = dsmIdentifier;
        end
    end
    %----------------------------------------------------------------------
    function [theDsmPath,foundIt] = FindDsm(blockHandle,dsmName)
        % Searches model to find data store memory with specified DSM name

        theDsmPath = [];
        foundIt = false;
        F = Simulink.FindOptions(SearchDepth=1,FollowLinks=true);
        theSystem = get_param(blockHandle,'Parent');
        while ((false == foundIt) && (false == isempty(theSystem)))
            dsms = Simulink.findBlocksOfType(theSystem,'DataStoreMemory','DataStoreName',dsmName,F);
            if (false == isempty(dsms))
                theDsmPath = dsms(1);
                foundIt = true;
            else
                theSystem = get_param(theSystem,'Parent');
            end
        end
    end
    %----------------------------------------------------------------------
    function [path,w,h] = Icon()
      path(1).x=[10 12 15 12 8 5 8 10 10 7 4 1 0 0 0 1 4 7 13 16 18 20 23 29 32 35 36 36 36 35 32 29 26 26 28 31 28 24 ...
       21 24 26 26 23 21 20 18 16 15 13 10 ];
      path(1).y=[4 4 6 8 8 6 4 4 0 0 1 3 5 6 7 9 11 12 12 11 11 11 12 12 11 9 7 6 5 3 1 0 0 4 4 6 8 8 6 4 4 0 0 1 2 4 ...
       2 1 0 0 ];
      path(1).rgb = [1.000000 0.000000 0.000000];
      path(1).stroke = [-1.000000 -1.000000 -1.000000];
      path(2).x=[2 8 16 15 8 3 ];
      path(2).y=[14 18 14 13 15 13 ];
      path(2).rgb = [0.000000 0.000000 0.000000];
      path(2).stroke = [-1.000000 -1.000000 -1.000000];
      path(3).x=[34 28 20 21 28 33 ];
      path(3).y=[14 18 14 13 15 13 ];
      path(3).rgb = [0.000000 0.000000 0.000000];
      path(3).stroke = [-1.000000 -1.000000 -1.000000];
      w = 35;
      h = 18;
    end    
  end
end
