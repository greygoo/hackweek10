//##########################################################
//##                                                      ##
//##               hackweek 10 trackbot                   ##
//##                      based on                        ##
//##          ReadWrite Example code for Dynamixel.       ##
//##                                           2009.11.10 ##
//##########################################################
#include <stdio.h>
#include <termio.h>
#include <unistd.h>
#include <dynamixel.h>

// Control table address
#define P_GOAL_POSITION_L	30
#define P_GOAL_POSITION_H	31
#define P_PRESENT_POSITION_L	36
#define P_PRESENT_POSITION_H	37
#define P_MOVING		46

// Defulat setting
#define DEFAULT_BAUDNUM		1 // 1Mbps

void PrintCommStatus(int CommStatus);
void PrintErrorCode(void);

// Clockwise: direction = 0 ; Counter Clockwise: direction != 1
int move(int id, int direction, int distance, int min, int max)
{
	int PresentPos;
	int Goal;
	int CommStatus;

			PresentPos = dxl_read_word( id, P_PRESENT_POSITION_L );
			CommStatus = dxl_get_result();

			if( CommStatus == COMM_RXSUCCESS )
			{
				if( direction == 0 ) // move CW
				{
					Goal = PresentPos - distance;
					if(Goal <= min)
					{
						Goal = min;
					}
				}
				else // move CCW 
				{
					Goal = PresentPos + distance;
					if(Goal >= max)
					{
						Goal = max;
					}
				}

				printf( "%d   %d\n",Goal, PresentPos );

				if ( Goal >= min && Goal <= max )
				{
					dxl_write_word( id, P_GOAL_POSITION_L, Goal );
					PrintErrorCode();
					return(1);
				}
				else
				{
					printf("Goal exceeds boundaries\n");
				}
				return(0);
			}
			else
			{
				PrintCommStatus(CommStatus);
				return(0);
			}
}

int initialize(int baudnum, int deviceIndex)
{
	if( dxl_initialize(deviceIndex, baudnum) == 0 )
	{
		printf( "Failed to open USB2Dynamixel!\n" );
		printf( "Press Enter key to terminate...\n" );
		getchar();
		return 0;
	}
	else
	{
		printf( "Succeed to open USB2Dynamixel!\n" );
		return 1;
	}
} 

int check_moving(int id)
{
	int Moving;
	int CommStatus;

	Moving = dxl_read_byte( id, P_MOVING );
	CommStatus = dxl_get_result();
	if( CommStatus == COMM_RXSUCCESS )
	{
		PrintErrorCode();
	}
	else
	{
		PrintCommStatus(CommStatus);
	}	
	return(Moving);
}



int main()
{
	// USB Serial Device to use
	int deviceIndex = 0;

	initialize(DEFAULT_BAUDNUM, deviceIndex);

	move(11, 0, 300, 0, 300);
	do
	{
	} while(check_moving(11) == 1);

//	getchar();
	move(11, 1, 300, 0, 300);

	/*
	while(1)
	{
		printf( "Press Enter key to continue!(press ESC and Enter to quit)\n" );
		if(getchar() == 0x1b)
			break;

	}
	*/

	// Close device
	dxl_terminate();
	printf( "Press Enter key to terminate...\n" );
	getchar();
	return 0;
}
// Print communication result
void PrintCommStatus(int CommStatus)
{
	switch(CommStatus)
	{
	case COMM_TXFAIL:
		printf("COMM_TXFAIL: Failed transmit instruction packet!\n");
		break;

	case COMM_TXERROR:
		printf("COMM_TXERROR: Incorrect instruction packet!\n");
		break;

	case COMM_RXFAIL:
		printf("COMM_RXFAIL: Failed get status packet from device!\n");
		break;

	case COMM_RXWAITING:
		printf("COMM_RXWAITING: Now recieving status packet!\n");
		break;

	case COMM_RXTIMEOUT:
		printf("COMM_RXTIMEOUT: There is no status packet!\n");
		break;

	case COMM_RXCORRUPT:
		printf("COMM_RXCORRUPT: Incorrect status packet!\n");
		break;

	default:
		printf("This is unknown error code!\n");
		break;
	}
}

// Print error bit of status packet
void PrintErrorCode()
{
	if(dxl_get_rxpacket_error(ERRBIT_VOLTAGE) == 1)
		printf("Input voltage error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_ANGLE) == 1)
		printf("Angle limit error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_OVERHEAT) == 1)
		printf("Overheat error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_RANGE) == 1)
		printf("Out of range error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_CHECKSUM) == 1)
		printf("Checksum error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_OVERLOAD) == 1)
		printf("Overload error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_INSTRUCTION) == 1)
		printf("Instruction code error!\n");
}
