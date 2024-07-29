classdef CoreIndexMask
%  CoreIndexMask specifies system role of this model in partitioning context
%
%    Documentation for CoreIndexMask
%       doc CoreIndexMask
%
%
properties
  %  Processor core roles. Assume ASDID core field has 3 bits of
  %  storage - so valid ASDID messaging core indexes are 0 through 7.
Roles
end
methods
function Transform5_2
   %  Combine UseLazySister and CrossCoreType into CrossCoreApiEnum
end
function CB_TriggerInit
   %  If the location has changed, force the block to run its init by
   %  changing a parameter. This in turn causes the icon to refresh.
end
function CB_Config
   %  Only show the legacy mask controls if BTC is empty. Challenge is
   %  that while the mask dialog is open and interactive editing is
   %  taking place, the mask workspace does not update until after all
   %  callbacks and initialization code has completed - so we cannot
   %  reliably evaluate the parameter here if the user just entered it.
   %  However, we can just compare the string to the empty expression
   %  e.g. [] and that will have to do for now.
end
function DoInit
   %  Validate block and handle ability to use legacy mask when the
   %  build-time configuration is not specified via classdef. Input
   %  both the current Config as well as Config from before any
   %  BTConfig.Assert calls. The BeforeAssert capture allows detection
   %  of empty build-time configuration (i.e. use dialog)
end
function GetConfigFromLegacyMask
   %  Creates BTC from the block's legacy mask controls
end
function SetBasePeriod
   %  SetBasePeriod  sets the base period given a numeric value. It is
   %  called in a pair with GetBasePeriod as follows:
   %  1- GetBasePeriod captures the base period from a running model
   %  which would *not* allow modification while running
   %  2 - SetBasePeriod is called after simulation ends to record the
   %  captured base period which is received by our internal SFunc
end
function GetBasePeriod
   %  GetBasePeriod is to be called while a model is in compiled
   %  state (or paused simulation) to discover the chosen base period.
end
function GetCoreIndexBlockHandle
   %  Given any block handle, discovers associated CoreIndex block, or
   %  empty if none found.
end
function IsIndirectAddressing
   %  Returns true if model is configured for indirect addressing
end
function GetConfig
   %  Retrieves build-time configuration for CoreIndex - if no block is
   %  present, then configuration defaults are returned
end
function GetCoreIndex
   %  Returns design-time specification of model role (as a number)
end
function GetLimitedProcessorCoreIndex
   %  To support up-integration scenarios where the number of
   %  partitions may far exceed any limit we might set for processor
   %  cores, enable this re-purposing by limiting the core index
end
function CoreIndexMask
   %  CoreIndexMask specifies system role of this model in partitioning context
   %
   %    Documentation for CoreIndexMask/CoreIndexMask
   %       doc CoreIndexMask
   %
end
end
end
