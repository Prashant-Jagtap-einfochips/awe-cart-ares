How to build revisions of this toolbox:

*** Make sure to read the warning in the last step about accidentally committing the compiler installation to SVN ***
*** Also - do *not* leave the startup.m file around with that name after building the toolbox. It will be run on startup!

1. Download the compiler from here:
   https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win64/Personal%20Builds/mingw-builds/6.3.0/threads-posix/sjlj/

2. Unzip the downloaded *.7z file into this folder's mingw64-6.3.0 subdirectory
   (e.g. so that the bin, etc, include, and other folders are directly under mingw64-6.3.0)

3. Rename the file "rename_to_startup.m" to "startup.m". We need this name
   but it causes problems if a file with this exact name is found anywhere
   under the project root folder (it does not matter if it is not on the path.)
   Somehow it still gets run and breaks the compiler operation... so rename
   it temporarily just for the purposes of packaging the toolbox.

3. Open the mingw6.3.0.prj file in MATLAB. This will present the "create toolbox" dialog from which you can customize anything if necessary and launch the process to package the toolbox.

4. The packaged up *.mltbx file can join its previous incarnations in the following network drive:
    \\solid\ASDDSP\tools\mingw

5. BTW-- It's not recommended to commit the compiler binaries into SVN. We included the mingw64-6.3.0 folder in SVN as a convenience to ensure you get the exact name correctly. However as an unwanted side effect SVN now thinks you have massively modified this version controlled folder with the introduction of all the compiler installation files. Please be cautious and do *not* commit all these added files to SVN.
    
Cheers!