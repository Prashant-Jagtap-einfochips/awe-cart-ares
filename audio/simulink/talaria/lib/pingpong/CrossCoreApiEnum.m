classdef CrossCoreApiEnum
%  CrossCoreApiEnum Enumerated class definition for CrossCoreApi
%  Usage:
%     value = CrossCoreApiEnum.<enumeration-member>;
%     value = "<enumeration-member>";
%     value = <uint32 equivalent of enumeration-member>;
%  API descriptions:
%    None : no API generated to manage cross-core communications. Cross core
%       model-access pointers declared as extern pointers only.
%    FourList : ping-pong API generated, but with no local communication
%       between sisters to coordinate remove communications. Instead every
%       core must individually handle off-chip communications
%    FourListChipManager : API generated to support 4 cross-core transfer
%       lists: local-src,local-dst,remote-src,remote-dst. LazySister sends
%       own local buffer management details to sister core via chip-local
%       transfer lists. The chip-manager core then coordinates off-chip
%       communcations centrally for all cores on the same chip. This scheme
%       only supports a maximum of two cores per chip.
%
%    Documentation for CrossCoreApiEnum
%       doc CrossCoreApiEnum
%
%
% EnumerationMemberList: None,FourList,FourListChipManager
end
