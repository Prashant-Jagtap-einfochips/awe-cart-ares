import os
import sys
import getopt
import struct
import importlib
import inspect
from pprint import pprint as pp
from pathlib import Path

__usage__ = """ \

Usage:
  GenJModuleSources.py [-h] -a app_root -j jframework -o output_dir

Description:

Options:
  -h,   --help    Display usage information (this message)
  -j,   --jfr     JFramework
  -a,   --app     OpenJADE root folder
  -o,   --out     Output Folder
  -b,   --bafname    BAF object name (default BAF_obj)
  -c,   --context    Use context (no argument; default False)
"""


#*********************************************************************************
# parse_args(argv)
#*********************************************************************************
def parse_args(argv):
    app_root = None
    jframework_file = None
    output_dir = None
    context = False
    bafname='BAF_obj'

    try:
        opts, args = getopt.getopt(argv, 'ha:j:o:b:c', ['help','app=','jfr=','out=', 'baf=', 'ctx'])
    except getopt.GetoptError:
        usage()
        sys.exit()
    for opt, arg in opts:
        if opt in ('-h', '--help'):
            usage()
            sys.exit()
        elif opt in ('-a', '--app'):
            app_root = os.path.abspath(arg)
            if not os.path.isdir(app_root):
                print ('Error: OpenJADE root folder ' + app_root + ' could not be found')
                sys.exit()
        elif opt in ('-j', '--jfr'):
            jframework_file = os.path.abspath(arg)
            if not os.path.isfile(jframework_file):
                print ('Error: input jframework file ' + jframework_file + ' could not be found')
                sys.exit()
        elif opt in ('-o', '--out'):
            output_dir = os.path.abspath(arg)
            if not os.path.isdir(output_dir):
                print ('Error: output directory ' + output_dir + ' could not be found')
                sys.exit()
        elif opt in ('-c', '--context'):
            context = True
        elif opt in ('-b', '--bafname'):
            bafname = arg

    if (app_root == None) or (jframework_file == None) or (output_dir == None):
        usage()
        sys.exit()

    return app_root, jframework_file, output_dir, context, bafname

#*********************************************************************************
# usage()
#*********************************************************************************
def usage():
    sys.stdout.write(__usage__)

#*********************************************************************************
# main()
#*********************************************************************************
def main( argv=sys.argv[1:] ):
    app_root, jframework_file, output_dir, context, bafname = parse_args(argv)
    jframework_path = os.path.dirname(jframework_file)
    jframework_name = os.path.splitext(os.path.basename(jframework_file))[0]

    sys.path.append( app_root+'/Tools/JConfig')
    sys.path.append( app_root+'/Tools/JAutogen')
    sys.path.append( jframework_path )

    from JAutogen import JAutogenCode

    jfr = importlib.import_module(jframework_name, package=None)

    #jfrnames = [name for name,obj in inspect.getmembers(jfr, predicate=inspect.isclass)]
    #jfrclss = [obj for name,obj in inspect.getmembers(jfr, predicate=inspect.isclass)]
    #print(jfrnames)
    #pp(inspect.getclasstree(jfrclss))

    jframework_class = None
    for name, obj in inspect.getmembers(jfr, predicate=inspect.isclass):
        if obj.__module__ == jframework_name:
            print (f'Info: using {obj.__name__} from {obj.__module__} to generate sources')
            jframework_class = obj

    if jframework_class == None:
        print ('Error: no suitable class found for generating sources')
        return

    jmodule_dir = os.path.join(output_dir, 'Baf')
    if os.path.isdir(jmodule_dir):
        print ('Warning: pre-existing jmodule directory ' + jmodule_dir + '. Overwriting ...')

    Path(os.path.join(jmodule_dir, 'Audio_Graph')).mkdir(parents=True, exist_ok=True)
    Path(os.path.join(jmodule_dir, 'JModule_Lib')).mkdir(parents=True, exist_ok=True)
    genConfig={}
    genConfig['Runtime']={}
    genConfig['Runtime']['Context'] = context
    genConfig['Runtime']['ObjectName'] = bafname
    jautogen = JAutogenCode(genConfig=genConfig)

    baf = { 'Baf': jframework_class() }

    jautogen.GenerateJFrameworkExSingle(baf, name='Baf', genpath=output_dir)

#*********************************************************************************
if __name__ == '__main__':
    main()







