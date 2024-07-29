classdef TranslateTriggerMask
% TranslateTriggerMask 
%    Tune parameters feed tuning data to this blocks which in turn
%    feeds translated data into target overlay parameters (TOPs).
%    This block collaborates with Tune and TOP blocks to accomplish
%    automatic triggering of translation logic as tuning parameters
%    change.
%
%    Documentation for TranslateTriggerMask
%       doc TranslateTriggerMask
%
%
properties
  %  This DataMap keeps track whether the translations have run
DataMap
end
methods
function InitAllTriggers
   % InitAllTranslateTriggers finds all TranslateTrigger blocks in
   % provided subsystem and asserts their translator subsystems to have
   % the appropriate function packaging. This is useful as part of
   % eliminating parameterized links in older libraries with
   % translations.
end
function CreateEmptyTranslation
   %  Create and hookup a blank example translation unless there's
   %  already something connected.
end
function SetStoredValue
   %  remove the last block name from the path
end
function CB_TriggerInit
   %  Hide the Error box
end
function TranslateTriggerMask
   % TranslateTriggerMask 
   %    Tune parameters feed tuning data to this blocks which in turn
   %    feeds translated data into target overlay parameters (TOPs).
   %    This block collaborates with Tune and TOP blocks to accomplish
   %    automatic triggering of translation logic as tuning parameters
   %    change.
   %
   %    Documentation for TranslateTriggerMask/TranslateTriggerMask
   %       doc TranslateTriggerMask
   %
end
end
end
