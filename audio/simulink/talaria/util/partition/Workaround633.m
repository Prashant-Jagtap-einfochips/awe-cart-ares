function Workaround633
% WORKAROUND633 Attempt to repair MathWorks code mapping object
%  [TALARIA-633] workaround: PingPongStruct CSC is wiped into zombie
%  state for select signals sometimes after all the object deleting
%  in assertCoreIndexRole. The mapping object knows about the
%  mapping, but the Embedded Coder GUI and model do not.
