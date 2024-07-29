import sys
import os



class JAConv:
    def sectionname_codepragma(self, funcname):
        return funcname.upper() + '_IMEM_SECTION' if funcname != None else ''

    def filename_bankmappingheader(self, fname):
        return fname + '_BankMapping.h' if fname != None else ''




class JMatlabConv(JAConv):
    MATLAB_BLOCK_SIGNAL = ''

    def structname_blocksignal(self, matlab_model_name=None):
        if matlab_model_name != None:
            return 'B_' + matlab_model_name + '_T'
        else:
            return ''

    def structname_blockstate(self, matlab_model_name=None):
        if matlab_model_name != None:
            return 'DW_' + matlab_model_name + '_T'
        else:
            return ''

    def funcname_exportbuffer(self, buffer_name=None, buffer_size=None):
        if buffer_name != None and buffer_size != None:
            return 'float* getBufPointer_' + buffer_name + '_' + buffer_size + '(void)'
        else:
            return ''

    def variablename_buffermember(self, struct_name=None, variable_name=None):
        if struct_name != None and variable_name != None:
            return struct_name + '.' + variable_name
        else:
            return ''

    def variablename_blocksignal(self, matlab_model_name=None):
        if matlab_model_name != None:
            return matlab_model_name + '_B'
        else:
            return ''

    def variablename_blockstate(self, matlab_model_name=None):
        if matlab_model_name != None:
            return matlab_model_name + '_DW'
        else:
            return ''

    def funcname_subsystem(self, matlab_model_name=None, subsystem_name=None):
        if matlab_model_name != None and subsystem_name != None:
            return matlab_model_name + '_' + subsystem_name
        else:
            return ''

    def funcname_model_initialize(self, matlab_model_name=None):
        return matlab_model_name + '_initialize' if matlab_model_name != None else ''

    def funcname_model_singlestep(self, matlab_model_name=None):
        return matlab_model_name + '_step' if matlab_model_name != None else ''

    def funcname_model_multistep(self, matlab_model_name=None, stepname=None):
        if matlab_model_name != None and stepname != None:
            return matlab_model_name + '_step' + stepname
        else:
            return ''

    def funcname_model_feature(self, matlab_model_name=None, feaname=None):
        if matlab_model_name != None and feaname != None:
            return matlab_model_name + '_' + feaname
        else:
            return ''

    def funcname_model_feature_multirate(self, matlab_model_name=None, stepNum=None):
        if matlab_model_name != None and stepNum != None:
            return matlab_model_name + 'TID' + stepNum
        else:
            return ''