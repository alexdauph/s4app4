/*******************************************************************************
  BSD Internal Stack API Header File

  Company:
    Microchip Technology Inc.
    
  File Name:
    berkeley_manager.h

  Summary:
    BSD Internal Stack API Header File

  Description:
    This file provides the BSD Internal Stack API definitions.
    
*******************************************************************************/
// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright ? 2012 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED ?AS IS? WITHOUT WARRANTY OF ANY KIND,
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

#ifndef __BERKELEY_MANAGER_H_
#define __BERKELEY_MANAGER_H_


typedef enum
{
    SKT_CLOSED,             // Socket closed state indicating a free descriptor
    SKT_CREATED,            // Socket created state for TCP and UDP sockets
    SKT_BOUND,              // Socket bound state for TCP and UDP sockets
    SKT_BSD_LISTEN,         // Listening state for TCP BSD listener handle "socket"
    SKT_LISTEN,             // TCP server listen state
    SKT_IN_PROGRESS,        // TCP client connection in progress state
    SKT_EST,                // TCP client or server established state
    SKT_DISCONNECTED,       // TCP client or server no longer connected to the remote host (but was historically)
    SKT_DISCONNECTING,      // socket has issued a disconnect request and waits for this to complete
                            // this is for a server socket for which the parent is still listening
                            // and needs to wait before returning to SKT_LISTEN to accept new incoming connections
} BSD_SCK_STATE; // Berkeley Socket (BSD) states

struct BSDSocket
{
    uint16_t                localPort;      // local port
    uint16_t                remotePort;     // remote port
    uint32_t                remoteIP;       // remote IP

    uint16_t                SocketType;     // Socket type
    uint8_t                 bsdState;       // Socket state: BSD_SCK_STATE
    uint8_t                 addressFamily; 
    int16_t                 nativeSkt;      // corresponding native socket                
    uint16_t                backlog;        // maximum number or client connection

#if defined(TCPIP_STACK_USE_IPV6)
    uint32_t                remoteIPv6[3];  // remote IP for IPv6
#endif
    uint32_t                localIP;        // bound address
#if defined(TCPIP_STACK_USE_IPV6)
    uint32_t                localIPv6[3];
#endif
    uint32_t                rcvBufSize;
    uint32_t                sndBufSize;
    NET_PRES_SKT_HANDLE_T   SocketID;       // Socket ID
    uint16_t                rcvTmOut;
    uint16_t                sndTmOut;
    uint16_t                lingerTmo;
    uint16_t                parentId;       // server sockets created by accept have a parent:
                                            // the original listening socket
    union {
        struct {
            uint16_t tcpLinger          : 1;
            uint16_t tcpKeepAlive       : 1;
            uint16_t tcpNoDelay         : 1;
            uint16_t tcpExclusiveAccess : 1;
            uint16_t tcpTresFlush       : 2;
            uint16_t tcpGracefulDisable : 1;
            uint16_t udpBcastEnabled    : 1;
            uint16_t isServer           : 1;
            uint16_t needsSignal        : 1;    // socket needs signal function from the native socket
            uint16_t needsClose         : 1;    // socket needs to be closed after signaling
            uint16_t reserved           : 5;
        };
        struct {
            uint16_t w :16;
        };
    };
}; // Berkeley Socket structure



/*****************************************************************************
  Function:
    void BerkeleySocketInit(const TCPIP_STACK_MODULE_CTRL* const stackData,
                        const BERKELEY_MODULE_CONFIG* berkeleyData)

  Summary:
    Initializes the Berkeley socket structure array.

  Description:
    This function initializes the Berkeley socket array. This function should
    be called before any BSD socket call.

  Precondition:
    None.

  Parameters:
    None.

  Returns:
    None

  Remarks:
    None.
 */
bool BerkeleySocketInit(const TCPIP_STACK_MODULE_CTRL* const stackData,
                        const BERKELEY_MODULE_CONFIG* berkeleyData);


/*****************************************************************************
  Function:
    void BerkeleySocketDeinit(void)

  Summary:
    De-Initializes the Berkeley socket structure array.

  Description:
    This function deinitializes the Berkeley socket array. This function should
    be called when closing out any BSD socket call.

  Precondition:
    None.

  Parameters:
    None.

  Returns:
    None

  Remarks:
    None.
 */
void BerkeleySocketDeInit(const TCPIP_STACK_MODULE_CTRL* const stackData);




// enable debug features
#define __BERKELEY_DEBUG    0
typedef struct
{
    int16_t bsdState;   // bsd socket status
    int16_t presSktId;  // NET_PRES socket ID
}BSD_SKT_INFO;

#endif  // __BERKELEY_MANAGER_H_


