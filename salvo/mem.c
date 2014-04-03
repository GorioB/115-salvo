/************************************************************ 
The contents of this file are subject to the Pumpkin Salvo
License (the "License").  You may not use this file except
in compliance with the License. You may obtain a copy of
the License at http://www.pumpkininc.com, or from
warranty@pumpkininc.com.

Software distributed under the License is distributed on an
"AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express
or implied. See the License for specific language governing
the warranty and the rights and limitations under the
License.

The Original Code is Salvo - The RTOS that runs in tiny
places(TM). Copyright (C) 1995-2002 Pumpkin, Inc. and its
Licensor(s). All Rights Reserved.

$Source: C:\\RCS\\d\\salvo\\src\\mem.c,v $
$Author: aek $
$Revision: 3.28 $
$Date: 2003-07-24 15:18:23-07 $

Global variable declarations.

************************************************************/

#if !defined(__OSMEM_C)
#define __OSMEM_C
 
#include <salvo.h>

/************************************************************ 
****                                                     ****
**                                                         **
ICCAVR users can force Salvo's global objects into a unique
program area called salvoram. When used, specify the start
and end addresses of this program area via:  
          
 	-bsalvoram:0xstart.0xend    
                       
as a linker option. Otherwise the salvoram program area will
simply follow the data program area (default). 
     
MPLAB-C18 requires an explicitly defined segment. Can only
do all-or-nothing -- that's why OSLOC_ALL cannot be used.

**                                                         **
****                                                     ****
************************************************************/
#if ( (OSCOMPILER == OSIMAGECRAFT) && (OSTARGET == OSAVR) )
#pragma data:salvoram  
#endif  


#if ( (OSCOMPILER == OSMPLAB_C18) \
  &&  (OSMPLAB_C18_LOC_ALL_NEAR == TRUE) )
#pragma udata access OSVars
#endif


/* by placing OScTcbP as the first Salvo object, we're  */
/*  able to avoid problems that crop up with targets    */
/*  that allow placement of variables at address 0      */
/*  (e.g. PIC18+MPLAB-C18, 8051+xdata, etc.). This      */
/*  works because we never point to OScTcbP.            */
#if OSENABLE_TASKS
#include <salvoprg.h>
OSgltypeCTcbP OScTcbP;
#endif


#if OSUSE_ARRAYS
#if OSARRAY_SIZE_IS_BYTE
const OStypePrioA OSBits[8] = 
        { 0x0001, 0x0002, 0x0004, 0x0008,
          0x0010, 0x0020, 0x0040, 0x0080 };
#else
const OStypePrioA OSBits[16] = 
        { 0x0001, 0x0002, 0x0004, 0x0008,
          0x0010, 0x0020, 0x0040, 0x0080,
          0x0100, 0x0200, 0x0400, 0x0800,
          0x1000, 0x2000, 0x4000, 0x8000 }; 
#endif /* #if OSTASKS < 9 */    
#endif


#if OSENABLE_TASKS
#include <salvoprg.h>
OSgltypeTcb OStcbArea[OSTASKS];
#endif


#if OSENABLE_TASKS && !OSUSE_ARRAYS
#include <salvoprg.h>
OSgltypeTcbP OSeligQP;
#endif


#if OSENABLE_TASKS && OSUSE_ARRAYS
#include <salvoprg.h>
OSgltypePrioA OSeligQP;
#endif


#if OSENABLE_EVENTS
#include <salvoprg.h>
OSgltypeEcb OSecbArea[OSEVENTS];
#endif


#if OSENABLE_SIGQ
#include <salvoprg.h>
OSgltypeSigQP OSsigQinP;
#endif


#if OSENABLE_SIGQ
#include <salvoprg.h>
OSgltypeSigQP OSsigQoutP;
#endif


#if OSENABLE_EVENT_FLAGS && OSEVENT_FLAGS
#include <salvoprg.h>
OSgltypeEfcb OSefcbArea[OSEVENT_FLAGS];
#endif


#if OSENABLE_MESSAGE_QUEUES && OSMESSAGE_QUEUES
#include <salvoprg.h>
OSgltypeMqcb OSmqcbArea[OSMESSAGE_QUEUES];
#endif


#if OSENABLE_DELAYS || OSENABLE_TIMEOUTS
#include <salvoprg.h>
OSgltypeTcbP OSdelayQP;
#endif


#if OSENABLE_STACK_CHECKING
#include <salvoprg.h>
OSgltypeDepth OSstkDepth, OSmaxStkDepth;
#endif


#if OSGATHER_STATISTICS && OSENABLE_COUNTS
#include <salvoprg.h>
OSgltypeCount OSctxSws;
#endif


#if OSGATHER_STATISTICS && OSENABLE_COUNTS && OSENABLE_IDLE_COUNTER
#include <salvoprg.h>
OSgltypeCount OSidleCtxSws;
#endif


#if OSGATHER_STATISTICS && OSENABLE_TIMEOUTS
#include <salvoprg.h>
OSgltypeErr OStimeouts;
#endif


#if OSLOGGING
#include <salvoprg.h>
OSgltypeErr OSerrs, OSwarns;
#endif
 
 
#if OSLOG_MESSAGES > OSLOG_NONE
#include <salvoprg.h>
OSgltypeLogMsg OSlogMsg[80];
#endif


#if OSENABLE_TICKS
#include <salvoprg.h>
OSgltypeTick OStimerTicks;
#endif


#if OSENABLE_PRESCALAR
#include <salvoprg.h>
OSgltypePS OStimerPS;
#endif


#if OSCTXSW_METHOD == OSRTNADDR_IS_VAR
#include <salvoprg.h>
OStypeRtnAddr OSrtnAddr;
#endif


#if OSENABLE_DELAYS
#include <salvoprg.h>
OSgltypeLostTick OSlostTicks;
#endif


/* levels are supported, target-dependent */
#if ( (OSCTXSW_METHOD == OSVIA_OSDISPATCH) \
  &&  (OSMONITOR_KEYWORD_EXISTS == FALSE) )
#include <salvoprg.h>
OSgltypeSRGIE OSsrGIE;
#endif


#if ( (OSCTXSW_METHOD == OSVIA_OSDISPATCH) \
  ||  (OSCTXSW_METHOD == OSVIA_OSDISPATCH_WLABEL) )
#include <salvoprg.h>
OSgltypeFrameP OSframeP;
#endif


/* eight levels are more than enough ... */
#if ( OSCOMPILER == OSIAR_ICC ) && ( OSTARGET == OSPIC18 )

#define OSLOC_SAVEPIC18INTS __nonbanked /* always */

#if OSPIC18_INTERRUPT_MASK & 0x80
#include <salvoprg.h>
OSLOC_SAVEPIC18INTS OStypeInt8u OSsavePIC18GIE;
#endif

#if OSPIC18_INTERRUPT_MASK & 0x40
#include <salvoprg.h>
OSLOC_SAVEPIC18INTS OStypeInt8u OSsavePIC18PEIE;
#endif

#undef OSLOC_SAVEPIC18INTS

#endif


/* revert to normal data program area */
#if ( (OSCOMPILER == OSIMAGECRAFT) && (OSTARGET == OSAVR) )
#pragma data:data  
#endif  




#endif /* __OSMEM_C #include guard */
