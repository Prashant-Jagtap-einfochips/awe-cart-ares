function getRateFactorMap
% getRateFactorMap gets the rate factor map from a set of sample times
% obtained via "Simulink.BlockDiagram.getSampleTimes" on a compiled model.
%  Given a sample time, the map returns the rate factor as compared to the
%  base rate, aka fastest periodic sample time.
