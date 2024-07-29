function keepAndPrepare
% --------------------------------------------------------------------------
%  Function: keepAndPrepare
%  This is a helper function to the main function 'createSubModels' it
%  represents the complement to deleteAndStub. This routine keeps the block
%  but also renames cross-core buffers to make them multi-model unique
