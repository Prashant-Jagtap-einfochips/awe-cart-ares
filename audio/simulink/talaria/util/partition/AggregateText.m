function AggregateText
% AGGREGATETEXT Concatenates content from text files named by model
%    Model partitioning generates multiple derivative models for deployment
%    across processing units. This routine pulls the pieces back together in
%    terms of text-file artifacts created by each of these derivative
%    models.
% 
%    Usage:
%     [theText] = AggregateText(theText,modelPaths,isModelRef,inputSearchFile,beginText,endText)
% 
%    Arguments:
%     theText : char array to hold the text
%     config : MATLAB struct with the following fields
%       modelPaths : cell array of derivative model paths
%       isModelRef : boolean true = modelPaths are referenced models
%                    boolean false = modelPaths are top models (i.e. normal)
%       inputSearchFile : text file name to search for including wildcards
%       fileDescription : short file description / type for print messages
%       beginText : optional marker text indicating start of text to capture
%       endText : options marker text indicating end of text to capture
