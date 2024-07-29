function aModelHandle = GenPartsModelCode(modelName,varargin)
% GenPartsModelCode wraps slbuild in retry for DSM problem.
% Yet another workaround for MathWork's "mapping" object. DataStore
% Memories sometimes disppear from the mapping object and will not come
% back. The "simplest" workaround so far is to save the model, close,
% and re-open. See TALARIA-582, TALARIA-589
    try
        % TODO -- use slbuild instead?
        aModelHandle = LoadPartsModel(modelName,true);
        slbuild(aModelHandle,varargin{:});
    catch ME
        % There might be multiple ways to manifest this error. Instead of
        % trying to dig around in the exception causes to match the 
        % indentifier "coderdictionary:api:invalidMappingDataStoreName",
        % just get the basic report and search for a string fragment that
        % might get split across multiple lines.
        searchExp = strrep('data store does not exist',' ','[\s\n]*');
        reportStr = getReport(ME,'basic');        
        tryRebuild = (false == isempty(regexp(reportStr,searchExp,'once')));        
        if (tryRebuild)
            % This is the exception thrown when a DSM disappears. It's
            % also possible that the DSM really is not in the model, so
            % we'll just try this once.
            % Save, close, open, try again
            fprintf('Detected potential "disappearing DSM problem". Attempting workaround...\n');
            % Ensure we have a model name instead of a numeric handle
            modelName = get_param(modelName,'Name');
            save_system(aModelHandle);
            close_system(aModelHandle);
            aModelHandle = LoadPartsModel(modelName,true);
            slbuild(aModelHandle,varargin{:});
        else
            rethrow(ME);
        end
    end
end