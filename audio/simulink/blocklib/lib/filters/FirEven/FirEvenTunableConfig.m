classdef FirEvenTunableConfig < FirEvenConfig
% FirEvenTunableConfig Build-time configuration class for the FirEvenTunable block
% Usage:
%    aConfig = FirEvenTunableConfig
%

%   Copyright 2023 Bose Corporation

  properties (GetObservable,SetObservable)
    FirEvenCoeffs 
    MaxNumFilters {mustBeInteger} = 2
    TuneVarActiveCoeffSet {mustBeInteger}
  end
  properties(GetObservable, SetObservable, Dependent)
    TuneVarFirEvenCoeffsFlipped
  end
  properties(GetObservable, SetObservable, Dependent, SetAccess=private, Hidden)
    PostOverlayRequired logical
  end
  methods
    %----------------------------------------------------------------------
    function this = FirEvenTunableConfig()
        % FirEvenTunableConfig constructor: assign component property objects
        % and adjust component property defaults and/or lock values
        this.Default(@this.FirEvenCoeffs, @() this.GetDefaultFirEvenCoeffs());
        this.Default(@this.TuneVarActiveCoeffSet, @() mod(0:this.NumChannels-1, this.MaxNumFilters));
    end
    %----------------------------------------------------------------------
    function Validate(this)
        % Validate called from mask initialization to check settings ok
        Validate@FirEvenConfig(this);

        % Check that MaxNumFilters
        assert(size(this.FirEvenCoeffs,1) <= this.MaxAvgCoeffLength,...
            "property [FirEvenCoeffs] dimension 2 of value %d must be less than or equal to property [MaxAvgCoeffLength] of value %d",...
            size(this.FirEvenCoeffs,1), this.MaxAvgCoeffLength);
        assert(size(this.FirEvenCoeffs,2) <= this.MaxNumFilters, ...
            "property [FirEvenCoeffs] dimension 2 of value %d must be less than or equal to property [MaxNumFilters] of value %d",...
            size(this.FirEvenCoeffs,2), this.MaxNumFilters);
        assert(numel(this.TuneVarActiveCoeffSet) == this.NumChannels,...
            "property [TuneVarActiveCoeffSet] of length %d must have length equal to property [NumChannels] of value %d ",...
            numel(this.TuneVarActiveCoeffSet), this.NumChannels);
    end
    %----------------------------------------------------------------------
    function DoMaskInit(this,blockHandle)
        % DoMaskInit called from mask initialization to perform automation
        % of code generation settings and other canvas interactions.
        blockType = get_param(blockHandle, 'MaskType');
        if (blockType == "Blocklib FirEven")
            DoMaskInit@FirEvenConfig(this,blockHandle);
        elseif (blockType == "Blocklib FirEvenTunable")
            DoMaskInit@BtcBaseClass(this,blockHandle);
        else
            error("Unsupported Block %s" , blockType);
        end      
    end
    %----------------------------------------------------------------------
    function val = GetDefaultFirEvenCoeffs(this)
        % Create default pass through coeffs
        % [1 1 1 1 ...]
        % [0 0 0 0 ...]
        % [ ......    ]

        val = zeros(this.MaxAvgCoeffLength, this.MaxNumFilters);
        val(1, :) = 1;
    end
    %----------------------------------------------------------------------
    function val = get.TuneVarFirEvenCoeffsFlipped(this)
        AlwaysFlip = true;
        IsCoeffsRawFlipped = false;
        val = FirEvenTranslation(this.FirEvenCoeffs, this.Breed, this.MaxNumFilters, this.MaxAvgCoeffLength, this.FrameSize, IsCoeffsRawFlipped, AlwaysFlip);
    end
    %----------------------------------------------------------------------
    function val = get.PostOverlayRequired(this)
        if this.Breed == AsdHwEnum.Hexagon
            val = true;
        else
            val = false;
        end
    end
  end
end
