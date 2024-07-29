#!/usr/bin/env python
"""
components.py contains object definitions of various components of an audio graph.
"""

__author__ = "cd1043538 | Desai, Chirayu Ketan"
__copyright__ = "COPYRIGHT (c) BOSE CORPORATION ALL RIGHTS RESERVED"


class Feature:
    """
    Representation of a JModule and its attributes
    """

    def __init__(self):
        pass

    id = "",
    label = "",
    type = "",
    params = {},
    processes = []

    def __repr__(self):
        return "Feature: " + str(self.__dict__)

    def __eq__(self, other):
        return self.label.split(",")[0] == other.label.split(",")[0]

    def __hash__(self):
        return hash(('label', self.label.split(",")[0]))


class Process:
    """
    Representation of a Process in a JModule and its attributes
    """

    def __init__(self):
        pass

    type = ""
    label = ""
    params = {}
    id = ""
    parent = ""
    portIn = []
    portOut = []

    def __repr__(self):
        return "Process: " + str(self.__dict__)

    def __eq__(self, other):
        return self.id == other.id and self.label == other.label

    def __hash__(self):
        return hash(('id', self.id, 'label', self.label))


class Port:
    """
    Representation of a Port within a Process of a JModule and its attributes
    """

    def __init__(self):
        pass

    label = ""
    type = ""
    parent_process = ""
    id = ""
    parent = ""
    params = []
    buffer = ""

    def __repr__(self):
        return "Port: " + str(self.__dict__)

    def __eq__(self, other):
        return self.id == other.id and self.label == other.label

    def __hash__(self):
        return hash(('id', self.id, 'label', self.label))


class Edge:
    """
    Representation of an Edge(Connection) between two ports
    """

    def __init__(self):
        pass

    source = ""
    target = ""

    def __repr__(self):
        return "Edge: " + str(self.__dict__)


class Buffer:
    """
    Flagged wrapper for data buffers between processes
    """
    name = ""
    free = False

    def __init__(self, name, free):
        self.name = name
        self.free = free

    def __repr__(self):
        return "Buffer: " + str(self.__dict__)
