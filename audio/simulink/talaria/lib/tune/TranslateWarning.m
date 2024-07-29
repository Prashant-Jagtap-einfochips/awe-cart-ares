function [  ] = TranslateWarning( varargin )
%TranslateWarning (For use in Functions triggered by TranslationTrigger blocks.)
%Print warning message during tune data translation.
%  Do not include a final newline as this behavior is already built in.
%  Note that escape sequences such as '\n' will not work here. To emit
%  special characters, pass them in as arguments to %c or %s format
%  specifiers.

%   Copyright 2018-2019 Bose Corporation
coder.extrinsic('gcb')
coder.extrinsic('warning')
coder.allowpcode('plain'); % enable code generation from p-file

    if (coder.target('MATLAB'))
        warning('Translation warning encountered in %s',MaskUtil.GetBlockLinkString(gcb));
        warning(varargin{:});
        return;
    end
    if (coder.target('Sfun'))
        % S-Function (e.g. protected models) do not support "gcb"
        warning(varargin{:});
        return;
    else
        % Generated C-language code
        count = numel(varargin);
        fixedArgs = varargin;
        for i=1:count
            if (ischar(varargin{i}))
                fixedArgs{i} = [varargin{i} char(0)];
            end
        end
        coder.ceval('TranslateWarning',fixedArgs{:});
    end

end
