/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "ris_priorities.h"


//******************************************************************************
namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// RisLib default thread priorities

int get_default_thread_priority()                      { return 50; }
int get_default_high_thread_priority()                 { return 80; }
int get_default_low_thread_priority()                  { return 20; }

int get_default_qcall_thread_priority()                { return 50; }
int get_default_qcall_timer_thread_priority()          { return 50; }

int get_default_short_thread_priority()                { return 80; }
int get_default_short_timer_thread_priority()          { return 50; }
int get_default_long_thread_priority()                 { return 50; }

int get_default_udp_rx_thread_priority()               { return 80; }
int get_default_tcp_server_thread_priority()           { return 50; }
int get_default_tcp_client_thread_priority()           { return 50; }

//******************************************************************************
}//namespace




