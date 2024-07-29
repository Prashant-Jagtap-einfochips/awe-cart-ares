classdef AsyncConsoleMask
%  Mask for the AsyncConsole block. Code to set TLC options, pass data to
%  the Level-2 MATLAB S-Function, and to display the block icon.
%
%    Documentation for AsyncConsoleMask
%       doc AsyncConsoleMask
%
%
methods
function Transform7_1
   %  Combine all async console options into a single droplist choice
   %  EnableConsole   DoSil    Option TLC-Value
   %      0             n/a      1        0
   %      1             0        2        1
   %      1             1        3        2
   %   ... new value of "3" (use srat) cannot be represented with old mask
end
function Transform1128
   %  Transform function automatically created by AutomateMask
end
function CB_Option
   %  Show or hide async thread controls depending on opton selection
   %  Note that we cannot access the evaluated parameter in the mask
   %  workspace yet because this callback happens before the mask
   %  workspace is updated. Instead, we must use the secret decoder
   %  ring provided by Simulink.Mask.EnumerationTypeOptions
end
function DoInit
   %  This routine updates the model's TLC options parameter to reflect
   %  a chosen asynchronous console choice. Additionally, it returns a
   %  boolean value AddSimEventList indicating whether the simulated
   %  events are to be built-in to the generated code.
end
function AsyncConsoleMask
   %  Mask for the AsyncConsole block. Code to set TLC options, pass data to
   %  the Level-2 MATLAB S-Function, and to display the block icon.
   %
   %    Documentation for AsyncConsoleMask/AsyncConsoleMask
   %       doc AsyncConsoleMask
   %
end
end
end
