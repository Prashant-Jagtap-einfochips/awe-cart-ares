from xml.etree.ElementTree import ElementTree


def changeSrcPath(projectFile, newPath, outFile):
    tree = ElementTree()
    tree.parse(source=projectFile)
    root = tree.getroot()

    lr = root.find('linkedResources')

    for child in lr:
        if child.find('name').text == 'src':
            #child.find('location').text = 'PARENT-2-PROJECT_LOC\\' + newPath
            child.find('location').text = newPath

    tree.write(outFile, xml_declaration=True, encoding='UTF-8')

def changeCProjPaths(projectFile, newPath, outFile, app_root):
    tree = ElementTree()
    tree.parse(source=projectFile)
    root = tree.getroot()

    macros = root.findall('.//macros')

    for macro in macros:
        for child in macro.findall('stringMacro'):
            if child.get('name') == 'SRCDIR':
                child.set('value', newPath)
            if child.get('name') == 'OJ_RTE_DIR':
                oj_rte_dir = '\"../../../audio/simulink/'+app_root+'/Runtime\"'
                child.set('value', oj_rte_dir)

    tree.write(outFile, xml_declaration=True, encoding='UTF-8')


def replaceCoreNum(projectFile, coreNum, outFile):
    with open(projectFile, "rt") as fin:
        with open(outFile, "wt") as fout:
            for line in fin:
                fout.write(line.replace('core1', 'core'+coreNum))