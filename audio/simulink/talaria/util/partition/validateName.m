function validateName
% validateName validates that the name is a valid C function name
%    V = validateName(P) checks the suggested name P for invalid characters
%    for a C function.  If found, replaces with valid syntax and returns
%    name V.  Otherwise returns the original name.
% 
%    Valid characters include
%       number: 48 ~ 57
%       the _ 95
%       Letters 65 ~ 90
%       letters 97 ~ 122
