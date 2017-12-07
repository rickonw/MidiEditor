#ifndef METRONOME_H
#define METRONOME_H


class MidiFile;

class Metronome {

	public:
		Metronome();

		void setFile(MidiFile *file);
		static Metronome *instance();

		static bool enabled();
		static void setEnabled(bool b);

	//public slots:
		void measureUpdate(int measure, int tickInMeasure);
		void meterChanged(int n, int d);
		void playbackStarted();
		void playbackStopped();

	private:
		static Metronome *_instance;
		MidiFile *_file;
		void reset();
		int num, denom, lastPos, lastMeasure;
		void click();
		static bool _enable;
};

#endif
