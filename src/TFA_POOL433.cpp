#include "TFA_POOL433.h"

#define FILTER_MIN 	950
#define FILTER_MAX 	9500
#define MEDIUM_LEN 	3000
#define START_LEN 	9000
#define STOP_LEN 	2000

#define REPEAT_DELAY 3000 //ms

//#define dbg(s) Serial.println(s)
//#define dbgs(s) Serial.print(s)
#define dbg(s)
#define dbgs(s)

const int TFA_POOL433::_BUFF_SIZE = 36;

volatile bool TFA_POOL433::_avail = false;
volatile byte TFA_POOL433::_buff[TFA_POOL433::_BUFF_SIZE];
volatile byte TFA_POOL433::_buffEnd = 0;
volatile bool TFA_POOL433::_STARTb = 0;



unsigned long TFA_POOL433::_lastPackageArrived;
byte TFA_POOL433::_lastBuff[TFA_POOL433::_BUFF_SIZE];
byte TFA_POOL433::_pin = 0;

TFA_POOL433::TFA_POOL433() {
}

void TFA_POOL433::start(int pin) {
	_pin = pin;
	pinMode(_pin, INPUT);
	// for (int i = 0; i < _BUFF_SIZE; i++) {
		// _buff[i] = 8;
		// _lastBuff[i] = 8;
	// }
	_lastPackageArrived = 0;
	_avail = false;
	_buffEnd=0;
	attachInterrupt(digitalPinToInterrupt(_pin), _handler, FALLING);
	dbg("tfa started");
}

void TFA_POOL433::stop(){
	detachInterrupt(digitalPinToInterrupt(_pin));
}

bool TFA_POOL433::isDataAvailable(){
	return _avail;
}

void TFA_POOL433::_handler() {
	static unsigned long lastMs = 0, currMs,diffMs;
	currMs = micros();
	diffMs = currMs - lastMs;
	lastMs = currMs;

	if (diffMs > FILTER_MIN && diffMs < FILTER_MAX)  //Filter out the too long and too short pulses
	{
		if (!_avail)  //avail means data available for processing
		{
			if (diffMs > START_LEN)  // INIT/STOP pulse
			{
			_STARTb = 1;
			//dbgs("START");
			_buffEnd = 0;
			}
			else if ( _STARTb == 1)
			{
				if (_buffEnd > _BUFF_SIZE)
				{
					_buffEnd = 0;
					_avail = false;
					_STARTb = 0;
					//dbg("Buffer owerflow1");
				} else if (diffMs < STOP_LEN)  // STOP pulse short
				{
					if (_buffEnd == _BUFF_SIZE )
					{
						//dbg("STOP");//
						if (_isRepeat()){	
							_avail = true;
							
							//dbg("S buffEnd:" + String(_buffEnd));
						}
						_STARTb = 0;
					}
					else if (_buffEnd > _BUFF_SIZE) // useful ?
						{
						_buffEnd = 0;
						_avail = false;
						_STARTb = 0;
						//dbg("Buffer owerflow2");
						}
					
				} else 
				{
					if (_buffEnd < _BUFF_SIZE)   //buffer is not full yet
					{
						if (diffMs < MEDIUM_LEN)  //0
						{
							_buff[_buffEnd++] = 0;
						//	dbgs("0");
						} else { //1
							_buff[_buffEnd++] = 1;
						//	dbgs("1");
						}
					}
				}
			}
		}	
	}
		
	
}

bool TFA_POOL433::_isRepeat() {
	bool result = false;
	for(int i=0;i<_buffEnd;i++){
		if (_buff[i]!=_lastBuff[i]){
			for (int j=0;j<_buffEnd;j++){
				_lastBuff[j] = _buff[j];
			}
			_lastPackageArrived = millis();
			return false;
		}
	}
	result = (millis()-_lastPackageArrived < REPEAT_DELAY);
	_lastPackageArrived = millis();
	return result;
}

void TFA_POOL433::getData(byte &id, byte &channel, int &temperature, bool &battery){

	int temp1 = 0;
	


/* PRINT BUFFER
	for(int i=0;i<_buffEnd;i++) {
		dbgs(_buff[i]);
	}
	//dbgs("\n");
*/

	id = _binToDecRev(_buff, 0, 3);
		//if(id == 9) {

			//id = _binToDecRev(_buff, 4, 11);		// 12 - 0, 13 - Tx Button
			channel = _binToDecRev(_buff, 14, 15) + 1;

			temp1 = _binToSignedRev(_buff, 16, 27);
			temperature = temperature = temp1*10;//(double)(temp1*10);

			//humi1 = _binToDecRev(binary, 28, 35); // NOT USED 
			//humidity = (double)humi1;

			if(_binToDecRev(_buff, 36, 36) == 1) {
				battery = 0;
			} else {
				battery = 1;
			}
	//	} else {
	//		return;
	//	} 

	_STARTb = 0;
	_avail = false;
	_buffEnd = 0;
	
}

tfaResult TFA_POOL433::getData(){
	tfaResult result;
	getData(result.id, result.channel, result.temperature, result.battery);
	_avail = false;
	return result;
}

int TFA_POOL433::_binToDecRev(volatile byte *binary, int s, int e) {
	int result = 0;
	unsigned int mask = 1;
	for(; e > 0 && s<=e; mask <<= 1)
	if(binary[e--] != 0)
		result |= mask;
	return result;
}

int TFA_POOL433::_binToSignedRev(volatile byte *binary, int s, int e) { //  0<=s<=e, binary[s(msb) .. e(lsb)]
	int result = _binToDecRev(binary, s, e);
	if (binary[s]) {
		result -= 1<<(e-s+1);
	}
	return result;
}

