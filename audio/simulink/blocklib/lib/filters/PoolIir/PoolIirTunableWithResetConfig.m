classdef PoolIirTunableWithResetConfig < PoolIirWithResetConfig
    % PoolIirTunableWithResetConfig Build-time configuration class for the PoolIirWithVariants block
    % Usage:
    %    aConfig = PoolIirTunableWithResetConfig
    %

    %   Copyright 2023 Bose Corporation

    properties (Description='TuneDefault', GetObservable, SetObservable)
        TuneVarNumStages
        TuneVarSosCoefficients
    end
    properties(GetObservable, SetObservable, Dependent = true, Hidden = true)
        % Delete the following properties when all features have upgraded
        % to the latest PoolIirTunable (with variants)
        DefaultNumStages        single
        DefaultSosCoefficients  single
    end

    methods
        %----------------------------------------------------------------------
        function this = PoolIirTunableWithResetConfig()
            % PoolIirTunableWithResetConfig constructor: assign component property objects
            % and adjust component property defaults and/or lock values
            this.Default(@this.TuneVarNumStages, @()repmat(floor(this.MaxNumStages/this.NumChannels), 1, this.NumChannels));
            this.Default(@this.TuneVarSosCoefficients, @() repmat([1 0 0 1 0 0], this.MaxNumStages, 1));
        end
        %----------------------------------------------------------------------
        function Validate(this)
            % Validate called from mask initialization to check settings ok

            %.End with a call to our base class to continue validation checks
            %.including any violations of property locking.
            Validate@PoolIirWithResetConfig(this);
        end
        %----------------------------------------------------------------------
        function DoMaskInit(this,blockHandle)
            % DoMaskInit called from mask initialization to perform automation
            % of code generation settings and other canvas interactions.

            % The same object is being used for both container block and inner block.
            % So, the class hierarchy is being flattened. Hence, the function is
            % called by both container and the inner block.
            blockType = get_param(blockHandle, 'MaskType');
            if (blockType == "Bose Pool IIR With Reset")
                DoMaskInit@PoolIirWithResetConfig(this,blockHandle);
            elseif (blockType == "Bose Pool IIR Tunable With Reset")
                DoMaskInit@BtcBaseClass(this,blockHandle);
            else
                error("Unsupported Block %s" , blockType);
            end
        end
    end

    % Remove this method when all features have upgraded to the latest
    % PoolIirTunable (with variants)
    methods
        function set.DefaultNumStages(obj,val)
            obj.TuneVarNumStages = val;
            warning('Deprecated PoolIirTunableWithResetConfig property [DefaultNumStages] used. Update to use [TuneVarNumStages]')
        end
        function val = get.DefaultNumStages(obj)
            val = obj.TuneVarNumStages;
            warning('Deprecated PoolIirTunableWithResetConfig property [DefaultNumStages] used. Update to use [TuneVarNumStages]')
        end

        function set.DefaultSosCoefficients(obj,val)
            obj.TuneVarSosCoefficients = val;
            warning('Deprecated PoolIirTunableWithResetConfig property [DefaultSosCoefficients] used. Update to use [TuneVarSosCoefficients]')
        end
        function val = get.DefaultSosCoefficients(obj)
            val = obj.TuneVarSosCoefficients;
            warning('Deprecated PoolIirTunableWithResetConfig property [DefaultSosCoefficients] used. Update to use [TuneVarSosCoefficients]')
        end
    end
end
