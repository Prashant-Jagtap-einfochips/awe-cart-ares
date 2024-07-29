classdef FASMask
    % Mask for Fresh Air Speakers (FAS)

    methods (Static)
        function DefaultFlagCallback(H,flagName,paramName)
            % If the checkbox is set to 'on', the default configuration is
            % used, otherwise the configuration can be modified.
            thisMask = Simulink.Mask.get(H);
            flag = get_param(H,flagName);
            config = thisMask.getParameter(paramName);
            if (strcmp(flag,'on'))
                config.Enabled = 'off';
            else
                config.Enabled = 'on';
            end
        end
    end
end