//##########################################################
//##                                                      ##
//##               hackweek 10 trackbot                   ##
//##                      based on                        ##
//##          ReadWrite Example code for Dynamixel.       ##
//##                                           2009.11.10 ##
//##########################################################

// move relative to direction
// Clockwise: direction = 0 ; Counter Clockwise: direction != 1
int dyn_rel_move(int id, int direction, int distance, int min, int max);

// Initialize USB2Dynamixel
int dyn_initialize(int baudnum, int deviceIndex);

// Center all servos
void dyn_center_all(int target);

// Check if dynamixel is still moving
int dyn_check_moving(int id);

// Get dynamixel position
int dyn_get_position(int id);

// control torque
int dyn_torque(int id, int state);

// control servo speed
void dyn_set_speed(int id, int speed);

// Print communication result
void dyn_PrintCommStatus(int CommStatus);

// Print error bit of status packet
void dyn_PrintErrorCode();
