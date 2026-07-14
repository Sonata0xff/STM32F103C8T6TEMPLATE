#include "esc.h"
#ifdef ESC_API_EN

//---------------private func--------------
//power start running func.
void ESC_APP_On()
{
	
}
//app env init. 
void ESC_APP_Init()
{
	
}


//---------------public func---------------
ESC_ERROR ESC_APP_Main()
{
	ESC_APP_On();
	ESC_APP_Init();
	return ESC_ERR_NERR;
}

//---------------debug func----------------
#ifdef ESC_DEBUG_MODE
__weak ESC_ERROR ESC_TRANS_LOW_STAB_Check_func()
{
	return ESC_ERR_NERR;
}
#endif
#endif