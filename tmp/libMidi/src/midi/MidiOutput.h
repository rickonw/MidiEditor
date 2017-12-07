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

#ifndef MIDIOUTPUT_H_
#define MIDIOUTPUT_H_



class MidiEvent;
class RtMidiIn;
class RtMidiOut;

class MidiOutput {

	public:
		static void init();
		static void sendCommand(ByteArray array);
		static void sendCommand(MidiEvent *e);
		static string outputPorts();
		static bool setOutputPort(string name);
		static string outputPort();
		static void sendEnqueuedCommand(ByteArray array);
		static bool isAlternativePlayer;
		static map<int, Qlist<int> > playedNotes;
		static void setStandardChannel(int channel);
		static int standardChannel();
		static void sendProgram(int channel, int prog);

	private:
		static string _outPort;
		static RtMidiOut *_midiOut;
		//static SenderThread *_sender;
		static int _stdChannel;
};

#endif
