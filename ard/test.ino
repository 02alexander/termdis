#include <string.h>

String str;

void setup()
{
	Serial.begin(9600);
}

void loop()
{
	if (Serial.available() > 0) {
		str = Serial.readString();
		for (int i = 0; i < str.length(); ++i) {
			int n = str[i];
			Serial.print(n);
			Serial.print(" ");
		} 
		Serial.print("\n");
	}	
}