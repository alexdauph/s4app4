#include "packet.h"
#include "app_commands.h"

unsigned int pkt_index = 0;
unsigned int pkt_count = 0;
// int pkt_buff0[PKT_LEN] = {0};
// int pkt_buff1[PKT_LEN] = {0};
// int *pkt_build_ptr = pkt_buff0;
// int *pkt_send_ptr = pkt_buff1;

extern UDP_DATA appData;

void PKT_Init(void)
{
    pkt_index = 0;
    pkt_count = 0;
    UDP_Build_Buffer_Ptr = UDP_Buffer0;
    UDP_Send_Buffer_Ptr = UDP_Buffer1;
}

void PKT_Add(int val_x, int val_y, int val_z)
{
    unsigned int temp_x, temp_y, temp_z;

    if (val_x < 0)
        temp_x = (unsigned int)(-1 * val_x);
    else
        temp_x = (unsigned int)val_x;

    if (val_y < 0)
        temp_y = (unsigned int)(-1 * val_y);
    else
        temp_y = (unsigned int)val_y;
    if (val_z < 0)
        temp_z = (unsigned int)(-1 * val_z);
    else
        temp_z = (unsigned int)val_z;
    RGBLED_SetValue((temp_x >> 4UL) & 0xFF, (temp_y >> 4UL) & 0xFF, (temp_z >> 4UL) & 0xFF);

    if (pkt_count == 0)
    {
        UDP_Build_Buffer_Ptr[0] = (pkt_index & 0xFF000000) >> 24UL;
        UDP_Build_Buffer_Ptr[1] = (pkt_index & 0x00FF0000) >> 16UL;
        UDP_Build_Buffer_Ptr[2] = (pkt_index & 0x0000FF00) >> 8UL;
        UDP_Build_Buffer_Ptr[3] = (pkt_index & 0x000000FF) >> 0UL;
    }

    if (pkt_count < 40)
    {
        UDP_Build_Buffer_Ptr[PKT_X_OFFSET + (pkt_count << 2) + 0] = (val_x & 0xFF000000) >> 24UL;
        UDP_Build_Buffer_Ptr[PKT_X_OFFSET + (pkt_count << 2) + 1] = (val_x & 0x00FF0000) >> 16UL;
        UDP_Build_Buffer_Ptr[PKT_X_OFFSET + (pkt_count << 2) + 2] = (val_x & 0x0000FF00) >> 8UL;
        UDP_Build_Buffer_Ptr[PKT_X_OFFSET + (pkt_count << 2) + 3] = (val_x & 0x000000FF) >> 0UL;
        UDP_Build_Buffer_Ptr[PKT_Y_OFFSET + (pkt_count << 2) + 0] = (val_y & 0xFF000000) >> 24UL;
        UDP_Build_Buffer_Ptr[PKT_Y_OFFSET + (pkt_count << 2) + 1] = (val_y & 0x00FF0000) >> 16UL;
        UDP_Build_Buffer_Ptr[PKT_Y_OFFSET + (pkt_count << 2) + 2] = (val_y & 0x0000FF00) >> 8UL;
        UDP_Build_Buffer_Ptr[PKT_Y_OFFSET + (pkt_count << 2) + 3] = (val_y & 0x000000FF) >> 0UL;
        UDP_Build_Buffer_Ptr[PKT_Z_OFFSET + (pkt_count << 2) + 0] = (val_z & 0xFF000000) >> 24UL;
        UDP_Build_Buffer_Ptr[PKT_Z_OFFSET + (pkt_count << 2) + 1] = (val_z & 0x00FF0000) >> 16UL;
        UDP_Build_Buffer_Ptr[PKT_Z_OFFSET + (pkt_count << 2) + 2] = (val_z & 0x0000FF00) >> 8UL;
        UDP_Build_Buffer_Ptr[PKT_Z_OFFSET + (pkt_count << 2) + 3] = (val_z & 0x000000FF) >> 0UL;
        pkt_count++;
    }
}

void PKT_Tasks(void)
{
    if (pkt_count == 40 && appData.clientState == UDP_TCPIP_WAITING_FOR_COMMAND)
    {
        if (UDP_Build_Buffer_Ptr == UDP_Buffer0)
        {
            UDP_Build_Buffer_Ptr = UDP_Buffer1;
            UDP_Send_Buffer_Ptr = UDP_Buffer0;
        }
        else
        {
            UDP_Build_Buffer_Ptr = UDP_Buffer0;
            UDP_Send_Buffer_Ptr = UDP_Buffer1;
        }

        pkt_count = 0;
        pkt_index++;
        UDP_bytes_to_send = PKT_LEN;
        UDP_Send_Packet = true;
    }
}