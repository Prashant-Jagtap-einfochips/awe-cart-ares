function runTestMethods( TestCase, varargin )
%RUNTESTMETHODS Runs test methods in a particular test case 
%   RUNTESTMETHODS(TESTCASE, loggingLevel) runs all test methods in the
%   matlab.unittest.TestCase object TESTCASE that are not marked with the
%   tag 'benchmark'.  TESTCASE can either be a single string representing
%   the test case to run, or a cell array of strings for multiple to run
%   together. 'loggingLevel' is a number specifying the verbosity level for 
%   test results; right now this supports level 0 (default, concise) and
%   greater than 0 (more detailed with test summary).
%
%   e.g.
%   runTestMethods('HelloWorldTestCase');
%     Runs all test methods in HelloWorldTestCase.m class that do not have
%     a tag 'Benchmark'.
%
%   runTestMethods('HelloWorldTestCase', 1);
%     Runs all test methods in HelloWorldTestCase.m class and extracts 
%     pass/fail information for nightly build postprocessor.
%
%   runTestMethods({'TestCase1', 'TestCase2'});
%     Runs all tests methods in TestCase1.m and TestCase2.m that do not
%     have a tag 'Benchmark'.  The results are combined in one table,
%     rather than a table for each test case.
%
%   To run an individual test use native Matlab functions:
%     >> run(NameOfTestCaseClass, 'nameOfTestMethod');
%
%   To run a set of tests with a particular tag
%     >> runtests('NameOfTestCase', 'Tag', 'TagName');
%   Note that the second argument should always be 'Tag', while the first
%   and third should be set by the user.

%   Copyright 2019 Bose Corporation
import matlab.unittest.TestSuite
import matlab.unittest.TestRunner
import matlab.unittest.selectors.HasTag

%   Copyright 2018 Bose Corporation
% Input may either be test case name as string, or set of test cases in a
% cell array.
if isa(TestCase,'cell')
    % Extract the list of test cases to run
    tests = [];
    for i = 1:length(TestCase)
        tc = TestCase{i};
        suite = TestSuite.fromClass(meta.class.fromName(tc));
        tests = [tests, suite.selectIf(~HasTag('OverwriteReference'))];
    end
else
    suite = TestSuite.fromClass(meta.class.fromName(TestCase));
    tests = suite.selectIf(~HasTag('OverwriteReference'));
end

% Run the tests:
runner = TestRunner.withTextOutput;
result = runner.run(tests);

% Display test results
disp(' ');
disp('Test Summary:');
disp(table(result));

if nargin > 1
    loggingLevel = varargin{1};
else
    loggingLevel = 0;
end

% More display with pass/fail info for nightly build
if loggingLevel > 0
    disp(result);
    pass = cell2mat({result.Passed});
    if any(~pass)
        % This exact display is intended for external build parser which
        % looks for '^Error:' to mark the job as pass/fail
        disp('Error: Some tests did not pass.');
        disp(' ');
    end
end

end

