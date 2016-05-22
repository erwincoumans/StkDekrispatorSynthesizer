#ifndef VOICE_WAVE_H
#define VOICE_WAVE_H

#include "Stk.h"
#include <vector>
#include "MyInstrument.h"
#include "OnePole.h"//??

namespace stk
{
	
class VoiceWave
{
	
	std::vector<MyInstrument>		m_instruments;
	
	std::vector< int > stringState_; // 0 = off, 1 = decaying, 2 = on
	std::vector< unsigned int > decayCounter_;
	std::vector< unsigned int > filePointer_;
	std::vector< StkFloat > pluckGains_;
	
//	OnePole   pickFilter_;
	OnePole   couplingFilter_;
	StkFloat  couplingGain_;
	StkFrames excitation_;
	StkFrames lastFrame_;
	
public:
	
	VoiceWave(int numVoices);
	
	virtual ~VoiceWave();
	
	//! Set the loop gain for one or all strings.
	/*!
	 If the \c string argument is < 0, the loop gain is set for all
	 strings.
  */
	void setLoopGain( StkFloat gain, int string = -1 );
	
	//! Set instrument parameters for a particular frequency.
	void setFrequency( StkFloat frequency, unsigned int string = 0 );
	
	//! Start a note with the given frequency and amplitude.
	/*!
	 If the \c amplitude parameter is less than 0.2, the string will
	 be undamped but it will not be "plucked."
	 */
	void noteOn( StkFloat frequency, StkFloat amplitude, unsigned int string = 0 );
	
	//! Stop a note with the given amplitude (speed of decay).
	void noteOff( StkFloat amplitude, unsigned int string = 0 );
	
	//! Perform the control change specified by \e number and \e value (0.0 - 128.0).
	/*!
	 If the \c string argument is < 0, then the control change is
	 applied to all strings (if appropriate).
  */
	void controlChange( int number, StkFloat value, int string = -1 );
	
	//! Return the last computed output value.
	StkFloat lastOut( void ) { return lastFrame_[0]; };
	
	//! Take an optional input sample and compute one output sample.
	StkFloat tick( StkFloat input = 0.0 );

	
};
};


#endif //VOICE_WAVE_H