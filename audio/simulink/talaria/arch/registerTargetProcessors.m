function processor = registerTargetProcessors(varargin)
% This function was generated using target data export via target.upgrade
% on the previous sl_customizations file. However, then we did this:
%  * make double 32-bit
%  * set to Little endian
%  * overwrite any pre-existing SharcPlus registration
%  * rename from registerUpgradedTargets to registerTargetProcessors

    % Create target.LanguageImplementation "Analog Devices-SharcPlus"
    languageimplementation = target.create("LanguageImplementation");
    languageimplementation.AtomicFloatSize = 32;
    languageimplementation.AtomicIntegerSize = 32;
    languageimplementation.DataTypes.Char.Size = 8;
    languageimplementation.DataTypes.Double.Size = 32;
    languageimplementation.DataTypes.Float.Size = 32;
    languageimplementation.DataTypes.Half.IsSupported = false;
    languageimplementation.DataTypes.Half.Size = 16;
    languageimplementation.DataTypes.Int.Size = 32;
    languageimplementation.DataTypes.Long.Size = 32;
    languageimplementation.DataTypes.LongLong.IsSupported = false;
    languageimplementation.DataTypes.LongLong.Size = 64;
    languageimplementation.DataTypes.Pointer.Size = 32;
    languageimplementation.DataTypes.PtrDiffT.Size = 32;
    languageimplementation.DataTypes.Short.Size = 16;
    languageimplementation.DataTypes.SizeT.Size = 32;
    languageimplementation.WordSize = 32;
    
    % Added
    languageimplementation.Endianess = "Little";

    languageimplementation.Name = "Analog Devices-SharcPlus";
    % Check if it's already there!
    sp = target.get('Processor','Analog Devices-SharcPlus');
    if (isempty(sp))
        % Create target.Processor "Analog Devices-SharcPlus"
        processor = target.create("Processor");
        processor.LanguageImplementations(1) = languageimplementation;
        processor.Manufacturer = "Analog Devices";
        processor.Name = "SharcPlus";
        % Add the target objects to MATLAB memory
        fprintf('--- Adding SharcPlus processor ---\n');
        target.add(processor,'SuppressOutput',true, varargin{:});
    else
        warning('--- Overwriting existing SharcPlus processor ---');
        sp.LanguageImplementations(1) = languageimplementation;
    end

    languageimplementation.Name = "Analog Devices-GUL";
    % Check if it's already there!
    sp = target.get('Processor','Analog Devices-GUL');
    if (isempty(sp))
        % Create target.Processor "Analog Devices-GUL"
        processor = target.create("Processor");
        processor.LanguageImplementations(1) = languageimplementation;
        processor.Manufacturer = "Analog Devices";
        processor.Name = "GUL";
        % Add the target objects to MATLAB memory
        fprintf('--- Adding GUL processor ---\n');
        target.add(processor,'SuppressOutput',true, varargin{:});
    else
        warning('--- Overwriting existing GUL processor ---');
        sp.LanguageImplementations(1) = languageimplementation;
    end

    languageimplementation.Name = "Analog Devices-GLXP";
    % Check if it's already there!
    sp = target.get('Processor','Analog Devices-GLXP');
    if (isempty(sp))
        % Create target.Processor "Analog Devices-GLXP"
        processor = target.create("Processor");
        processor.LanguageImplementations(1) = languageimplementation;
        processor.Manufacturer = "Analog Devices";
        processor.Name = "GLXP";
        % Add the target objects to MATLAB memory
        fprintf('--- Adding GLXP processor ---\n');
        target.add(processor,'SuppressOutput',true, varargin{:});
    else
        warning('--- Overwriting existing GLXP processor ---');
        sp.LanguageImplementations(1) = languageimplementation;
    end
end
