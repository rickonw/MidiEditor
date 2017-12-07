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

#ifndef MIDIFILE_H_
#define MIDIFILE_H_

#include "../protocol/ProtocolEntry.h"
#include "../pub/DataType.h"

class MidiEvent;
class TimeSignatureEvent;
class TempoChangeEvent;
class Protocol;
class MidiChannel;
class MidiTrack;

class MidiFile : public ProtocolEntry {


	public:
		MidiFile(string path, bool *ok, string *log = 0);
		MidiFile();
		// needed to protocol fileLength
		MidiFile(int maxTime, Protocol *p);
		bool save(string path);
		ByteArray writeDeltaTime(int time);
		int maxTime();
		int endTick();
		int timeMS(int midiTime);
		int measure(int midiTime, int &midiTimeInMeasure);
		map<int, MidiEvent*> *tempoEvents();
		map<int, MidiEvent*> *timeSignatureEvents();
		void calcMaxTime();
		int tick(int ms);
		int tick(int startms, int endms, Qlist<MidiEvent*> **events, int *endTick, int *msOfFirstEvent);
		int measure(int startTick, int endTick, Qlist<TimeSignatureEvent*> **eventList, int *tickInMeasure = 0);
		int msOfTick(int tick, Qlist<MidiEvent*> *events = 0, int msOfFirstEventInList = 0);

		Qlist<MidiEvent*> *eventsBetween(int start, int end);
		int ticksPerQuarter();
		multimap<int, MidiEvent*> *channelEvents(int channel);

		Protocol *protocol();
		MidiChannel *channel(int i);
		void preparePlayerData(int tickFrom);
		multimap<int, MidiEvent*> *playerData();

		static string instrumentName(int prog);
		static string controlChangeName(int control);
		int cursorTick();
		int pauseTick();
		void setCursorTick(int tick);
		void setPauseTick(int tick);
		string path();
		bool saved();
		void setSaved(bool b);
		void setPath(string path);
		bool channelMuted(int ch);
		int numTracks();
		Qlist<MidiTrack*> *tracks();
		void addTrack();
		void setMaxLengthMs(int ms);

		ProtocolEntry *copy();
		void reloadState(ProtocolEntry *entry);
		MidiFile *file();
		bool removeTrack(MidiTrack *track);
		MidiTrack *track(int number);

		int tonalityAt(int tick);
		void meterAt(int tick, int *num, int *denum);

		static int variableLengthvalue(ifstream *content);
		static ByteArray writeVariableLengthValue(int value);
		static int defaultTimePerQuarter;

		void registerCopiedTrack(MidiTrack *source, MidiTrack *destination, MidiFile *fileFrom);
		MidiTrack *getPasteTrack(MidiTrack *source, MidiFile *fileFrom);

		Qlist<int> quantization(int fractionSize);

	//signals:
		void cursorPositionChanged();
		void recalcWidgetSize();
		void trackChanged();

	private:
		bool readMidiFile(ifstream *content, string *log);
		bool readTrack(ifstream *content, int num, string *log);
		int deltaTime(ifstream *content);

		int timePerQuarter;
		MidiChannel *channels[19];

		string _path;
		int midiTicks, maxTimeMS, _cursorTick, _pauseTick, _midiFormat;
		Protocol *prot;
		multimap<int, MidiEvent*> *playerMap;
		bool _saved;
		Qlist<MidiTrack*> *_tracks;
		map<MidiFile*, map<MidiTrack*, MidiTrack*> > pasteTracks;

		void printLog(string *log);
};

#endif
