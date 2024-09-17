
# Format code to this project coding standards
astyle:
	astyle --project=.astylerc --exclude=BuildDll --exclude=Include --exclude=build \
           --exclude=audio --recursive *.h,*.cpp,*.c

