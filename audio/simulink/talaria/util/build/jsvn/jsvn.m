function jsvn( varargin )
%JSVN    Wrapper for built-in SVNKit subversion command-line client.
%   usage: jsvn <subcommand> [options] [args]
%   Type 'jsvn help <subcommand>' for help on a specific subcommand.
%   Type 'jsvn --version' to see the program version and RA modules
%     or 'jsvn --version --quiet' to see just the version number.
%
%   Most subcommands take file and/or directory arguments, recursing
%   on the directories.  If no arguments are supplied to such a
%   command, it recurses on the current directory (inclusive) by default.
%
%   Available subcommands:
%     <a href="matlab: disp('>> jsvn --version');jsvn --version">--version</a>
%     <a href="matlab: disp('>> jsvn help add');jsvn help add">add</a>
%     <a href="matlab: disp('>> jsvn help blame');jsvn help blame">blame</a> (praise, annotate, ann)
%     <a href="matlab: disp('>> jsvn help cat');jsvn help cat">cat</a>
%     <a href="matlab: disp('>> jsvn help changelist');jsvn help changelist">changelist</a> (cl)
%     <a href="matlab: disp('>> jsvn help checkout');jsvn help checkout">checkout</a> (co)
%     <a href="matlab: disp('>> jsvn help cleanup');jsvn help cleanup">cleanup</a>
%     <a href="matlab: disp('>> jsvn help commit');jsvn help commit">commit</a> (ci)
%     <a href="matlab: disp('>> jsvn help copy');jsvn help copy">copy</a> (cp)
%     <a href="matlab: disp('>> jsvn help delete');jsvn help delete">delete</a> (del, remove, rm)
%     <a href="matlab: disp('>> jsvn help diff');jsvn help diff">diff</a> (di)
%     <a href="matlab: disp('>> jsvn help export');jsvn help export">export</a>
%     <a href="matlab: disp('>> jsvn help help');jsvn help help">help</a> (?, h)
%     <a href="matlab: disp('>> jsvn help import');jsvn help import">import</a>
%     <a href="matlab: disp('>> jsvn help info');jsvn help info">info</a>
%     <a href="matlab: disp('>> jsvn help list');jsvn help list">list</a> (ls)
%     <a href="matlab: disp('>> jsvn help lock');jsvn help lock">lock</a>
%     <a href="matlab: disp('>> jsvn help log');jsvn help log">log</a>
%     <a href="matlab: disp('>> jsvn help merge');jsvn help merge">merge</a>
%     <a href="matlab: disp('>> jsvn help mergeinfo');jsvn help mergeinfo">mergeinfo</a>
%     <a href="matlab: disp('>> jsvn help mkdir');jsvn help mkdir">mkdir</a>
%     <a href="matlab: disp('>> jsvn help move');jsvn help move">move</a> (mv, rename, ren)
%     <a href="matlab: disp('>> jsvn help patch');jsvn help patch">patch</a>
%     <a href="matlab: disp('>> jsvn help propdel');jsvn help propdel">propdel</a> (pdel, pd)
%     <a href="matlab: disp('>> jsvn help propedit');jsvn help propedit">propedit</a> (pedit, pe)
%     <a href="matlab: disp('>> jsvn help propget');jsvn help propget">propget</a> (pget, pg)
%     <a href="matlab: disp('>> jsvn help proplist');jsvn help proplist">proplist</a> (plist, pl)
%     <a href="matlab: disp('>> jsvn help propset');jsvn help propset">propset</a> (pset, ps)
%     <a href="matlab: disp('>> jsvn help resolve');jsvn help resolve">resolve</a>
%     <a href="matlab: disp('>> jsvn help resolved');jsvn help resolved">resolved</a>
%     <a href="matlab: disp('>> jsvn help revert');jsvn help revert">revert</a>
%     <a href="matlab: disp('>> jsvn help status');jsvn help status">status</a> (stat, st)
%     <a href="matlab: disp('>> jsvn help switch');jsvn help switch">switch</a> (sw, relocate)
%     <a href="matlab: disp('>> jsvn help unlock');jsvn help unlock">unlock</a>
%     <a href="matlab: disp('>> jsvn help update');jsvn help update">update</a> (up)
%     <a href="matlab: disp('>> jsvn help upgrade');jsvn help upgrade">upgrade</a>
%
%   SVNKit is a pure Java (TM) version of Subversion - a tool for version control.
%   For additional information, see <a href="matlab: web('http://svnkit.com/','-browser')">http://svnkit.com/</a>

%   Developed by Per-Anders Ekström, 2018-2023 Kvot AB.

% Require arguments and being character vectors
if ~nargin || ~iscellstr( varargin ) %#ok<ISCLSTR>
    fprintf( 'Type <a href="matlab: %s help">%s help</a> for usage.\n', mfilename, mfilename );
    return
end

% Convert relative paths (starting with '.') to absolute paths
varargin = cellfun( @relativePathHandler, varargin, 'UniformOutput', false );

% Create a Java String array of the input arguments
jStringArray = javaArray( 'java.lang.String', nargin );
for i=1:nargin
    jStringArray(i) = java.lang.String( varargin{i} );
end

% Register available commands
registerCommands()

% Register available options
registerOptions()

% Create a SVNCommandLine-object requiring at least one command argument
commandLine = org.tmatesoft.svn.cli.SVNCommandLine( true );
try
    % Initialize with the input arguments
    commandLine.init( jStringArray )
catch ME
    % Error is from Java, only print the message
    if isa( ME, 'matlab.exception.JavaException' )
        error( '%s\n', ME.ExceptionObject.getMessage )
    else % otherwise rethrow the error
        rethrow( ME )
    end
end

% Create a SVNCommandEnvironment-object
% Define program name, output, error and input stream
env = org.tmatesoft.svn.cli.svn.SVNCommandEnvironment(...
    java.lang.String( mfilename ),...
    java.lang.System.out, java.lang.System.err, java.lang.System.in );
try
    % Initialize commands, options and validate options
    env.init( commandLine )
    % Initialize client options/manager, set whether to ignore externals
    env.initClientManager()
    % Run the command
    env.run();
    % Dispose the SVNCommandEnvironment-object
    env.dispose()
catch ME
    % Dispose the SVNCommandEnvironment-object
    env.dispose()
    % Error is from Java, only print the message
    if isa( ME, 'matlab.exception.JavaException' )
        error( '%s\n', ME.ExceptionObject.getMessage )
    else % otherwise rethrow the error
        rethrow( ME )
    end
end

function arg = relativePathHandler( arg )
% Convert relative path (starting with '.') to absolute path
if startsWith( arg, '.' ) % Assume argument is a relative path
    % Local path initialized to current working directory
    prepath = pwd;
    % Separate path into parts
    [subpath,name,ext] = fileparts( arg );
    % Start constructing an absolute path
    arg = [name,ext];
    if strcmp( subpath, '.' )
        % Original path was: ./ or ./<file or dir>
        arg = fullfile( prepath, arg );
    elseif strcmp( subpath, '..' )
        % Original path was: ../ or ../<file or dir>
        arg = fullfile( fileparts( prepath ), arg );
    elseif strcmp( arg, '.' )
        % Original path was: .
        arg = prepath;
    elseif strcmp( arg, '..' )
        % Original path was: ..
        arg = fileparts( prepath );
    else
        while true
            % Separate subpath into parts
            [subpath,name] = fileparts( subpath );
            if strcmp( name, '.' )
                % Shorten the local path one step up 
                prepath = fileparts( prepath );
            else
                % Continue the reconstruction of the path
                arg = fullfile( name, arg );
            end
            switch subpath
                case '.'
                    % Original path was: ./<nested local path>
                    arg = fullfile( prepath, arg );
                    break
                case '..'
                    % Original path was: ../<nested local path>
                    arg = fullfile( fileparts( prepath ), arg );
                    break
            end
        end
    end
end

function registerCommands()
% Register available commands
registerCommand = @( command )org.tmatesoft.svn.cli.AbstractSVNCommand.registerCommand( command );
registerCommand(org.tmatesoft.svn.cli.svn.SVNAddCommand());
registerCommand(org.tmatesoft.svn.cli.svn.SVNBlameCommand());
registerCommand(org.tmatesoft.svn.cli.svn.SVNCatCommand());
registerCommand(org.tmatesoft.svn.cli.svn.SVNChangeListCommand());
registerCommand(org.tmatesoft.svn.cli.svn.SVNCheckoutCommand());
registerCommand(org.tmatesoft.svn.cli.svn.SVNCleanupCommand());
registerCommand(org.tmatesoft.svn.cli.svn.SVNCommitCommand());
registerCommand(org.tmatesoft.svn.cli.svn.SVNCopyCommand());
registerCommand(org.tmatesoft.svn.cli.svn.SVNDeleteCommand());
registerCommand(org.tmatesoft.svn.cli.svn.SVNDiffCommand());
registerCommand(org.tmatesoft.svn.cli.svn.SVNExportCommand());
registerCommand(org.tmatesoft.svn.cli.svn.SVNHelpCommand());
registerCommand(org.tmatesoft.svn.cli.svn.SVNImportCommand());
registerCommand(org.tmatesoft.svn.cli.svn.SVNInfoCommand());
registerCommand(org.tmatesoft.svn.cli.svn.SVNListCommand());
registerCommand(org.tmatesoft.svn.cli.svn.SVNLockCommand());
registerCommand(org.tmatesoft.svn.cli.svn.SVNLogCommand());
registerCommand(org.tmatesoft.svn.cli.svn.SVNMergeCommand());
registerCommand(org.tmatesoft.svn.cli.svn.SVNMkDirCommand());
registerCommand(org.tmatesoft.svn.cli.svn.SVNMoveCommand());
registerCommand(org.tmatesoft.svn.cli.svn.SVNPropDelCommand());
registerCommand(org.tmatesoft.svn.cli.svn.SVNPropEditCommand());
registerCommand(org.tmatesoft.svn.cli.svn.SVNPropGetCommand());
registerCommand(org.tmatesoft.svn.cli.svn.SVNPropListCommand());
registerCommand(org.tmatesoft.svn.cli.svn.SVNPropSetCommand());
registerCommand(org.tmatesoft.svn.cli.svn.SVNResolveCommand());
registerCommand(org.tmatesoft.svn.cli.svn.SVNResolvedCommand());
registerCommand(org.tmatesoft.svn.cli.svn.SVNStatusCommand());
registerCommand(org.tmatesoft.svn.cli.svn.SVNSwitchCommand());
registerCommand(org.tmatesoft.svn.cli.svn.SVNRevertCommand());
registerCommand(org.tmatesoft.svn.cli.svn.SVNUnLockCommand());
registerCommand(org.tmatesoft.svn.cli.svn.SVNUpdateCommand());
registerCommand(org.tmatesoft.svn.cli.svn.SVNMergeInfoCommand());
registerCommand(org.tmatesoft.svn.cli.svn.SVNPatchCommand());
registerCommand(org.tmatesoft.svn.cli.svn.SVNUpgradeCommand());

function registerOptions()
% Register available options
registerOption = @( option )org.tmatesoft.svn.cli.SVNCommandLine.registerOption( option );
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.VERBOSE);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.UPDATE);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.NON_RECURSIVE);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.DEPTH);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.SET_DEPTH);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.QUIET);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.NO_IGNORE);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.INCREMENTAL);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.XML);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.CONFIG_DIR);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.IGNORE_EXTERNALS);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.IGNORE_KEYWORDS);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.CHANGELIST);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.HELP);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.QUESTION);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.VERSION);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.RECURSIVE);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.REVISION);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.CHANGE);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.REVPROP);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.STRICT);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.FILE);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.ENCODING);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.TARGETS);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.FORCE);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.FORCE_LOG);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.MESSAGE);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.WITH_REVPROP);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.EDITOR_CMD);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.NO_UNLOCK);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.DRY_RUN);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.RECORD_ONLY);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.USE_MERGE_HISTORY);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.EXTENSIONS);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.IGNORE_ANCESTRY);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.SHOW_COPIES_AS_ADDS);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.NATIVE_EOL);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.RELOCATE);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.AUTOPROPS);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.NO_AUTOPROPS);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.KEEP_CHANGELISTS);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.PARENTS);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.KEEP_LOCAL);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.ACCEPT);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.REMOVE);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.DIFF);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.OLD);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.NEW);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.SUMMARIZE);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.NOTICE_ANCESTRY);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.NO_DIFF_DELETED);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.NO_DIFF_ADDED);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.STOP_ON_COPY);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.LIMIT);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.AUTHOR_OF_INTEREST);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.REGULAR_EXPRESSION);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.GIT_DIFF_FORMAT);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.USERNAME);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.PASSWORD);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.NO_AUTH_CACHE);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.NON_INTERACTIVE);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.FORCE_INTERACTIVE);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.WITH_ALL_REVPROPS);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.SHOW_REVS);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.REINTEGRATE);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.ALLOW_MIXED_REVISIONS);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.DIFF_CMD);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.TRUST_SERVER_CERT);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.TRUST_SERVER_CERT_FAILURES);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.CONFIG_OPTION);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.STRIP);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.SHOW_INHERITED_PROPS);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.INCLUDE_EXTERNALS);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.PROPERTIES_ONLY);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.IGNORE_PROPERTIES);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.REVERSE_DIFF);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.IGNORE_WHITESPACE);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.SHOW_ITEM);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.NO_NEWLINE);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.PIN_EXTERNALS);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.REMOVE_UNVERSIONED);
registerOption(org.tmatesoft.svn.cli.svn.SVNOption.REMOVE_IGNORED);