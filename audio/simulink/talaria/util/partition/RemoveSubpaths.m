function RemoveSubpaths
%  RemoveSubpaths removes any paths in provided cellArray of strings that
%  are subpaths of other members in the array (i.e. deeper in the filesystem
%  or subsystem hierarchy). The provided cellArray must already be sorted!
%  Note that most path-finding routines happen to return sorted results.
