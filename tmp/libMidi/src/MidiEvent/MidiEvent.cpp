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

#include "MidiEvent.h"
#include "NoteOnEvent.h"
#include "OffEvent.h"
#include "UnknownEvent.h"
#include "TempoChangeEvent.h"
#include "TimeSignatureEvent.h"
#include "ProgChangeEvent.h"
#include "ControlChangeEvent.h"
#include "ChannelPressureEvent.h"
#include "KeyPressureEvent.h"
#include "TextEvent.h"
#include "../midi/MidiFile.h"
#include "KeySignatureEvent.h"
#include "PitchBendEvent.h"
#include "SysExEvent.h"
#include "../midi/MidiTrack.h"

#include "../midi/MidiChannel.h"

quint8 MidiEvent::_startByte = 0;

MidiEvent::MidiEvent(int channel, MidiTrack *track) : ProtocolEntry() {
	_track = track;
	numChannel = channel;
	timePos = 0;
	midiFile = 0;
}

MidiEvent::MidiEvent(MidiEvent &other) : ProtocolEntry(other) {
	_track = other._track;
	numChannel = other.numChannel;
	timePos = other.timePos;
	midiFile = other.midiFile;
}

MidiEvent *MidiEvent::loadMidiEvent(ifstream *content, bool *ok,
		bool *endEvent, MidiTrack *track, quint8 startByte, quint8 secondByte)
{

	// first try to load the event. If this does not work try to use
	// old first byte as new first byte. This is implemented in the end of this
	// method using recursive calls.
	// if startByte (paramater) is not 0, this is the second call so first and
	// second byte must not be loaded from the stream but from the parameters.

	*ok = true;

	quint8 tempByte;

	quint8 prevStartByte = _startByte;

	if(!startByte){
		(*content)>>tempByte;
	} else {
		tempByte = startByte;
	}
	_startByte = tempByte;

	int channel = tempByte & 0x0F;


	switch(tempByte&0xF0){

		case 0x80: {
			// Note Off
			if(!startByte){
				(*content)>>tempByte;
			} else {
				tempByte = secondByte;
			}
			int note = tempByte;
			if(note<0 || note>127){
				*ok = false;
				return 0;
			}
			// skip byte (velocity)
			(*content)>>tempByte;

			OffEvent *event = new OffEvent(channel, 127-note, track);
			*ok = true;
			return event;
		}


		case 0x90: {
			// Note On
			if(!startByte){
				(*content)>>tempByte;
			} else {
				tempByte = secondByte;
			}
			int note = tempByte;
			if(note<0 || note>127){
				*ok = false;
				return 0;
			}
			(*content)>>tempByte;
			int velocity = tempByte;
			*ok = true;

			if(velocity>0){
				NoteOnEvent *event = new NoteOnEvent(note, velocity, channel, track);
				return event;
			} else {
				OffEvent *event = new OffEvent(channel, 127-note, track);
				return event;
			}
		}

		case 0xA0: {
			// Key Pressure
			if(!startByte){
				(*content)>>tempByte;
			} else {
				tempByte = secondByte;
			}
			int note = tempByte;
			if(note<0 || note>127){
				*ok = false;
				return 0;
			}
			(*content)>>tempByte;
			int value = tempByte;

			*ok = true;

			return new KeyPressureEvent(channel, value, note, track);
		}

		case 0xB0: {
			// Controller
			if(!startByte){
				(*content)>>tempByte;
			} else {
				tempByte = secondByte;
			}
			int control = tempByte;
			(*content)>>tempByte;
			int value = tempByte;
			*ok = true;
			return new ControlChangeEvent(channel, control, value, track);
		}

		case 0xC0: {
			// programm change
			if(!startByte){
				(*content)>>tempByte;
			} else {
				tempByte = secondByte;
			}
			*ok = true;
			return new ProgChangeEvent(channel, tempByte, track);
		}

		case 0xD0: {
			// Key Pressure
			if(!startByte){
				(*content)>>tempByte;
			} else {
				tempByte = secondByte;
			}
			int value = tempByte;

			*ok = true;

			return new ChannelPressureEvent(channel, value, track);
		}

		case 0xE0: {

			// Pitch Wheel
			if(!startByte){
				(*content)>>tempByte;
			} else {
				tempByte = secondByte;
			}
			quint8 first = tempByte;
			(*content)>>tempByte;
			quint8 second = tempByte;

			int value = (second << 7) | first;

			*ok = true;

			return new PitchBendEvent(channel, value, track);
		}

		case 0xF0: {
			// System Message
			channel = 16; // 16 is channel without number


			switch(tempByte & 0x0F){

				case 0x00: {

					// SysEx
					ByteArray array;
					while(tempByte!=0xF7){
						(*content)>>tempByte;
						if(tempByte!=0xF7){
							array.append((char)tempByte);
						}
					}
					*ok = true;
					return new	SysExEvent(channel, array, track);
				}

				case 0x0F: {
					// MetaEvent
					if(!startByte){
						(*content)>>tempByte;
					} else {
						tempByte = secondByte;
					}
					switch(tempByte){
						case 0x51: {
							// TempoChange
							//(*content)>>tempByte;
							//if(tempByte!=3){
							//	*ok = false;
							//	return 0;
							//}
							quint32 value;
							(*content)>>value;
							// 1te Stelle abziehen,
							value-=50331648;
							return new TempoChangeEvent(17, (int)value, track);
						}
						case 0x58: {
							// TimeSignature
							(*content)>>tempByte;
							if(tempByte!=4){
								*ok = false;
								return 0;
							}

							(*content)>>tempByte;
							int num = (int)tempByte;
							(*content)>>tempByte;
							int denom = (int)tempByte;
							(*content)>>tempByte;
							int metronome = (int)tempByte;
							(*content)>>tempByte;
							int num32 = (int)tempByte;
							return new TimeSignatureEvent(18, num, denom, metronome, num32, track);
						}
						case 0x59: {
							// keysignature
							(*content)>>tempByte;
							if(tempByte!=2){
								*ok = false;
								return 0;
							}
							qint8 t;
							(*content)>>t;
							int tonality = (int)t;
							(*content)>>tempByte;
							bool minor = true;
							if(tempByte == 0){
								minor = false;
							}
							return new KeySignatureEvent(channel, tonality, minor, track);
						}
						case 0x2F: {
							// end Event
							*endEvent = true;
							*ok = true;
							return 0;
						}
						default: {
							if(tempByte >= 0x01 && tempByte <=0x07){

								// textevent
								// read type
								TextEvent *textEvent = new TextEvent(channel, track);
								textEvent->setType(tempByte);
								int length = MidiFile::variableLengthvalue(content);
								// use wchar_t because some files use Unicode.
								wchar_t str [128] = L"";
								for(int i = 0; i<length; i++){
									(*content)>>tempByte;
									wchar_t temp [2] = { btowc(tempByte) };
									wcsncat(str, temp, 1);
								}
								textEvent->setText(Wchar_tToString(str));
								*ok = true;
								return textEvent;

							} else {

								// tempByte is meta event type
								int typeByte = ((char)tempByte);

								// read length
								int length = MidiFile::variableLengthvalue(content);

								// content
								ByteArray array;
								for(int i = 0; i<length; i++){
									(*content)>>tempByte;
									array.append((char)tempByte);
								}
								*ok = true;
								return new UnknownEvent(channel, typeByte, array, track);
							}
						}
					}

				}
			}

		}
	}

	// if the event could not be loaded try to use old firstByte before the new
	// data.
	// To do this, pass prefFirstByte and secondByte (the current firstByte)
	// and use it recursive.
	_startByte = prevStartByte;
	return loadMidiEvent(content, ok, endEvent, track, _startByte, tempByte);
}

void MidiEvent::setTrack(MidiTrack *track, bool toProtocol){
	ProtocolEntry *toCopy = copy();

	_track = track;
	if(toProtocol){
		protocol(toCopy, this);
	} else {
		delete toCopy;
	}
}

MidiTrack *MidiEvent::track(){
	return _track;
}

void MidiEvent::setChannel(int ch, bool toProtocol){
	int oldChannel = channel();
	ProtocolEntry *toCopy = copy();
	numChannel = ch;
	if(toProtocol){
		protocol(toCopy, this);
        multimap<int,MidiEvent*>::iterator iter, begin,end;
        begin = file()->channelEvents(oldChannel)->lower_bound(midiTime());
        end = file()->channelEvents(oldChannel)->upper_bound(midiTime());
        for(iter = begin;iter != end;iter++) {
            if(iter->second == this) {
                file()->channelEvents(oldChannel)->erase(iter++);
            }
        }

		//file()->channelEvents(oldChannel)->remove(midiTime(), this);
		// tells the new channel to add this event
		setMidiTime(midiTime(), toProtocol);
	} else {
		delete toCopy;
	}
}

int MidiEvent::channel(){
	return numChannel;
}

string MidiEvent::toMessage(){
	return "";
}

ByteArray MidiEvent::save(){
	return ByteArray();
}

void MidiEvent::setMidiTime(int t, bool toProtocol){

	// if its once TimeSig / TempoChange at 0, dont delete event
	if(toProtocol && (channel()==18 || channel()==17)){
		if(midiTime()==0 && midiFile->channel(channel())->eventMap()->count(0)
				== 1)
		{
			return;
		}
	}

	ProtocolEntry *toCopy = copy();
    multimap<int,MidiEvent*>::iterator iter, begin,end;
    begin = file()->channelEvents(numChannel)->lower_bound(timePos);
    end = file()->channelEvents(numChannel)->upper_bound(timePos);
    for(iter = begin;iter != end;iter++) {
		if(iter->second == this) {
			file()->channelEvents(numChannel)->erase(iter++);
		}
	}

	//file()->channelEvents(numChannel)->remove(timePos, this);
	timePos = t;
	if(timePos>file()->endTick()){
		file()->setMaxLengthMs(file()->msOfTick(timePos)+100);
	}
	if(toProtocol){
		protocol(toCopy, this);
	} else {
		delete toCopy;
	}

	file()->channelEvents(numChannel)->insert(make_pair(timePos, this));
}

int MidiEvent::midiTime(){
	return timePos;
}

void MidiEvent::setFile(MidiFile *f){
	midiFile = f;
}

MidiFile *MidiEvent::file(){
	return midiFile;
}

int MidiEvent::line(){
	return 0;
}

/*
void MidiEvent::draw(QPainter *p, Color c){
	p->setPen(Qt::gray);
	p->setBrush(c);
	p->drawRoundedRect(x(), y(), width(), height(), 1, 1);
}
*/
ProtocolEntry *MidiEvent::copy(){
	return new MidiEvent(*this);
}

void MidiEvent::reloadState(ProtocolEntry *entry){

	MidiEvent *other = dynamic_cast<MidiEvent*>(entry);
	if(!other){
		return;
	}
	_track = other->_track;
    multimap<int,MidiEvent*>::iterator iter, begin,end;
    begin = file()->channelEvents(numChannel)->lower_bound(timePos);
    end = file()->channelEvents(numChannel)->upper_bound(timePos);
    for(iter = begin;iter != end;iter++) {
		if(iter->second == this) {
			file()->channelEvents(numChannel)->erase(iter++);
		}
	}

	//file()->channelEvents(numChannel)->remove(timePos, this);
	numChannel = other->numChannel;

    begin = file()->channelEvents(numChannel)->lower_bound(timePos);
    end = file()->channelEvents(numChannel)->upper_bound(timePos);
    for(iter = begin;iter != end;iter++) {
		if(iter->second == this) {
			file()->channelEvents(numChannel)->erase(iter++);
		}
	}
	//file()->channelEvents(numChannel)->remove(timePos, this);
	timePos = other->timePos;
	file()->channelEvents(numChannel)->insert(make_pair(timePos, this));
	midiFile = other->midiFile;
}

string MidiEvent::typeString(){
	return "Midi Event";
}

bool MidiEvent::isOnEvent(){
	return true;
}

map<int, string> MidiEvent::knownMetaTypes(){
	map<int, string> meta;
	for(int i = 1; i<8; i++){
		meta.insert(make_pair(i, "Text Event"));
	}
	meta.insert(make_pair(0x51, "Tempo Change Event"));
	meta.insert(make_pair(0x58, "Time Signature Event"));
	meta.insert(make_pair(0x59, "Key Signature Event"));
	meta.insert(make_pair(0x2F, "End of Track"));
	return meta;
}

void MidiEvent::setTemporaryRecordID(int id){
	_tempID = id;
}

int MidiEvent::temporaryRecordID(){
	return _tempID;
}

void MidiEvent::moveToChannel(int ch){

	int oldChannel = channel();

	if(oldChannel > 15){
		return;
	}

	if(oldChannel == ch){
		return;
	}

	midiFile->channel(oldChannel)->removeEvent(this);

	ProtocolEntry *toCopy = copy();

	numChannel = ch;

	protocol(toCopy, this);

	midiFile->channel(ch)->insertEvent(this, midiTime());
}
