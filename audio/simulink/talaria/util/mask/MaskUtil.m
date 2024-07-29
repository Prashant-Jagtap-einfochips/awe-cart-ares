classdef MaskUtil
%  MaskUtil Collection of utility routines to assist with mask operations
%
%    Documentation for MaskUtil
%       doc MaskUtil
%
%
methods
function MaskUtil
   %  MaskUtil Collection of utility routines to assist with mask operations
   %
   %    Documentation for MaskUtil/MaskUtil
   %       doc MaskUtil
   %
end
function NotThisFileCallerInfo
   %  NotThisFileCallerInfo investigates the call stack and returns
   %  caller filename, line #, and function name if available for a
   %  caller that is outside the provided filename
   %  Usage:
   %     msg = MaskUtil.NotThisFileCallerInfo(thisFile,includeFunctionName);
   %  thisFile : string/char name of the current file
   %  includeFunctionName : optional boolean (default true) of whether
   %  to include the calling function name in the output message
   %  Examples:
   %     callerInfo = MaskUtil.NotThisFileCallerInfo(mfilename,false);
   %     callerInfo = MaskUtil.NotThisFileCallerInfo('MyFile')
end
function PrintLastError
   %  Prints out information from the last error
end
function GetFileEditLink
   %  GetFileEditLink returns text for a hyperlink to edit a file path
   %  Usage:
   %     linkText = MaskUtil.GetFileEditLink(filePath,optionalDisplayText)
end
function FindPorts
   %  FindPorts recurse in provided system to return port handles.
   %  Ports are signals with Compiled properties (at the ends of lines)
   %  Usage:
   %     lineHs = MaskUtil.FindLines(system, <additional arguments>);
   % 
   %  See also: MaskUtil.FindLines
end
function FindLines
   %  FindLines recurse in provided system to return line handles.
   %  Lines are connector conduits with a Src and Dst. Although lines
   %  have storage class and other properties, oddly, they cannot be
   %  found that way - use FindPorts instead. Think of lines more as
   %  only graphical objects. Some propery searches work (e.g. Parent)
   %  but most property-based searches return nothing.
   %  Usage:
   %     lineHs = MaskUtil.FindLines(system, <additional arguments>);
   % 
   %  See also: MaskUtil.FindPorts
end
function CreateWikiName
   %  CreateWikiName attempts to create a valid PascalCase name from
   %  the provided name, but with the additional restriction that no
   %  underscores are present and that every uppercase character be
   %  followed by a lower-case character. This case restriction
   %  eliminates single letter words and uppercase acronyms. Note
   %  that single words *are* allowed in WikiCase (e.g. Filter) unlike
   %  full wiki LinkPatterns in which at least two words must be
   %  present to become a WikiWord or LinkPattern. Note that the common
   %  solution to single-letter words is to capitalize the word "I" but
   %  then force the next character to be lowercase - but make the word
   %  "a" lowercase. This method is *not* attempted here for "a" and
   %  "I" because recognition of English-language words is not done.
   %  Usage:
   %    newName = CreatePascalName(origName);
end
function MakeCell
   %  MakeCell returns a cell array - if the provided value is already
   %  a cell array, then nothing changes.
   %  Usage:
   %    value = MaskUtil.MakeCell(value);
end
function TransformFunctionDelete
   %  TransformFunctionDelete is a helper function for library transform
   %  functions to delete mask parameters.
   %  Simulink mask transform functions accept an "inData" parameter
   %  and return an "outData" result. The TransformFunctionDelete function
   %  serves to copy parameter from inData to outData with parameter
   %  removal accomplished.
   %  Usage:
   %    outData = MaskUtil.TransformFunctionDelete(inData,deadNamesCell)
end
function TransformFunctionRename
   %  TransformFunctionRename is a helper function for library transform
   %  functions to rename mask parameters.
   %  Simulink mask transform functions accept an "inData" parameter
   %  and return an "outData" result. The TransformFunctionRename function
   %  serves to copy parameter from inData to outData with parameter
   %  renaming accomplished.
   %  Usage:
   %     outData = MaskUtil.TransformFunctionRename(inData, ...
   %                   {'oldname1','oldname2'}, ...
   %                   {'newname1','newname2'}
end
function FindTidFromFactor
   %  FindTidFromFactor retrieves all the sample times present in a model
   %  to find one that matches the specified rateFactor when compared
   %  to the provided root period. If the root period is not provided,
   %  the fastest discrete period present in the model will be used.
   %  Once a match is found, the 0-based task TID and sample period is
   %  returned back. Note that retrieving sample times is an expensive
   %  operation unless the model is already in a compiled state (e.g.
   %  during simulation or "writeRTW" code generation).
   %  Usage:
   %     [stepTID,samplePeriod] = MaskUtil.FindTidFromFactor(H,rateFactor,rootPeriod)
   %  
   %  Note: emits error if a match is not be found
end
function FindSampleTime
   %  FindSampleTime retrieves all the sample times present in a model
   %  to find one that matches the specified target period. Once found,
   %  the task TID (0 based) and step factor (e.g. 1 or greater) is
   %  returned back. Note that retrieving sample times is an expensive
   %  operation unless the model is already in a compiled state (e.g.
   %  during simulation or "writeRTW" code generation).
   %  Usage:
   %     [stepTID,stepFactor] = MaskUtil.FindSampleTime(H,targetPeriod)
end
function CreateEmptyMask
   %  CreateEmptyMask Destroys existing mask on the block (if any) and creates an empty
   %  new mask with only default elements that come with a new mask.
   %  Usage:
   %   theMask = MaskUtil.CreateEmptyMask(H)
end
function IsInBlockDiagram
   %  IsInBlockDiagram returns true if the given block is in the specified diagramName
   %  Usage:
   %   itIs = MaskUtil.IsInBlockDiagram(H,diagramName)
end
function EnableBlockPort
   %  EnableBlockPort Based on block name (not full name), swaps in a
   %  terminator/ground or an outport/inport. Optionally a port number
   %  can be specified.
   %  Usage:
   %    MaskUTil.EnableBlockPort(H,portBlockName,enable,portNumStr);
   %  H: handle/URL of parent subsystem containing the port
   %  portBlockName : name of block that is the in/out port
   %  enable : true if port is to be enabled, false to ground/terminate
   %  portNumStr : optional port number as a char/string
end
function RecursivelyDeleteLine
   %  RecursivelyDeleteLine deletes the given line and its
   %  line-children if UNCONNECTED
   %  Usage:
   %   MaskUtil.RecursivelyDeleteLine(aLine)
end
function DeleteUnconnectedPortsAndLines
   %  DeleteUnconnectedPortsAndLines Deletes any lines and in/out ports
   %  that are not connected to a block in the specified system.
   %  Usage:
   %   MaskUtil.DeleteUnconnectedPortsAndLines(System)
end
function ConnectPort
   %  ConnectPort accepts a block handle and connects it to a block
   %  named portName. The portName block is reused if it already exists
   %  regardless of block type (which might cause trouble in some
   %  cases). Otherwise, a new block is created and named.
   %  Usage:
   %   [extPortBlk] = ConnectPort(aBlockPort, gotoFromTag, portName, portNum, structName, InitialValue)
   %  Arguments:
   %  -----------
   %  gotoFromTag controls which type of block to create:
   %    "RT" = create rate transition
   %    "TM" = create terminator (or ground for inputs)
   %    "DELAY" = create a delay block
   %    any other string = create a goto or from block
   %    '' (blank string) = create a inport or outport block.
   %  portName : used to find existing block, or name new one
   %  portNum : sets port number when blank gotoFromTag (i.e. a port)
   %  structName : when non-blank, sets output lines to struct CSC and
   %    defines the struct name that will contain the signal
   %  InitialValue : (optional) When non-empty will set an output
   %    port's initial value. NOTE this capability is only available
   %    for conditional/triggered subsystems!
end
function ConnectNewBlock
   %  ConnectNewBlock connects a new block to a given block port
   %  Usage:
   %   newBlock = MaskUtil.ConnectNewBlock(aBlockPort, newBlockLibraryPath, blockName, distance, newBlocksPortName)
end
function EnsureBlocksDoNotExist
   %  EnsureBlocksDoNotExist Ensures provided blocks not found in curSys.
   %  Generates error if blocks are found.
   %  Usage:
   %     MaskUtil.EnsureBlocksDoNotExist(curSys,blockNamesCell);
end
function CopyBlock
   %  CopyBlock Creates a copy of source block directly above it.
   %  Usage:
   %     newBlock = MaskUtil.CopyBlock(sourceBlock, copyName);
   % 
   %  Note that copyName can use string substitution with the following
   %  variables:
   %   $(NAME)
   %  e.g. MaskUtil.CopyBlock(gcb,'CopyOf$(NAME)');
end
function CopyBlockSize
   %  CopyBlockSize Copies block size from refBlock to blockToChange
   %  Usage:
   %     MaskUtil.CopyBlockSize(blockToChange, refBlock);
   % 
   %  See also MaskUtil.ResetSizeByLibraryLink
end
function ResetSizeByLibraryLink
   %  ResetSizeByLibraryLink Given a block that is a library link,
   %  resets the block size to match the size of its referenced library.
   %  Note: uses current block selection if H is not specified or empty.
   %  Usage:
   %    MaskUtil.ResetSizeByLibraryLink(H)
   % 
   %  See also MaskUtil.CopyBlockSize
end
function SetBlockColor
   %  SetBlockColor Set the foreground and background color of the
   %  specified block. If not specified, default foreground is 'black'
   %  and default backround is 'white'. 
   %  Note: uses current block selection if H is not specified or empty.
   %  Usage:
   %     MaskUtil.SetBlockColor(H,foreground,background)
end
function SelectedBlockHandles
   %  SelectedBlockHandles returns array of handles for selected blocks
   %  in specified subsystem (or gcs if not specified)
   %  Usage:
   %     blockHandles = MaskUtil.SelectedBlockHandles();
end
function BlockHandlesToPaths
   %  BlockHandlesToPaths returns a string array of block paths given
   %  an array of block handles. Simulink's getfullname returns a
   %  different type of object (char array vs cell array) based on
   %  whether there is only one block handle. Strings are better in
   %  handling singletons or arrays all with the same indexing.
   %  Usage:
   %     stringBlockPaths = MaskUtil.BlockHandlesToPaths(someHandles);
end
function FindPromotedParameter
   %  FindPromotedParameter return the originating parameter that was
   %  promoted into the provided mask/param-name. If more than one
   %  originating parameter is present, only the first one is returned.
   %  Usage:
   %     [origParam,origBlock] = MaskUtil.FindPromotedParameter(theMask,paramName);
end
function UnpromoteParameter
   %  UnpromoteParameter will eliminate promoted references to any
   %  parameters of the given block in preparation for its deletion.
   %  If no parameter name was specified, unpromote them ALL. By
   %  providing no name, every parameter of the block will be removed
   %  Usage:
   %   MaskUtil.UnpromoteParameter(H,paramName)
end
function SetPromotedParameter
   %  SetPromotedParameter will set a mask parameter... even if the
   %  parameter has been promoted! When parameter promotion is
   %  detected, this method searches up the model hierarchy to find an
   %  encapsulating mask. Then it searches that mask's parameters to
   %  find one that is the promoted embodiment of our original
   %  parameter and sets it. This is very handy for self-modifying
   %  blocks or as when you add them to a subsystem in a library block,
   %  their self-modified parameters will become parameterized links
   %  unless you promote these parameters up to the top level library
   %  mask. Then just use this routine to self modify. Note that this
   %  method utlimately uses safe_setparam, so the parameter is not set
   %  if the value has not changed.
   %  Usage:
   %   paramOwner = MaskUtil.SetPromotedParameter(H,paramName,paramValue)
end
function FindParentMask
   %  FindParentMask searches up the model hierarchy and returns the
   %  first mask it finds. Both the parent mask and the relative path
   %  (from parent down to starting child "H") are returned.
   %  Usage:
   %   [parentMask,pathCell,parentName] = MaskUtil.FindParentMask(H)
end
function GetEvaluatedParameter
   %  GetEvaluatedParameter Amazingly retrieves the value of a mask
   %  parameter that is specified with "evaluate" behavior on.
   %  [value,foundIt] = GetEvaluatedParameter(H,paramName)
   %  The foundIt ouput returns false if the paramater is not found.
   %  When the foundIt output is not captured and the parameter is not
   %  found, an error is signalled instead.
   %  The block must be fully loaded first. Use load_system.
   %  Usage:
   %   [value,foundIt] = MaskUtil.GetEvaluatedParameter(H,paramName)
end
function FindParameterValue
   %  FindParameterValue attempts to resolve the specified parameter by
   %  searching the base workspace and mask workspaces for a parameter
   %  TODO: Follow MATLAB's symbol resolution more closely 
   %  See https://www.mathworks.com/help/simulink/ug/resolving-symbols.html
   %  Usage:
   %   [theValue,foundIt] = MaskUtil.FindParameterValue(H,parameterName)
end
function ConfigSetFeatureIfPresent
   %  Deprecated - replaced by BTConfig.InitSetFeatureIfPresent
   %  See also BTConfig.InitSetFeatureIfPresent
end
function ConfigHideIfPresent
   %  Deprecated - replaced by BTConfig.CallbackHideIfPresent
   %  See also BTConfig.CallbackHideIfPresent
end
function ConfigNodeIsPresent
   %  Deprecated - replaced by BTConfig.NodeIsPresent
   %  See also BTConfig.NodeIsPresent
end
function ConfigExpand
   %  Deprecated - replaced by BTConfig.Expand
   %  See also BTConfig.Expand
end
function ConfigAssert
   %  Deprecated - replaced by BTConfig.Assert
   %  See also BTConfig.Assert
end
function ValueToStr
   %  ValueToStr Performs mat2str except is also supports greater than
   %  2 dimensions. The numeric class is included in the output string.
   %  Usage:
   %     stringValue = MaskUtil.ValueToStr(numericValue)
   %  Optionally supply precision:
   %     stringValue = MaskUtil.ValueToStr(numericValue, numDigits)
   %  Unprintable numericValues will return <?class?> where "class" is
   %  the class of the provided numericValue
end
function GetDialogValue
   %  GetDialogValue Retrieves potentially unapplied mask dialog values.
   %  Given a mask parameter name, reads parameter's value directly
   %  from the mask dialog GUI widgets. Returns empty if mask dialog is
   %  not currently.
   %  Usage:
   %   [theValue] = MaskUtil.GetDialogValue(block, parameterName);
end
function StringSubstitution
   %  Searches for variable names embedded in the provided string with
   %  the markup '$(varname)' where varname is a workspace or mask
   %  parameter to be evaluated. Returns a new string parameter values
   %  substituted for their '$(name)' placeholders.
   %  Usage:
   %   replacedStr = MaskUtil.StringSubstitution(sourceString)
end
function IsCommented
   %  IsCommented checks if the provided block is commented out. Since
   %  Simulink does not propagate the "commented" property down the
   %  subsystem hierarchy, we must instead search all the way to the
   %  top to see if the containing subsystems are commented out.
   %  Usage:
   %   [isCommented] = MaskUtil.IsCommented(H)
end
function MagicLineRepair
   %  MagicLineRepair attempts to reconnect the "closest unconnected line"
   %  to any unconnected ports of the block. This only works well if an
   %  unconnected line is sitting right next to each unconnected port. 
   %  Usage:
   %   MaskUtil.MagicLineRepair(block)
end
function BigTextEditDialog
   %  BigTextEditDialog opens a window to edit the specified parameter of the given block H
   %  Usage:
   %   [cancelled] = BigTextEditDialog(H,paramName,windowTitle)
end
function ListBoxDialogResizeHandler
   %  Internal use callback handler for ListBoxDialog resizing.
   %  Based on when the child controls were added to the figure, the
end
function ListBoxDialog
   %  ListBoxDialog opens a dialog window with a list box in it
   %  Usage:
   %   [outText,cancelled] = ListBoxDialog(itemsCell,windowTitle,sizeInCm)
end
function TextEditDialogResizeHandler
   %  Internal use callback handler for TextEditDialog resizing.
   %  Based on when the child controls were added to the figure, the
   %  Children array is in this order: apply,cancel,text
end
function TextEditDialog
   %  TextEditDialog opens a dialog window with a text edit control in it.
   %  Usage:
   %   [outText,cancelled] = MaskUtil.TextEditDialog(inText,windowTitle,sizeInCm)
end
function getDataTypeName
   % getDataTypeName Converts a data type ID to data type name
   %  Usage:
   %   [typeName] = MaskUtil.getDataTypeName(dataTypeID)
end
function getDataTypeID
   % GETDATATYPEID Converts a data type string to data type ID
   %  Usage:
   %   [dataTypeID] = MaskUtil.getDataTypeID(dataType)
end
function DisableLibraryLink
   %  DisableLibraryLink Will do exactly as it states and temporarily
   %  disable the nearest parental library link. If optional destroyIt
   %  is provided and true, then the link will be permanently removed.
   %  Usage:
   %   MaskUtil.DisableLibraryLink(H,destroyIt)
end
function FindAncestorLibRef
   %  FindAncestorLibRef Searches up subsystem hierachy to find
   %  ancestor that is a library reference. By default,
   %  includeDisabledLinks will be false
   %  Usage:
   %   [ancestorLibRef,ancestorBlock,isUnresolved] = MaskUtil.FindAncestorLibRef(H,includeDisabledLinks)
end
function GetIdentifierBasedOnSID
   %  Grabs a block's SID and generates a source-code appropriate
   %  unique identifier to describe the block. If provided, the
   %  optionalNameRoot is used as a prefix and the full model name will
   %  be replaced with the model root name
   %  Usage:
   %   [codeString] = MaskUtil.GetIdentifierBasedOnSID(H,optionalNameRoot)
end
function ErrorTextGet
   %  ErrorTextGet Retrieves the specified block's error box text as a
   %  cell array.
   %  Pre-requirement: During block mask creation, the error box must
   %  have been previously created via MaskUtil.AddErrorBoxToMaskDialog
   %  Usage:
   %   [errCell] = MaskUtil.ErrorTextGet(H)
   % 
   %  See also MaskUtil.AddErrorBoxToMaskDialog
end
function ErrorBoxHideSuccesses
   %  ErrorBoxHideSuccesses closes the error box is the current image
   %  indicates success
   %  Usage:
   %   MaskUtil.ErrorBoxHideSuccesses(H)
end
function EscapeTextForXml
   %  EscapeTextForHtml Escapes problematic characters for XML content
   %  Usage:
   %   htmlText = MaskUtil.EscapeTextForXml(theText)
end
function EscapeTextForHtml
   %  EscapeTextForHtml Escapes problematic characters for HTML content
   %  Usage:
   %   [htmlText] = MaskUtil.EscapeTextForHtml(theText)
end
function ErrorBoxDisplay
   %  ErrorBoxDisplay Pass in a cell array of error messages. This call
   %  will hide the error box if the cell array is empty, or display
   %  the errors in the error box if there are any. Optional
   %  'isSuccess' argument when true causes the success icon instead of
   %  error icon to display in the box.
   %  Pre-requirement: During block mask creation, the error box must
   %  have been previously created via MaskUtil.AddErrorBoxToMaskDialog
   %  Usage:
   %   MaskUtil.ErrorBoxDisplay(H, errCell, isSuccess, isWarning)
   % 
   %  See also AddErrorBoxToMaskDialog
end
function AddErrorBoxToMaskDialog
   %  AddErrorBoxToMaskDialog Call this during block mask creation code
   %  to enable ErrorBox commands like ErrorBoxDisplay.
   %  Usage:
   %   MaskUtil.AddErrorBoxToMaskDialog(theMask)
   % 
   %  See also MaskUtil.ErrorBoxDisplay
end
function locate_prompt
   %  locate_prompt Sets a mask dialog control to display its prompt at
   %  the indicated location {'top' or 'left'}
   %  Usage
   %   MaskUtil.locate_prompt(theMask, dialogControlName, location)
end
function same_row
   %  same_row Sets a mask dialog control to display on the same row as
   %  the previous control (i.e. "current" row).
   %  Usage:
   %   MaskUtil.same_row(mask, dialogControlName, stretch)
end
function GetBlockHeight
   %  GetBlockHeight(H,[inOutAuto]) Calculates the specified block's
   %  height in terms of how many in or out ports it has.
   %  Usage:
   %   [portPixelSpace] = MaskUtil.GetBlockHeight(H,inOutAuto)
end
function SetBlockHeight
   %  SetBlockHeight(H,[portPixelSpace],[inOutAuto]) Resizes the
   %  specified block's height based on how many in or out ports it
   %  has.
   %  Note: uses current block selection if H is not specified or empty.
   %  Usage:
   %   MaskUtil.SetBlockHeight(H,portPixelSpace,inOutAuto)
end
function ForceInitBlock
   %  ForceInitBlock Causes the specified block to run its
   %  initialization routines (e.g. masked blocks run their init code).
   %  This does *not* invoke a block's InitFcn callback, which is
   %  different.
   %  Usage:
   %   MaskUtil.ForceInitBlock(theBlock)
end
function TryDeleteBlock
   %  TryDeleteBlock deletes the specified block but only if it exists
   %  Usage:
   %   MaskUtil.TryDeleteBlock(blockName);
end
function SubBlockName
   %  SubBlockName Returns full name of block inside subsystem H.
   %  The full path of the parent subsytem block 'H' is joined with the
   %  local name of the block provided (i.e. the block name within the
   %  parent subsystem).
   %  Usage:
   %    blockName = MaskUtil.SubBlockName(H,blockLocalName);
end
function safe_set
   %  safe_set Works like the built-in set command, except
   %  the block is only "modified" if the specified value is different
   %  than the existing value. This method prevents triggering init
   %  callbacks when nothing has actually changed.
   %  Usage:
   %   [changed] = MaskUtil.safe_set(H, paramName, paramValue);
end
function safe_setparam
   %  safe_setparam Works like the built-in set_param command, except
   %  the block is only "modified" if the specified value is different
   %  than the existing value. This method prevents triggering init
   %  callbacks when nothing has actually changed.
   %  Usage:
   %   [changed] = MaskUtil.safe_setparam(H, paramName, paramValue);
end
function ExtractValidName
   %  ExtractValidName Extracts a valid feature name from the provided
   %  featureName string.
   %  Usage:
   %   [validName, cleanName, errCell] = MaskUtil.ExtractValidName(featureName,paramDescription,emptyOK)
end
function ReportWarning
   %  ReportInfo works like printf reporting information to the Simulink
   %  diagnostic viewer window.
   %  Usage:  (like sprintf)
   %   MaskUtil.ReportWarning(formatStr, ...)
end
function ReportInfo
   %  ReportInfo works like printf reporting information to the Simulink
   %  diagnostic viewer window.
   %  Usage:  (like sprintf)
   %   MaskUtil.ReportInfo(formatStr, ...)
end
function ReportError
   %  ReportError works like printf to report an error to the Simulink
   %  diagnostic viewer window. This method does not trigger MATLAB
   %  script error status (i.e. the script keeps on running).
   %  Usage:  (like sprintf)
   %   MaskUtil.ReportError(formatStr, ...)
end
function RemoveLink
   %  RemoveLink Strips HTML links from the provided string, leaving
   %  only the link's "display text" behind.
   %  Usage:
   %   [str,foundOne] = MaskUtil.RemoveLink(inStr)
end
function GetBlockErrorLink
   %  GetBlockErrorLink Returns link-string to view specified block
   %   in "error style" highlighting.
   %  Usage:
   %   [linkString] = MaskUtil.GetBlockErrorLink(H)
end
function GetBlockLinkString
   %  GetBlockLinkString Generates an HTML string link to the block
   %  specified using the MATLAB hilite system. This is useful in
   %  warning and error messages to reference relevant blocks.
   %  optionalHiLiteStyle see https://www.mathworks.com/help/simulink/slref/hilite_system.html
   %  ('find', 'error', etc.)
   %  Usage:
   %  [linkString] = MaskUtil.GetBlockLinkString(strBlockH, optionalDisplayText, optionalHiLiteStyle)
end
function GetFullNameStringExpression
   %  GetFullNameStringExpression Handles newlines in a block's name by
   %  building a string expression that includes the newlines and a
   %  version without them. Handy for creating html href links.
   %  Usage:
   %   [withNewlines,withoutNewlines] = MaskUtil.GetFullNameStringExpression(strBlockH,optionalDisplayText)
end
end
end
