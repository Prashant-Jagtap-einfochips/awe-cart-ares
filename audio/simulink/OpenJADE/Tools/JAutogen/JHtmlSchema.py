import sys
import os


from JConv import *
from JLogPrint import *


class JFLHtmlSchema:
    def __init__(self, alignment=4):
        self.align = ' ' * alignment
        self.cnt = 0
    
    def Schema_startHtml(self):
        self.cnt += 1
        return '<html>\n'

    def Schema_endHtml(self):
        self.cnt -= 1
        return '</html>\n'

    def Schema_headHtml(self, titleinfo=None):
        ss = []
        ss.append(self.align * self.cnt + '<head>\n')
        self.cnt += 1
        ss.append(self.align * self.cnt + '<title>' + titleinfo + '</title>\n')
        ss.append(self.align * self.cnt + '<style>\n')
        self.cnt += 1
        ss.append(self.align * self.cnt + 'body {background-color: powderblue;}\n')
        ss.append(self.align * self.cnt + 'table.RTC_Bam {text-align: left; border: 1px solid black; border-collapse: collapse; width: 50%; background: linear-gradient(to right, #fffbd5, #FDC830);}\n')
        ss.append(self.align * self.cnt + 'table.RTC_Fea {text-align: left; border: 1px solid black; border-collapse: collapse; width: 50%; background: linear-gradient(to right, #a8ff78, #78ffd6);}\n')
        ss.append(self.align * self.cnt + 'li {font-weight: bold;}\n')
        ss.append(self.align * self.cnt + 'td {border: 1px solid black;}\n')
        self.cnt -=1
        ss.append(self.align * self.cnt + '</style>\n')
        self.cnt -= 1
        ss.append(self.align * self.cnt + '</head>\n')
        return ''.join(ss)

    def Schema_startBody(self, topicinfo=None, subtopic=None):
        ss = []
        ss.append(self.align * self.cnt + '<body>\n')
        self.cnt += 1
        ss.append(self.align * self.cnt + '<h1>' + topicinfo + '</h1>\n')
        ss.append(self.align * self.cnt + '<b>' + subtopic + '</b>\n')
        ss.append(self.align * self.cnt + '<ul>\n')
        self.cnt += 1
        return ''.join(ss)

    def Schema_endBody(self):
        ss = []
        self.cnt -= 1
        ss.append(self.align * self.cnt + '</ul>\n')
        self.cnt -= 1
        ss.append(self.align * self.cnt + '</body>\n')
        return ''.join(ss)

    def Schema_itemFeatureStart(self, featurename=None):
        ss = []
        ss.append(self.align * self.cnt + '<li style="color:Tomato;">Feature: ' + featurename + '\n')
        self.cnt += 1
        ss.append(self.align * self.cnt + '<ul>\n')
        self.cnt += 1
        return ''.join(ss)

    def Schema_itemFeatureEnd(self):
        ss = []
        self.cnt -= 1
        ss.append(self.align * self.cnt + '</ul>\n')
        self.cnt -= 1
        ss.append(self.align * self.cnt + '</li>\n')
        return ''.join(ss)

    def Schema_itemBamproxyStart(self, bamproxyname=None):
        ss = []
        ss.append(self.align * self.cnt + '<li style="color:Blue; list-style-type:square;">Bamproxy: ' + bamproxyname + '\n')
        self.cnt += 1
        return ''.join(ss)

    def Schema_itemBamproxyEnd(self):
        ss = []
        self.cnt -= 1
        ss.append(self.align * self.cnt + '</li>\n')
        return ''.join(ss)

    def Schema_itemRtcTopEx(self, item1=None, item2=None):
        ss = []
        ss.append(self.align * self.cnt + '<li style="color:Blue; list-style-type:square;">\n')
        self.cnt += 1
        ss.append(self.align * self.cnt + '<table class="RTC_Bam">\n')
        self.cnt += 1
        ss.append(self.align * self.cnt + '<colgroup>\n')
        ss.append(self.align * self.cnt + '<col span=' + '"' + '1' + '"' + 'style= ' + '"' + 'width: 5%;' + '"' + '>\n')
        ss.append(self.align * self.cnt + '</colgroup>\n')
        ss.append(self.align * self.cnt + '<tr>\n')
        ss.append(self.align * self.cnt + '  <td><font size="2" face="Arial">' + item1 + '</font></td>\n')
        ss.append(self.align * self.cnt + '  <td><font size="2" face="Arial">' + ', '.join(item2) + '</font></td>\n')
        ss.append(self.align * self.cnt + '</tr>\n')
        self.cnt -= 1
        ss.append(self.align * self.cnt + '</table>\n')
        self.cnt -= 1
        ss.append(self.align * self.cnt + '</li>\n')
        return ''.join(ss)

    def Schema_itemRtcTop(self, item1=None, item2=None):
        ss = []
        ss.append(self.align * self.cnt + '<table class="RTC_Fea">\n')
        self.cnt += 1
        ss.append(self.align * self.cnt + '<colgroup>\n')
        ss.append(self.align * self.cnt + '<col span=' + '"' + '1' + '"' + 'style= ' + '"' + 'width: 5%;' + '"' + '>\n')
        ss.append(self.align * self.cnt + '</colgroup>\n')
        ss.append(self.align * self.cnt + '<tr>\n')
        ss.append(self.align * self.cnt + '  <td><font size="2" face="Arial">' + item1 + '</font></td>\n')
        ss.append(self.align * self.cnt + '  <td><font size="2" face="Arial">' + ', '.join(item2) + '</font></td>\n')
        ss.append(self.align * self.cnt + '</tr>\n')
        self.cnt -= 1
        ss.append(self.align * self.cnt + '</table>\n')
        return ''.join(ss)