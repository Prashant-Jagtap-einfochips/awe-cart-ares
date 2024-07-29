classdef ObfuscationRelease
%  ObfuscationRelease coordinates creation of obfuscated Simulink content
%  Designed to work with SVN code control and Bose ASD common repository
%  orgnaization for feature design (e.g. the "module" folder).
%  To create a release using smart default settings, just supply the tag:
%     ObfuscationRelease.Create('V220031')
%  
%  Release from which branch of SVN: (discovered from current workspace)
%  The currently open project and its checked out repository is of critical
%  importance. The current repository branch much have a parallel branch in
%  the destination release repository. If checked out on "trunk", then the
%  obfucated repo must also have a "trunk". Otherwise, checkout error.
% 
%  Where to checkout Sandboxes: (Property SandboxRootFolder)
%  To perform the release, this routine checks out both a copy of the HCI
%  repo (ContentSubFolder) and a copy of the last obfuscated content into a
%  local sandbox. It is CRITICAL that the location of this sandbox is NOT
%  inside the root folder of the current project. Otherwise, MATLAB's
%  version control will lock up the process. Opening and closing
%  feature-work Simulink projects can be quite slow, so special effort to
%  change projects as infrequently as possible is important.
% 
%  Overview of the process:
%  The release process follows these high level steps
%   1. Ensure the release tag looks good and does not yet exist
%   2. Checkout obfuscated repo - what's there from last time
%   3. Checkout HCI repo ContentSubFolder, HciProjectFolders, TestSubFolder
%   4. Delete all the obfuscated sandbox files leaving only svn/git metadata
%   5. Full copy of all files from the HCI sandbox to the obfuscated sandbox
%   6. Open the Simulink project inside the destination hci sandbox to check
%   7. Open the Simulink project inside the destination obf sandbox
%   8. In-place obfuscation in destination sandbox, igoring all unfound links
%   9. In-place protectedCopy of sandbox, keeping obfuscated SLX
%  10. RunTalariaTests in the obj sandbox project
%  11. Cleanup the obfuscated sandbox, removing all but ContentSubFolder
%  12. Ask for confirmation from the user to commit & tag
%     ... commit destination sandbox and tag both repos.
%
%    Documentation for ObfuscationRelease
%       doc ObfuscationRelease
%
%
properties
  %  String version control tag for the release e.g. VAABBCC where AA is
  %  the major MATLAB revision, BB and CC are major and minor codes.
  %  However, any format is acceptible as long as it matches with the
  %  TagFormatRegEx property.
TagName
  %  Source HCI design SVN server URL
HciServerUrlSvn
  %  Destination SVN URL to receive obfuscated release
ObfServerUrlSvn
  %  Additional obfuscation version control checkout branch suffix. The
  %  URL used to checkout previous obfuscation release will have these
  %  characters appended to the branch name
ObfBranchSuffix
  %  Expected SVN version string found in svn --version, '' to skip
ExpectedSvnVersion
  %  Expected SVN platform string found in svn --version, '' to skip
ExpectedSvnPlatform
  %  Local filesystem release sandbox area - SVN checkouts are done here.
  %  Relative to the root project that is open when starting the release.
SandboxRootFolder
  %  Regular expression pattern for valid tag name. Talaria would use 'V\d{4}[A-Z]{2}
TagFormatRegEx
  %  Release content folder (in SVN and local) with content to release.
  %  This folder is present in both HCI and obfuscated sandboxes.
  %  Relative to the root project that is open when starting the release.
HciContentSubFolder
  %  Folder name(s) containing tests (e.g. "test"). These folder(s)
  %  will be loaded into the HCI sandbox. 
TestSubFolders
  %  If specified, passed to matlab.unittest.TestSuite.fromFolder when
  %  building test suite from the TestSubFolders. See selectors:
  %  https://www.mathworks.com/help/matlab/ref/matlab.unittest.selectors-package.html
  %  For simplicity, use this object's ExceptTests to create one of these.
TestSelector
  %  HCI project folders is a string array of folders in the development
  %  project root that are required to load the feature content. These
  %  folders are copied to use during SLX obfuscation and/or tests, but
  %  they are deleted before commiting the obfuscated release.
HciProjectFolders
  %  Feature diagram filename patterns using MATLAB's "dir". Matching
  %  diagrams will be obfuscated and included in the release. Specify
  %  empty to disable all block diagram obfuscation and *.slx elimination.
  %  The wildcard "**" means 'any folder names or depth including nothing'
  %  Example for any slx files in a "lib" folder: "**/lib/*.slx"
  %  Example for a specific slx file: "**/ThisOne.slx"
  %  Example multiple globs: ["**/First.slx" "components/**/*.slx"]
FeatureDiagramGlobs
  %  Regular-expression patterns for block names that when matched,
  %  prevent the block's name from changing in the obfuscation process.
  %  Use these patterns to enable CscUtil to find blocks and subsystems in
  %  the obfuscated diagram
SavedBlockNameRegexPatterns
  %  Override standard directives for the delete/protect step. Leave this
  %  string empty for standard feature obfuscation releases
OverriddenDeleteProtectDirectives
  %  Additional +-= directives for the delete/protect step
AdditionalDeleteProtectDirectives
  %  Function handle (no arguments) to invoke once hci project is loaded.
  %  The function must return "true" to continue the release.
HciProjectLoadCheckFcn
  %  path to Git repository for obfuscated release e.g.
  %  "https://github.com/BoseCorp/..."
GitURL
  % PreserveHelp -  true to create *.m files to match *.p files with preserved help content
PreserveHelp
  % CheckNotes -  true checks HciContentSubFolder for module_notes.txt file and searches for release name
CheckNotes
  % RunTests -  true calls RunTalariaTests after obfuscation
RunTests
  % CommitChanges -  true updates to obfuscated repo are checked in
CommitChanges
  % CreateTags -  true creates repo tags
CreateTags
  % ReopenOriginalProjectAfterRelease -  true opens original project after release finishes with no error
ReopenOriginalProjectAfterRelease
  % PromptToApproveRelease -  true interactively prompts for "YES" to proceed with any repo changes
PromptToApproveRelease
  % UnifiedObfuscationRepo -  true to add feature repo to obfuscation path DstServerUrl (default)
UnifiedObfuscationRepo
  % ReleaseOnGit -  true to release on git also
ReleaseOnGit
  %  SVN URL for source repo tag
HciTagSvn
  %  SVN URL for destination (obfuscated) repo tag
ObfTagSvn
  %  Local filesystem path where source will be checked out
HciSandbox
  % HciSandboxContent -  content subfolder of sandbox
HciSandboxContent
  %  Local filesystem path where destination will be checked out
ObfSandbox
  % ObfSandboxContent -  content subfolder of sandbox
ObfSandboxContent
  %  Local filesystem path where destination will be checked out
ObfSandboxGit
  %  SVN URL to checkout into SrcSandbox
HciCheckoutUrlSvn
  %  SVN URL to checkout into DestSandbox
ObfCheckoutUrlSvn
  %  Initial filesystem working directory upon construction
pwd
  %  Path to this script itself
MyScriptFolder
  %  Feature name as discovered from SVN URLs
FeatureRepo
  %  Feature working branch as discovered from SVN URLs
FeatureWorkingBranchSvn
  %  Working branch on Git (also discovered from SVN URLs)
FeatureWorkingBranchGit
  %  Names of obfuscated diagrams that have been prepared for release
ObfuscatedDiagramNameList
  %  Filesystem paths of self-modifying diagrams found during obfuscation
  %  which could not be collapsed/absorbed and so must be separately
  %  obfuscated and included
SelfModifyingLibraryList
  %  Filesystem paths of diagrams matching FeatureDiagramGlobs and
  %  intended to be obfuscated
DiagramsToObfuscate
  %  Filesystem paths of collapsed libraries found during obfuscation
  %  which were absorbed into the feature diagrams and so deleted and not
  %  included in the release
AbsorbedLibraryList
  %  Filesystem paths of external referenced libraries found during
  %  obfuscation and left in place since they are not included in this
  %  module
ExternalReferencedLibraryList
  %  "Cached" values from the original root project. The release process
  %  closes this project, making it's properties inaccessible afterwards
RootFolder
  %  Indicates whether the version of svn involved requires all filesystem
  %  paths to be "cygwin" paths
CygwinPaths
end
methods
function Create
   %  Given a tag name, Create will construct an Obfuscation release
   %  with default settings and then immediately perform the release.
   %  But in normal practice, features should create their own release
   %  script/function to create an ObfuscationRelease object, adjust
   %  its settings (e.g. TagFormatRegEx) and then PerformRelease.
end
function PerformRelease
   %  PerformRelease launches the activities of release, culminating in the
   %  creation of matching SVN tags in the HCI and obfuscated repositories
end
function PrintTimeSoFar
   %ObfuscationRelease/PrintTimeSoFar is a function.
   %    PrintTimeSoFar(~)
end
function CleanUpForExit
   %  Finishes the process by cleaning paths and opening original project
end
function TagAndPushOnGit
   %ObfuscationRelease/TagAndPushOnGit is a function.
   %    TagAndPushOnGit(this)
end
function TagSvnRepo
   %ObfuscationRelease/TagSvnRepo is a function.
   %    TagSvnRepo(this, fromURL, tagURL, theTagName)
end
function CommitAndPushGitSandbox
   %  Save obfuscation release to git server
end
function CommitObfuscatedSvnSandbox
   %  Save obfuscation release to svn server
end
function EnsureMexDllSvnExecutable
   %  Seek out all mex/dll files in the newly formed release and ensure they
   %  all have their SVN flag set for being executable (cygwin's svn needs this flag)
end
function HandleGitAddRemove
   %ObfuscationRelease/HandleGitAddRemove is a function.
   %    HandleGitAddRemove(this)
end
function HandleSvnAddRemove
   %  Discover and react to any file additions or removals
end
function DestroyTempDevFolders
   %  Destroy the Dev folders we copied just to allow for project
   %  references and obfuscation loading of SLX files.
end
function DestroyProtectObf
   %  Clear out unwanted files/folders in the obfuscated sandbox and p-code
end
function RemoveTranslateAndInterpretContent
   %  Performs assertCoreIndexRole of "0" to targetify SLX files in path
end
function RefreshAndUnlock
   %  Internal use method to ensure library has freshly resolved links and
   %  is unlocked... ready for modification
end
function SneakyNoVersionUpdateSaveSystem
   %  SneakyNoVersionUpdateSaveSystem performs save_system, but first
   %  adjusts the model version number so that the save has the same
   %  version as before (i.e. keeps the model version the same). The
   %  purpose of this seemingly dangerous trick is to make the obfuscated
   %  diagram "look" exactly like the HCI diagram so that library version
   %  tracking remains accurate.
end
function GetRootAndRenameBlocks
   %  Search through provided outputModelName to find blocks to rename or not
end
function PreviewSavedBlockNames
   %  PreviewSavedBlockNamesperforms diagnostic analysis of an SLX file to
   %  preview expected block path name changes. Prints out a report of
   %  whether blocks with unchanging names could be used by mask init code
   %  to perform custom storage class assignments.
   %  Usage:
   %     PreviewSavedBlockNames(this,slxFile)
end
function ObfuscateModel
   % OBFUSCATEMODEL Creates obfuscated version of input model
   %    Creates copy of model with no library links, non-useful block names,
   %    blocks located right on top of each other, and other steps to make
   %    understanding the model more difficult. Does not encrypt model or any
   %    MATLAB function block code, but trusted partners who "look inside" will
   %    understand that the contents are not intended for view.
   %  Usage:
   %     ObfuscateModel(modelName,[outputModelName])
   %  Where
   %     modelName is string name of model or library to obfuscate
   %     outputModelName will default modelName plus a suffix, saved in same
   %        folder
   %  Examples:
   %     ObfuscateModel('MyFeature');
   %     ObfuscateModel('MyFeature','MyFeature_Obfuscated'); %debug
end
function DoFeatureDiagramObfuscation
   %  DoFeatureDiagramObfuscation searches the FeatureDiagramGlobs and
   %  obfuscates the diagrams. Recurses down into self-modifying libraries.
end
function VerifyTests
   %  Opens teset project sandbox dir and "RunTalariaTests" with argument of the HCI
   %  folder's TestSubFolder
   %  Usage:
   %     VerifyTests(this);
end
function ExceptTests
   %  Creates a test selector excluding all test names that contain any of
   %  the specified substrings.
   %  Examples
   %    relObj.TestSelector = relObj.ExceptTests("FailingTestCase");
   %    relObj.TestSelector = relObj.ExceptTests(["BadOne","BadTwo"]);
end
function TestsWillSkip
   %ObfuscationRelease/TestsWillSkip is a function.
   %    skipTests = TestsWillSkip(this)
end
function CleanSandboxGit
   %  revert modified and delete unversioned files
end
function CleanObfSandboxSvn
   %  CleanSandbox performs a revert & cleanup
end
function CleanHciSandboxSvn
   %  CleanSandbox performs a revert & cleanup
end
function EnsureHciProjectReady
   %  If a check function is specified, load project and run it
end
function VerifyModuleNotes
   %  Verify that the module notes text file has been updated for this tag
end
function DeleteExcept
   %  Deletes everything in the given folder but the exceptions
   %  folder : string path of folder to delete
   %  exceptions : horz string array of top level exception folders / files
   %  Note this ensures mex files are not loaded (so they could be deleted)
end
function SlideFolder
   %  Internal method to copy files around. Used to copy from HCI to
   %  obfuscated and from one obfuscated release area to another. Called by
   %  SlideSvnDstToGitDst and SlideSrcToDst
   %  dstFolder is folder path to directory to be deleted and receive copy
   %  dstNoDelete is folder name of metadata (e.g. .svn or .git)
   %  srcFolder is folder path of directory copied from
   %  srcNoCopy is folder name of metadata not copied (.svn or .git)
end
function SlideObfSvnToObfGit
   %  destroy non-metadata content in the git release area and copy over
   %  the non-metadata content from the svn obfuscated release area
end
function SlideHciToObf
   %  SlideHciToObf destroys all of the obfuscation sandbox file content
   %  except .svn metadata. Then copies all of the HCI sandbox content to 
   %  the to-be-obfuscated sandbo except for the svn and TestSubFolders
end
function CheckoutHciSandbox
   %  The HCI sandbox is special because we only want to pull some of the
   %  content of the repository. However, the SVN sparse-directories feature
   %  simply does not work with svn:externals. The only way to acquire any
   %  externals at all is to perform a full recursive (infinity depth)
   %  checkout. With any other depth, externals just will never show up ever.
   %  If you checkout an empty depth sandbox and then later add folders then
   %  the empty checkout has to delete the folders you add later. Adds and
   %  removes a bunch of files slowing down the process. Instead, checkout
   %  each folder individually. Checkout are faster for next time, but ensure
   %  slide-over to obfuscated area has to ignore the .svn folders. This seems
   %  the fastest repeat-use design.
end
function CheckoutObfSandboxGit
   %  Check out the obfuscated release destination folder for git
end
function CheckoutObfSandboxSvn
   %  Check out the obfuscated release destination folder for SVN
end
function VerifyTagNotAlreadyThereGit
   %  Ensure the tag does not already exist
end
function VerifyTagNotAlreadyThereSvn
   %  Ensure the tag does not already exist
end
function VerifyTagName
   %  Validate given tag expression
   %  Define the FEATURE variable for potential keyword substitution
end
function EnsureProjectOpen
   %  Opens specified project if it's not already the root project
end
function CloseCurrentProject
   %ObfuscationRelease/CloseCurrentProject is a function.
   %    CloseCurrentProject(this)
end
function DoSystemCommand
   %  DoSystemCommon accepts arguments like printf to create a command-line
   %  which is then run with results returned.
end
function makeSvnUrl
   %  makeSvnUrl piece together parts of an SVN URL together to return a joined
   %  single URL string. This method cheats by using MATLAB's filesystem
   %  joining routine "fullfile" and then afterward replaces backslashes (if
   %  any) with forward slashes as used by SVN. And finally brings back the
   %  double slash in https://
end
function FindFeatureRepo
   %  FindFeatureRepo breaks the current project's SVN repository
   %  location into 3 parts and sets feature repo and working branch
   %   1- the src server (explicitly specified in SrcServerUrl)
   %   2- the FeatureRepository (name of feature, top-level repo)
   %   3- the FeatureWorkingBranch (everything else after that)
end
function GetSvnFilesystemPath
   %  Converts windows path to cygwin path if this.CygwinPaths is true
end
function VerifySvn
   %  Ensure the avowed version of SVN is being accessed
end
function VerifyNotInReleaseSandbox
   %  Make sure the currently open project / path isn't already inside the
   %  release sandbox. This happens a lot when releases fail.
end
function ObfuscationRelease
   %  Construct ObfuscationRelease object with default settings
   %
   %    Documentation for ObfuscationRelease/ObfuscationRelease
   %       doc ObfuscationRelease
   %
end
end
end
