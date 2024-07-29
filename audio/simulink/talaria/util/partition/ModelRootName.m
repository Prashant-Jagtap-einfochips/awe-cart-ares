function ModelRootName
% MODELROOTNAME Returns current model name to the left of any underscores.
%    This functions works in the context of partitioned model names when the
%    source model's name does *not* contain any underscores. In other words,
%    it returns the "Model" from a bdroot name like "Model_1_1".
