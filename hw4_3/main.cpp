#include"mbed.h"
#include "bbcar.h"


Ticker servo_ticker;
Ticker encoder_ticker;
Timer t;
PwmOut pin5(D5), pin6(D6);
DigitalInOut ping(D11);

BufferedSerial pc(USBTX,USBRX); //tx,rx
BufferedSerial uart(D1,D0); //tx,rx
BBCar car(pin5, pin6, servo_ticker);
BufferedSerial xbee(D10, D9);

char value;
int angle;

int output[3] = {0};
int store = 1;

float dis = 0;
float dis_xbee = 0;

char buffer[10];

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
            dis_xbee = dis * 177700.4f;
            sprintf(buffer, "%f", dis_xbee);
            xbee.write(buffer, sizeof(buffer));           
            t.stop();
            t.reset();
      }
   }
}

