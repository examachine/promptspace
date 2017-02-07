//
//
// C++ Implementation for module: Log
//
// Description: 
//
//
// Author: exa
//
//

#include <iostream>
#include "Log.hxx"


#ifdef LOG

ostream & nlog = clog;

#else

nlog_t nlog;

#endif

#ifdef ERROR

ostream & nerr = cerr;

#else

nerr_t nerr;

#endif
