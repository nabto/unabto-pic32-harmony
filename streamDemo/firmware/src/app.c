/*******************************************************************************

  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    app.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It 
    implements the logic of the application's state machine and it may call 
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Included Files 
// *****************************************************************************
// *****************************************************************************

#include "app.h"
#include <bsp_config.h>
#include <crypto/crypto.h>
#include <stdint.h>

#include <unabto/unabto_common_main.h>
#include <unabto/unabto_app.h>
#include <unabto/unabto_util.h>
//#include <unabto/unabto_app_adapter.h>
#include <unabto/unabto_hmac_sha256.h>
#include <unabto/unabto_prf.h>
#include <stream_echo.h>

APP_DATA appData;

const char* idSuffix = ".starterkit.u.nabto.net";
char idBuffer[64];

const char* server_id_end = ".starterkit.u.nabto.net";

void stream_echo_init();

void APP_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_INIT;

}

void APP_Tasks ( void )
{
    /* Check the application's current state. */
    switch ( appData.state )
    {
        /* Application's initial state. */
        case APP_STATE_INIT:
        {            
            SYS_STATUS tcpipStat = TCPIP_STACK_Status(sysObj.tcpip);
            if(tcpipStat == SYS_STATUS_READY) {
                appData.state = APP_STATE_INIT_UNABTO;
            }
            break;
        }

        case APP_STATE_INIT_UNABTO:
        {
            unabto_udp_debug_init("192.168.1.57", 4242);
            TCPIP_NET_HANDLE defaultIf = TCPIP_STACK_NetDefaultGet();
            const uint8_t* physicalAddress = TCPIP_STACK_NetAddressMac(defaultIf);
            if (physicalAddress == NULL) {
                NABTO_LOG_FATAL(("physical address should be present"));
                break;
            }
            nabto_main_setup* nms;
            nms = unabto_init_context();
            memset(idBuffer, 0, sizeof(idBuffer));
            sprintf(idBuffer, "%02x%02x%02x%s", physicalAddress[3], physicalAddress[4], physicalAddress[5], idSuffix);
            nms->id = idBuffer;

            nms->cryptoSuite = CRYPT_W_AES_CBC_HMAC_SHA256;
            nms->secureAttach = 1;
            nms->secureData = 1;
            memset(nms->presharedKey, 0, PRE_SHARED_KEY_SIZE);
            if (!unabto_init()) {
                NABTO_LOG_FATAL(("could not initialize nabto"));
                break;
            }
            stream_echo_init();
            appData.state = APP_STATE_SERVICE_TASKS;
            break;
        }
        
        case APP_STATE_SERVICE_TASKS:
        {            
            unabto_tick();
            break;
        }

        /* The default state should never be executed. */
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}


application_event_result application_event(application_request* request, unabto_query_request* read_buffer, unabto_query_response* write_buffer) {
    return AER_REQ_NO_QUERY_ID;
}

/*******************************************************************************
 End of File
 */
