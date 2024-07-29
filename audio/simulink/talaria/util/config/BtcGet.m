function theValue = BtcGet(propertyPath)
%BTCGET Retrieves a BTC property given simple func handle of propertyPath.
%   This function is for use OUTSIDE OF THE MODEL CANVAS as it cannot
%   access mask workspaces. For use on the MATLAB command line, or other
%   callers that have access to global data / base workspace.
%   Because this function is not part of an object, it requires passing the
%   propertyPath argument as a simple function handle such as
%   @ModelSettings.Input.ChannelCount. This enables the function to
%   discover the object involved and call "Get" on it. This function's
%   return value includes calculated defaults.
% Usage:
%   value = BtcGet(@propertyPath);
% Example
%   value = BtcGet(@Config.TuneDefaults.Coefficients);

if (false == isa(propertyPath,'function_handle'))
    error('Passed in propertyPath must be a function handle that starts with an "@", not a [%s]', ...
        class(propertyPath));
end

% retrieve the string content of the function- it is the full propertyPath
functionContent = func2str(propertyPath);
% eliminate the varargin values that appear under some conditions
functionContent = strrep(functionContent,'@(varargin)','');
functionContent = strrep(functionContent,'(varargin{:})','');

% Parse out the object name
objectName = extractBefore(functionContent,'.');
if (isempty(objectName))
    error('Passed in propertyPath must include a dot like @Config.Value... not just [%s]', functionContent);
end

% Parse out the rest of the property path AS A STRING... because from this
% scope we cannot play the function handle game any longer
propertyPath = extractAfter(functionContent,'.');

% retrieve the object from the caller's scope
theObj = evalin('caller',objectName);

% Now that we have the object and propertyPath, perform the Get!
theValue = theObj.Get(propertyPath);


warning('BtcGet is deprecated. Access property normally [%s].', ...
    functionContent);
