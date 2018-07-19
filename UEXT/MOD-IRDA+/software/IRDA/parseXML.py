#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
#  xml.py
#  
#  Copyright 2014 Stefan Mavrodiev <support@olimex.com>
#  
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#  
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#  
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
#  MA 02110-1301, USA.


import xml.dom.minidom


# Create list for all command objects
command_list = []


# Create container class
class Commands(object):
    def __init__(self, title=None, description=None, type=None, address=None, command=None):
        self.title = title
        self.description = description
        self.type = type
        self.address = address
        self.command = command
        

def writeXML(xml_file="commands.xml"):

    try:

        doc = xml.dom.minidom.Document()

        #Create base element
        base = doc.createElement("commands")
        doc.appendChild(base)

        for item in command_list:

            command = doc.createElement("command")

            title = doc.createElement("title")
            title_string = doc.createTextNode(str(item.title))
            title.appendChild(title_string)

            description = doc.createElement("description")
            description_string = doc.createTextNode(str(item.description))
            description.appendChild(description_string)

            type = doc.createElement("type")
            type_string = doc.createTextNode(str(item.type))
            type.appendChild(type_string)

            address = doc.createElement("address")
            address_string = doc.createTextNode(str(item.address))
            address.appendChild(address_string)

            com = doc.createElement("com")
            com_string = doc.createTextNode(str(item.command))
            com.appendChild(com_string)

            command.appendChild(title)
            command.appendChild(description)
            command.appendChild(type)
            command.appendChild(address)
            command.appendChild(com)

            base.appendChild(command)

        print doc.toprettyxml()

        doc.writexml(open(xml_file, 'w'),
                     indent="   ",
                     addindent="    ",
                     newl='\n')

        doc.unlink()
        return True

    except Exception:
        return False


def readXML(xml_file="commands.xml"):

    try:
        global command_list
        command_list = []
        # Open XML file
        dom = xml.dom.minidom.parse(xml_file)

        # Read all commands
        commands = dom.getElementsByTagName("command")

        for command in commands:
            command_list.append(Commands(__get_title__(command),
                                         __get_description__(command),
                                         __get_type__(command),
                                         __get_address__(command),
                                         __get_command__(command)))

        return True
    except Exception:
        return False

     
def __get_text__(nodelist):
    text = []
    for node in nodelist:
        if node.nodeType == node.TEXT_NODE:
            text.append(node.data)
    return ''.join(text)

    
def __get_title__(command):

    title = command.getElementsByTagName("title")[0]
    return __get_text__(title.childNodes)


def __get_description__(command):
    
    title = command.getElementsByTagName("description")[0]
    return __get_text__(title.childNodes)


def __get_type__(command):
    
    title = command.getElementsByTagName("type")[0]
    return __get_text__(title.childNodes)


def __get_address__(command):
    
    title = command.getElementsByTagName("address")[0]
    return int(__get_text__(title.childNodes))

    
def __get_command__(command):
        
    title = command.getElementsByTagName("com")[0]
    return int(__get_text__(title.childNodes))

    
    



