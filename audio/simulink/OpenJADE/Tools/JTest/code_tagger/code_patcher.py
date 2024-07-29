#!/usr/bin/env python
################################################################################
#
#  BOSE CORPORATION
#  COPYRIGHT (c) BOSE CORPORATION ALL RIGHTS RESERVED
#  This program may not be reproduced, in whole or in part in any
#  form or any means whatsoever without the written permission of:
#     BOSE CORPORATION
#     The Mountain
#     Framingham, MA 01701-9168
#
################################################################################

import os
import shutil

def location_to_tuple(pos):
    """ Convert a source location to a tuple; easier to use as a sorting key or for comparisons """
    return (pos.line, pos.column)

def format_location(location):
    """ Format a source location object information; exclude source file """
    return f'<Ln {location.line}, Col {location.column}>'

def format_extent(extent):
    """ Format an extent object information; exclude source file """
    return f'{format_location(extent.start)}/{format_location(extent.end)}'

class CodePatcher:
    """ Record patches, validate them, order them, and apply them """
    def __init__(self):
        self._lines = {}
        self._patches = {}

    class Patch:
        """ Abstract base class for patching operations """
        def __init__(self, src_file, start, end):
            self._src_file = src_file
            self._start = start
            self._end = end

        def __repr__(self):
            if location_to_tuple(self._start) == location_to_tuple(self._end):
                location_str = f'{format_location(self._start)}'
            else:
                location_str = f'{format_location(self._start)}/{format_location(self._end)}'

            return f'{self.__class__.__name__.upper()}: {self._src_file} {location_str} {getattr(self, "_what", "")}'

        @property
        def start(self):
            """ Patch start """
            return self._start

        @property
        def end(self):
            """ Patch end """
            return self._end

        def apply(self, lines):
            """ Default implementation for the apply function """
            print('Warning: applying a no-op patch')

    class Insert(Patch):
        """ Concrete class for inserting content """
        def __init__(self, src_file, start, what):
            super(CodePatcher.Insert, self).__init__(src_file, start, start)
            self._what = what
            print(f'Insert in {self._src_file} {format_location(self._start)} {what}')

        def apply(self, lines):
            line = lines[self._start.line - 1]
            lines[self._start.line - 1] = f'{line[:self._start.column - 1]}{self._what}{line[self._start.column - 1:]}'

    class Delete(Patch):
        """ Concrete class for deleting content """
        def __init__(self, src_file, extent):
            super(CodePatcher.Delete, self).__init__(src_file, extent.start, extent.end)
            print(f'Delete from {self._src_file} {format_extent(extent)}')

        def apply(self, lines):
            lines[self._start.line - 1:self._end.line] = [lines[self._start.line - 1][:self._start.column - 1] + lines[self._end.line - 1][self._end.column - 1:]]

    class Replace(Patch):
        """ Concrete class for replacing content """
        def __init__(self, src_file, extent, what):
            super(CodePatcher.Replace, self).__init__(src_file, extent.start, extent.end)
            self._what = what
            print(f'Replace in {self._src_file} {format_extent(extent)} {what}')

        def apply(self, lines):
            lines[self._start.line - 1:self._end.line] = [lines[self._start.line - 1][:self._start.column - 1] + self._what + lines[self._end.line - 1][self._end.column - 1:]]

    def _get_lines(self, src_file):
        if not src_file in self._lines:
            with open(src_file, 'r', encoding='utf-8') as src_fp:
                self._lines[src_file] = src_fp.readlines()

        return self._lines[src_file]

    def _apply(self, src_file):
        self.sort(src_file)
        lines = self._get_lines(src_file)

        for patch in reversed(self._patches[src_file]):
            print(patch)
            patch.apply(lines)

    def insert(self, src_file, where, what):
        """ Insert content in a source file, before the specified position """
        self._patches[src_file] = self._patches.get(src_file, []) + [self.Insert(src_file, where, what)]

    def delete(self, src_file, extent):
        """ Delete content from a source file, in the specified extent """
        self._patches[src_file] = self._patches.get(src_file, []) + [self.Delete(src_file, extent)]

    def replace(self, src_file, extent, what):
        """ Replace content in a source file, in the specified extent """
        self._patches[src_file] = self._patches.get(src_file, []) + [self.Replace(src_file, extent, what)]

    def sort(self, src_file):
        """ Sort patches by extent start """
        self._patches[src_file].sort(key=lambda p: location_to_tuple(p.start))

    def validate(self, src_file):
        """ Try to ensure patches consistency """
        print(f'Validating patches for {src_file}\n')
        self.sort(src_file)
        previous = self._patches[src_file][0]

        for patch in self._patches[src_file][1:]:
            assert(location_to_tuple(patch.start) > location_to_tuple(previous.start))

    def apply(self):
        ''' Apply patches for all source files '''
        for src_file in self._patches:
            self.validate(src_file)
            self._apply(src_file)

            if not os.path.isfile(f'{src_file}.orig'):
                shutil.copyfile(src_file, f'{src_file}.orig')

            # with open(f'{src_file}.patched', 'w', encoding='utf-8') as src_fp:
            with open(src_file, 'w', encoding='utf-8') as src_fp:
                src_fp.write(''.join(self._get_lines(src_file)))
