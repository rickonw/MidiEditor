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
#include "DataType.h"


ByteArray::ByteArray() {
	return;
}

void ByteArray::append(int val) {
	return;
}

int ByteArray::count() {
	return 0;
}

int ByteArray::length() {
	return 0;
}


void ByteArray::clear() {
	return;
}


Color::Color(int a, int b, int c, int d) {
	return;
}

template <class T>

T Qlist<T>::at(int i)
{
	Qlist<T>::iterator pnlist = this->begin();
	advance(pnlist, i);
	if (pnlist != this->end()) {
		return *pnlist;
	}
	else {
		return NULL;
	}
}

template <class T>
bool Qlist<T>::contains(T val)
{
	return !!find(this->begin(), this->end(), val));
}



void Wchar_tToString(std::string& szDst, wchar_t *wchar)
{
    wchar_t * wText = wchar;
    DWORD dwNum = WideCharToMultiByte(CP_OEMCP,NULL,wText,-1,NULL,0,NULL,FALSE);
    char *psText;
    psText = new char[dwNum];
    WideCharToMultiByte (CP_OEMCP,NULL,wText,-1,psText,dwNum,NULL,FALSE);
    szDst = psText;
    delete []psText;
}


