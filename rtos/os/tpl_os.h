/**
 * @file tpl_os.h
 *
 * @section desc File description
 *
 * This header file provides standard OSEK-VDX API. We do not give detailed
 * documentation here, you just have to follow the specification reference for
 * each element you need more informations.
 *
 * @section copyright Copyright
 *
 * Trampoline OS
 *
 * Trampoline is copyright (c) IRCCyN 2005+
 * Copyright ESEO for function and data structures documentation
 * Trampoline is protected by the French intellectual property law.
 *
 * This software is distributed under the Lesser GNU Public Licence
 *
 * @section infos File informations
 *
 * $Date: 2010-07-09 11:04:08 +0200 (Ven, 09 jul 2010) $
 * $Rev: 1184 $
 * $Author: jlb $
 * $URL: https://trampoline.rts-software.org/svn/trunk/os/tpl_os.h $
 */

#ifndef TPL_OS_H
#define TPL_OS_H

#ifdef __cplusplus
extern "C" {
#endif
  
#include "tpl_os_definitions.h"
#include "tpl_os_application_def.h"

/*
 * Inclusion of the header for task related API services
 */
#include "tpl_os_task.h"

/*
 * Inclusion of the header for event related API services
 */
#include "tpl_os_event.h"

/*
 * Inclusion of the header for alarm related API services
 */
#include "tpl_os_alarm.h"

/*
 * Inclusion of the header for resource related API services
 */
#include "tpl_os_rez.h"

/*
 * Inclusion of the header for interrupt related API services
 */
#include "tpl_os_it.h"

/*
 * Inclusion of the header for service id and parameters access
 */
#include "tpl_os_error.h"

#if WITH_COM == YES
/*
 * Inclusion of the header for communication related API services
 */
#include "tpl_com_internal.h"
#endif

#if WITH_AUTOSAR == YES
/*
 * Inclusion of the header for schedule table related API services
 * Only for AUTOSAR extension
 */
#include "tpl_as_schedtable.h"

/*
 * Inclusion of the header for counters related API services
 * Only for AUTOSAR extension
 */
#include "tpl_as_counter.h"

/* WITH_AUTOSAR */
#endif

/**
 * @typedef AppModeType
 *
 * Application execution mode
 *
 * see paragraph 13.7.1 page 66 of OSEK/VDX 2.2.2 spec
 */
typedef tpl_application_mode AppModeType;

/*
 * System services
 * see paragraph 13.7.2,
 * pages 66+ of OSEK/VDX 2.2.2 spec
 */

#define API_START_SEC_CODE
#include "tpl_memmap.h"

/**
 * Gives the current application mode
 *
 * @return the application mode
 *
 * see paragraph 13.7.2.1 page 66 of OSEK/VDX 2.2.2 spec
 */
FUNC(AppModeType, OS_CODE) GetActiveApplicationMode(void);


/**
 * Starts Trampoline
 *
 * @param Mode choosen application mode
 *
 * see paragraph 13.7.2.2 page 66 of OSEK/VDX 2.2.2 spec
 */
extern FUNC(void, OS_CODE) StartOS(
    CONST(AppModeType, AUTOMATIC) mode
);


/**
 * Stops Trampoline
 *
 * @param Error error occured
 *
 * see paragraph 13.7.2.3 page 67 of OSEK/VDX 2.2.2 spec
 */
FUNC(void, OS_CODE) ShutdownOS(
    CONST(StatusType, AUTOMATIC) error
);

/**
 * Call Terminate Task function when no TerminateTask hasn't been called
 * or when TerminateTask didn't success because of resource hold or
 * interrupts disabled.
 *
 */
FUNC(void, OS_CODE) CallTerminateTask(void);


/**
 * Call Terminate ISR2 function when TerminateISR didn't success doing it
 * because of resource hold or interrupts disabled.
 *
 */
FUNC(void, OS_CODE) CallTerminateISR2(void);

#define API_STOP_SEC_CODE
#include "tpl_memmap.h"

#ifdef __cplusplus
}
#endif

#endif /* TPL_OS_H */

/* End of file tpl_os.h */
