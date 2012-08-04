/**
 * @file tpl_memmap.h
 *
 * @section desc File description
 *
 * Trampoline wrapper to memory mapping platform specific header
 *
 * @section copyright Copyright
 *
 * Trampoline OS
 *
 * Trampoline is copyright (c) IRCCyN 2005-2007
 * Trampoline is protected by the French intellectual property law.
 *
 * This software is distributed under the Lesser GNU Public Licence
 *
 * @section infos File informations
 *
 * $Date: 2010-01-26 16:44:22 +0100 (Mar, 26 jan 2010) $
 * $Rev: 1032 $
 * $Author: jlb $
 * $URL: https://trampoline.rts-software.org/svn/trunk/os/tpl_memmap.h $
 */

#include "tpl_app_define.h"

#if WITH_MEMMAP != YES && WITH_MEMMAP != NO
  #error "Wrong configuration of the OS, WITH_MEMMAP should be YES or NO"
#endif

#if WITH_MEMMAP == YES
  #include "MemMap.h"
#endif

/* End of file tpl_memmap.h */
