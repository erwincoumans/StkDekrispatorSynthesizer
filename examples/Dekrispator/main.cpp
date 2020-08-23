/**************  Test Main Program Individual Voice *********************/

#include "SKINImsg.h"
#include "Instrmnt.h"
#include "JCRev.h"
#include "Messager.h"
#include "RtAudio.h"

#include <signal.h>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <assert.h>


#include "../../CloudSeed/Default.h"
#include "../../CloudSeed/ReverbController.h"
#include "../../CloudSeed/FastSin.h"
#include "../../CloudSeed/AudioLib/ValueTables.h"

using namespace CloudSeed;
bool isInitialized = false;

#include <vector>

std::vector<char> custom_pool;

size_t pool_index = 0;
int allocation_count = 0;

void* custom_pool_allocate(size_t size)
{
	printf("================\n");
	printf("allocation %d\n", allocation_count++);
	printf("current usage: %d\n", pool_index);
	printf("allocation: %d\n", size);

	if (pool_index + size >= custom_pool.size())
	{
		printf("Running out of memory!\n");
	}
	assert(pool_index + size < custom_pool.size());
	void* ptr = &custom_pool[pool_index];
	pool_index += size;
	return ptr;
}


extern "C"
{
	ReverbController* Create(int samplerate)
	{
		if (!isInitialized)
		{
			AudioLib::ValueTables::Init();
			FastSin::Init();
			isInitialized = true;
		}

		return new ReverbController(samplerate);
	}

	 void Delete(ReverbController* item)
	{
		delete item;
	}

	 int GetSamplerate(ReverbController* item)
	{
		return item->GetSamplerate();
	}

	 void SetSamplerate(ReverbController* item, int samplerate)
	{
		return item->SetSamplerate(samplerate);
	}

	 int GetParameterCount(ReverbController* item)
	{
		return item->GetParameterCount();
	}

	 float* GetAllParameters(ReverbController* item)
	{
		return item->GetAllParameters();
	}

	 float GetScaledParameter(ReverbController* item, Parameter param)
	{
		return item->GetScaledParameter(param);
	}

	 void SetParameter(ReverbController* item, Parameter param, float value)
	{
		item->SetParameter(param, value);
	}

	 void ClearBuffers(ReverbController* item)
	{
		item->ClearBuffers();
	}

	 void Process(ReverbController* item, float* input, float* output, int bufferSize)
	{
		item->Process(input, output, bufferSize);
	}
}
ReverbController* reverb = 0;


extern "C" {
	bool    demoMode = true;
	bool    freeze = false;

	double masterVolume = 0.3;//in range 0..1
};

#define MAX_MIDI_DEVICES 10
//Simple C-API to Dekrispator
extern "C" {
	void make_sound_double(double *buf , uint16_t length) ;
	void randomGen_init(void);
	void	Synth_Init(void);	
	void MIDI_Decode(uint8_t * outBuf);
	void playSomeNote(float freq);
	void stopSomeNote();
	void sequencer_newSequence_action();
	
}


using std::min;
using namespace stk;

StkFloat float_random(StkFloat max) // Return random float between 0.0 and max
{	
  StkFloat temp = (StkFloat) (max * rand() / (RAND_MAX + 1.0) );
  return temp;	
}

void usage(void) {
  // Error function in case of incorrect command-line argument specifications.
  std::cout << "\nuseage: App_Dekrispator flags \n";
  std::cout << "    where flag = -s RATE to specify a sample rate,\n";
  std::cout << "    flag = -ip for realtime SKINI input by pipe\n";
  std::cout << "           (won't work under Win95/98),\n";
  std::cout << "    and flag = -is <port> for realtime SKINI input by socket.\n";
  exit(0);
}

bool done;
static void finish(int ignore){ done = true; }

// The TickData structure holds all the class instances and data that
// are shared by the various processing functions.
struct TickData {
  JCRev    reverbs[2];
  Messager messagers[MAX_MIDI_DEVICES];
	int numMessagers ;
	
  Skini::Message message;
  StkFloat lastSample;
  StkFloat t60;
  int counter;
  bool settling;
  bool haveMessage1;
  StkFloat droneChance, noteChance;
  StkFloat drumChance, voiceChance;
  int tempo;
  int chanceCounter;
  int key;
  int ragaStep;
  int ragaPoint;
  int endPhase;
  StkFloat rateScaler;

  // Default constructor.
  TickData()
    : numMessagers(0), t60(4.0), counter(0),
      settling( false ), haveMessage1( false ), droneChance(0.01), noteChance(0.01),
      drumChance(0.0), voiceChance(0.0), tempo(3000), chanceCounter(3000), key(0), ragaPoint(6), endPhase(0) {}
};



#define DELTA_CONTROL_TICKS 64 // default sample frames between control input checks

int numActive = 0;

// The processMessage() function encapsulates the handling of control
// messages.  It can be easily relocated within a program structure
// depending on the desired scheduling scheme.
void processMessage( TickData* data )
{
	uint8_t rawMidi[4]={0,0,0,0};
	rawMidi[0] = 0;
	rawMidi[1] = data->message.type;
	
	register StkFloat value1 = data->message.floatValues[0];
	register StkFloat value2 = data->message.floatValues[1];
	unsigned int channel = (unsigned int) data->message.channel;
	//printf("message : %ld, value1=%f, value2=%f\n", data->message.type,value1,value2);
	
	if (data->message.intValues.size()>=2)
	{
		rawMidi[2] = data->message.intValues[0];
		rawMidi[3] = data->message.intValues[1];
	}
	
	switch (data->message.type)
	{
		case __SK_NoteOn_:
		{
			demoMode = false;
			
			if ( value2 > 0.0 ) { // velocity > 0
				unsigned int iNote = data->message.intValues[0];
				
				if ( channel == 0 ) 
				{ // do basic voice management
				 
				 //only one voice...
				 if (numActive>0)
					break;
				 numActive++;
				 float freq = Midi2Pitch[iNote];
					
					playSomeNote(freq);
					/*
					unsigned int s;
					if ( data->keysDown >= (int) nStrings ) 
						break; // ignore extra note on's

					// Find first unused string
					for ( s=0; s<nStrings; s++ )
					{
						if ( !data->voices[s].inUse ) 
							break;
					}
					if ( s == nStrings ) 
						break;
					data->voices[s].inUse = true;
					data->voices[s].iNote = iNote;
					//data->guitar->noteOn( Midi2Pitch[iNote], value2 * ONE_OVER_128, s );
					data->keysDown++;
					// If first key down, turn on feedback gain
					if ( data->keysDown == 1 )
					{
						data->feedbackGain = data->oldFeedbackGain;
					}
				}
				else 
					if ( channel <= nStrings )
					{
						data->guitar->noteOn( Midi2Pitch[iNote], value2 * ONE_OVER_128, channel-1 );
					}
				 */
				}
				
				break;
			}

			break;
		}
		case __SK_NoteOff_:
		{
			numActive = 0;//numActive--;
		
			stopSomeNote();
			break;
		}
		case __SK_ControlChange_:
		{
			MIDI_Decode(rawMidi);
			break;
		}	
		default:
		{
		}
	};

  data->haveMessage1 = false;
  return;

 settle:
  // Exit and program change messages are preceeded with a short settling period.
  data->counter = (int) (data->t60 * Stk::sampleRate());
  data->settling = true;
  std::cout << "What Need Have I for This?" << std::endl;
}


// The tick() function handles sample computation and scheduling of
// control updates.  It will be called automatically by RtAudio when
// the system needs a new buffer of audio samples.
int tick( void *outputBuffer, void *inputBuffer1, unsigned int nBufferFrames,
         double streamTime, RtAudioStreamStatus status, void *dataPointer )
{
	
//	double buf[10]={0,0,0,0,0,0,0,0,0};
//	uint16_t length=4 ;
	
	
  TickData *data = (TickData *) dataPointer;
  StkFloat temp, ins[2], outs[2], *samples = (StkFloat *) outputBuffer;
  int i, voiceNote, counter, nTicks = (int) nBufferFrames;


  while ( nTicks > 0 && !done ) {

    if ( !data->haveMessage1 ) 
	{
		
		for (int i=0;i<data->numMessagers&& !data->haveMessage1;i++)
		{
      data->messagers[i].popMessage( data->message );
      if ( data->message.type > 0 ) {
        data->counter = (long) (data->message.time * Stk::sampleRate());
        data->haveMessage1 = true;
      }
      else
        data->counter = DELTA_CONTROL_TICKS;
		}
	}

    counter = min( nTicks, data->counter );
    data->counter -= counter;
	  bool newsynth = true;
	  if (newsynth)
	  {
		  
		  
		  for (int i=0;i<counter;i++)
		  {
			  //Dekrispator sound
			  make_sound_double(ins,1);
			  
			  float insf[2], outsf[2];
			  insf[0] = ins[0];
			  insf[1] = ins[1];
			  Process(reverb, insf, outsf, 1);
			  outs[0] = outsf[0];
			  outs[1] = outsf[1];
			  //outs[0] = ins[0];
			  //outs[1] = ins[1];
			  //outs[0] = data->reverbs[0].tick(outs[0]);
			  //outs[1] = data->reverbs[1].tick(outs[1]);
			  *samples++ = outs[0]*masterVolume;
			  *samples++ = outs[1]*masterVolume;
		  }
		  nTicks-=counter;
	  }
    if ( nTicks == 0 ) break;

    // Process control messages.
    if ( data->haveMessage1 ) 
		processMessage( data );
  }

  return 0;
}


int main( int argc, char *argv[] )
{
	custom_pool.resize(64 * 1024 * 1024);
	double sampleRate = 44100.0;
	reverb = Create(sampleRate);

	//Dekrispator init
	randomGen_init();
	Synth_Init();
	//end Dekrispator init
	
//	FILE* f = fopen("bla.txt","wb");
//	fclose(f);
	
  TickData data;
  RtAudio dac;
  int i;

  int audioDevice=-1;


  // If you want to change the default sample rate (set in Stk.h), do
  // it before instantiating any objects!  If the sample rate is
  // specified in the command line, it will override this setting.
  
  Stk::setSampleRate(sampleRate);
  
	{
	 RtMidiIn *midiin = 0;
	 midiin = new RtMidiIn();
	unsigned int i = 0, nPorts = midiin->getPortCount();
	if ( nPorts == 0 ) {
		std::cout << "No input Midi ports available, just running demo mode." << std::endl;
		delete midiin;
		midiin = 0;
	} else
	{
		for ( i=0; i<nPorts; i++ ) {
			std::string portName = midiin->getPortName(i);
			std::cout << "  Input port #" << i << ": " << portName << '\n';
		}

		delete midiin;
		midiin = 0;
		
		for ( i=0; i<nPorts && i<MAX_MIDI_DEVICES; i++ ) {
			data.messagers[data.numMessagers++].startMidiInput(i);
		}

		
	}
	
	}
	
	
  // Parse the command-line arguments.
  unsigned int port = 2001;
  for ( i=1; i<argc; i++ ) {
    if ( !strcmp( argv[i], "-is" ) ) {
      if ( i+1 < argc && argv[i+1][0] != '-' ) port = atoi(argv[++i]);
		if (data.numMessagers<MAX_MIDI_DEVICES)
		{
		data.messagers[data.numMessagers++].startSocketInput( port );
		}
    }
    else if (!strcmp( argv[i], "-ip" ) )
	{
		if (data.numMessagers<MAX_MIDI_DEVICES)
		{
      data.messagers[data.numMessagers++].startStdInput();
		}
	}
    else if ( !strcmp( argv[i], "-s" ) && ( i+1 < argc ) && argv[i+1][0] != '-')
      Stk::setSampleRate( atoi(argv[++i]) );
    else if ( !strcmp( argv[i], "-o" ) && ( i+1 < argc ) && argv[i+1][0] != '-')
      audioDevice = atoi(argv[++i]) ;
    else
      usage();
  }

  if (data.numMessagers == 0)
  {
	        data.messagers[data.numMessagers++].startStdInput();

  }
  // Allocate the dac here.
  RtAudioFormat format = ( sizeof(StkFloat) == 8 ) ? RTAUDIO_FLOAT64 : RTAUDIO_FLOAT32;
  RtAudio::StreamParameters parameters;
  parameters.deviceId = audioDevice>=0 ? audioDevice :  dac.getDefaultOutputDevice();
  parameters.nChannels = 2;
  unsigned int bufferFrames = RT_BUFFER_SIZE;
  try {
    dac.openStream( &parameters, NULL, format, (unsigned int)Stk::sampleRate(), &bufferFrames, &tick, (void *)&data );
  }
  catch ( RtAudioError& error ) {
    error.printMessage();
    goto cleanup;
  }

  data.reverbs[0].setT60( data.t60 );
  data.reverbs[0].setEffectMix( 0.5 );
  data.reverbs[1].setT60( 2.0 );
  data.reverbs[1].setEffectMix( 0.5 );

  
  ClearBuffers(reverb);

 
  data.rateScaler = 22050.0 / Stk::sampleRate();

  // Install an interrupt handler function.
	(void) signal( SIGINT, finish );

  // If realtime output, set our callback function and start the dac.
  try {
    dac.startStream();
  }
  catch ( RtAudioError &error ) {
    error.printMessage();
    goto cleanup;
  }

  // Setup finished.
  while ( !done ) {
    // Periodically check "done" status.
    Stk::sleep( 50 );
  }

  // Shut down the output stream.
  try {
    dac.closeStream();
  }
  catch ( RtAudioError& error ) {
    error.printMessage();
  }

 cleanup:

  Delete(reverb);

  return 0;

}
