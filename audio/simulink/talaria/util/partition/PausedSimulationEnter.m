function PausedSimulationEnter
%  PausedSimulationEnter simulates the specified model and returns with the
%  model STILL paused just after its initial frame. The caller can then
%  proceed to gather information from the model that's available in the
%  simulation / compiled state such as sample times. The caller is
%  responsible for calling PausedSimulationExit() when they are done with
%  the required compiled state of the model.
%  This function adds a temporary block which must be removed by the
%  matching call to PausedSimulationExit
