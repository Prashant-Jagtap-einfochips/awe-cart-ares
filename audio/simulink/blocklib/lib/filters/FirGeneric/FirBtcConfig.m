classdef FirBtcConfig < FirConfig
% Blocklib FirBtc----------------------------------------------------------
% Wrapper around the Blocklib FirGeneric block. Setting coefficients, 
% coefficient mapping and input mapping is handled in the Build Time 
% Configuration. 
% Generic multi-channel FIR block supports multiple inputs, outputs and 
% coefficient sets. Some inputs may drive multiple outputs, and this input 
% sharing is specified by an input mapping per output. Some filters may be 
% shared among multiple outputs, and this filter sharing is specified by a 
% filter mapping per output. Input sharing assumes all filters sharing the 
% same input have the same length to support state buffer sharing. 
% 
% Input signals:
%   AudioIn: input channels [frame size x number of inputs] 
% 
% Output signals: 
%   y: output channels [frame size x number of outputs]
%
% Btc Paramaters
%   CoeffsRaw: [sum(CoeffsLengths) x 1] Raw coefficient sets. coefficients 
%       are in standard format stored consecutively in a 1d array 
%       Example: [h0[0], h0[1], ... h0[N0-1], h1[0], h1[1], ... h1[N1-1] ......]
%   CoeffsLengths: [number of filters x 1] Array of filter lengths
%       Example: [N0, N1, N2 ....] where Ni is the length of the ith filter 
%   TuneVarInputMapping: [number of outputs x 1] Zero-based input mapping for each
%       output. Output[i] = Fir(Input[TuneVarInputMapping[i]], ...)
%   TuneVarCoeffsMapping: [number of outputs x 1] Zero-based filter mapping for each
%       output. Output[i] = Fir(Filter[TuneVarCoeffsMapping[i]], ...)
%   BankNumber: [1x1] Value assigned must be an integer greater than or equal to 0.
%       A non-zero value creates a separate TOP bank for FIR target coefficients, 
%       which can then be mapped to different memory section from the rest of the TOPs.
%
% Breed Specific Requirements:
%   Different hardware targets have requirements on coefficient format,
%   lengths and etc. These requirements are handled inside the Build-Time
%   Configuration. See FirTranslation.m for more information on Breed
%   Specific Coefficient Requirements for Fir.
%--------------------------------------------------------------------------

  properties (GetObservable,SetObservable)
    % column vector of all filter coefficients. These coefficients may be of
    % different length. 
    CoeffsRaw       (:,1) single
    
    % row vector, where the i-th value specifies the length of the i-th
    % filter.
    CoeffsLengths   (1,:) {mustBeNonnegative, mustBeInteger}
    
    % row vector, maps each output of the block to an input. The i-th value
    % of the TuneVarInputMapping vector returns the index (zero-indexed) of the 
    % input that maps to the i-th output 
    TuneVarInputMapping    (1,:) {mustBeNonnegative, mustBeInteger} 

    % row vector, maps each output of the block to a filter. The i-th value
    % of the TuneVarCoeffsMapping vector returns the index (zero-indexed) of the 
    % filter that maps to the i-th output 
    TuneVarCoeffsMapping   (1,:) {mustBeNonnegative, mustBeInteger}

    % Bank Number for the FirCoeffsTarget TOP
    BankNumber {mustBeInteger, mustBeNonnegative}   = 0
    
  end
  properties(GetObservable, SetObservable, Dependent, Hidden)
    % Coefficients after translation to target specific format. Flipping
    % Dependent on which Breed is selected.
    TuneVarCoeffsTarget    (:,1) single

    % Coefficients after translation to target specific format. Flipping
    % occurs regardless of which Breed is selected.
    TuneVarCoeffsFlipped   (1,:) single

    % CoeffsLengths after translating to use as default value in TuneVar
    TuneVarCoeffsLengths   (1,:) {mustBeNonnegative, mustBeInteger}

    % NumFilters is the total number of specified filters
    NumFilters (1,1) {mustBeInteger}
  end
  properties(GetObservable, SetObservable, Dependent, SetAccess=private, Hidden)
    PostOverlayRequired logical
  end
  methods
    %----------------------------------------------------------------------
    function this = FirBtcConfig()
        % FirBtcConfig constructor: assign component property objects
        % and adjust component property defaults and/or lock values
        DefaultNumCoeffSets = 2;
        this.Default(@this.CoeffsRaw,@() repmat([1,zeros(1,this.MaxAvgCoeffLength-1)],1,DefaultNumCoeffSets));
        this.Default(@this.CoeffsLengths, @() repmat(this.MaxAvgCoeffLength,1,DefaultNumCoeffSets));
        this.Default(@this.TuneVarInputMapping, @() mod(0:(this.NumOutputs-1), this.NumInputs));
        this.Default(@this.TuneVarCoeffsMapping, @() mod(0:(this.NumOutputs-1), numel(this.TuneVarCoeffsLengths)));
    end
    %----------------------------------------------------------------------
    function Validate(this)
        % Validate called from mask initialization to check settings ok
        
        % MaxAvgCoeffLength should be multiple of 4 and greater than
        % framesize for certain Breeds
        if any(this.Breed == [AsdHwEnum.ArmNeonV7V8, AsdHwEnum.Hexagon])
            assert( mod(this.MaxAvgCoeffLength, 4) == 0 , ...
                "For Fir Breed of [%s], property [MaxAvgCoeffLength] of value %d is not a multiple of 4", string(this.Breed), this.MaxAvgCoeffLength );
            assert( this.MaxAvgCoeffLength >= this.FrameSize, ...
                "For Fir Breed of [%s], property [MaxAvgCoeffLength] of value %d is not a greater than property [FrameSize] of value", string(this.Breed), this.MaxAvgCoeffLength, this.FrameSize);
        end

        % MaxAvgCoeffLength must be greater than sum of CoeffsLengths
        assert(this.MaxAvgCoeffLength*this.NumFilters >= sum(this.CoeffsLengths), ...
            "Sum of property [CoeffsLengths] is equal to %d, which is not less than or equal to [MaxAvgCoeffLength] x [NumFilters] = %d", sum(this.CoeffsLengths), this.MaxAvgCoeffLength*this.NumFilters);
        assert(this.MaxAvgCoeffLength*this.NumFilters >= sum(this.TuneVarCoeffsLengths), ...
            "Sum of property [TuneVarCoeffsLengths] is equal to %d, which is not less than or equal to [MaxAvgCoeffLength] x [NumFilters] = %d", sum(this.CoeffsLengths), this.MaxAvgCoeffLength*this.NumFilters);

        % Coefficients must match CoeffsLengths 
        if not(numel(this.CoeffsRaw) >= sum(this.CoeffsLengths))
            error("For Blocklib Fir Block, total number of coefficients of property [CoeffsRaw] is %d which should be greater than or equal to %d, " + ...
                "the total number of coefficients specified by property [CoeffsLengths].", ...
                numel(this.CoeffsRaw), sum(this.CoeffsLengths))
        end
        if not(numel(this.TuneVarCoeffsTarget) >= sum(this.TuneVarCoeffsLengths))
            error("For Blocklib Fir Block, total number of coefficients of property [CoeffsTarget] is %d which should be greater than or equal to %d, " + ...
                "the total number of coefficients specified by property [TuneVarCoeffsLengths].", ...
                numel(this.CoeffsTarget), sum(this.TuneVarCoeffsLengths))
        end        
        Validate@FirConfig(this);
    end
    %----------------------------------------------------------------------
    function DoMaskInit(this,blockHandle)
        % DoMaskInit called from mask initialization to perform automation
        % of code generation settings and other canvas interactions.
        blockType = get_param(blockHandle, 'MaskType');
        if (blockType == "Blocklib Fir")
            DoMaskInit@FirConfig(this, blockHandle);
        elseif (blockType == "Blocklib FirBtc")
            DoMaskInit@BtcBaseClass(this, blockHandle);
        else
            error("Unsupported Block %s", blockType);
        end
    end
    %----------------------------------------------------------------------
    function val = get.TuneVarCoeffsLengths(this)
        IsCoeffsRawFlipped = false;
        [~, CoeffsTargetLengths] = FirTranslation(  this.CoeffsRaw, ...
                                                    this.CoeffsLengths,...
                                                    this.Breed, ...
                                                    this.NumFilters, ...
                                                    this.MaxAvgCoeffLength, ...
                                                    this.FrameSize, ...
                                                    IsCoeffsRawFlipped);
        val = CoeffsTargetLengths;
    end
    %----------------------------------------------------------------------
    function val = get.TuneVarCoeffsTarget(this)
        IsCoeffsRawFlipped = false;
        val = FirTranslation(   this.CoeffsRaw, ...
                                this.CoeffsLengths,...
                                this.Breed, ...
                                this.NumFilters, ...
                                this.MaxAvgCoeffLength, ...
                                this.FrameSize, ...
                                IsCoeffsRawFlipped);

    end
    %----------------------------------------------------------------------
    function val = get.TuneVarCoeffsFlipped(this)
        IsCoeffsRawFlipped = false;
        AlwaysFlip = true;
        val = FirTranslation(   this.CoeffsRaw, ...
                                this.CoeffsLengths,...
                                this.Breed, ...
                                this.NumFilters, ...
                                this.MaxAvgCoeffLength, ...
                                this.FrameSize, ...
                                IsCoeffsRawFlipped,...
                                AlwaysFlip);
    end
    %----------------------------------------------------------------------
    function set.TuneVarCoeffsLengths(this,val)
        % Backwards compatability
        warning("Deprecated setting of FirBtc and FirTunable property [TuneVarCoeffsLengths]. This is now a Dependent property, set property [CoeffsLengths] instead.");
        this.CoeffsLengths = val;
    end
    %----------------------------------------------------------------------
    function val = get.NumFilters(this)
        val = numel(this.CoeffsLengths);
    end
    %----------------------------------------------------------------------
    function val = get.PostOverlayRequired(this)
        if this.Breed == AsdHwEnum.Hexagon
            val = true;
        else
            val = false;
        end
    end  
    %----------------------------------------------------------------------
  end

properties (GetObservable, SetObservable, Dependent, Hidden) % [Rename-Compatibility:1]
   InputMapping
   CoeffsMapping
   CoeffsTarget
end
methods
   function set.InputMapping(obj,val)
      obj.TuneVarInputMapping = val;
      warning('Deprecated FirBtcConfig property [InputMapping] used. Update to use [TuneVarInputMapping]')
   end
   function val = get.InputMapping(obj)
      val = obj.TuneVarInputMapping;
      warning('Deprecated FirBtcConfig property [InputMapping] used. Update to use [TuneVarInputMapping]')
   end
   function set.CoeffsMapping(obj,val)
      obj.TuneVarCoeffsMapping = val;
      warning('Deprecated FirBtcConfig property [CoeffsMapping] used. Update to use [TuneVarCoeffsMapping]')
   end
   function val = get.CoeffsMapping(obj)
      val = obj.TuneVarCoeffsMapping;
      warning('Deprecated FirBtcConfig property [CoeffsMapping] used. Update to use [TuneVarCoeffsMapping]')
   end
   function set.CoeffsTarget(obj,val)
      obj.TuneVarCoeffsTarget = val;
      warning('Deprecated FirBtcConfig property [CoeffsTarget] used. Update to use [TuneVarCoeffsTarget]')
   end
   function val = get.CoeffsTarget(obj)
      val = obj.TuneVarCoeffsTarget;
      warning('Deprecated FirBtcConfig property [CoeffsTarget] used. Update to use [TuneVarCoeffsTarget]')
   end
end % [Rename-Compatibility:1]

end
