function RTC_CreateXML
%  ** Note [TALARIA-260] Use of this XML is deprecated. **
%  ** Please use <modename>_RTC_Tokens.xml instead.     **
%  RTC_CreateXML Create an XML Runtime Control (RTC) specification document. 
%  The document lists all RTC services, their associated payloads, and any
%  additional characteristics captured in the model.
%  NOTE that the values captured are *not* evaluated. The XML captures raw
%  user-entered strings that define the RTC - not actual values used in
%  operation of the RTC service. See the generated code for "actuals".
%  See also FEATUREPATHUTIL.CATALOGRTC
