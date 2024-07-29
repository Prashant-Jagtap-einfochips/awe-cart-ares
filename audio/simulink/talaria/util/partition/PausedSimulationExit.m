function PausedSimulationExit
%  This function matches with PausedSimulationEnter to enable gathering of
%  model data only available during compiled or active simulation state.
%  Open the analysis by PausedSimulationEnter and ensure to close it with
%  PausedSimulationExit when done. This terminating function removes a
%  temporary block added to the model and cleans up other states.
