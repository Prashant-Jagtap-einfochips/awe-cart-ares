function removeTargetProcessors()
% removeTargetProcessors is a completely made up function to take a shot at
% hygenic living during the upgrade to 2020b by removing any processors
% that were added by the project in "registerTargetProcessors".


    sp = target.get('Processor','Analog Devices-SharcPlus');
    if (false == isempty(sp))
        fprintf('--- Removing SharcPlus processor ---\n');
        target.remove(sp,'SuppressOutput',true);
    end

    sp = target.get('Processor','Analog Devices-GUL');
    if (false == isempty(sp))
        fprintf('--- Removing GUL processor ---\n');
        target.remove(sp,'SuppressOutput',true);
    end

    sp = target.get('Processor','Analog Devices-GLXP');
    if (false == isempty(sp))
        fprintf('--- Removing GLXP processor ---\n');
        target.remove(sp,'SuppressOutput',true);
    end
end

