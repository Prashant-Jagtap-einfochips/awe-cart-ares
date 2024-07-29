function EvalToFlatNumeric
%  EvalToFlatNumeric: Helper function for BoseFlatStructure.tlc.
%  This is a workaround to "defect" [MathWork's Case# 02525444] where signal
%  lines' InitialValues are saved to the model file but cannot be reloaded.
%  These initial values were used for static C lagnuage initializers in
%  generated code. This workaround stores a signal's initial value in a
%  custom storage class property (CodeInitialValue) which is then flattened
%  (by calling this function) and swapped in as a signal InitialValue by the
%  BoseFlatStructure.tlc custom storage class TLC file.
