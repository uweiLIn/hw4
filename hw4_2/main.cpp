#include"mbed.h"
#include"bbcar.h"

BufferedSerial pc(USBTX,USBRX); //tx,rx
BufferedSerial uart(D1,D0); //tx,rx


Ticker servo_ticker;
Ticker encoder_ticker;
PwmOut pin5(D5), pin6(D6);
DigitalIn encoder(D11);

BBCar car(pin5, pin6, servo_ticker);

//char value;

float factor = 0;

int main(){
   uart.set_baud(9600);
   pc.set_baud(9600);
   while(1){
      if(uart.readable()){
            char recv[1];
            uart.read(recv, sizeof(recv));
            //value = *recv;
            //printf("%c ",x)
            if (recv[0] == 0) {
                printf("no suittable line");
                car.goStraight(50);
            } else if (recv[0] == 1) {
                factor = recv[1] / 100.0;
                if (factor < 0.1 && factor > -0.1) {
                    factor = 0;
                }
                car.turn(50, factor);
            }
      }
   }
}