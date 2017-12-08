/*
 * MidiEditor
 * Copyright (C) 2010  Markus Schwenk
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DATATYPE_H
#define DATATYPE_H

/**
 * \class Protocol
 *
 * \brief Protocol uses two stacks to store the history of the program.
 *
 * Protocol stores the history of the program, using two Stacks (the Stacks
 * are implemented as Lists): the redo stack and the undo stack. Both save
 * ProtocolSteps, in the order they have to be released to reload the old
 * state of the program.
 *
 * To add a ProtocolItem, there has to be opened a ProtocolStep calling
 * startNewAction(). To use the Protocol, open a ProtocolStep,
 * add some ProtocolItems calling enterUndoStep() and call endAction() to put
 * the ProtocolStep onto the undoStack.
 *
 * Starting a new Action will clear the redo stack.
 */

#include <vector>
#include <cassert>
#include <string>
#include <streambuf>
#include <map>
#include <list>
#include <csignal>
#include <ctime>
#include <iostream>
#include <fstream>
#include <queue>
#include <stack>
#include <math.h>

#include <Windows.h>

using namespace std;

typedef unsigned char qint8;
typedef unsigned char quint8;
typedef unsigned int quint32;
typedef unsigned short quint16;

typedef string StringList;
typedef string QString;
typedef ifstream QDataStream;

//typedef unsigned char       BYTE;
//typedef unsigned short      WORD;
typedef unsigned long       DWORD;

//#define printLog(x)

#define foreach(element, iter, container) \
    for (iter = (container).begin(); iter != (container).end(); iter++)\
           for (element = iter->second ; false;)


//#define foreach(container,it) \
//for( it = (container).begin();it!=(container).end();++it)

class Color{
    public:
        Color(int a, int b, int c, int d);

    private:
};

class QByteArray{

	public:
	    QByteArray();
        void append(int val);
        int  count();
		void clear();
		int  length();

	private:
		unsigned char *data;
		int num;
};



template <class T>
class QList : public std::list<T> {
    public:
        T  at(int i);
        bool contains(T val);
};


void Wchar_tToString(string& szDst, wchar_t *wchar);


#endif

