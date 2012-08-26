/**
 * @file tpl_asm_definitions.h
 *
 * @section descr File description
 *
 * Common definitions for ARM assembler source files
 *
 * @section copyright Copyright
 *
 * Trampoline OS
 *
 * Trampoline is copyright (c) IRCCyN 2005+
 * Copyright ESEO for function and data structures documentation and ARM port
 * Trampoline is protected by the French intellectual property law.
 *
 * This software is distributed under the Lesser GNU Public Licence
 *
 * @section infos File informations
 *
 * $Date: 2010-04-14 09:41:19 +0200 (Mer, 14 avr 2010) $
 * $Rev: 1133 $
 * $Author: jlb $
 * $URL: https://trampoline.rts-software.org/svn/trunk/machines/arm/tpl_asm_definitions.h $
 */
#ifndef TPL_ASM_DEFINITIONS_H
#define TPL_ASM_DEFINITIONS_H

#include "tpl_os_definitions.h"
#include "tpl_service_ids.h"

/*
 * tpl_kern data structure offsets (for use into assembler sources)
 */
#define TPL_KERN_OFFSET_S_OLD 0
#define TPL_KERN_OFFSET_S_RUNNING 4
#define TPL_KERN_OFFSET_OLD 8
#define TPL_KERN_OFFSET_RUNNING 12
#define TPL_KERN_OFFSET_RUNNING_ID 16
#define TPL_KERN_OFFSET_NEED_SWITCH 20

/*
 * ARM modes codes
 */
#define CPSR_USR_MODE 0b10000
#define CPSR_FIQ_MODE 0b10001
#define CPSR_IRQ_MODE 0b10010
#define CPSR_SVC_MODE 0b10011
#define CPSR_ABT_MODE 0b10111
#define CPSR_UND_MODE 0b11011
#define CPSR_SYS_MODE 0b11111
#define CPSR_MODE_MASK 0b11111

/*
 * Interrupt masks
 */
#define CPSR_IRQ_LOCKED 0b10000000
#define CPSR_FIQ_LOCKED 0b01000000

#endif /* TPL_ASM_DEFINITIONS_H */

/* Enf of file tpl_asm_definitions.h */
