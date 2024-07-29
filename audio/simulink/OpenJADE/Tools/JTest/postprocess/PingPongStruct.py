import sys
import os
import math
import getopt
import xml.etree.ElementTree as et

__usage__ = """ \

Usage:
  PingPongStruct.py [-h] -m model_name -s soc_id -c core_id -x xml_file -i source_file -o postprocess_file

Description:

Assumptions:

Options:
  -h,   --help  Display usage information (this message)
  -m,   --model model_name
  -s,   --soc   soc_id
  -c,   --core  core_id
  -x,   --xml   input <Model>_<SOC_ID>_<CORE_ID>_PingPongStruct.xml file path
  -i,   --in    input <Model>_<SOC_ID>_<CORE_ID>_PingPongStruct.c file path
  -o,   --out   output <Model>_<SOC_ID>_<CORE_ID>_PingPongStruct_pp.c file path

"""


#*********************************************************************************
# Function: verify_paths(xml_path, in_path, out_path)
#
#*********************************************************************************
def verify_paths(xml_path, in_path, out_path):
    if (xml_path == None) or (in_path == None) or (out_path == None):
        print ('Error: <Model>_<SOC_ID>_<CORE_ID>_PingPongStruct.xml/.c file and output preprocessed file need to be specified')
        return False

    if (not os.path.isfile(xml_path)):
        print ('Error: <Model>_<SOC_ID>_<CORE_ID>_PingPongStruct.xml file ' + xml_path + ' could not be found')
        return False

    if (not os.path.isfile(in_path)):
        print ('Error: <Model>_<SOC_ID>_<CORE_ID>_PingPongStruct.c file ' + in_path + ' could not be found')
        return False

    outDir = os.path.dirname(out_path)
    if (not os.path.isdir(outDir)):
        os.makedirs(outDir, exist_ok = True)

    return True

#*********************************************************************************
# Function: post_process
#
#*********************************************************************************
def post_process(model_name, soc_id, core_id, xml_path, in_path, out_path):
    if verify_paths(xml_path, in_path, out_path) == False:
        return

    if (model_name == None) or (soc_id == None) or (core_id == None):
        print('Error: model_name, soc_id, core_id missing')
        return

    xml_tree = et.parse(xml_path)
    xml_root = xml_tree.getroot()

    with open(in_path, 'r') as fp:
        in_lines = fp.readlines()

    if core_id == 2:
        replace_core2 = {}

    replace_common = {}
    for struct_el in xml_root.findall('Struct'):
        struct_id = struct_el.get('id')
        expected_prefix = model_name+'_'+str(soc_id)+'_'+str(core_id)+'_'
        expected_suffix = '_buffers'+str(soc_id)+'_'+str(core_id)

        target_prefix = model_name+'_'
        target_suffix = '_buffers'

        if expected_prefix in struct_id:
            if core_id == 2:
                from_parts = [struct_id+'_type', struct_id+expected_suffix]
                to_parts = ['extern ' + struct_id+'_type', struct_id.replace(expected_prefix, target_prefix)+target_suffix]
                from_str = from_parts[0] + ' ' + from_parts[1]
                to_str = to_parts[0] + ' ' + to_parts[1]
                replace_core2[struct_id] = {'from':from_str, 'to':to_str, 'from_parts': from_parts, 'to_parts': to_parts}
            from_str = struct_id+expected_suffix
            to_str = struct_id.replace(expected_prefix, target_prefix)+target_suffix
            replace_common[struct_id] = {'from':from_str, 'to':to_str}

    if core_id == 2:
        core2_lines = []
        x_prev = ''
        replace_flag = False
        for x in in_lines:
            replace_flag = False
            for k, v in replace_core2.items():
                if (v['from_parts'][0] == x_prev.strip()) and (v['from_parts'][1] in x):
                    core2_lines.append( x_prev.rstrip() + ' ' + x.lstrip() )
                    replace_flag = True
                    x_prev = ''
                    break
            if (not replace_flag):
                core2_lines.append(x_prev)
                x_prev = x

        if (not replace_flag):
            core2_lines.append(x_prev)
        in_lines = core2_lines

    with open(out_path, 'w') as fp:
        for x in in_lines:
            y = x
            replace_flag = False
            if core_id == 2:
                for k, v in replace_core2.items():
                    if v['from'] in x:
                        y = x.replace(v['from'],v['to'])
                        replace_flag = True
                        break

            if replace_flag == False:
                for k, v in replace_common.items():
                    if v['from'] in x:
                        y = x.replace(v['from'],v['to'])
                        replace_flag = True
                        break

            if replace_flag == False:
                z = x.strip()
                if z.startswith('pp_control[') and z.endswith('].transfer_index = 0;'):
                    from_str = '].transfer_index = 0;'
                    to_str = '].transfer_index = '+str(core_id-1)+';'
                    y = x.replace(from_str, to_str)
                    replace_flag = True

            fp.write(y)

#*********************************************************************************
# Function: usage()
# Description:
#
#*********************************************************************************
def usage():
    sys.stdout.write(__usage__)
    sys.exit()


#*********************************************************************************
# Function: main()
# Description:
#
#*********************************************************************************
def main( argv=sys.argv[1:] ):
    model_name = None
    soc_id = None
    core_id = None
    xml_path = None
    in_path = None
    out_path = None

    try:
        opts, args = getopt.getopt(argv, "hm:s:c:x:i:o:", ["help", "model=", "soc=", "core=", "xml=", "in=", "out="])
    except getopt.GetoptError:
        usage()
    for opt, arg in opts:
        if opt in ("-h", "--help"):
            usage()
        elif opt in ("-m", "--model"):
            model_name = arg
        elif opt in ("-s", "--soc"):
            soc_id = int(arg,10)
        elif opt in ("-c", "--core"):
            core_id = int(arg,10)
        elif opt in ("-x", "--xml"):
            xml_path = os.path.abspath(arg)
        elif opt in ("-i", "--in"):
            in_path = os.path.abspath(arg)
        elif opt in ("-o", "--out"):
            out_path = os.path.abspath(arg)

    post_process(model_name, soc_id, core_id, xml_path, in_path, out_path)

#*********************************************************************************
if __name__ == "__main__":
    main()


