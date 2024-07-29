function createSubModels
% --------------------------------------------------------------------------
%  Function: createSubModels
% 
%  This function will create sub-models based on the model tags and the
%  information from the function "getSubTags".  
% 
%  Inputs:
%    modelName : The name of the original source model
%    tagModelPath : Filepath of the TAGGED model (for analysis, copies, etc.)
%    config : structure of configuration data, potential fields include:
%        subProp: A structure defining the I/O and data contectivity of the
%           atomic susbsystems. This structure is created by getSubTags().
%        tagList: a list of the unique tags in the model
%        rateFactorMap : lookup table of sample times
%        coreTag : for single-core systems, string tag name of core
%  Outputs:
%    None
