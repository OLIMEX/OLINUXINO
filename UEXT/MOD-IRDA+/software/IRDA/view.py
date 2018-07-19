#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
#  view.py
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


import curses
import time
import parseXML

import irda

screen = None


def __init__():
    global screen
    screen = curses.initscr()
    curses.noecho()
    curses.cbreak()
    curses.start_color()

    curses.init_pair(1, curses.COLOR_YELLOW, curses.COLOR_BLACK)
    curses.init_pair(2, curses.COLOR_GREEN, curses.COLOR_BLACK)
    curses.init_pair(3, curses.COLOR_WHITE, curses.COLOR_BLACK)
    curses.init_pair(4, curses.COLOR_BLACK, curses.COLOR_CYAN)

    screen.bkgd(' ', curses.color_pair(2))

    screen.refresh()


def make_input_box(text):
    win = curses.newwin(6, 40, screen.getmaxyx()[0]/2-3, screen.getmaxyx()[1]/2-20)
    win.border(0)
    curses.echo()
    win.addstr(1, 1, str(text), curses.A_BOLD)
    win.addstr(2, 1, "="*38)
    string = win.getstr(3, 1, 38)
    curses.noecho()

    return string


def new_command():

    xy = screen.getmaxyx()

    screen.keypad(1)
    position = 1
    x = None
    h = curses.color_pair(1)
    n = curses.A_NORMAL

    # Create new window to select mode
    win = curses.newwin(6, 40, xy[0]/2-3, xy[1]/2-20)

    while x != ord('\n'):
        win.clear()
        win.border(0)
        win.addstr(1, 1, "Select protocol for the command:", curses.A_BOLD)
        win.addstr(2, 1, "="*38)

        if position == 1:
            win.addstr(3, 1, "1. SIRC", h)
        else:
            win.addstr(3, 1, "1. SIRC", n)
        if position == 2:
            win.addstr(4, 1, "2. RC5", h)
        else:
            win.addstr(4, 1, "2. RC5", n)

        win.refresh()
        x = screen.getch()

        if x == ord('1'):
            position = 1
        elif x == ord('2'):
            position = 2

        elif x == 258:
            if position < 2:
                position += 1
            else:
                position = 1
        elif x == 259:
            if position > 1:
                position -= 1
            else:
                position = 2

    if position == 1:
        mode = "SIRC"
    else:
        mode = "RC5"

    win.clear()

    #Enter title
    title = make_input_box("Enter title:")

    #Enter description
    description = make_input_box("Enter description")

    win.border(0)
    win.addstr(1, 1, "Waiting for input", curses.A_BOLD)
    win.addstr(2, 1, "="*38)
    win.refresh()

    #Capture command
    ir = irda.IRDA()
    if mode == "SIRC":
        ir.setOperationMode(ir.MODE_SIRC)
    else:
        ir.setOperationMode(ir.MODE_RC5)
    time.sleep(0.1)

    data = 0xFFFF
    while data == 0xFFFF or data == 0:
        data = ir.readData()
        time.sleep(0.1)

    win.addstr(3, win.getmaxyx()[1]/2 - len("DONE")/2, "DONE")
    win.refresh()
    time.sleep(1)

    if mode == "SIRC":
        command, address = irda.IRDA.decode_SIRC(data)
    else:
        command, address = irda.IRDA.decode_RC5(data)

    while data != 0xFFFF:
        data = ir.readData()
        time.sleep(0.1)

    parseXML.command_list.append(parseXML.Commands(title, description, mode, address, command))
    parseXML.writeXML()


def list_commands():

    xy = screen.getmaxyx()

    win_head = curses.newwin(5, xy[1] - 2, 1, 1)
    win_head.border(0)
    win_head.bkgd(' ', curses.color_pair(2))

    win_head.addstr(1, 1, "List commands: ", curses.A_BOLD)
    win_head.addstr(2, 1, '='*(xy[1] - 4))
    win_head.addstr(3, 1, "Title")
    win_head.addstr(3, 21, "Type")
    win_head.addstr(3, 31, "Address")
    win_head.addstr(3, 41, "Command")
    win_head.addstr(3, 51, "Description")
    win_head.refresh()

    win_list = curses.newwin(xy[0] - 10, xy[1] - 2, 6, 1)
    win_list.border(0)
    win_list.bkgd(' ', curses.color_pair(2))
    win_list.keypad(1)
    win_list.refresh()

    win_menu = curses.newwin(3, xy[1] - 2, xy[0] - 4, 1)
    win_menu.border(0)
    win_menu.bkgd(' ', curses.color_pair(2))
    win_menu.addstr(1, 1, "CTRL+W:", curses.color_pair(3) | curses.A_UNDERLINE | curses.A_BOLD)
    win_menu.addstr(1, win_menu.getyx()[1], "Exit", curses.color_pair(4))
    win_menu.addstr(1, win_menu.getyx()[1]+10, "CTRL+X:", curses.color_pair(3) | curses.A_UNDERLINE | curses.A_BOLD)
    win_menu.addstr(1, win_menu.getyx()[1], "Execute", curses.color_pair(4))
    win_menu.addstr(1, win_menu.getyx()[1]+10, "CTRL+D:", curses.color_pair(3) | curses.A_UNDERLINE | curses.A_BOLD)
    win_menu.addstr(1, win_menu.getyx()[1], "Delete", curses.color_pair(4))
    win_menu.addstr(1, win_menu.getyx()[1]+10, "CTRL+N:", curses.color_pair(3) | curses.A_UNDERLINE | curses.A_BOLD)
    win_menu.addstr(1, win_menu.getyx()[1], "New", curses.color_pair(4))
    win_menu.refresh()

    pos = 0

    h = curses.color_pair(1)
    n = curses.A_NORMAL

    parseXML.readXML()

    while True:
        win_list.clear()
        win_list.border(0)

        for index in range(len(parseXML.command_list)):
            if pos == index:
                win_list.addstr(1+index, 1, str(parseXML.command_list[index].title), h)
                win_list.addstr(1+index, 21, str(parseXML.command_list[index].type), h)
                win_list.addstr(1+index, 31, str(parseXML.command_list[index].address), h)
                win_list.addstr(1+index, 41, str(parseXML.command_list[index].command), h)
                win_list.addstr(1+index, 51, str(parseXML.command_list[index].description), h)
            else:
                win_list.addstr(1+index, 1, str(parseXML.command_list[index].title), n)
                win_list.addstr(1+index, 21, str(parseXML.command_list[index].type), n)
                win_list.addstr(1+index, 31, str(parseXML.command_list[index].address), n)
                win_list.addstr(1+index, 41, str(parseXML.command_list[index].command), n)
                win_list.addstr(1+index, 51, str(parseXML.command_list[index].description), n)

        win_list.refresh()
        x = win_list.getch()

        if x == curses.KEY_DOWN:
            pos += 1
        elif x == curses.KEY_UP:
            if pos != 0:
                pos -= 1
        elif x == 23:
            break
        elif x == 24 or x == 10:
            ir = irda.IRDA()
            if parseXML.command_list[pos].type == "RC5":
                ir.setOperationMode(ir.MODE_RC5)
            else:
                ir.setOperationMode(ir.MODE_SIRC)
            time.sleep(0.1)

            ir.sendData(parseXML.command_list[pos].address, parseXML.command_list[pos].command)

        #Delete
        elif x == 4:
            del parseXML.command_list[pos]
            parseXML.writeXML()
            parseXML.readXML()
            if pos != 0:
                pos -= 1
            else:
                pos = 0

        #New
        elif x == 14:
            new_command()


def cleanup():
    curses.endwin()
    #~ os.system('clear')
    return


__init__()