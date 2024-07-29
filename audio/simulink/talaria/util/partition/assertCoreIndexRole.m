function assertCoreIndexRole
% assertCoreIndexRole create/modify core index block and comment out tuning
% blocks.
%    Sets the core index for the model, adding a core index block if one
%    does not exist.
%    Source Model Role: everything stays
%    Partitioned Core Role: Translation and Interpretation blocks removed
%    Translation Engine Role: Everything *except* translation blocks removed
%    Interpretation Engine Rol: All *except* interpretation blocks removed
