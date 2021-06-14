#include"mbed.h"
#include "bbcar.h"


Ticker servo_ticker;
Ticker encoder_ticker;
Timer t;
PwmOut pin5(D5), pin6(D6);
DigitalInOut ping(D10);

BufferedSerial pc(USBTX,USBRX); //tx,rx
BufferedSerial uart(D1,D0); //tx,rx
BBCar car(pin5, pin6, servo_ticker);

char value;
int angle;

int output[3] = {0};
int store = 1;

float dis = 0;


int main() {
    int i = 0;

    pc.set_baud(9600);
    uart.set_baud(9600);

    while(1){
      if(uart.readable()){
            char recv[1];
            uart.read(recv, sizeof(recv));
            value = *recv;
            if ((i < 3) && (recv[0] != '.') && (store)) {
                        output[i] = *recv;
                        i++;
                    }
            if(value == '.'){
                store = 0;
                if(i == 1) angle = output[0];                                   // one digit
                if(i == 2) angle = 10*output[0] + output[1];                    // two digits
                if(i == 3) angle = 100*output[0] + 10*output[1] + output[2];    // three digits
                i = 0;
                for (int j = 0; j < 3; j++) {
                    output[j] = 0;
                }
            }
            if (angle < 345 && angle >= 75) {            
                car.turn(50,0.2);
            } else if (angle >15 && angle < 295) {
                car.turn(50,-0.2);
            } else {
                car.goStraight(50);
            }

            // ping code
            ping.output();
            ping = 0; wait_us(200);
            ping = 1; wait_us(5);
            ping = 0; wait_us(5);

            ping.input();
            while(ping.read() == 0);
            t.start();
            while(ping.read() == 1);
            dis = t.read();
            printf("Distance is %fcm\r\n", dis*17700.4f);
            t.stop();
            t.reset();
      }
   }
}

