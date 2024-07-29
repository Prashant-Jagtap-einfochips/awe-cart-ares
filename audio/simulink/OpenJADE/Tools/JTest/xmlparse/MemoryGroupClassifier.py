#*********************************************************************************
# This script is classifier for memory group
#
#*********************************************************************************

from xmlparse.JTestMemoryGroupClassifier import JTestMemoryGroupClassifier
from xmlparse.MARSMemoryGroupClassifier import MARSMemoryGroupClassifier

class MemoryGroupClassifier:
    def __init__(self, type=None):
        if type in ['JTest','BS','Vega','Melville', 'Spetter']:
            self.classifier = JTestMemoryGroupClassifier()
        elif type in ['MARS']:
            self.classifier = MARSMemoryGroupClassifier()
        else:
            self.classifier = None

    def register_classifier(self, classifier):
            self.classifier = classifier

    def groups(self):
        if self.classifier != None:
            return self.classifier.groups()
        else:
            return None

    def classify(self, section_name):
        if self.classifier != None:
            return self.classifier.classify(section_name)
        else:
            return None

