import os
import sys
import glob



###### Get OpenJADE Version ######
OPENJADE_VERSION_KEYWORD = '#define _OPENJADE_VERSION_'
OPENJADE_VERSION_FILENAME = '_BAF_Version.h'
OPENJADE_VERSION_RELATIVE_PATH = '\\Runtime\\' + OPENJADE_VERSION_FILENAME

def getOpenJADEVersion(app_root):
   ver = 'None'
   full_path = None
   current_path = app_root
   # Search in child folders recursively
   search_path = glob.glob(current_path + '/**/' + OPENJADE_VERSION_FILENAME, recursive=True)
   if bool(search_path):
      full_path = search_path[0]
   else:
      # Search in parent folders
      while '\\' in current_path:
         if os.path.exists(current_path + OPENJADE_VERSION_RELATIVE_PATH):
            full_path = current_path + OPENJADE_VERSION_RELATIVE_PATH
            break
         else:
            current_path = current_path[:current_path.rfind('\\')]
   if full_path:
      # Read BAF Version
      with open(full_path, 'r') as filehandle:
         lines = filehandle.readlines()
         for each_line in lines:
            if OPENJADE_VERSION_KEYWORD in each_line:
               ver = each_line[each_line.find('"') + 1 : each_line.rfind('"')]
               break
   else:
      print('WARNING: Cannot find _BAF_Version.h')
   return ver

def compOpenJADEVersion(ver1=None, ver2=None):
   # Format is 00.AA.BB
   if ver1 == ver2:
      return 0
   else:
      v1 = ver1.split('.')[0]
      v2 = ver2.split('.')[0]
      return compOpenJADEVersion(ver1[ver1.find('.') + 1:], ver2[ver2.find('.') + 1:]) if v1 == v2 else 1 if v1 > v2 else -1

