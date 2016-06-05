/**********************************************************************************************************************
File: user_app.c                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app as a template:
 1. Copy both user_app.c and user_app.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "user_app" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "UserApp" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "USER_APP" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

Description:
This is a user_app.c file template 

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:


Protected System functions:
void UserAppInitialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void UserAppRunActiveState(void)
Runs current task state.  Should only be called once in main loop.


**********************************************************************************************************************/

#include "configuration.h"
#define NUM 1000
#define CYCLE_TIME 50
/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserAppFlags;                       /* Global state flags */

u8 G_u8Index2=0;                                      /*used to record G_u8Char2 Count*/

u8 G_u8Char2[7];                                      /*The buffer to Store the letter of your name each time*/

u8 G_u8Count;                                         /*used to record  how many times my name have received*/ 

bool G_Sign=FALSE;                                    /*used to answer when to BLINK and scream*/
/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */

extern u8 G_au8DebugScanfBuffer[];                     /* From debug.c */
extern u8 G_u8DebugScanfCharCount;                     /* From debug.c  */

/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp_StateMachine;            /* The state machine function pointer */
static u32 UserApp_u32Timeout;                      /* Timeout counter used across states */


/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------
Function: UserAppInitialize

Description:
Initializes the State Machine and its variables.

Requires:
  -

Promises:
  - 
*/
void UserAppInitialize(void)
{
  /*Show the Line1*/
  static u8 String0[]="A3.LeiDiao";
  LCDCommand(LCD_CLEAR_CMD);
  LCDMessage(LINE1_START_ADDR+5, String0);
  
  /*Set the colour of LCD*/
  LedOn(LCD_BLUE);
  LedOff(LCD_GREEN);
  LedOff(LCD_RED);
  
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp_StateMachine = UserAppSM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp_StateMachine = UserAppSM_FailedInit;
  }

} /* end UserAppInitialize() */


/*----------------------------------------------------------------------------------------------------------------------
Function UserAppRunActiveState()

Description:
Selects and runs one iteration of the current state in the state machine.
All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
  - State machine function pointer points at current state

Promises:
  - Calls the function to pointed by the state machine function pointer
*/
void UserAppRunActiveState(void)
{
  UserApp_StateMachine();

} /* end UserAppRunActiveState */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for a message to be queued */
static void UserAppSM_Idle(void)
{
  /*used to record G_u8DebugScanfCharCount*/
  static u8  u8Buffer_Char_Count;
  
  /*used to record String3 Count*/
  static u8  i;
  
  /*used to record String3 Count*/
  static u8  j;
  
  /*used to test if you have input the letter of your name*/
  static u8  t=0;
  
  /*used to record u8Char0 Count*/
  static u16 u16Index0=0;
  
  /*used to record u8Char1 Count*/
  static u16 u16Index1=0; 
  
  /*used to record G_u8Char2 Count*/
  /*static u8  G_u8Index2=0;
  
  /*used to record  how many times my name have received*/  
 // static u8  G_u8Count=0;
  
  /*used to store the characters you have received*/
  static u8  u8Char0[NUM];
  
  /*used to store the characters shown in my LCD each time*/
  static u8  u8Char1[21];
  
  /*The buffer to Store the letter of your name each time*/
 // static u8  G_u8Char2[7];
  
  /*used to time*/
  static u8  Time=CYCLE_TIME;
  
  /*used to remind*/
  static u8  String1[]="\n\rTotal characters received:";
  static u8  String2[]="\n\rCharacter count cleared\n\r";
  static u8  String5[]="\n\rThe current letters are:";
  static u8  String6[]="The next will coming.\n\r";
  
  /*used to compare with my name*/ 
  static u8  String3[7]={'L','E','I','D','I','A','O'};
  static u8  String4[7]={'l','e','i','d','i','a','o'};

/*Give what it has received from the Tera Term to Char0 and Char1 and Compare each letter with
String3 and String4*/  
  Time--; 
  if(Time==0 )
  { 
    Time=CYCLE_TIME;
 
    if(G_u8DebugScanfCharCount)
    {
      /*Used with BUTTON0 to Clear the line*/
      ButtonAcknowledge(BUTTON0);
      
      u8Buffer_Char_Count= DebugScanf(u8Char0+u16Index0);
      u8Char1[u16Index1]=u8Char0[u16Index0];
     /*Compare the letter with String3 and String4*/
      if((u8Char0[u16Index0]==String3[i])||(u8Char0[u16Index0]==String4[j]))
      {
        t=1;
        G_u8Char2[G_u8Index2]=u8Char0[u16Index0];
        G_u8Index2++;
        i++;
        j++;
        G_u8Char2[G_u8Index2]='\0';
        if(G_u8Index2==7)
        {
          G_Sign=TRUE;
          G_u8Count++;
          i=0;
          j=0;
          G_u8Index2=0;
        }
      }  
      u16Index0++;
      u16Index1++;
      u8Char1[u16Index1]='\0';
      if(u16Index1==20)
      {
        u16Index1=0;
      }   
    }
    
    /*make the twenty letters shown in the LINE2*/
    LCDClearChars(LINE2_START_ADDR, 20);
    LCDMessage(LINE2_START_ADDR,u8Char1);
     
     /*BUTTON0*/
     if( WasButtonPressed(BUTTON0) )
     {
       LCDClearChars(LINE2_START_ADDR, 20);
       u16Index1=0;
     }
  }
    /*BUTTON1*/
  if( WasButtonPressed(BUTTON1) )
  {
     ButtonAcknowledge(BUTTON1); 
     DebugPrintf(String1);
     DebugPrintNumber(u16Index0);
     DebugLineFeed();
  }
     /*BUTTON2*/
  if( WasButtonPressed(BUTTON2) )
  {
     ButtonAcknowledge(BUTTON2); 
     u16Index0=0;
     u16Index1=0;  
     DebugPrintf(String2);
  }
     /*BUTTON3*/
  if( WasButtonPressed(BUTTON3) )
  {
     ButtonAcknowledge(BUTTON3);
     DebugPrintf(String5);
     if(t)
     {
       DebugPrintf(G_u8Char2);
       DebugLineFeed();
     }
     else
     {
       DebugPrintf(String6);
       DebugLineFeed();
     }
   }


} /* end UserAppSM_Idle() */
     

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserAppSM_Error(void)          
{
  
} /* end UserAppSM_Error() */


/*-------------------------------------------------------------------------------------------------------------------*/
/* State to sit in if init failed */
static void UserAppSM_FailedInit(void)          
{
    
} /* end UserAppSM_FailedInit() */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
