#include "mbed.h"
#include "bbcar.h"
#include "bbcar_rpc.h"
#include "mbed_rpc.h"

Ticker servo_ticker;
// Ticker encoder_ticker;

// volatile int steps;
// volatile int last;

PwmOut pin5(D5), pin6(D6);
BufferedSerial xbee(D1, D0);
//DigitalIn encoder(D11);


BBCar car(pin5, pin6, servo_ticker);

// void encoder_control() {
//    int value = encoder;
//    if (!last && value) steps++;
//    last = value;
// }




int main() {
   char buf[256], outbuf[256];
   FILE *devin = fdopen(&xbee, "r");
   FILE *devout = fdopen(&xbee, "w");

   // encoder_ticker.attach(&encoder_control, 10ms);

   while (1) {
      memset(buf, 0, 256);
      for( int i = 0; ; i++ ) {
         char recv = fgetc(devin);
         if(recv == '\n') {
            printf("\r\n");
            break;
         }
         buf[i] = fputc(recv, devout);
      }
      RPC::call(buf, outbuf);
   }
}