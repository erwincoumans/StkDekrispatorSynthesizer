
#include "VoiceWave.h"
#include "MyInstrument.h"

#include <vector>

using namespace stk;



VoiceWave::VoiceWave(int numVoices)
{
	m_instruments.resize(numVoices);
	stringState_.resize(numVoices); // 0 = off, 1 = decaying, 2 = on
	decayCounter_.resize(numVoices);
	filePointer_.resize(numVoices);
	pluckGains_.resize(numVoices);

	couplingGain_ = 0.01;
	couplingFilter_.setPole( 0.9 );
	//pickFilter_.setPole( 0.95 );
	lastFrame_.resize(1, 1, 0.0);
	
}

VoiceWave::~VoiceWave()
{
}

//! Set the loop gain for one or all strings.
/*!
 If the \c string argument is < 0, the loop gain is set for all
 strings.
 */
void VoiceWave::setLoopGain( StkFloat gain, int string )
{
}

//! Set instrument parameters for a particular frequency.
void VoiceWave::setFrequency( StkFloat frequency, unsigned int string)
{
	m_instruments[string].setFrequency(frequency);
}

//! Start a note with the given frequency and amplitude.
/*!
 If the \c amplitude parameter is less than 0.2, the string will
 be undamped but it will not be "plucked."
 */
void VoiceWave::noteOn( StkFloat frequency, StkFloat amplitude, unsigned int string )
{
	
	stringState_[string] = 2;
	filePointer_[string] = 0;
	m_instruments[string].noteOn(frequency, amplitude);
	pluckGains_[string] = amplitude;

}

//! Stop a note with the given amplitude (speed of decay).
void VoiceWave::noteOff( StkFloat amplitude, unsigned int string )
{
	m_instruments[string].noteOff(amplitude);
	stringState_[string] = 1;
}

//! Perform the control change specified by \e number and \e value (0.0 - 128.0).
/*!
 If the \c string argument is < 0, then the control change is
 applied to all strings (if appropriate).
 */
void VoiceWave::controlChange( int number, StkFloat value, int string )
{
}

//! Return the last computed output value.
//StkFloat lastOut( void ) { return lastFrame_[0]; };

//! Take an optional input sample and compute one output sample.
StkFloat VoiceWave::tick( StkFloat input)
{
	StkFloat temp, output = 0.0;
	int numInstruments = m_instruments.size();
	
	lastFrame_[0] /=numInstruments; // evenly spread coupling across strings
	for ( unsigned int i=0; i<numInstruments; i++ ) {
		if ( stringState_[i] ) {
			temp = input;
			// If pluckGain < 0.2, let string ring but don't pluck it.
			if ( filePointer_[i] < excitation_.frames())// && pluckGains_[i] > 0.2 )
				temp += pluckGains_[i] * excitation_[filePointer_[i]++];
			temp += couplingGain_ * couplingFilter_.tick( lastFrame_[0] ); // bridge coupling
			output += m_instruments[i].tick( temp );
			// Check if string energy has decayed sufficiently to turn it off.
			if ( stringState_[i] == 1 ) {
				if ( fabs( m_instruments[i].lastOut() ) < 0.001 ) decayCounter_[i]++;
				else decayCounter_[i] = 0;
				if ( decayCounter_[i] > (unsigned int) floor( 0.1 * Stk::sampleRate() ) ) {
					stringState_[i] = 0;
					decayCounter_[i] = 0;
				}
			}
		}
	}
	
	return lastFrame_[0] = output;
	
}
