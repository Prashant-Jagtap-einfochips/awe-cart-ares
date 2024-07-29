function partitionModel
% partitionModel Partitions source model into separate core models
%    Walks through the steps of model partitioning that happen *after* the
%    <modelName>_atomicList assignments file has been filled out. These
%    steps include tagging atomic blocks with their assignments and creating
%    the intermediate models.
% 
%    Usage:
%    [N,T,R,C] = partitionModel(M,O) partitions model M into multiple models to be
%    deployed to separte cores, listing each new model name in the cell
%    array N. The name T of the model copy which has been analyzed and
%    simulated is also returned for further processing. Note that if the 
%    <model-name>_atomicList.m function exists, partitioning will be for
%    multi-core deployment. Data from model references R also returned.
%    Constructs a struct C to hold common parameters (checksum map, tag list, etc)
%    to make it easier to pass input arguments. O is an instance of
%    PartitionGenerateOptions containing options such as whether to use the
%    parallel computing toolbox and whether to perform a clean build or to 
%    skip build steps based on the checksum generated from previous build.
% 
%    Copyright 2018-2019 Bose Corporation
