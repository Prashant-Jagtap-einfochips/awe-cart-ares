function [  ] = InterpretError( varargin )
%InterpretERROR Print error or warning message during tune data
%interpretation Do not include a final newline as this behavior is already built in.
%Note that escape sequences such as '\n' will not work here. To emit
%special characters, pass them in as arguments to %c or %s format
%specifiers.

%   Copyright 2018-2019 Bose Corporation
coder.extrinsic('gcb')
coder.extrinsic('warning')
coder.allowpcode('plain'); % enable code generation from p-file

    if (coder.target('MATLAB'))
        % Running inside MATLAB - Simulation or other.
        warning('Interpretation error encountered in %s\n',MaskUtil.GetBlockLinkString(gcb));
        error(varargin{:});
    end
    if (coder.target('Sfun'))
        % S-Function (e.g. protected models) do not support "gcb"
        error(varargin{:});
    else
        % Generated C-language code
        count = numel(varargin);
        fixedArgs = varargin;
        for i=1:count
            if (ischar(varargin{i}))
                fixedArgs{i} = [varargin{i} char(0)];
            end
        end
        coder.ceval('InterpretError',fixedArgs{:});
    end

end
