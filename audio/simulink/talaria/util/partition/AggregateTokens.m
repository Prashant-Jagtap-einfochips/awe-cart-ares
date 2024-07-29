function AggregateTokens
%  AggregateTokens unifies token files, calculates and saves compatibility
%  Usage:
%   strCompatDefine = AggregateTokens(config,elementType,attributesCell,isXmlExtract)
%  config : config structure with all the metadata
%  elementType : name of XML element to extract and use (e.g. TOP or TSP)
%  attributesCell : cell array of XML attributes to extract for hashing
%  isXmlExtract : boolean true of XML is embedded in header files and must
%  be extracted by [XMLBEGIN] and [XMLEND] markers
