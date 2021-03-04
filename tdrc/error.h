#ifndef _H_ERROR_H
#define _H_ERROR_H

typedef enum error_code_e error_code_t;
enum error_code_e
{
	E_TD_NOERROR = 0,
	E_TD_ERROR = -1,

	E_TD_SCANNER_STACK_SIZE_PROBLEM								= -1000,
	E_TD_SCANNER_READLY_IN_STACK								= -1001,
	E_TD_SCANNER_CAN_NOT_OPEN_FILE								= -1002,
	E_TD_SCANNER_OUT_OF_MEMORY									= -1003,
};

#endif //_H_ERROR_H
