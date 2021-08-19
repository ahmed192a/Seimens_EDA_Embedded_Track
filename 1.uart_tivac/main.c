/*
 * main.c
 *  Created on: May 27, 2021
 *      Author: ah132
 */

#include "microconfig.h"



/*******************************************************************************
 *                      Global Variables                                       *
 *******************************************************************************/
extern char latitudeResult[];
extern char longitudeResult[];
extern char AltitudeResult[];
extern uint8_t DATAflag;
extern double r_distance;
extern volatile unsigned char g_Interrupt_Flag ;

/*******************************************************************************
 *                      Main Function                                          *
 *******************************************************************************/

void main(void){
    uint16_t arrived = 0;

    // INITIALIZING
      PLL_Init();           // Set the internal frequency to 80MHz

    // configuration of uart 0 for pc communication
    UART_ConfigType uart0={EIGHT_BITS,DISABLED,ONE_STOP_BIT,u0,9600};
    UART_init(&uart0);

    // configuration of uart 1 port C for gps
    UART_ConfigType uart1={EIGHT_BITS,DISABLED,ONE_STOP_BIT,u1_C,9600};
    UART_init(&uart1);

    GPIOF_Init();                 // Initialize the GPIO of PF2 BLUE LED
    SW1_Int_Interrupt();          // Enable Interrupt on PF4 Switch 1

#if SDCARDUSE
    uint8_t returnStatus,sdcardType;
    fileConfig_st *srcFilePtr;
    do{
        returnStatus = SD_Init(&sdcardType);
        if(returnStatus)
        {
            if(returnStatus == SDCARD_NOT_DETECTED)
            {
                UART0_str("\n\r SD card not detected..");
            }
            else if(returnStatus == SDCARD_INIT_FAILED)
            {
                UART0_str("\n\r Card Initialization failed..");
            }
            else if(returnStatus == SDCARD_FAT_INVALID)
            {
                UART0_str("\n\r Invalid Fat filesystem");
            }
            //while(1);
        }
        else
        {
            UART0_str("\n\rSD Card Detected!");
            break;
        }
    } while(1);
    delay_ms();

//    //  WRITE
//    srcFilePtr = FILE_Open("GPS.KML",WRITE,&returnStatus);
//    if(srcFilePtr == 0)
//    {
//        UART0_str("\n\rFile Opening Failed");
//    }
//    else
//    {
//        UART0_str("\n\rEntering Data..\n");
//        FILE_Putstr(srcFilePtr, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
//        FILE_Putstr(srcFilePtr, "<kml >\n");
//        FILE_Putstr(srcFilePtr, "<Document>\n");
//        FILE_Putstr(srcFilePtr, "<Style id=\"yellowPoly\">\n<LineStyle>\n<color>7f00ffff</color>\n<width>100</width>\n</LineStyle>\n");
//        FILE_Putstr(srcFilePtr, "<PolyStyle>\n<color>7f00ff00</color>\n</PolyStyle>\n</Style>\n<Placemark><styleUrl>#yellowPoly</styleUrl>\n<LineString>\n<extrude>1</extrude>\n<tesselate>1</tesselate>\n<altitudeMode>absolute</altitudeMode>\n<coordinates>\n");
//        UART0_str("\n\rData saved, closing the file.\n");
//        FILE_Close(srcFilePtr);
//    }


#endif

#if LCDUSE
    LCD_init();
    LCD_clearScreen();
    LCD_displayString("Welcome to ..");
#endif

    while(1){
        if(g_Interrupt_Flag==1 && arrived == 0){
#if SYSTICK_USE
            wait_ms(200);
#endif
            //readGPSs();
            readGPSa();
#if SDCARDUSE
            //  WRITE
            if(r_distance<100){
                srcFilePtr = FILE_Open("GPS.TXT",WRITE,&returnStatus);
                if(srcFilePtr == 0)
                {
                    UART0_str("\n\rFile Opening Failed");
                }
                else
                {
                    UART0_str("\n\rEntering Data..\n");
                    FILE_Putstr(srcFilePtr, longitudeResult);
                    FILE_PutCh(srcFilePtr, ',');
                    FILE_Putstr(srcFilePtr, latitudeResult);
                    FILE_PutCh(srcFilePtr, ',');
                    FILE_Putstr(srcFilePtr, AltitudeResult);
                    FILE_PutCh(srcFilePtr, ' ');
                    FILE_PutCh(srcFilePtr, EOFF);
                    UART0_str("\n\rData saved, closing the file.\n");
                    FILE_Close(srcFilePtr);
                }
            }
#endif
            if( r_distance>=100 ) {

                Led_on();
#if SDCARDUSE
            //  WRITE
//                srcFilePtr = FILE_Open("GPS.KML",WRITE,&returnStatus);
//                if(srcFilePtr == 0)
//                {
//                    UART0_str("\n\rFile Opening Failed");
//                }
//                else
//                {
//                    UART0_str("\n\rEntering Data..\n");
//                    FILE_Putstr(srcFilePtr, "\n</coordinates>\n</LineString></Placemark>\n</Document></kml>\n");
//
//                    UART0_str("\n\rData saved, closing the file.\n");
//                    FILE_Close(srcFilePtr);
//                }

#endif
                arrived =1;
                g_Interrupt_Flag=0;
            }
        }
    }
}
