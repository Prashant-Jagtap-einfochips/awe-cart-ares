import sys
import os


from JCppSchema import *
from JLogPrint import *


OJFeature_standardheader = ['#include <stdio.h>', '#include <stdlib.h>', '#include <string.h>',
                            '#include <stdint.h>', '#include <stddef.h>']


class JFeature:
    NAME = 'OJFeature'
    def __init__(self):
        self.h_filename = JFeature.NAME + '.h'
        self.cpp_filename = JFeature.NAME + '.cpp'
        self.classname = JFeature.NAME
        self.publicfunc = []

        self.Schema = JCppFeatureBaseSchema()

    def OJFEA_HeaderGen(self, filepath):
        #print('Generating Header File: ' + self.h_filename + ' ....')
        Jlog_print(JAG_GENERAL, 'Generating Header File: ' + self.h_filename + ' ....')
        incpath = filepath + 'inc/'
        if not os.path.exists(incpath):
            os.makedirs(incpath, exist_ok=True)
        with open(incpath + self.h_filename, 'w') as filehandle:
            # File Title
            filehandle.write(self.Schema.Schema_FileTitleComment())
            filehandle.write('\n')
            # File Comment
            filehandle.write(self.Schema.Schema_FileInfoComment(self.h_filename))
            filehandle.write('\n\n\n\n')
            # Header #ifndef
            filehandle.write(self.Schema.Schema_IfdefStatementBegin(self.h_filename[:-2].upper()))
            # Include Header Files
            filehandle.write('#include "OJBamProxy.h"\n\n')
            # Definition

            # Class Comment
            filehandle.write(self.Schema.Schema_ClassInfoComment(self.classname))
            # Declare Class
            filehandle.write(self.Schema.Schema_ClassDef(self.classname))
            filehandle.write(JCppSchema.ClassDefBegin)
            # Public section in class
            filehandle.write(JCppSchema.ClassPublicSection)
            for item in self.publicfunc:
                filehandle.write(self.Schema.Schema_ClassFuncInfoCommentinHeader(JCppSchema.ClassAlign1st, self.classname))
                filehandle.write('        ' + item + '\n')
            filehandle.write('\n')
            # Protect section in class
            filehandle.write(JCppSchema.ClassProtectedSection)
            filehandle.write('\n\n')
            # Private section in class
            filehandle.write(JCppSchema.ClassPrivateSection)
            filehandle.write('\n\n')
            filehandle.write(JCppSchema.ClassDefEnd)
            filehandle.write('\n\n\n\n')
            # Header #endif
            filehandle.write(self.Schema.Schema_IfdefStatementEnd(self.h_filename[:-2].upper()))

    def OJFEA_CppGen(self, filepath):
        #print('Generating CPP File: ' + self.cpp_filename + ' ....')
        Jlog_print(JAG_GENERAL, 'Generating CPP File: ' + self.cpp_filename + ' ....')
        srcpath = filepath + 'src/'
        if not os.path.exists(srcpath):
            os.makedirs(srcpath, exist_ok=True)
        with open(srcpath + self.cpp_filename, 'w') as filehandle:
            # File Title
            filehandle.write(self.Schema.Schema_FileTitleComment())
            filehandle.write('\n')
            # File Comment
            filehandle.write(self.Schema.Schema_FileInfoComment(self.cpp_filename))
            filehandle.write('\n\n\n\n')
            # Include Header Files
            filehandle.write(self.Schema.Schema_FileHeaderComment())
            for item in OJFeature_standardheader:
                filehandle.write(item + '\n')
            filehandle.write('#include ' + '"' + self.h_filename + '"')
            filehandle.write('\n\n\n\n')
            # Public Function Comment

            filehandle.write('\n\n')
            filehandle.write('\n')