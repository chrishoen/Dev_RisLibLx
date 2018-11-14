#pragma once

/*==============================================================================
This file provides a set of functions that are used to define default thread
priorities for base class threads that are defined in RisLib.

The priorities are exported via a set of functions instead of a set of 
enumerations so that they will be portable. Different .cpp files are
used for different operation systems (_priorites_win.cpp, 
_priorites_qnx.cpp)

==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

// RisLib default thread priorities
int get_default_thread_priority();
int get_default_high_thread_priority();
int get_default_low_thread_priority();

int get_default_qcall_thread_priority();

int get_default_short_thread_priority();
int get_default_long_thread_priority();

int get_default_udp_rx_thread_priority();
int get_default_tcp_server_thread_priority();
int get_default_tcp_client_thread_priority();

int get_default_log_thread_priority();
int get_default_print_thread_priority();

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

