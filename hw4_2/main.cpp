#include"mbed.h"
#include"bbcar.h"

BufferedSerial pc(USBTX,USBRX); //tx,rx
BufferedSerial uart(D1,D0); //tx,rx


Ticker servo_ticker;
Ticker encoder_ticker;
PwmOut pin5(D5), pin6(D6);
DigitalIn encoder(D11);

BBCar car(pin5, pin6, servo_ticker);

char read1, read2, read3;

float factor = 0;
int mode = 0;

int output[3] = 0;
int store = 1;
int value = 0;

int main(){
   uart.set_baud(9600);
   pc.set_baud(9600);
   while(1){
      if(uart.readable()){
            char recv[1];
            uart.read(recv, sizeof(recv));
            read1 = *recv;
            if (read1 == 0) {
                mode = 0;
                printf("no suittable line");
                car.goStraight(50);
            } else if (read1 == 1) {
                mode = 1;
            }
            if (mode) {
                int i = 0;
                store = 1;
                while (recv[0] != '\n') {   
                    uart.read(recv, sizeof(recv));
                    if ((i < 3) && (recv[0] != '.') && (store)) {
                        output[i] = *recv;
                        i++;
                    } else if (recv[0] == '.') {
                        store = 0;
                        int k = i - 1;
                        if (k == 0) value = output[0];
                        if (k == 1) value = 10 * output[0] + output[1];
                        factor = value / 100.0;
                        if (factor < 0.1 && factor > -0.1) {
                            factor = 0;
                        } 
                        // give open MV an allowable detect error so that car wont 
                        // swing from side to side 
                        car.turn(50, factor);
                    }
                }  
            }    
      }
   }
}