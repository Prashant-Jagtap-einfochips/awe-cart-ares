function GetModelChecksumMap
%  GetModelChecksumMap Returns map of model names to Simulink checksums
%     Assumption for this function is that the tagModelName model is in a
%     compiled state (e.g. paused simulation mode) so that calculation of
%     checksums takes a trivial amount of time. Otherwise, it's very slow!
