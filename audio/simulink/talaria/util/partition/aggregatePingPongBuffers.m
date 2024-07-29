function aggregatePingPongBuffers
%  aggregatePingPongBuffers creates a unified pingpongbuffer xml - actually
%  it just picks one and then cleans it up to speak for all cores.
%  Provide the name of the original source file as well as a cell array of
%  the already-code-generated partitioned models.
