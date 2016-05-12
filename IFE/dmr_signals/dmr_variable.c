#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>

unsigned char buff[35] = { 0x08, 0x81, 0xAF, 0x8F, 0x81, 0x91, 0x8F, 0xF4, 0x03, 0x53, 0x54, 0x50, 0xA9 };
unsigned char T1_cmd[] = { 0x9B, 0x9C, 0x9E, 0xA3 };
unsigned char T3_cmd[] = { 0x97, 0x98, 0x88, 0x8A, 0xB2 };
unsigned char rec_cmd[6][3] = { 
 	{0x4C, 0x53, 0x52},	/* LRU Status Request */
	{0x53, 0x54, 0x50},	/* Deck Stop Request */
	{0x45, 0x4C, 0x59},	/* Deck Play Request */
	{0x50, 0x53, 0x45},	/* LRU Pause Request */	
	{0x50, 0x53, 0x52}	/* Deck Program Status Request */
};
unsigned char cmd_status_rsp[6][7] = { 
    {0x4E, 0x53, 0x54, 0x30, 0x30, 0x30, 0x34},	/* LRU Pause Response */
	{0x52, 0x4C, 0x53, 0x01, 0xC0, 0x80, 0x00},	/* LRU Status Stopped Response */
	{0x52, 0x4C, 0x53, 0x01, 0xC0, 0x02, 0x80},	/* LRU Status Playing Response */
	{0x52, 0x4C, 0x53, 0x01, 0xC0, 0x02, 0x00},	/* LRU Status Play Start Response */
	{0x52, 0x4C, 0x53, 0x01, 0xC0, 0x03, 0x80}	/* LRU Status Paused Response */
};
unsigned char cmd_short_rsp[3][3] = { 
	{0x41, 0x43, 0x4B},	/* Deck Stop/Play Response */
	{0x4E, 0x53, 0x54}	/* Deck Program Status Response */
};




unsigned char vru_num[1];
unsigned char LRU_Filename[8];
unsigned char DMR_address[3];
unsigned char SCU_address[3];
unsigned char VRU_T2_CMD[] = { 0xF4 };
unsigned char vru_cmd_2[3];
unsigned char T1_num[1];
unsigned char T3_num[1];
unsigned char send_combine;
unsigned char send_tx[20];

int init = 1;
int T3_97_len = 0x08;
int T3_98_len = 0x0C;
int cmd_len = 0x03;
int cmd_rsp_len = 0x07;
int a;
int aa;


unsigned char comp_test1[3][20] = {
    {0x20, 0xAB, 0x25},
    {0x20, 0xAB, 0x21}
};
unsigned char comp_test2[20] = {0x20, 0xAB, 0x21};


mod255( unsigned char *x, int m )
{
  int i;
  
  unsigned int sum=0;
  unsigned int remainder=0;

  assert(m>=0);
  assert(m<(1<<24));		/* else `sum' might overflow 32 bits */
  for(i=0; i<m; i++)
    sum += x[i];		/* add up the 8-bit "digits" of `x[]' */
    printf("routine sum = %d\n",sum);  
  
  
    remainder = sum % 256;
    printf("Remainder after devide = %d\n", remainder);
    sum = 255 - remainder;  
    printf("sum remainder = 0x%02X\n", sum);
  return sum;
}
    


//returns the size of a character array using a pointer to the first element of the character array
int size(char *ptr)
{
    //variable used to access the subsequent array elements.
    int offset = 0;
    //variable that counts the number of elements in your array
    int count = 0;

    //While loop that tests whether the end of the array has been reached
    while (*(ptr + offset) != '\0')
    {
        //increment the count variable
        ++count;
        //advance to the next element of the array
        ++offset;
    }
    //return the size of the array
    return count;
}


int main()
{
    printf("Hello, World!\n");

/*    unsigned char hex[20] = { 0x0B, 0x98, 0x0C, 0x54, 0x43, 0x41, 0x56, 0x52, 0x55, 0x30, 0x30, 0x81, 0x91, 0x8F, 0xF8, 0x82 };	

    printf("The size of my character array is: %d\n", (size(&hex[0])-1));
 
    printf("Mod255 checksum: %x\n", mod255(&hex[0], (size(&hex[0])-1)) );
*/

/*    unsgined char DMR_address[3]; */


/*  ---------------------------- Initialization Section --------------------------------- */
    printf("Init = %d\n", init);
    if (init < 1)
    {
        T3_num[0] = buff[0];
        T1_num[0] = (T3_num[0] - 2);

        printf("T3-0 = 0x%02X\n", T3_num[0]);
        printf("T1-0 = 0x%02X\n", T1_num[0]);
        printf("Buff[0] = 0x%02X\n", buff[0]);
        printf("Buff[1] = 0x%02X\n", buff[1]);
        printf("T3_num[0] = 0x%02X\n", T3_cmd[0]);

        if ( buff[0] == T3_num[0] && buff[1] == T3_cmd[0] )
        {
            for ( a = 1; a <= T3_97_len; a++)
            {
                LRU_Filename[a-1] = buff[a];
            }

    
        send_tx[30] = 0;
        send_tx[0] = T1_num[0];
        send_tx[1] = 0x9C;
        send_tx[2] = mod255(&send_tx[0], (size(&send_tx[0])-1));
        
        printf("Power Up Status Section\n");
        
        /* ----- Added transmit buffer = send_tx array ---------------- */
        }
        
     }

     if ( buff[0] == T3_num[0] && buff[1] == T3_cmd[1] )
     {
         for ( a = 3; a < (3 + T3_98_len); a++)
         {
             LRU_Filename[a-3] = buff[a];
         }
        aa = 0;
         for ( a = 11; a <= 13; a++)
         {
            
	        DMR_address[aa] = buff[a];
            aa=aa+1;
         }
        
        send_tx[30] = 0;
        send_tx[0] = T1_num[0];
        send_tx[1] = 0x9C;
        send_tx[2] = (mod255(&send_tx[0], 2));
        init = 1;
        
        printf("Power Up Configuration Status\n\n\n");
        
        /* ----- Added transmit buffer = send_tx array ---------------- */
        
        
        
        printf("send_tx = 0x%02X\n", send_tx[2]);
       
     }   

/* -----------------  Control Unit Request Section ---------------------*/
    
    if (init == 1)
    {
        printf("Control Section\n");
        int i = 0;
        int s = 0;
        
        
        SCU_address[0] = buff[1];
        SCU_address[1] = buff[2];
        SCU_address[2] = buff[3];
        vru_cmd_2[0] = buff[9];
        vru_cmd_2[1] = buff[10];
        vru_cmd_2[2] = buff[11];

        
        if (memcmp(vru_cmd_2, rec_cmd[0], 3) == 0)  /* --- LRU Status Request --- */
        {
            printf("LRU Status Request\n");
            send_tx[0] = buff[0];
            send_tx[1] = buff[4];
            send_tx[2] = buff[5];
            send_tx[3] = buff[6];
            send_tx[4] = buff[1];
            send_tx[5] = buff[2];
            send_tx[6] = buff[3];
            send_tx[7] = buff[7];
            send_tx[8] = buff[8];
            //LRU Status Pause Response       Not sure how to differentiate between these
	    //LRU Status Stopped Response
	    
	    //LRu Status Playing Response
	    //LRU Status Play Start Response
	    //LRU Status Paused Response
        }

	else if (memcmp(vru_cmd_2, rec_cmd[1], 3) == 0)  /* --- Deck Stop/Play Request --- */
        {
            printf("Deck Stop/Play Response\n");
            send_tx[0] = buff[0];
            send_tx[1] = buff[4];
            send_tx[2] = buff[5];
            send_tx[3] = buff[6];
            send_tx[4] = buff[1];
            send_tx[5] = buff[2];
            send_tx[6] = buff[3];
            send_tx[7] = buff[7];
            send_tx[8] = buff[8];
            send_tx[9] = cmd_short_rsp[0][0];
            send_tx[10] = cmd_short_rsp[0][1];
            send_tx[11] = cmd_short_rsp[0][2];
            send_tx[12] = mod255(&send_tx[0], (size(&send_tx[0])-1));
	    	 
		    printf("send_tx = 0x%02X\n", send_tx[12]);
		    printf("\nsend_tx size = %d \n", (size(&send_tx[0])-1));
        }

        else if (memcmp(vru_cmd_2, rec_cmd[2], 3) == 0)  /* --- Deck Play Request --- */
        {
                printf("LRU Pause Request\n");
        	send_tx[0] = buff[0];
            	send_tx[1] = buff[4];
            	send_tx[2] = buff[5];
            	send_tx[3] = buff[6];
            	send_tx[4] = buff[1];
            	send_tx[5] = buff[2];
            	send_tx[6] = buff[3];
            	send_tx[7] = buff[7];
            	send_tx[8] = buff[8];
		send_tx[9] = cmd_status_rsp[0][0];
		send_tx[10] = cmd_status_rsp[0][1];
		send_tx[11] = cmd_status_rsp[0][2];
		send_tx[12] = cmd_status_rsp[0][3];
		send_tx[13] = cmd_status_rsp[0][4];
		send_tx[14] = cmd_status_rsp[0][5];
		send_tx[15] = cmd_status_rsp[0][6];
		send_tx[16] = mod255(&send_tx[0], (size(&send_tx[0])-1);
    
        }     
        else if (memcmp(vru_cmd_2, rec_cmd[3], 3) == 0)  /* --- LRU Pause Request --- */
        {
                printf("Deck Program Status Request\n");
            	send_tx[0] = buff[0];
            	send_tx[1] = buff[4];
            	send_tx[2] = buff[5];
            	send_tx[3] = buff[6];
            	send_tx[4] = buff[1];
            	send_tx[5] = buff[2];
            	send_tx[6] = buff[3];
            	send_tx[7] = buff[7];
            	send_tx[8] = buff[8];
            	send_tx[9] = cmd_short_rsp[1][0];
            	send_tx[10] = cmd_short_rsp[1][1];
            	send_tx[11] = cmd_short_rsp[1][2];
            	send_tx[12] = mod255(&send_tx[0], (size(&send_tx[0])-1));
            
        }     

/* -----------------------------  End of Control Section -----------------------------------*/



    }
}

