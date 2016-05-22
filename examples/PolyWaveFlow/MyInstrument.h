#ifndef MY_INSTRUMENT_H
#define MY_INSTRUMENT_H

#include "Stk.h"
#include <math.h>

#include "Fir.h"

namespace stk
{

	
	
#define MY_2PI (2. * 3.141592538)
#define MY_SAMPLERATE              48000
#define MyFs             			((float)(MY_SAMPLERATE))	// samplerate
#define MyTs						(1.f/MyFs)  // sample period
	
	struct MyOscillator
	{
		StkFloat 	amp;	// should be <= 1 for normal sound output
		StkFloat		last_amp;
		StkFloat 	freq;	// Hertz
		StkFloat 	phase;	// radians
		StkFloat		phi0;	// radians
		StkFloat		modInd;	// Modulation Index for FM
		StkFloat		mul;	// pitch frequency multiplier
		StkFloat 	out;	// output sample
		
		MyOscillator()
		{
			amp = 0.8;
			last_amp = 0.7;
			freq = 440;
			phase = 0;
			out = 0;
			modInd = 0;
			mul = 1;
		}
		/*----------------------------------------------------------------------------------------------*/
		StkFloat MyOpSampleCompute0() // accurate sine
		{
			StkFloat z;
			
			//while (op->phase < 0) // keep phase in [0, 2pi]
			//	op->phase += _2PI;
			while (phase >= MY_2PI)
				phase -= MY_2PI;
			
			z = sinf(phase);
			out = amp*z;
			
			phase += MY_2PI * MyTs * freq; // increment phase
			return out;
		}
		
		
	} ;
	
	enum {
		MY_ATTACK,   /*!< Attack */
		MY_DECAY,    /*!< Decay */
		MY_SUSTAIN,  /*!< Sustain */
		MY_RELEASE,  /*!< Release */
		MY_IDLE      /*!< Before attack / after release */
	};
	
	
	struct MyADSR
	{
		float 	attackRate_;
		float 	decayRate_;
		float 	sustainLevel_;
		float 	releaseRate_;
		float 	value_;
		float	lastOutput_;
		float 	target_;
		float	rate_;
		int 	state_;
		uint32_t cnt_; // sample counter for on state
		
		MyADSR()
		{
			lastOutput_ = 0.0;
			target_ = 0.0;
			value_ = 0.0;
			attackRate_ = 0.0001;
			decayRate_ = 0.001;
			sustainLevel_ = 0.5;
			releaseRate_ = 0.001;
			state_ = MY_ATTACK;
			cnt_ = 0;
		}
		float ADSR_computeSample()
		{
			(cnt_)++;
			
			switch (state_) {
					
				case MY_ATTACK:
					value_ += rate_;
					if (value_ >= target_)
					{
						value_ = target_;
						rate_ = decayRate_;
						target_ = sustainLevel_;
						state_ = MY_DECAY;
					}
					break;
					
				case MY_DECAY:
					value_ -= decayRate_;
					if (value_ <= sustainLevel_)
					{
						value_ = sustainLevel_;
						rate_ = 0.0f;
						state_ = MY_SUSTAIN;
					}
					break;
					
				case MY_RELEASE:
					value_ -= releaseRate_;
					if (value_ <= 0.0f)
					{
						value_ =  0.0f;
						state_ = MY_IDLE;
					}
			}
			
			lastOutput_ = value_;
			return value_;
		}
		
		void ADSR_keyOff()
		{
			cnt_ = 0;
			target_ = 0.0;
			rate_ = 	releaseRate_;
			state_ = MY_RELEASE;
		}
		
		void keyOn()
		{
			cnt_ = 0;
			target_ = 1.0f;
			rate_ = 	attackRate_;
			state_ = MY_ATTACK;
		}		
		
	};

	
class MyInstrument
{
	MyOscillator m_osc1;
	MyADSR m_adsr;
	StkFloat m_frequency;
	StkFloat m_lastOutput;
	StkFloat m_gateTime;
	
	//DelayA   delayLine_;
	//DelayL   combDelay_;
	Fir      loopFilter_;
	
	StkFloat loopGain_;
	StkFloat pluckPosition_;

	
	
public:
	
	MyInstrument();
	
	virtual ~MyInstrument();
	
	//! Return the last computed output value.
	StkFloat lastOut( void ) { return m_lastOutput; };
	
	//! Take an optional input sample and compute one output sample.
	StkFloat tick( StkFloat input = 0.0 );

	//! Set the delayline parameters for a particular frequency.
	void setFrequency( StkFloat frequency );

	//! Set the nominal loop gain.
	/*!
	 The actual loop gain is based on the value set with this
	 function, but scaled slightly according to the frequency.  Higher
	 frequency settings have greater loop gains because of
	 high-frequency loop-filter roll-off.
  */
	void setLoopGain( StkFloat loopGain );
	
	void noteOn( StkFloat frequency, StkFloat amplitude );
	
	//! Stop a note with the given amplitude (speed of decay).
	void noteOff( StkFloat amplitude );
	

	
};
};

#endif //MY_INSTRUMENT_H


