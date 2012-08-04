/**
 * @file tpl_compiler.h
 *
 * @section descr File description
 *
 * Trampoline compiler specific settings.
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
 * $Date: 2010-01-26 16:26:30 +0100 (Mar, 26 jan 2010) $
 * $Rev: 1031 $
 * $Author: jlb $
 * $URL: https://trampoline.rts-software.org/svn/trunk/os/tpl_compiler.h $
 */

#ifndef TPL_COMPILER_H
#define TPL_COMPILER_H

#include "tpl_app_define.h"

#if WITH_COMPILER_SETTINGS == YES

#include "Compiler.h"

#elif WITH_COMPILER_SETTINGS == NO

#define AUTOMATIC
#define TYPEDEF
#define STATIC static
#define NULL_PTR ((void *)0)
#define INLINE (#pragma INLINE)

#define FUNC(rettype, memclass) rettype memclass
#define P2VAR(ptrtype, memclass, ptrclass) ptrtype memclass * ptrclass
#define P2CONST(ptrtype, memclass, ptrclass) const ptrtype memclass * ptrclass
#define CONSTP2VAR(ptrtype, memclass, ptrclass) \
                     ptrtype memclass * const ptrclass
#define CONSTP2CONST(ptrtype, memclass, ptrclass) \
                     const ptrtype memclass * const ptrclass
#define P2FUNC(rettype, ptrclass, fctname) rettype (*ptrclass fctname)
#define CONST(consttype, memclass) const consttype memclass
#define VAR(vartype, memclass) vartype memclass


#define OS_CODE               AUTOMATIC
#define OS_VAR_NOINIT         AUTOMATIC
#define OS_VAR_POWER_ON_INIT  AUTOMATIC
#define OS_VAR_FAST           AUTOMATIC
#define OS_VAR                AUTOMATIC
#define OS_CONST              AUTOMATIC
#define OS_APPL_DATA          AUTOMATIC
#define OS_APPL_CONST         AUTOMATIC
#define OS_APPL_CODE          AUTOMATIC

#else
  #error "Configuration error WITH_COMPILER_SETTINGS is not defined"
#endif

/* TPL_COMPILER_H */
#endif

/* End of file tpl_compiler.h */
