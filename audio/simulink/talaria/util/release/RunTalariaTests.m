function result = RunTalariaTests(parentFolderNames,testSelector)
% run_tests runs all tests found in the test folder

%   Copyright 2019 Bose Corporation

import matlab.unittest.TestSuite;
import matlab.unittest.TestRunner;

if (nargin < 1)
    parentFolderNames = 'test';
end
if (nargin < 2)
    testSelector = [];
end

parentFolderNames = string(parentFolderNames);
alltests = [];
for i=1:numel(parentFolderNames)
    aFolder = parentFolderNames(i);
    if (isempty(testSelector))
        newTests = TestSuite.fromFolder(aFolder,'IncludingSubfolders',true);
    else
        newTests = TestSuite.fromFolder(aFolder,testSelector,'IncludingSubfolders',true);
    end
    alltests = [alltests newTests]; %#ok<AGROW> 
end
fprintf('Running %d tests...\n',numel(alltests));
textRunner = TestRunner.withTextOutput;
result = textRunner.run(alltests);

% Since we made a "withTextOutput" running anyway, finish up with a nice
% table output of the summarized result
disp(table(result));
