classdef TrivialTranslationMask
% TrivialTranslationMask Tune Variable mask
%    Tune Variables feed tuning data to translation blocks which in turn
%    feed translated data into target overlay parameters (TOPs).
%    This block collaborates with Translation Trigger blocks to accomplish
%    automatic triggering of translation logic as tuning variables
%    change.
%
%    Documentation for TrivialTranslationMask
%       doc TrivialTranslationMask
%
%
methods
function CB_TriggerInit
   %  If the location has changed, force the block to run its init by
   %  changing a parameter. This in turn causes the icon to refresh.
end
function Init
   %  TODO: Verify that the attached TuneVar and TOP are appropriate
   %  (may not be necessary to fuss with such things...)
end
function TrivialTranslationMask
   % TrivialTranslationMask Tune Variable mask
   %    Tune Variables feed tuning data to translation blocks which in turn
   %    feed translated data into target overlay parameters (TOPs).
   %    This block collaborates with Translation Trigger blocks to accomplish
   %    automatic triggering of translation logic as tuning variables
   %    change.
   %
   %    Documentation for TrivialTranslationMask/TrivialTranslationMask
   %       doc TrivialTranslationMask
   %
end
end
end
