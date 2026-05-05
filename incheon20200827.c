#include <mega16a.h>
#include <delay.h>
#define encoder (PINB.1 == 1 && PINB.2 == 1 && en2 == 1)
#define ledcom PORTD.6
#define ensw PINB.0
#define led1 PORTC.0
#define led2 PORTC.1
#define led3 PORTC.2
#define led4 PORTC.3
#define dc_sen PINB.4
#define step_sen PINB.5

bit pb1, pb2, pb3, pb4, pb5, pb6, pb7, en1, en2, fnd1_f, fnd2_f, fnd3_f, fnd4_f, pb1_f, pb2_f, pb3_f;
char noise;
unsigned char fndb, start_test, car1, car2=1, car3=1, car4=1, car5=1, car6, tong, lamp1, lamp2, lamp3, lamp4, lamp5,
                ensw_f, dc, step, hack, car_f, car_ch, 
                tong_f, lamp1_f, lamp2_f, lamp3_f, lamp4_f, lamp5_f, pb4_f, pb5_f, pb6_f, pb7_f, m1, m2=1, m3=1, m4=1, m5=1, m6;
unsigned int fndc, test, dc_c, step_c, p1,p2,p3,p4,p5,p6;
int adc_value, data, num1=15,num2=15,num3=15,num4= 15, led_f, pb3_c, tick, number;
int d1,d2,d3,d4,d5,d6,d7,d8,d9,d10,d11,d12,d13,d14,d15, cnt, buf;
int F[16] = {0x7e, 0x0c, 0xb6, 0x9e, 0xcc, 0xda, 0xf8, 0x0e, 0xfe, 0xce, 0x1e, 0x72, 0x6e, 0x7c, 0x80, 0};

int car_data[7] = {0,0,2535,9158,7942,1420,0};
int car_num[7] = {0,1,2,3,4,5,6};
int car_time[7] = {0,0,1300,1235,1230,1240,0};
char mode=0, car_num_led, pb1_flag, pb2_flag, pb3_flag, pb4_flag, pb5_flag, pb6_flag, pb7_flag, en_flag, err_flag, mode_led;
int en_cnt1, en_cnt2, pb1_cnt, pb2_cnt, pb3_cnt, pb4_cnt, err_cnt;
int mode_1_on, mode_1_cnt, mode_data;
int park_mode, park_flag, park_data, park_time, park_car, park_cnt, sen_flag,ag,cg_cnt;
int lev_mode,lev_flag,lev_data,lev_data_chk,lev_ok,lev_car,lev_time,pay_time,pay;
int dc_stop,dc_c2,step_flag,cg_cnt2,step_c2;


void en(){
    if(PINB.1 == 0 && PINB.2 == 1){
        en1 = 1;
        en2 = 1;  
    }           
    if(PINB.1 == 1 && PINB.2 == 0){
        en1 = 0;
        en2 = 1;     
    } 
    if encoder{
        en2 = 0;
        if(en1==1){
            en_cnt1++;
            
            if(en_cnt1==10){
                en_cnt1=0;
                if(mode==1 && car_ch < 6)
                {
                    car_ch++;  
                    d1 = d3 = d4 = d7 = d9 = 0;
                    mode_1_on = mode_1_cnt =0;       
                }
                if(mode==2)
                {
                    if(park_mode==1 || park_mode==3){
                        if(park_flag == 1 && num1 < 9)num1++;
                        if(park_flag == 2 && num2 < 9)num2++;
                        if(park_flag == 3 && num3 < 9)num3++;
                        if(park_flag == 4 && num4 < 9)num4++;
                    }
                    if(park_mode==2 && car_ch<6){
                        car_ch++;
                        d1 = d3 = d4 = d7 = d9 = 0;
                    }    
                }
                if(mode==3)
                {
                    if(lev_mode==1 || lev_mode==3){
                        if(lev_flag == 1 && num1 < 9)num1++;
                        if(lev_flag == 2 && num2 < 9)num2++;
                        if(lev_flag == 3 && num3 < 9)num3++;
                        if(lev_flag == 4 && num4 < 9)num4++;
                        
                    }
                }
            }
            
        }
        else if(en1==0){            
            en_cnt2++;
            
            if(en_cnt2==10){
                en_cnt2=0;
                if(mode==1 && car_ch > 1)
                {
                    car_ch--;
                    d1 = d3 = d4 = d7 = d9 = 0;
                     mode_1_on = mode_1_cnt =0; 
                }
                if(mode==2)
                {
                    if(park_mode==1||park_mode==3){
                        if(park_flag == 1 && num1 > 0)num1--;
                        if(park_flag == 2 && num2 > 0)num2--;
                        if(park_flag == 3 && num3 > 0)num3--;
                        if(park_flag == 4 && num4 > 0)num4--;
                    }
                    if(park_mode==2 && car_ch > 0){
                        car_ch--;
                        d1 = d3 = d4 = d7 = d9 = 0;   
                    }      
                }
                if(mode==3)
                {
                    if(lev_mode==1 || lev_mode==3){
                        if(lev_flag == 1 && num1 > 0)num1--;
                        if(lev_flag == 2 && num2 > 0)num2--;
                        if(lev_flag == 3 && num3 > 0)num3--;
                        if(lev_flag == 4 && num4 > 0)num4--;
                        
                    }
                }
            }
            
        }
    }
}
void ic4015(int data){
    if(start_test == 0){
        int i;
        for(i = 0; i < 15; i++){
            if(data&(0x4000)>>i){
                PORTB.7 = 1;
            }               
            else
                PORTB.7 = 0;
                PORTB.6 = 1;
                delay_us(1);
                PORTB.6 = 0;
        }             
    }
}
/*****ic4015_잔상해결_*****/
void ic4015_noise_solution(){
    noise++;
    if(noise == 1){
        ledcom = 1; 
        start_test = 0;   
    }              
    if(noise > 2 && noise < 5){       
        start_test = 1;
        ledcom = 0;
    }        
    if(noise > 5){ 
        ledcom = 1; 
        noise = 0;   
    }           
}
void led_control(){
    led_f++;               
    ic4015(d1+d2+d3+d4+d5+d6+d7+d8+d9+d10+d11+d12+d13+d14+d15);     
    ///차고 LED///
    if(err_flag == 0){ 
        if(car1 == 1 && car_f != 1){d7 = 0x0040;}
        if(car2 == 1 && car_f != 2){d9 = 0x0100;}  
        if(car3 == 1 && car_f != 3){d4 = 8;}
        if(car4 == 1 && car_f != 4){d6 = 0x0020;}
        if(car5 == 1 && car_f != 5){d1 = 1;} 
        if(car6 == 1 && car_f != 6){d3 = 4;}
        
        if(car1 == 0 && car_f != 1){d7 = 0;}
        if(car2 == 0 && car_f != 2){d9 = 0;}  
        if(car3 == 0 && car_f != 3){d4 = 0;}
        if(car4 == 0 && car_f != 4){d6 = 0;}
        if(car5 == 0 && car_f != 5){d1 = 0;} 
        if(car6 == 0 && car_f != 6){d3 = 0;} 
    }   
    ///2,5,8 녹색LED//
    if(tong == 0){d10 = 0; d8 = 0; d5 = 0; d2 = 0;}
    if(tong == 1 && led_f <= 500){d10 = 0x0200; d8=d5=d2=0;} 
    if(tong == 2 && led_f <= 500){d8 = 0x0080; d10=d5=d2=0;} 
    if(tong == 3 && led_f <= 500){d5 = 0x0010; d8=d10=d2=0;} 
    if(tong == 4 && led_f <= 500){d2 = 2; d8=d5=d10=0;}
    
    ///11~ 14 램프///
    if(lamp1 == 1 && led_f <= 500){d11 = 0x0400;}
    if(lamp2 == 1 && led_f <= 500){d12 = 0x0800;}
    if(lamp3 == 1 && led_f <= 500){d13 = 0x1000;}
    if(lamp4 == 1 && led_f <= 500){d14 = 0x2000;}
    if(lamp5 == 1 && led_f <= 500){d15 = 0x4000;} 
    if(lamp1 == 0)d11 =0;
    if(lamp2 == 0)d12 =0;
    if(lamp3 == 0)d13 =0;
    if(lamp4 == 0)d14 =0;
    if(lamp5 == 0)d15 =0;   
    
    ///차고LED 플리커///
    if(car_f == 1 && led_f > 500){d7 = 0;}
    if(car_f == 2 && led_f > 500){d9 = 0;}  
    if(car_f == 3 && led_f > 500){d4 = 0;}
    if(car_f == 4 && led_f > 500){d6 = 0;}
    if(car_f == 5 && led_f > 500){d1 = 0;} 
    if(car_f == 6 && led_f > 500){d3 = 0;}   
    if(car_f == 1 && led_f <= 500){d7 = 0x0040;}
    if(car_f == 2 && led_f <= 500){d9 = 0x0100;}
    if(car_f == 3 && led_f <= 500){d4 = 8;}
    if(car_f == 4 && led_f <= 500){d6 = 0x0020;}
    if(car_f == 5 && led_f <= 500){d1 = 1;}
    if(car_f == 6 && led_f <= 500){d3 = 4;}
    
    ///2,5,8 녹색LED 플리커/// 
    if(tong_f == 1 && led_f > 500){d10 = 0; d8 = 0; d5 = 0; d2 = 0;} 
    
    
    ///11~14 램프 플리커/// 
    if(lamp1_f == 1 && led_f > 500){d11 = 0;}
    if(lamp2_f == 1 && led_f > 500){d12 = 0;}
    if(lamp3_f == 1 && led_f > 500){d13 = 0;}
    if(lamp4_f == 1 && led_f > 500){d14 = 0;}
    if(lamp5_f == 1 && led_f > 500){d15 = 0;}
           
    if(led_f > 1000)
        led_f = 0;
    
    ///기판LED 관련//   
    switch (mode_led)
    {         
        case 0 : led1=led2=led3=led4=0; break;
        case 4 : led1=1; led2=led3=led4=0; break;
        case 3 : led2=1; led1=led3=led4=0; break;
        case 2 : led3=1; led1=led2=led4=0; break;
        case 1 : led4=1; led1=led2=led3=0; break;
    }
    if(err_flag == 1) d1=d3=d4=d6=d7=d9=0; 
}
void pbsw(){   
    if(pb1 == 1){pb1_f = 1;}else pb1_f = 0;                                   
    if(pb2 == 1){pb2_f = 1;}else pb2_f = 0; 
    if(pb3 == 1){pb3_f = 1;}else pb3_f = 0; 
    if(pb4 == 1){pb4_f = 1;}else pb4_f = 0; 
    if(pb5 == 1){pb5_f = 1;}else pb5_f = 0; 
    if(pb6 == 1){pb6_f = 1;}else pb6_f = 0; 
    if(pb7 == 1){pb7_f = 1;}else pb7_f = 0; 
    if(ensw == 0){ensw_f = 1;}else ensw_f = 0;     
}
void fnd(){
    fndc++;
    fndb++;    
    
    /*****1000_자리_나타내기_*****/
    /*
    num1 = number / 1000;   //1000의_자리
    buf = number % 1000;
    num2 = buf / 100;    //100의_자리
    buf = buf % 100;
    num3 = buf / 10;     //10의_자리
    num4 = buf % 10;     //1의_자리     
    */
    
    switch(fndb){
        case 1: if(fndc <= 500 || fnd1_f == 0){
                    PORTC.6 = 0; PORTC.5 = 0; PORTC.4 = 0; PORTA = F[num1];
                }
                if(fndc > 500 && fnd1_f == 1){
                    PORTC.6 = 0; PORTC.5 = 0; PORTC.4 = 0; PORTA = F[15];
                } 
        break;
        case 2: if(fndc <= 500 || fnd2_f == 0){
                    PORTC.6 = 0; PORTC.5 = 0; PORTC.4 = 1; PORTA = F[num2];
                }
                if(fndc > 500 && fnd2_f == 1){
                    PORTC.6 = 0; PORTC.4 = 0; PORTC.4 = 1; PORTA = F[15];
                } 
        break;
        case 3: if(fndc <= 500 || fnd3_f == 0){
                    PORTC.6 = 0; PORTC.5 = 1; PORTC.4 = 0; PORTA = F[num3];
                }
                if(fndc > 500 && fnd3_f == 1){
                    PORTC.6 = 0; PORTC.5 = 1; PORTC.4 = 0; PORTA = F[15];
                } 
        break;
        case 4: if(fndc <= 500 || fnd4_f == 0){
                    PORTC.6 = 0; PORTC.5 = 1; PORTC.4 = 1; PORTA = F[num4];
                }
                if(fndc > 500 && fnd4_f == 1){
                    PORTC.6 = 0; PORTC.5 = 1; PORTC.4 = 1; PORTA = F[15];
                }
                 
        break;  
        case 5: fndb = 0; break;
    }        
    if(fndc > 1000){
        fndc = 0;   
    } 
}
void dc_m(){
    dc_c++;
    if(dc == 1&&dc_stop==0){
        if(dc_c <= 50){
            PORTD.0 = 1; PORTD.1 = 0;
        }        
        else{ 
            PORTD.0 = PORTD.1 = 0;
        }
    }
    else if(dc == 2&&dc_stop==0){
        if(dc_c >= 50){
            PORTD.0 = 0; PORTD.1 = 1;
        }        
        else{ 
            PORTD.0 = PORTD.1 = 0;
        }
    }      
    else{
        PORTD.0 = PORTD.1 = 0;
    }    
    if(dc_c > 100)
        dc_c = 0;
    if(dc==1&&dc_stop == 1){
        dc_c2++;
        PORTD.0 = 0; PORTD.1 = 1;
        if(dc_c2==1500){
            dc_c2=0;
            dc=0;
            dc_stop=0;
            PORTD.0 = PORTD.1 = 0;
        }
    }
    if(dc==2&&dc_stop == 1){
        dc_c2++;
        PORTD.0 = 1; PORTD.1 = 0;
        if(dc_c2==350){
            dc_c2=0;
            dc=0;
            dc_stop=0;
            PORTD.0 = PORTD.1 = 0;
        }
    }
            

}
void step_m(){
    if(step == 2){
        step_c++;
        switch(step_c){
            case 10: PORTD.2 = 1; PORTD.3 = 0; PORTD.4 = 0; PORTD.5 = 0; ag--; break;
            case 20: PORTD.2 = 0; PORTD.3 = 0; PORTD.4 = 1; PORTD.5 = 0; ag--; break;
            case 30: PORTD.2 = 0; PORTD.3 = 1; PORTD.4 = 0; PORTD.5 = 0; ag--; break;
            case 40: PORTD.2 = 0; PORTD.3 = 0; PORTD.4 = 0; PORTD.5 = 1; ag--; step_c = 0; break;
        }
    }
    else if(step == 1){
        step_c2++;
        switch(step_c2){
            case 10: PORTD.2 = 0; PORTD.3 = 0; PORTD.4 = 0; PORTD.5 = 1; ag++; break;
            case 20: PORTD.2 = 0; PORTD.3 = 1; PORTD.4 = 0; PORTD.5 = 0; ag++; break;
            case 30: PORTD.2 = 0; PORTD.3 = 0; PORTD.4 = 1; PORTD.5 = 0; ag++; break;
            case 40: PORTD.2 = 1; PORTD.3 = 0; PORTD.4 = 0; PORTD.5 = 0; ag++; step_c2 = 0; break;
        }
    }
    else{         
        PORTD.2 = PORTD.3 = PORTD.4 = PORTD.5 = 0;    
    }
    
    if((step==1||step==2)&&step_flag==0)step_flag=1;
    
    if(ag == 0&&(park_mode==6||park_mode==14))num1=num2=num3=num4=12;
    if(ag == 0&&(lev_mode==7&&cg_cnt==3000))num1=num2=num3=num4=12;
    if(ag == 0&&lev_mode==15)num1=num2=num3=num4=12;
    if(step==0&&step_flag==1){    
        if(ag == 51)num1=num2=num3=num4=10;
        if(ag == 102)num1=num2=num3=num4=13;
        if(ag == 153)num1=num2=num3=num4=11;
        step_flag=0;
    }
    if(ag < 0)ag=203;
    if(ag > 204)ag=0; 
} 
void pb_run()   //스위치모음
{
    int i;
    if(pb1 == 1){                //주차모드
        pb1_cnt++;
        if(pb1_cnt == 3000){
            pb1_cnt = 0;
            mode = 2;
            park_mode = 1;
            park_flag = 1;
            num1=0,num2=0,num3=0,num4=0;
            tong = 1;
        }
    }
    else pb1_cnt = 0;
    
    if(pb2==1){                   //출차모드
        pb2_cnt++;
        if(pb2_cnt==3000){
            mode=3;
            pb2_cnt=0;
            lev_mode=1;                 
            lev_flag=1;
            num1=0,num2=0,num3=0,num4=0;
            lamp2=1;
        }
    }
    else pb2_cnt=0;
    
    if(pb3 == 1){       //3초이상일경우
        pb3_cnt++;
        if(pb3_cnt == 3000){
            pb3_cnt = 0;
            mode = 1;      //mode = 1일경우 확인동작진입
            car_ch = 1;
        }
    }
    else pb3_cnt = 0;
       
    if(pb4 == 1){
        pb4_cnt++;
        if(pb4_cnt == 3000){
            pb4_cnt = 0;
            if(mode == 1)     //mode = 0일떄 초기상태
            {
                mode = 0;
                car_ch = 0;
                d1=d3=d4=d6=d7=d9=0;
            }
        }
    }
    else pb4_cnt=0;             
    
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
    if(pb1 == 1 && pb1_flag ==0){
        pb1_flag=1;
        if(park_mode==4){
            tong_f=1;
            lamp1=1;
            park_mode=5;
        }
    }
    else if(pb1 == 0 && pb1_flag == 1)pb1_flag=0;
    
    if(pb2 == 1 && pb2_flag==0){
        pb2_flag=1;
        if(lev_mode==6){
            lev_mode=7;
            tong=1; tong_f=1;
            car_f = lev_car;
        }
    }
    else if(pb2==0 && pb2_flag==1)pb2_flag=0;
    
    if(pb3 == 1 && pb3_flag ==0){   //딜레이없을경우
        pb3_flag = 1;    
        if(mode_1_on == 1 && mode_1_cnt > 1){      //확인동작 이전버튼
            mode_1_cnt --;
        }
    }
    else if(pb3 == 0 && pb3_flag == 1) pb3_flag=0;
    
    if(pb4 == 1 && pb4_flag ==0){      //딜레이없을경우
        pb4_flag = 1;  
        if(mode_1_on == 1 && mode_1_cnt < 3){        //확인동작 다음버튼
            mode_1_cnt++;
        }
        if(mode==2){
            park_flag = 1;
            if(park_mode < 4)park_mode++;
            if(park_mode==2){
                park_time = num1*1000;
                park_time = park_time+num2*100;
                park_time = park_time+num3*10;
                park_time = park_time+num4;
                num1=15,num2=15,num3=15,num4=0;
            }
            if(park_mode==3){
                switch(car_ch){
                case 1 : if(car1==1)err_flag=1;
                         else car1 = 1, park_car = 1; break;
                case 2 : if(car2==1)err_flag=1;
                         else car2 = 1, park_car = 2; break;
                case 3 : if(car3==1)err_flag=1;
                         else car3 = 1, park_car = 3; break;
                case 4 : if(car4==1)err_flag=1;
                         else car4 = 1, park_car = 4; break;
                case 5 : if(car5==1)err_flag=1;
                         else car5 = 1, park_car = 5; break;
                case 6 : if(car6==1)err_flag=1;
                         else car6 = 1, park_car = 6; break;
                }
                
                if(err_flag==0){
                    num1 = num2 = num3 = num4 = 0;
                    park_flag=1;    
                }
            }
            if(park_mode==4){
                park_data = num1*1000;
                park_data = park_data+num2*100;
                park_data = park_data+num3*10;
                park_data = park_data+num4;
                num1=15,num2=15,num3=15,num4=15;
                
                car_data[park_car] = park_data;
                car_time[park_car] = park_time;
                park_data = park_time = 0;
                mode_led=0;
                car_f = car_ch = 0;
                tong = 1;
                park_flag = 0;
                switch(park_car){
                    case 1 : if(park_car == 1)car1 = 0; break;
                    case 2 : if(park_car == 2)car2 = 0; break;
                    case 3 : if(park_car == 3)car3 = 0; break;
                    case 4 : if(park_car == 4)car4 = 0; break;
                    case 5 : if(park_car == 5)car5 = 0; break;
                    case 6 : if(park_car == 6)car6 = 0; break;
                }
            }       
        }
        if(mode==3){
            lev_flag=1;
            if(lev_mode<4)lev_mode++;
            if(lev_mode==2){
                lev_data = num1*1000;
                lev_data = lev_data+num2*100;
                lev_data = lev_data+num3*10;
                lev_data = lev_data+num4;
                    
                for(i=0; i<7; i++){
                    lev_data_chk = car_data[i];
                    if(lev_data_chk == lev_data){
                        lev_ok=1;
                        if(lev_data_chk==0)lev_ok=0;
                        if(lev_ok==1)lev_car=i;
                    }
                    else err_flag = 1;
                }
                if(lev_ok==1) err_flag=0,lev_ok=0;
            }    
               
            if(lev_mode==3){
                num1=num2=num3=num4=0;
                car_f=0;    
            }
        }
    }
    else if(pb4 == 0 && pb4_flag == 1) pb4_flag = 0; 
    
    if(pb5 == 1 && pb5_flag==0){
        pb5_flag=1;
        if(lev_mode==3){
            mode_led=4;
            lamp5 = 1;
            lev_time = num1*1000;
            lev_time = lev_time+num2*100;
            lev_time = lev_time+num3*10;
            lev_time = lev_time+num4;
            
            pay_time = (lev_time-car_time[lev_car]);
            pay = pay_time / 100;
            if(pay == 0) pay = 1000;
            else if(pay!=0) pay = pay * 1000;
            
            pay_time = pay_time%100;
            if(pay_time>30)pay = pay+500;
            
            num1 = pay/1000;
            pay = pay%1000;
            num2 = pay/100;
            pay = pay%100;
            num3 = pay/10;
            pay = pay%10;
            num4 = pay;
            pay = 0;
            
            lev_mode=5; 
        }
        
    }
    else if(pb5 == 0 && pb5_flag == 1) pb5_flag = 0;
    
    if(pb6 == 1 && pb6_flag == 0){
        pb6_flag = 1;
        if(lev_mode == 5 && num1 > 0)num1--;
    }
    else if(pb6 == 0 && pb6_flag == 1)pb6_flag = 0;  
    
    if(pb7 == 1 && pb7_flag == 0){
        pb7_flag = 1;
        if(lev_mode == 5 && num2 > 0)num2--;
    }
    else if(pb7 == 0 && pb7_flag == 1)pb7_flag = 0;
    
    if(ensw == 0 && en_flag == 0)
    {   
        en_flag = 1;
        if(mode==1)
        {
            mode_1_on = 1;
            mode_1_cnt = 1;  
            
            if(car_ch==1 && car1 == 0) err_flag = 1;   //해당 차고에 차가 없을경우 에러플래그 생성
            if(car_ch==2 && car2 == 0) err_flag = 1;
            if(car_ch==3 && car3 == 0) err_flag = 1;
            if(car_ch==4 && car4 == 0) err_flag = 1;
            if(car_ch==5 && car5 == 0) err_flag = 1;
            if(car_ch==6 && car6 == 0) err_flag = 1;
        }
        if(mode==2)
        {
            if(park_flag < 5 && park_mode == 1)park_flag++;
            if(park_flag < 5 && park_mode == 3)park_flag++;
        }
        if(mode==3)
        {
            if(lev_flag < 5 && lev_mode == 1)lev_flag++;
            if(lev_flag < 5 && lev_mode == 3)lev_flag++;
        }
    }
    else if(ensw == 1 && en_flag == 1) en_flag=0;
         
}
void running()
{    
    //초기상태일경우
    if(mode == 0){
        lamp3 = 0;
        car_f = 0;
        car_ch = 0;
    }  
    
    //확인동작    
    if(mode == 1){                     
        if(err_flag==0)lamp3 = 1;     //에러가 아닐때 확인중램프 동작
        
        car_f = car_ch;              //car_ch -> 현재 선택중인 차고
        
        if(mode_1_on == 1 && err_flag == 0)  //해당차고에 차가있을경우 
        {
            switch (mode_1_cnt){
                case 1 : mode_data = car_data[car_ch]; break;  //차량번호
                case 2 : mode_data = car_num[car_ch];  break;  //주차위치
                case 3 : mode_data = car_time[car_ch];  break; //시간표시
            }
            mode_led = mode_1_cnt;                    //현재모드 디스플레이
            num1 = mode_data / 1000;
            mode_data = mode_data % 1000;
            num2 = mode_data /100;
            mode_data = mode_data % 100;
            num3 = mode_data /10;
            mode_data = mode_data % 10;
            num4 = mode_data ;
            
            if(num1 == 0){                           //디스플레이에 공백나타내기
                num1=15;
                if(num2 == 0){
                    num2=15;
                    if(num3 == 0)num3=15;
                }
            }
                                     
        }
        else if(mode_1_on == 1 && err_flag == 1) //해당차고에 차가없을경우 에러
        {
            num1 = num2 = num3 = num4 = 14;
            lamp3 = 0; 
            lamp4 = 1; lamp4_f = 1;
            err_cnt ++;
            
            if(err_cnt == 3000){
                err_cnt = 0;
                lamp4 = 0;
                err_flag = 0;
                mode_1_on = mode_1_cnt =0;
            }    
        }
        else num1 = num2 = num3 = num4 = 15,mode_led = 0;      
    }
//////////////////////////////////////////////////////////////////////////
    if(mode == 2){  ///주차모드
        if(park_flag == 0)fnd1_f=0, fnd2_f=0, fnd3_f=0, fnd4_f=0;  
        if(park_mode == 1){
            if(park_flag==1)fnd1_f=1, fnd2_f=0, fnd3_f=0, fnd4_f=0;
            if(park_flag==2)fnd1_f=0, fnd2_f=1, fnd3_f=0, fnd4_f=0;
            if(park_flag==3)fnd1_f=0, fnd2_f=0, fnd3_f=1, fnd4_f=0;
            if(park_flag==4)fnd1_f=0, fnd2_f=0, fnd3_f=0, fnd4_f=1;
            if(park_flag==5)fnd1_f=0, fnd2_f=0, fnd3_f=0, fnd4_f=0;
            mode_led = 3;
        }
        if(park_mode == 2){
            if(park_flag==1)fnd1_f=0, fnd2_f=0, fnd3_f=0, fnd4_f=1;
            car_f = car_ch;
            num4 = car_ch;
            switch(car_ch){
                case 1 : if(car1==1)lamp1 = 1;
                         else lamp1 = 0; break;
                case 2 : if(car2==1)lamp1 = 1;
                         else lamp1 = 0; break;
                case 3 : if(car3==1)lamp1 = 1;
                         else lamp1 = 0; break;
                case 4 : if(car4==1)lamp1 = 1;
                         else lamp1 = 0; break;
                case 5 : if(car5==1)lamp1 = 1;
                         else lamp1 = 0; break;
                case 6 : if(car6==1)lamp1 = 1;
                         else lamp1 = 0; break;
            }
            mode_led = 2;     
        } 
        if(park_mode==3 && err_flag == 0){
            if(park_flag==1)fnd1_f=1, fnd2_f=0, fnd3_f=0, fnd4_f=0;
            if(park_flag==2)fnd1_f=0, fnd2_f=1, fnd3_f=0, fnd4_f=0;
            if(park_flag==3)fnd1_f=0, fnd2_f=0, fnd3_f=1, fnd4_f=0;
            if(park_flag==4)fnd1_f=0, fnd2_f=0, fnd3_f=0, fnd4_f=1;
            if(park_flag==5)fnd1_f=0, fnd2_f=0, fnd3_f=0, fnd4_f=0;
            mode_led = 1;
        }
        if(park_mode==3 && err_flag == 1) //해당차고에 차가없을경우 에러
        {
            num1 = num2 = num3 = num4 = 14;
            fnd4_f=0;
            lamp3 = 0; 
            lamp4 = 1; lamp4_f = 1;
            err_cnt ++;
            
            if(err_cnt == 3000){
                err_cnt = 0;
                lamp4 = 0;
                err_flag = 0;
                park_mode = 2;
                num1 = num2 = num3 = num4 = 15;
            }    
        }
        if(park_mode==5){
            park_cnt++;
            if(park_cnt == 3000){
                park_cnt=0;
                park_mode=6;
            }
        }
        if(park_mode==6){  
            if(tong == 1)dc=1;
            
            if(dc_sen==1 && sen_flag==0){
                sen_flag=1;
            }
            if(dc_sen==0 && sen_flag==1){
                sen_flag=0;
                tong++;
                if((park_car == 1 || park_car == 2)&&tong==2){
                dc_stop = 1; park_mode = 7;
                }
                if((park_car == 3 || park_car == 4)&&tong==3){
                dc_stop = 1; park_mode = 7;
                }
                if((park_car == 5 || park_car == 6)&&tong==4){
                dc_stop = 1; park_mode = 7;
                }
            }
        } 
        if(park_mode==7){
            if(cg_cnt!=1000)cg_cnt++;
            if(cg_cnt==1000){
               if(park_car==2||park_car==4||park_car==6)step=1;
               if(park_car==1||park_car==3||park_car==5)step=2;
            }
            if(ag == 51||ag == 153){
                step = 0;
                if(cg_cnt2!=1000)cg_cnt2++;
                if(cg_cnt2==1000){
                    dc = 1;
                    if(dc_sen==1 && sen_flag==0){
                        sen_flag=1;
                    }
                    if(dc_sen==0 && sen_flag==1){
                        sen_flag=0;
                        tong=0;
                        dc_stop = 1;
                        car_f = park_car;
                        park_mode = 8;
                        cg_cnt=0;
                        cg_cnt2=0;
                    } 
                }         
            }
        }
        if(park_mode==8){
            if(cg_cnt!=3000)cg_cnt++;
            if(cg_cnt==3000){
                dc=1;
                num1=num2=num3=num4=15;
                if(dc_sen==1 && sen_flag==0){
                    sen_flag=1;
                }
                if(dc_sen==0 && sen_flag==1){
                    sen_flag=0;
                    num1=num2=num3=num4=0;
                    dc_stop = 1;
                    park_mode = 9;
                    cg_cnt=0;
                    car_f=0; 
                    switch(park_car){
                        case 1 : car1=1;break;
                        case 2 : car2=1;break;
                        case 3 : car3=1;break;
                        case 4 : car4=1;break;
                        case 5 : car5=1;break;
                        case 6 : car6=1;break;
                    }
                }
            }       
        }
        if(park_mode==9){
            if(cg_cnt!=1000)cg_cnt++;
            if(cg_cnt==1000){
                if(park_car==2||park_car==4||park_car==6){
                    step=2;
                    if(ag==153)step=0,park_mode=10,cg_cnt=0;
                }
                if(park_car==1||park_car==3||park_car==5){
                    step=1;
                    if(ag==51)step=0,park_mode=10,cg_cnt=0;
                }
            }
        }
        if(park_mode==10){
            if(cg_cnt!=1000)cg_cnt++;
            if(cg_cnt==1000){
                dc=2;
                if(dc_sen==1 && sen_flag==0){
                    sen_flag=1;
                }
                if(dc_sen==0 && sen_flag==1){
                    sen_flag=0; dc_stop=1;
                    cg_cnt=0; park_mode=11;
                    if(park_car==1||park_car==2)tong=2;
                    if(park_car==3||park_car==4)tong=3;
                    if(park_car==5||park_car==6)tong=4;  
                }
                
            }
        
        }                
        if(park_mode==11){
            if(cg_cnt!=1000)cg_cnt++;
            if(cg_cnt==1000){
                if(ag==51)step=1;
                if(ag==153)step=2;
                if(ag==102)step=0,cg_cnt=0,park_mode=12; 
            }
        }
        if(park_mode==12){
            if(cg_cnt!=1000)cg_cnt++;
            if(cg_cnt==1000){
                dc=2;
                if(dc_sen==1 && sen_flag==0){
                    sen_flag=1;
                }
                if(dc_sen==0 && sen_flag==1){
                    sen_flag=0;
                    tong--;
                    if(tong==1)dc_stop=1,cg_cnt=0,park_mode=13;  
                }
            }
        }
        if(park_mode==13){
            if(cg_cnt!=1000)cg_cnt++;
            if(cg_cnt==1000){
                step=2;         
                if(ag==0)step=0,cg_cnt=0,park_mode=14; 
            }   
        }
        if(park_mode==14){
            if(cg_cnt!=3000)cg_cnt++;
            if(cg_cnt==3000){
                cg_cnt=0;
                num1=num2=num3=num4=15;
                tong=0; park_car=0;
                park_mode=0;
                mode=0;
                lamp1=0;
            }
        }                           
    }   
    
///////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
//출차모드//

    if(mode==3){
        if(lev_mode==1){
            if(lev_flag==1)fnd1_f=1, fnd2_f=0, fnd3_f=0, fnd4_f=0;
            if(lev_flag==2)fnd1_f=0, fnd2_f=1, fnd3_f=0, fnd4_f=0;
            if(lev_flag==3)fnd1_f=0, fnd2_f=0, fnd3_f=1, fnd4_f=0;
            if(lev_flag==4)fnd1_f=0, fnd2_f=0, fnd3_f=0, fnd4_f=1;
            if(lev_flag==5)fnd1_f=0, fnd2_f=0, fnd3_f=0, fnd4_f=0;
            mode_led=1;
        }
        if(lev_mode==2 && err_flag == 0){
            mode_led=2;
            num1=num2=num3=15;
            num4 = lev_car;
            car_f = lev_car;
        }
        if(lev_mode==2 && err_flag == 1)
        {
            num1 = num2 = num3 = num4 = 14;
            fnd1_f=fnd2_f=fnd3_f=fnd4_f=0;
            mode_led=0;
            lamp2 = 0; 
            lamp4 = 1; lamp4_f = 1;
            err_cnt ++;
            
            if(err_cnt == 3000){
                err_cnt = 0;
                lamp2 = 1;
                lamp4 = 0;
                mode_led=1;
                err_flag = 0;
                lev_mode = 1;
                num1 = num2 = num3 = num4 = 0;
                lev_flag=1;
            }    
        }
        if(lev_mode==3){
            if(lev_flag==1)fnd1_f=1, fnd2_f=0, fnd3_f=0, fnd4_f=0;
            if(lev_flag==2)fnd1_f=0, fnd2_f=1, fnd3_f=0, fnd4_f=0;
            if(lev_flag==3)fnd1_f=0, fnd2_f=0, fnd3_f=1, fnd4_f=0;
            if(lev_flag==4)fnd1_f=0, fnd2_f=0, fnd3_f=0, fnd4_f=1;
            if(lev_flag==5)fnd1_f=0, fnd2_f=0, fnd3_f=0, fnd4_f=0;
            mode_led=3;
        }
        if(lev_mode==5){
            fnd1_f=fnd2_f=fnd3_f=fnd4_f=1;
            if(num1 == 0 && num2 == 0){
                mode_led = 0;
                lamp5 = 0;
                num1=num2=num3=num4=15;
                lev_mode=6;
            }   
        }
        if(lev_mode==7){
            fnd1_f=fnd2_f=fnd3_f=fnd4_f=0;
            if(cg_cnt!=3000)cg_cnt++;
            if(cg_cnt==3000){
                dc=1;
                car_f = 0;
                if(dc_sen==1 && sen_flag==0){
                    sen_flag=1;
                }
                if(dc_sen==0 && sen_flag==1){
                    sen_flag=0;
                    tong++;
                    if((lev_car == 1 || lev_car == 2)&&tong==2){
                    dc_stop = 1; lev_mode = 8; cg_cnt=0;
                    }   
                    if((lev_car == 3 || lev_car == 4)&&tong==3){
                    dc_stop = 1; lev_mode = 8; cg_cnt=0;
                    }
                    if((lev_car == 5 || lev_car == 6)&&tong==4){
                    dc_stop = 1; lev_mode = 8; cg_cnt=0;
                    }
                }           
            }
        }
        if(lev_mode==8){
            if(cg_cnt!=1000)cg_cnt++;
            if(cg_cnt==1000){
               if(lev_car==2||lev_car==4||lev_car==6)step=1;
               if(lev_car==1||lev_car==3||lev_car==5)step=2;
            }
            if(ag == 51||ag == 153){
                step = 0;
                if(cg_cnt2!=1000)cg_cnt2++;
                if(cg_cnt2==1000){
                    dc = 1;
                    if(dc_sen==1 && sen_flag==0){
                        sen_flag=1;
                    }
                    if(dc_sen==0 && sen_flag==1){
                        sen_flag=0;
                        tong=0;
                        dc_stop = 1;
                        car_f = lev_car;
                        lev_mode = 9;
                        cg_cnt=0;
                        cg_cnt2=0;
                        num1=num2=num3=num4=15;
                    } 
                }
            }
            
        }
        if(lev_mode==9){
            if(cg_cnt!=3000)cg_cnt++;
            if(cg_cnt==3000){
                dc=2;
                if(dc_sen==1 && sen_flag==0){
                    sen_flag=1;
                }
                if(dc_sen==0 && sen_flag==1){
                    sen_flag=0;
                    num1=num2=num3=num4=0;
                    dc_stop = 1;
                    lev_mode = 10;
                    cg_cnt=0;
                    switch(lev_car){
                        case 1 : car1=0;break;
                        case 2 : car2=0;break;
                        case 3 : car3=0;break;
                        case 4 : car4=0;break;
                        case 5 : car5=0;break;
                        case 6 : car6=0;break;
                    }
                }    
            }
        }
        if(lev_mode==10){
            if(cg_cnt!=1000)cg_cnt++;
            if(cg_cnt==1000){
                if(lev_car==2||lev_car==4||lev_car==6){
                    step=2;
                    if(ag==153)step=0,lev_mode=11,cg_cnt=0;
                }
                if(lev_car==1||lev_car==3||lev_car==5){
                    step=1;
                    if(ag==51)step=0,lev_mode=11,cg_cnt=0;
                }    
            }
        } 
        if(lev_mode==11){
            if(cg_cnt!=1000)cg_cnt++;
            if(cg_cnt==1000){
                dc=2;
                if(dc_sen==1 && sen_flag==0){
                    sen_flag=1;
                }
                if(dc_sen==0 && sen_flag==1){
                    sen_flag=0; dc_stop=1;
                    cg_cnt=0; lev_mode=12;
                    car_f=0;
                    if(lev_car==1||lev_car==2)tong=2;
                    if(lev_car==3||lev_car==4)tong=3;
                    if(lev_car==5||lev_car==6)tong=4;  
                }
                
            }
        }
        if(lev_mode==12){
            if(cg_cnt!=1000)cg_cnt++;
            if(cg_cnt==1000){
                if(ag==51)step=1;
                if(ag==153)step=2;
                if(ag==102)step=0,cg_cnt=0,lev_mode=13;
            }
        }
        if(lev_mode==13){
            if(cg_cnt!=1000)cg_cnt++;
            if(cg_cnt==1000){
                dc=2;
                if(dc_sen==1 && sen_flag==0){
                    sen_flag=1;
                }
                if(dc_sen==0 && sen_flag==1){
                    sen_flag=0;
                    tong--;
                    if(tong==1)dc_stop=1,cg_cnt=0,lev_mode=14;  
                }
            }
        }
        if(lev_mode==14){
            if(cg_cnt!=1000)cg_cnt++;
            if(cg_cnt==1000){  
                step=2;         
                if(ag==0)step=0,cg_cnt=0,lev_mode=15;
            }
        }
        if(lev_mode==15){
            if(cg_cnt!=3000)cg_cnt++;
            if(cg_cnt==3000){
                car_data[lev_car] = 0;
                car_time[lev_car] = 0;
                cg_cnt=0;
                num1=num2=num3=num4=15;
                tong=0; tong_f=0; 
                lev_car=0;
                lev_mode=0;
                mode=0;
                lamp2=0;
                lev_flag=0;       
            }
        }
    
    } 
    
}
interrupt [TIM0_OVF] void timer0_ovf_isr(void){
    TCNT0=0xB0;  
    dc_m(); 
}
interrupt [TIM2_OVF] void timer2_ovf_isr(void){
    TCNT2=0x06; 
    
    running();
    pb_run();
    step_m();
    fnd();            
    ic4015_noise_solution();
    led_control();   
    en();
}
unsigned int read_adc(){
    ADMUX=0;
    delay_us(10);
    ADCSRA|=(1<<ADSC);
    while ((ADCSRA & (1<<ADIF))==0);
    ADCSRA|=(1<<ADIF);
    return ADCW;
}
void main(void){
DDRA = 0xfe; DDRB = 0xc0; DDRC = 0xff; DDRD = 0xff;
TCCR0=(0<<WGM00) | (0<<COM01) | (0<<COM00) | (0<<WGM01) | (0<<CS02) | (0<<CS01) | (1<<CS00);
TCNT0=0xB0;
OCR0=0x00;
TCCR2=(0<<PWM2) | (0<<COM21) | (0<<COM20) | (0<<CTC2) | (0<<CS22) | (1<<CS21) | (1<<CS20);
TCNT2=0x06;
OCR2=0x00;
TIMSK = 0x41;
ADMUX=0;
ADCSRA=(1<<ADEN) | (0<<ADSC) | (0<<ADATE) | (0<<ADIF) | (0<<ADIE) | (1<<ADPS2) | (1<<ADPS1) | (0<<ADPS0);

#asm("sei")

while (1){   
        pbsw();      
        adc_value = read_adc();     
        if(adc_value > 240 && adc_value < 300){pb1 = 1;}
        if(adc_value > 380 && adc_value < 450){pb2 = 1;}
        if(adc_value > 490 && adc_value < 530){pb3 = 1;}
        if(adc_value > 550 && adc_value < 590){pb4 = 1;}
        if(adc_value > 610 && adc_value < 640){pb5 = 1;} 
        if(adc_value > 655 && adc_value < 685){pb6 = 1;}
        if(adc_value > 700 && adc_value < 730){pb7 = 1;}
        if(adc_value > 800){pb7 = 0; pb6 = 0; pb5 = 0; pb4 = 0; pb3 = 0; pb2 = 0; pb1 = 0;}
    }
}        
/****버튼****/ 
    //switch_flag_사용법
    /*if(pb1 == 1 && pb1_f == 0){
       num1++;   
    } */