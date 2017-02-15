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

#include <unabto/unabto_common_main.h>
#include <unabto/unabto_app.h>
#include <unabto/unabto_util.h>
#include <unabto/unabto_app_adapter.h>
#include <unabto/unabto_hmac_sha256.h>

APP_DATA appData;

const char* idSuffix = ".starterkit.u.nabto.net";
char idBuffer[32];

bool hmac_test();

void APP_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_INIT;

}

bool hmac_test() {
    uint8_t testKey[] = {
        0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
        0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
        0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
        0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
        0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
        0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
        0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
        0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
        0xaa, 0xaa, 0xaa }; 

    uint8_t testData[] = {
        0x54, 0x65, 0x73, 0x74, 0x20, 0x55, 0x73, 0x69, 0x6e, 0x67, 0x20, 0x4c, 0x61, 0x72, 0x67, 0x65, 0x72, 0x20, 0x54, 0x68, 0x61, 0x6e, 0x20, 0x42, 0x6c, 0x6f, 0x63, 0x6b, 0x2d, 0x53, 0x69, 0x7a, 0x65, 0x20, 0x4b, 0x65, 0x79, 0x20, 0x2d, 0x20, 0x48, 0x61, 0x73, 0x68, 0x20, 0x4b, 0x65, 0x79, 0x20, 0x46, 0x69, 0x72, 0x73, 0x74
    };

    uint8_t hashResult[] = {
        0x60, 0xe4, 0x31, 0x59, 0x1e, 0xe0, 0xb6, 0x7f, 0x0d, 0x8a, 0x26, 0xaa, 0xcb, 0xf5, 0xb7, 0x7f, 0x8e, 0x0b, 0xc6, 0x21, 0x37, 0x28, 0xc5, 0x14, 0x05, 0x46, 0x04, 0x0f, 0x0e, 0xe3, 0x7f, 0x54
    };

    unabto_buffer keys[1];
    unabto_buffer messages[1];

    keys[0].data = testKey;
    keys[0].size = 131;
    messages[0].data = testData;
    messages[0].size = 54;

    uint8_t mac[32];
    
    unabto_hmac_sha256_buffers(keys, 1, messages,1, mac, 32);

    return (memcmp(mac, hashResult, 32) == 0);
}


const char* server_id_end = ".starterkit.u.nabto.net";

void APP_Tasks ( void )
{

    /* Check the application's current state. */
    switch ( appData.state )
    {
        /* Application's initial state. */
        case APP_STATE_INIT:
        {
            if (!hmac_test()) {
                break;
            }
            
            SYS_STATUS tcpipStat = TCPIP_STACK_Status(sysObj.tcpip);
            if(tcpipStat == SYS_STATUS_READY) {
                appData.state = APP_STATE_INIT_UNABTO;
            }
            break;
        }

        case APP_STATE_INIT_UNABTO:
        {
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
            nms->cryptoSuite = CRYPT_W_NULL_DATA;
            //nms->cryptoSuite = CRYPT_W_AES_CBC_HMAC_SHA256;
            nms->secureAttach = 1;
            nms->secureData = 1;
            memset(nms->presharedKey, 0, PRE_SHARED_KEY_SIZE);
            if (!unabto_init()) {
                NABTO_LOG_FATAL(("could not initialize nabto"));
                break;
            }
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



// Function prototypes
uint8_t setLed(uint8_t led_id, uint8_t led_on);
uint8_t readLed(uint8_t led_id);

/***************** The uNabto application logic *****************
 * This is where the user implements his/her own functionality
 * to the device. When a Nabto message is received, this function
 * gets called with the message's request id and parameters.
 * Afterwards a user defined message can be sent back to the
 * requesting browser.
 ****************************************************************/
application_event_result application_event(application_request* request, unabto_query_request* read_buffer, unabto_query_response* write_buffer) {
    switch(request->queryId) {
        case 1: {
            //  <query name="light_write.json" description="Turn light on and off" id="1">
            //    <request>
            //      <parameter name="light_id" type="uint8"/>
            //      <parameter name="light_on" type="uint8"/>
            //    </request>
            //    <response>
            //      <parameter name="light_state" type="uint8"/>
            //    </response>
            //  </query>

            uint8_t light_id;
            uint8_t light_on;
            uint8_t light_state;

            // Read parameters in request
            if (!unabto_query_read_uint8(read_buffer, &light_id)) return AER_REQ_TOO_SMALL;
            if (!unabto_query_read_uint8(read_buffer, &light_on)) return AER_REQ_TOO_SMALL;

            // Set onboard led according to request
            light_state = setLed(light_id, light_on);

            // Write back led state
            if (!unabto_query_write_uint8(write_buffer, light_state)) return AER_REQ_RSP_TOO_LARGE;

            return AER_REQ_RESPONSE_READY;
        }
        case 2: {
            //  <query name="light_read.json" description="Read light status" id="2">
            //    <request>
            //      <parameter name="light_id" type="uint8"/>
            //    </request>
            //    <response>
            //      <parameter name="light_state" type="uint8"/>
            //    </response>
            //  </query>

            uint8_t light_id;
            uint8_t light_state;

            // Read parameters in request
            if (!unabto_query_read_uint8(read_buffer, &light_id)) return AER_REQ_TOO_SMALL;

            light_state = readLed(light_id);

            // Write back led state
            if (!unabto_query_write_uint8(write_buffer, light_state)) return AER_REQ_RSP_TOO_LARGE;

            return AER_REQ_RESPONSE_READY;
        }
    }
    return AER_REQ_INV_QUERY_ID;
}

// Set first onboard LED and return state,
// only using ID #1 in this simple example
uint8_t setLed(uint8_t led_id, uint8_t led_on)
{
    if (led_id == 1) {
        BSP_LED_STATE state = BSP_LED_STATE_OFF;
        if (led_on) {
            state = BSP_LED_STATE_ON;
        }
        BSP_LEDStateSet(BSP_LED_1, state);
        return readLed(led_id);
    }
    return -1;
}

// Return first onboard LED state,
// only using ID #1 in this simple example
uint8_t readLed(uint8_t led_id) {
    if (led_id == 1) {
        BSP_LED_STATE state = BSP_LEDStateGet(BSP_LED_1);
        if (state == BSP_LED_STATE_ON) {
            return 1;
        } else {
            return 0;
        }
    }
    return -1;
}

 

/*******************************************************************************
 End of File
 */
