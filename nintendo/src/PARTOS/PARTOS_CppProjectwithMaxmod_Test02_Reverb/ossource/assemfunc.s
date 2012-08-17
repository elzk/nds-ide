/*
 * assemfunc.s
 *
 *  Created on: 2010. 5. 15.
 *  Readed on : 2010. 5. 18.
 *      Author: gamja9e
 */

	.section ".text"
    .align  4
    .arm

   .global vPortDisableInterruptsFromThumb
   .global vPortEnableInterruptsFromThumb
   .global portRESTORE_CONTEXT_FIRST
   .global portRESTORE_CONTEXT
   .global vPreemptiveTick


vPortDisableInterruptsFromThumb:
	STMDB	SP!, {R0}
	MRS	R0, CPSR
	ORR	R0, R0, #0xC0
	MSR	CPSR, R0
	LDMIA	SP!, {R0}
	BX		R14

vPortEnableInterruptsFromThumb:
	STMDB	SP!, {R0}
	MRS	R0, CPSR
	BIC	R0, R0, #0xC0
	MSR	CPSR, R0
	LDMIA	SP!, {R0}
	BX		R14

	.extern pxCurrentTCB
	.extern ulCriticalNesting
portRESTORE_CONTEXT_FIRST:
    // ���ͷ�Ʈ ����
	MOV R3, #0x4000000
    STR R3, [ R3, #0x208 ]
    // ISR ���� ����
	MRS R3, CPSR
    BIC R3, R3, #0xDF
    ORR R3, R3, #0x92
    MSR CPSR, R3
portRESTORE_CONTEXT:
	LDR		R0, =pxCurrentTCB
	LDR		R0, [R0]
	LDR		r0, [R0]
	// ulCriticalNesting �о ����
	LDR		R1, =ulCriticalNesting
	LDMFD	r0!, {R2}
	STR		R2, [R1]
	LDMFD	r0!, {R2}
	MSR	SPSR, R2
	// PC ���� �о 4�� ���� �� Lr�� ����
	LDR		r1, [R0, #60]
	sub		lr, r1, #4
	// ���ͷ�Ʈ ����
	MOV R3, #0x4000001
    STR R3, [ R3, #0x207 ]
    // lr, pc�� ������ �������� ���
	LDMFD	r0, {R0-R14}^
	// pc�� ����
	movs	pc, lr

	.extern vTaskIncrementTick
	.extern vTaskSwitchContext
vPreemptiveTick:
    MOV R3, #0x4000000
    STR R3, [ R3, #0x208 ]
    // �½�ũ�� SPSR�� LR�� ���ÿ��� ��
    pop {r2, lr}
    // ���ؽ�Ʈ�� ������ ��ġ�� �� ��� �̵�
    SUB R0, SP, #72
    // System ��� r4-r14 �������͸� �����Ѵ�.
    ADD R3, R0, #24
    STMIA R3, { R4-R14 }^
    // ���ͷ�Ʈ ����� ���(ISR)�� ��ȯ
    MSR CPSR, R2
	// {spsr, IME, REG_BASE lr_irq} ������ SPSR ����
    LDMFD SP!, { R5-R6, R7, R8 }
    STR R5, [ R0, #4 ]
    // {r0-r3, r12, lr} ������ ����
    LDMFD SP!, { R5-R8, R12, LR }
    ADD	  r3, r0, #8
    STMIA R3, { R5-R8 }
    STR	  r12, [ r0, #56 ]
    // LR��  �½�ũ�� PC�� ����
    STR   LR, [ R0, #68 ]
	// ulCriticalNesting ����
    LDR R5, =ulCriticalNesting
    LDR R5, [R5]
    STR R5, [R0]
	// ���ؽ�Ʈ ������ġ �� ���� ��ġ �ּҸ�  pxCurrentTCB�� ����
    LDR R5, =pxCurrentTCB
    LDR R5, [R5]
    STR R0, [R5]

	BL vTaskIncrementTick
	BL vTaskSwitchContext

	// ���ؽ�Ʈ ���� ����
	B portRESTORE_CONTEXT
