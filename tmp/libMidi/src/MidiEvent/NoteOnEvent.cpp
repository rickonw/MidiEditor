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

#include "NoteOnEvent.h"

#include "OffEvent.h"

NoteOnEvent::NoteOnEvent(int note, int velocity, int ch, MidiTrack *track) : OnEvent(ch, track){
	_note = note;
	_velocity = velocity;
	// has to be done here because the line is not known in OnEvents constructor
	// before
	OffEvent::enterOnEvent(this);
}

NoteOnEvent::NoteOnEvent(NoteOnEvent &other) : OnEvent(other){
	_note = other._note;
	_velocity = other._velocity;
}

int NoteOnEvent::note(){
	return _note;
}

int NoteOnEvent::velocity(){
	return _velocity;
}

void NoteOnEvent::setVelocity(int v){
	ProtocolEntry *toCopy = copy();
	if(v<0){
		v = 0;
	}
	if(v>127){
		v = 127;
	}
	_velocity = v;
	protocol(toCopy, this);
}

int NoteOnEvent::line(){
	return 127-_note;
}

void NoteOnEvent::setNote(int n){
	ProtocolEntry *toCopy = copy();
	_note = n;
	protocol(toCopy, this);
}

ProtocolEntry *NoteOnEvent::copy(){
	return new NoteOnEvent(*this);
}

void NoteOnEvent::reloadState(ProtocolEntry *entry){
	NoteOnEvent *other = dynamic_cast<NoteOnEvent*>(entry);
	if(!other){
		return;
	}
	OnEvent::reloadState(entry);

	_note = other->_note;
	_velocity = other->_velocity;
}


string NoteOnEvent::toMessage(){
	return "noteon "+to_string(channel())+" "+
			to_string(note())+" "+to_string(velocity());
}

string NoteOnEvent::offEventMessage(){
	return "noteoff "+to_string(channel())+" "+to_string(note());
}

ByteArray NoteOnEvent::save(){
	ByteArray array = ByteArray();
	array.append(0x90 | channel());
	array.append(note());
	array.append(velocity());
	return array;
}

ByteArray NoteOnEvent::saveOffEvent(){
	ByteArray array = ByteArray();
	array.append(0x80 | channel());
	array.append(note());
	array.append((char)0x0);
	return array;
}

string NoteOnEvent::typeString(){
	return "Note On Event";
}
