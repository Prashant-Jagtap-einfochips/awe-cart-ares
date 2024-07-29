import sys
import os


from JCppSchema import *
from JLogPrint import *


OJBamProxy_standardheader = ['#include <stdlib.h>', '#include <string.h>', '#include <stdint.h>', '#include <stddef.h>', '#include "rtwtypes.h"', '#include "Feature_Utils.h"', '#include "BamProxy.h"']
OJBamPRoxy_Func = ['uint32_t top_write_request(uint32_t* data, uint32_t topid, uint32_t callhandler, uint32_t offset, uint32_t byte_count, uint32_t* pPayload);',
                   'uint32_t top_write_request(uint32_t* data, uint32_t topid, uint32_t callhandler, uint32_t payload);',
                   'uint32_t top_write_request(uint32_t* data, uint32_t topid, uint32_t callhandler, float payload);',
                   'uint32_t top_write_request(uint32_t* data, uint32_t topid, uint32_t callhandler, uint32_t offset, uint32_t payload);',
                   'uint32_t top_write_request(uint32_t* data, uint32_t topid, uint32_t callhandler, uint32_t offset, float payload);',
                   'uint32_t top_read_request(uint32_t* data, uint32_t topid, uint32_t callhandler, uint32_t offset, uint32_t byte_count);']

class JBamProxy:
    NAME = 'OJBamProxy'
    DATATYPE = [['signed char', 'int8_t'],
                ['unsigned char', 'uint8_t'],
                ['signed short', 'int16_t'],
                ['unsigned short', 'uint16_t'],
                ['signed int', 'int32_t'],
                ['unsigned int', 'uint32_t'],
                ['signed long long', 'int64_t'],
                ['unsigned long long', 'uint64_t'],
                ['float', 'real32_t'],
                ['double', 'real64_t'],
                ['signed char', 'int8_T'],
                ['unsigned char', 'uint8_T'],
                ['signed short', 'int16_T'],
                ['unsigned short', 'uint16_T'],
                ['signed int', 'int32_T'],
                ['unsigned int', 'uint32_T'],
                ['signed long long', 'int64_T'],
                ['unsigned long long', 'uint64_T'],
                ['float', 'real32_T'],
                ['double', 'real64_T'],
                ['double', 'real_T'],
                ['double', 'time_T'],
                ['unsigned char', 'boolean_T'],
                ['signed int', 'int_T'],
                ['unsigned int', 'uint_T'],
                ['unsigned long', 'ulong_T'],
                ['unsigned long long', 'ulonglong_T'],
                ['unsigned char', 'uchar_T'],
                ['signed char', 'char_T'],
                ['signed char', 'byte_T']
                ]
    def __init__(self):
        self.h_filename = JBamProxy.NAME + '.h'
        self.cpp_filename = JBamProxy.NAME + '.cpp'
        self.classname = JBamProxy.NAME
        self.publicfunc = []
        self.privatevar = []

        self.Schema = JCppBamProxyBaseSchema()

    def OJBP_HeaderGen(self, filepath):
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
            for item in OJBamProxy_standardheader:
                filehandle.write(item + '\n')
            filehandle.write('\n\n\n\n')
            # Definition
            # RET_CODE_TYPE definition
            filehandle.write('#define TALARIA_HEADER_SIZE       2\n\n')
            #filehandle.write('#ifndef RET_CODE_TYPE\n')
            #filehandle.write('typedef enum\n')
            #filehandle.write('{\n')
            #filehandle.write('    RET_SUCCESS = 0,\n')
            #filehandle.write('    RET_ERROR,\n')
            #filehandle.write('    RET_BUSY,\n')
            #filehandle.write('    RET_SOURCE_OOR,\n')
            #filehandle.write('    RET_MAX_INPUT_EXCEEDED,\n')
            #filehandle.write('    RET_MAX_OUTPUT_EXCEEDED,\n')
            #filehandle.write('    RET_TIMEOUT,\n')
            #filehandle.write('    RET_INDX_OOR, \n')
            #filehandle.write('    RET_MIC_LEVEL_FAILED,\n')
            #filehandle.write('    RET_LOCK_FAIL,\n')
            #filehandle.write('    RET_INVALID,\n')
            #filehandle.write('    RET_NO_ACTION,\n')
            #filehandle.write('    RET_NO_CONNECT,\n')
            #filehandle.write('} RET_CODE_TYPE;\n')
            #filehandle.write('#endif\n')
            filehandle.write('\n')
            # Data Type definition
            #for item in JBamProxy.DATATYPE:
            #    filehandle.write(self.Schema.Schema_Typedef(item[1], item[0]))
            filehandle.write('\n\n\n')
            # Class Comment
            filehandle.write(self.Schema.Schema_ClassInfoComment(self.classname))
            # Declare Class
            filehandle.write(self.Schema.Schema_ClassDef(self.classname, 'public', 'Bam_Proxy'))
            filehandle.write(JCppSchema.ClassDefBegin)
            # Public section in class
            filehandle.write(JCppSchema.ClassPublicSection)
            for item in self.publicfunc:
                filehandle.write(self.Schema.Schema_ClassFuncInfoCommentinHeader(JCppSchema.ClassAlign1st, self.classname))
                filehandle.write('        ' + item + '\n')
            for fnc in OJBamPRoxy_Func:
                filehandle.write(self.Schema.Schema_ClassFuncInfoCommentinHeader(JCppSchema.ClassAlign1st, self.classname))
                filehandle.write('        ' + fnc + '\n')
            filehandle.write('\n')
            # Protect section in class
            filehandle.write(JCppSchema.ClassProtectedSection)
            filehandle.write('\n\n')
            # Private section in class
            filehandle.write(JCppSchema.ClassPrivateSection)
            for item in self.privatevar:
                filehandle.write('        ' + item + '\n')
            filehandle.write('\n\n')
            filehandle.write(JCppSchema.ClassDefEnd)
            filehandle.write('\n\n\n\n')
            # Header #endif
            filehandle.write(self.Schema.Schema_IfdefStatementEnd(self.h_filename[:-2].upper()))

    def OJBP_CppGen(self, filepath):
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
            for item in OJBamProxy_standardheader:
                filehandle.write(item + '\n')
            filehandle.write('#include ' + '"' + self.h_filename + '"')
            filehandle.write('\n\n\n\n')
            # Public Function Comment
            filehandle.write(self.Schema.Schema_PublicFuncComment(self.classname))
            filehandle.write(self.Schema.Schema_BamProxyBaseFillImplement1(self.classname))
            filehandle.write('\n')
            filehandle.write(self.Schema.Schema_BamProxyBaseFillImplement2(self.classname))
            filehandle.write('\n')
            filehandle.write(self.Schema.Schema_BamProxyBaseFillImplement3(self.classname))
            filehandle.write('\n')
            filehandle.write(self.Schema.Schema_BamProxyBaseFillImplement4(self.classname))
            filehandle.write('\n')
            filehandle.write(self.Schema.Schema_BamProxyBaseFillImplement5(self.classname))
            filehandle.write('\n')
            filehandle.write(self.Schema.Schema_BamProxyBaseFillImplement6(self.classname))
            filehandle.write('\n')