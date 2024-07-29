function MultiSrcEQ_init(numIn)
    % build time paramters for the Multi-source entertainment input EQ
    % number of Entertainment input source
    num_ent_audio_sources = 7;
    % number of enetertainment input channel per audio source
    num_ent_channel = numIn ;
    % number of stages per audio source
    num_stages_per_audio_source = 12;
    % maximum number of stages allowed
    max_num_ent_input_eq_stages = num_ent_audio_sources * num_stages_per_audio_source;
    %passthrough raw SOS
    passthru_raw_sos = [1 0 0 1 0 0];
    % raw SOS for Multi-source EQ
    MultiSrcrawsosCoefficient = repmat(passthru_raw_sos,num_ent_channel*num_ent_audio_sources,1);
    % Number of stages for Multi-source EQ
    MultiSourceNumStages = ones(1,num_ent_audio_sources*num_ent_channel);
    
    % Set the default coeff set to passthrugh for pooliir per channel
    DefaultrawsosCoeff = {};
    DefaultrawsosCoeff = struct('Value',repmat(passthru_raw_sos,num_ent_channel,1));
    
    DefaultrawNumStages= {};
    DefaultrawNumStages = struct('Value',ones(1,num_ent_channel));
   
    % call Pooliir translation fucntion for translating the rawSOS
    [DefaultPoolNumStages,DefaultPoolsosCoeff] = pooliirTranslationFcn(DefaultrawsosCoeff, DefaultrawNumStages, ...
                                                     num_stages_per_audio_source);	
    % Create a Multi-Source EQ configuration struct
    MultiSourceEQConfig = struct('num_ent_audio_sources',num_ent_audio_sources,...
                                 'num_ent_channel',num_ent_channel,...
                                 'num_stages_per_audio_source',num_stages_per_audio_source,...
                                 'max_num_ent_input_eq_stages',max_num_ent_input_eq_stages,...
                                 'MultiSrcrawsosCoefficient',MultiSrcrawsosCoefficient,...
                                 'MultiSourceNumStages',MultiSourceNumStages,...
                                 'DefaultPoolNumStages',DefaultPoolNumStages,...
                                 'DefaultPoolsosCoeff',DefaultPoolsosCoeff);
                            
    % assign the Multi-Source EQ configuration  to Base workspace 													 
    assignin('base', 'MultiSourceEQConfig', MultiSourceEQConfig);
