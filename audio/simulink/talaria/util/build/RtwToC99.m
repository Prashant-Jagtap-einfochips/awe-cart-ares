function newName = RtwToC99(inputName)
% ReplaceTypeRtwToC99 performs a string replacement of common rtwtypes with
% their C99 equivalents

oldNames = {'\<real64_T\>','\<real_T\>','\<real32_T\>','\<int8_T\>','\<uint8_T\>','\<int16_T\>','\<uint16_T\>','\<int32_T\>','\<uint32_T\>','\<boolean_T\>'};
newNames = {'double','double','float','int8_t','uint8_t','int16_t','uint16_t','int32_t','uint32_t','uint8_t'};

newName = regexprep(inputName,oldNames,newNames);
