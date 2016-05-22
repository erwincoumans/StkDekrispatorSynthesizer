
#include "MyInstrument.h"

using namespace stk;
#include <math.h>



MyInstrument::MyInstrument()
{
	m_gateTime = 4500;
}

MyInstrument::~MyInstrument()
{
}


//! Take an optional input sample and compute one output sample.
StkFloat MyInstrument::tick( StkFloat input )
{
	m_osc1.freq = m_frequency;
	StkFloat y = 0.2 * m_osc1.MyOpSampleCompute0();
	//y =  0.25*(y+input);
	
	y = (y > 1.0f) ? 1.0f : y; //clip too loud left samples
	
	m_lastOutput = y;
	y *= m_adsr.ADSR_computeSample();	
	if (m_adsr.cnt_ >= m_gateTime)
		m_adsr.ADSR_keyOff();
	
	m_lastOutput = y;
	
	return m_lastOutput;
}

void MyInstrument::setFrequency( StkFloat frequency )
{
	m_frequency = frequency;
	m_adsr.keyOn();
	
}

//! Set the nominal loop gain.
/*!
 The actual loop gain is based on the value set with this
 function, but scaled slightly according to the frequency.  Higher
 frequency settings have greater loop gains because of
 high-frequency loop-filter roll-off.
 */
void MyInstrument::setLoopGain( StkFloat loopGain )
{
	if ( loopGain < 0.0 || loopGain >= 1.0 ) {
	//	oStream_ << "Twang::setLoopGain: parameter is out of range!";
	//	handleError( StkError::WARNING ); return;
	}
	
	loopGain_ = loopGain;
	StkFloat gain = loopGain_ + (m_frequency * 0.000005);
	if ( gain >= 1.0 ) gain = 0.99999;
	loopFilter_.setGain( gain );
}

void MyInstrument::noteOn( StkFloat frequency, StkFloat amplitude )
{
	setFrequency( frequency) ;
	setLoopGain( 0.995 );
	m_gateTime = 450000;
}

//! Stop a note with the given amplitude (speed of decay).
void MyInstrument::noteOff( StkFloat amplitude )
{
	setLoopGain( (1.0 - amplitude) * 0.9 );
	m_gateTime = 4500;

}



