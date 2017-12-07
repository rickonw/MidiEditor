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

#ifndef MIDIINPUT_H_
#define MIDIINPUT_H_

#include "../pub/DataType.h"

class MidiEvent;
class RtMidiIn;
class RtMidiOut;
class MidiTrack;

class MidiInput {

	public:

		static void init();

		static void sendCommand(ByteArray array);
		static void sendCommand(MidiEvent *e);

		static string inputPorts();
		static bool setInputPort(string name);
		static string inputPort();

		static void startInput();
		static multimap<int, MidiEvent*> endInput(MidiTrack *track);

		static void receiveMessage(double deltatime,
				std::vector< unsigned char > *message, void *userData = 0);

		static void setTime(int ms);

		static bool recording();
		static void setThruEnabled(bool b);
		static bool thru();

	private:
		static string _inPort;
		static RtMidiIn *_midiIn;
		static multimap<int, std::vector<unsigned char> > *_messages;
		static int _currentTime;
		static bool _recording;
		static Qlist<int> toUnique(Qlist<int> in);
		static bool _thru;

};

#endif
