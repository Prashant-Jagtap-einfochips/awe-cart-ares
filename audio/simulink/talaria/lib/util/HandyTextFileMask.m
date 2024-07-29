classdef HandyTextFileMask
% HANDYTEXTFILEMASK This block is a handy shortcut to editing a text
% file.
%    Also if the text file does not exist, the mask dialog pops up
%    during model load to give users the option to create the file with
%    default content.
%
%    Documentation for HandyTextFileMask
%       doc HandyTextFileMask
%
%
methods
function Transform76
   %  Gracefully replace TemplateSource values that have been removed
end
function RemoveDotCommentsInCurrentEditor
   %  This routine acquires the text in the current MATLAB editor
   %  window and removes all lines that start with a "%." comment.
   %  After any whitespace, the line must start with three
   %  characters to be included in the removal:
   %  "%" "." and "something other than ."
   %  Optional argument theDoc removes from specified document
   %  object instead of from active one
end
function CreateFileIfMissing
   %  CreateFileIfMissing creates the standard file if one is not
   %  already found either on path or in the desired place but not
   %  on path. Provide handle to a handy block. Returns full path
   %  to file found or created.
   %  Usage:
   %     [filename, wasCreated] = HandyTextFileMask.CreateFileIfMissing(handyBlock);
end
function RecreateFile
   %  RecreateFile Adds handy block (if necessary), deletes any
   %  existing file, and recreates the file fresh.
   %  Usage:
   %     fileName = HandyTextFileMask.RecreateFile(containingBlockPath);
end
function AddHandyBlock
   %  AddHandyBlock adds a handy text file block inside the
   %  provided subsystem path. Optional arguments specify the name
   %  of the file template (Config is default) and name of the new
   %  block (which will override behavior to use block name instead
   %  of parent subsystem name)
   %  Usage:
   %     newBlock = HandyTextFileMask.AddHandyBlock(parentBlock,tmpalteName,newBlockName);
end
function DoSubstitutions
   %  Use MaskUtil which will substitute any defined symbols which
   %  in this case are just the function arguments.
end
function CB_CreateFile
   %  Get file prefix and file location
end
function CB_TemplateSource
   %  Called every time the TemplateSource parameter changes and
   %  also  when the dialog opens.
end
function GetFileName
   %  GetFileName gets file prefix, suffix, and folder of new file
   %  Determines which operational case is at play in terms of
   %  library links, overridden prefix names, and other behaviors,
   %  especially weirdness with file extensions
end
function UpdateTemplateList
   %  Update template list in the dialog drop-down popup
end
function GetTemplateFolder
   %  First find folder of the HandyTextFile block (this block)
end
function DoLoadFcn
   %  Act differently when in *our* library
end
function HandyTextFileMask
   % HANDYTEXTFILEMASK This block is a handy shortcut to editing a text
   % file.
   %    Also if the text file does not exist, the mask dialog pops up
   %    during model load to give users the option to create the file with
   %    default content.
   %
   %    Documentation for HandyTextFileMask/HandyTextFileMask
   %       doc HandyTextFileMask
   %
end
end
end
