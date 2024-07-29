function createFileChecksums
%  Given a config structure (for rootModelName,newModelPaths,refModelNames),
%  and engineModel names; this function creates a new map object specifying 
%  file-checksums for all derivative models. 
%  From the root model this includes one or more target models and 0-4 engine models. 
%  For referenced models this includes a single target model and 0-4 engine models.
