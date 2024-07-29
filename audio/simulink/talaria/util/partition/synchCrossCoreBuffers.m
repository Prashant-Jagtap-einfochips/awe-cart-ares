function synchCrossCoreBuffers
% SYNCHCROSSCOREBUFFERS Checks for places where cross core buffers need
% synchronization and adds delay when necessary
%    Walks through the tagged model and computes the delay that will be
%    created by sending a data buffer from one core to another.  If a
%    subsystem with inputs from two sources that have different cross core
%    delays is encounterd, a delay block is added to keep them in synch.
