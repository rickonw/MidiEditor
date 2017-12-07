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

#ifndef MIDIEVENT_H_
#define MIDIEVENT_H_

#include "../protocol/ProtocolEntry.h"
#include "../pub/DataType.h"

class MidiFile;
class MidiTrack;

class MidiEvent : public ProtocolEntry {

	public:
		MidiEvent(int channel, MidiTrack *track);
		MidiEvent(MidiEvent &other);

		static MidiEvent *loadMidiEvent(ifstream *content,
				bool *ok, bool *endEvent, MidiTrack *track, quint8 startByte = 0,
				quint8 secondByte = 0);

		enum {
			TEMPO_CHANGE_EVENT_LINE = 128,
			TIME_SIGNATURE_EVENT_LINE,
			KEY_SIGNATURE_EVENT_LINE,
			PROG_CHANGE_LINE,
			CONTROLLER_LINE,
			KEY_PRESSURE_LINE,
			CHANNEL_PRESSURE_LINE,
			TEXT_EVENT_LINE,
			PITCH_BEND_LINE,
			SYSEX_LINE,
			UNKNOWN_LINE
		};
		void setTrack(MidiTrack *track, bool toProtocol=true);
		MidiTrack *track();
		void setChannel(int channel, bool toProtocol=true);
		int channel();
		virtual void setMidiTime(int t, bool toProtocol = true);
		int midiTime();
		void setFile(MidiFile *f);
		MidiFile *file();
		bool shownInEventWidget();

		virtual int line();
		virtual string toMessage();
		virtual ByteArray save();
		//virtual void draw(QPainter *p, Color c);

		virtual ProtocolEntry *copy();
		virtual void reloadState(ProtocolEntry *entry);

		virtual string typeString();

		virtual bool isOnEvent();

		static map<int, string> knownMetaTypes();

		void setTemporaryRecordID(int id);
		int temporaryRecordID();

		virtual void moveToChannel(int channel);

	protected:
		int numChannel, timePos;
		MidiFile *midiFile;
		static quint8 _startByte;
		MidiTrack *_track;
		int _tempID;
};

#endif
