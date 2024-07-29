import os
import re
import sys
import getopt
import struct
import xml.etree.ElementTree as et
from xml.etree.ElementTree import XMLParser

__usage__ = """ \

Usage:
  GenFeatureTuneFiles.py [-h] -f feature_xml -i tunevar_xml -o out_dir

Description:

Options:
  -h,   --help    Display usage information (this message)
  -f,   --feature Input <Model>_Features.xml
  -i,   --in      Input <Model>_TuneVar.xml
  -o,   --out     Output folder to hold <Feature>.tune files
"""

#*********************************************************************************
# cleanhtml(raw_html)
#
#*********************************************************************************
def cleanhtml(embedded_html):
    #if the element was empty
    if embedded_html == None:
        return None

    #replace newlines with spaces
    #embedded_html = embedded_html.replace('\n', ' ').replace('\r','')

    #if the element just contains plain text return that
    if not re.search('<html>', embedded_html, flags=re.IGNORECASE):
        return ''.join(('# ' + x + '\n') for x in embedded_html.split('\n'))

    embedded_html = re.sub('<html>', '<html>', embedded_html, flags=re.IGNORECASE)
    embedded_html = re.sub('</html>', '</html>', embedded_html, flags=re.IGNORECASE)
    embedded_html = re.sub('<font', '<font', embedded_html, flags=re.IGNORECASE)
    embedded_html = re.sub('/font>', '/font>', embedded_html, flags=re.IGNORECASE)

    raw_html = ''.join(et.fromstring(embedded_html).itertext())

    html_cleaner = re.compile('<.*?>')

    cleantext = re.sub(html_cleaner, '', raw_html)

    cleantext = ''.join(('# ' + x + '\n') for x in cleantext.split('\n'))

    return cleantext

#*********************************************************************************
# getTuneVarsForFeature(tunevar_root, model, feature)
#
#*********************************************************************************
def getTuneVarsForFeature(tunevar_root, model, feature, out_dir):
    out_file = os.path.join(out_dir, feature.lower() + '.tune')
    count = 0
    with open(out_file, 'w') as f:
        for overlay_group in tunevar_root.iter('OverlayGroup'):
            tunevarList = [x for pkg in overlay_group.iter('Package') for x in pkg.iter('TuneVar')] if bool(overlay_group.findall('Package')) else [x for x in overlay_group.iter('TuneVar')]
            for tunevar in tunevarList:
                tunevar_id = tunevar.get('id').strip()
                if tunevar_id.startswith(model+feature):
                    default_value = tunevar.find('DefaultValue').text
                    matlab_value = tunevar.find('MATLABValue').text
                    description_text = cleanhtml(tunevar.find('Description').text)
                    if description_text != None:
                        f.write (description_text + '\n')
                    matlab_value=matlab_value.replace('double','')
                    matlab_value=matlab_value.replace('single','')
                    matlab_value=matlab_value.replace( ';',' ')
                    matlab_value=matlab_value.replace(')','')
                    matlab_value=matlab_value.replace('(','')
                    matlab_value=matlab_value.replace(']','')
                    matlab_value=matlab_value.replace('[','')
#                    f.write (tunevar_id + ' ' + default_value + '\n\n')
                    f.write (tunevar_id + ' ' + matlab_value + '\n\n')
                    count += 1
    print ('Info: model-%s feature-%s count %d' % (model, feature, count))
    return count

#*********************************************************************************
# getSymfunForFeature(tunevar_root, model, feature)
#
#*********************************************************************************
def getSymfunForFeature(tunevar_root, model, feature, out_dir):
    out_file = os.path.join(out_dir, feature.lower() + '.symfun')
    count = 0
    with open(out_file, 'w') as f:
        f.write ('# ')
        f.write (feature.upper())
        f.write ('.SYMFUN\n')
        f.write ('#\n')
        f.write ('#   Provides Symfun Translation for ')
        f.write (feature.lower())
        f.write (' parameters.\n')
        f.write ('#\n')
        f.write ('#   BOSE CORPORATION\n')
        f.write ('#   COPYRIGHT (c) 2007-2020 BOSE CORPORATION ALL RIGHTS RESERVED\n')
        f.write ('#   This program may not be reproduced, in whole or in part in an\n')
        f.write ('#   form or any means whatsoever without the written permission of:\n')
        f.write ('#\n')
        f.write ('#       BOSE CORPORATION\n')
        f.write ('#       The Mountain\n')
        f.write ('#       Framingham, MA 01701-9168\n')
        f.write ('#\n')
        f.write ('#\n')
        f.write ('#############################################################################\n')
        f.write ('SYMFUN_VERSION 2.0;\n\n')
        f.write ('FEATURE ')
        f.write ("'"+feature.lower()+':['+feature.lower()+"]'")
        f.write (', $CARVARIANT_HIERARCHY;\n\n')
        f.write ('HIERARCHY $CARVARIANT_HIERARCHY\n')
        f.write ('{\n')
        f.write ('    ####################\n')
        f.write ('    # Tuning parameter #\n')
        f.write ('    ####################\n\n')
        for overlay_group in tunevar_root.iter('OverlayGroup'):
            tunevarList = [x for pkg in overlay_group.iter('Package') for x in pkg.iter('TuneVar')] if bool(overlay_group.findall('Package')) else [x for x in overlay_group.iter('TuneVar')]
            for tunevar in tunevarList:
                tunevar_id = tunevar.get('id').strip()
                if tunevar_id.startswith(model+feature):
                    f.write (' PARAMETER '+feature.lower()+':['+feature.lower()+']:'+tunevar_id + ';\n')
                    count += 1
        f.write ('\n    ###########\n')
        f.write ('    # MAPPING #\n')
        f.write ('    ###########\n')
        for overlay_group in tunevar_root.iter('OverlayGroup'):
            tunevarList = [x for pkg in overlay_group.iter('Package') for x in pkg.iter('TuneVar')] if bool(overlay_group.findall('Package')) else [x for x in overlay_group.iter('TuneVar')]
            for tunevar in tunevarList:
                tunevar_id = tunevar.get('id').strip()
                if tunevar_id.startswith(model+feature):
                    f.write (' MAP '+tunevar_id +',_{' +tunevar_id + '};\n')
        f.write ('\n}  # end HIERARCHY\n\n')
        f.write ('1;')
    print ('Info: model-%s feature-%s count %d' % (model, feature, count))
    return count
#*********************************************************************************
# usage()
#*********************************************************************************
def usage():
    sys.stdout.write(__usage__)

#*********************************************************************************
# parse_args(argv)
#*********************************************************************************
def parse_args(argv):
    feature_xml = None
    tunevar_xml = None
    out_dir = None

    try:
        opts, args = getopt.getopt(argv, 'hf:i:o:', ['help','feature=','in=','out='])
    except getopt.GetoptError:
        usage()
        return None
    for opt, arg in opts:
        if opt in ('-h', '--help'):
            usage()
            return None
        elif opt in ('-f', '--feature'):
            feature_xml = os.path.abspath(arg)
            if not os.path.isfile(feature_xml):
                print ('Error: <Model>_Features.xml ' + feature_xml + ' could not be found')
                return None
        elif opt in ('-i', '--in'):
            tunevar_xml = os.path.abspath(arg)
            if not os.path.isfile(tunevar_xml):
                print ('Error: <Model>_TuneVar.xml ' + tunevar_xml + ' could not be found')
                return None
        elif opt in ('-o', '--out'):
            out_dir = os.path.abspath(arg)
            if not os.path.isdir(out_dir):
                print ('Error: output directory ' + out_dir + ' could not be found')
                return None

    if (feature_xml == None) or (tunevar_xml == None) or (out_dir == None):
        usage()
        return None

    return {'feature_xml':feature_xml, 'tunevar_xml':tunevar_xml, 'out_dir':out_dir}

#*********************************************************************************
# main()
#*********************************************************************************
def main( argv=sys.argv[1:] ):
    params = parse_args(argv)
    if params == None:
        return

    feature_tree = et.parse(params['feature_xml'])
    feature_root = feature_tree.getroot()

    tunevar_tree = et.parse(params['tunevar_xml'])
    tunevar_root = tunevar_tree.getroot()
    count = 0
    for model in feature_root.iter('Model'):
        model_id = model.get('id').strip()
        for feature in model.iter('Feature'):
            feature_id = feature.get('id').strip()
            count += getTuneVarsForFeature ( tunevar_root, model_id, feature_id, params['out_dir'] )
            getSymfunForFeature ( tunevar_root, model_id, feature_id, params['out_dir'] )
    print ('Info: model-%s total count %d' % (model_id, count))

#*********************************************************************************
if __name__ == '__main__':
    main()

