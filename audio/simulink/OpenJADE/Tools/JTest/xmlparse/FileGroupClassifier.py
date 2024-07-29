#*********************************************************************************
# This script is classifier for file group
#
#*********************************************************************************

from xmlparse.JTestFileGroupClassifier import JTestFileGroupClassifier
from xmlparse.BSFileGroupClassifier import BSFileGroupClassifier
from xmlparse.VegaFileGroupClassifier import VegaFileGroupClassifier
from xmlparse.MelvilleFileGroupClassifier import MelvilleFileGroupClassifier
from xmlparse.SpetterFileGroupClassifier import SpetterFileGroupClassifier
from xmlparse.MARSFileGroupClassifier import MARSFileGroupClassifier

class FileGroupClassifier:
    def __init__(self, type=None, fl_path=None):
        if type == 'JTest':
            self.classifier = JTestFileGroupClassifier(fl_path)
        elif type == 'BS':
            self.classifier = BSFileGroupClassifier()
        elif type == 'Vega':
            self.classifier = VegaFileGroupClassifier()
        elif type == 'Melville':
            self.classifier = MelvilleFileGroupClassifier(fl_path)
        elif type == 'Spetter':
            self.classifier = SpetterFileGroupClassifier()
        elif type == 'MARS':
            self.classifier = MARSFileGroupClassifier()
        else:
            self.classifier = None

    def register_classifier(self, classifier):
            self.classifier = classifier

    def groups(self):
        if self.classifier != None:
            return self.classifier.groups()
        else:
            return None

    def classify(self, input_file):
        if self.classifier != None:
            return self.classifier.classify(input_file)
        else:
            return None
