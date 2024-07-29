import sys
import os
import json

from JLogPrint import *



class JFmrSchemaConfig:
    DefaultBafObjectName = 'BAF_obj'
    def __init__(self, filename=None):
        self.schemaConfig = filename

    def GetBafCallbackContext(self):
        return self.schemaConfig['Runtime']['Context'] if self.schemaConfig is not None and 'Runtime' in self.schemaConfig and 'Context' in self.schemaConfig['Runtime']  else False

    def GetBafObjectName(self):
        return self.schemaConfig['Runtime']['ObjectName'] if self.schemaConfig is not None and 'Runtime' in self.schemaConfig and 'ObjectName' in self.schemaConfig['Runtime']  else self.DefaultBafObjectName


class JFLSchemaConfig:
    def __init__(self, filename=None):
        self.schemaConfig = filename

    def GetCommonHeaderFile(self, fltype=None):
        head_l = None
        if self.schemaConfig != None and 'FeatureLib' in self.schemaConfig and fltype in self.schemaConfig['FeatureLib']:
            if 'Common Header File' in self.schemaConfig['FeatureLib'][fltype]:
                head_l = [x for x in self.schemaConfig['FeatureLib'][fltype]['Common Header File']]
        return head_l

    def GetAliasNameofFeature(self, feature_type):
        aliasName = None
        if self.schemaConfig != None:
            feaAliasDict = self.schemaConfig['FeatureLib']['Alias_Name']['Feature']
            for feaT, feaA in feaAliasDict.items():
                if feaT == feature_type:
                    aliasName = feaA
                    break
        return aliasName