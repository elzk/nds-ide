/**
 * Fabien JUIF
 */
#ifndef __CONSTS_H__
#define __CONSTS_H__

#include <stdint.h> /* int64_t */

/* Sizes */
#define FIFO_SIZE	 20  /* number of cells in fifo */
#define REGISTERS_NB	 100 /* number of shared registers */
#define FILE_PATH_LEN	 64
#define REGISTER_ID_BITS 32 /* number of bits to identify a register */
/*#define DEVICES_ID_BITS	 16 *//* number of bits to identify a device */

/* Mask */
#define DEVICE_MASK   0xFFFF00000000 /* get first sixteen bits */
#define REGISTER_MASK 0x0000FFFFFFFF /* get last sixteen bits */
#define REGISTER_MAX  0x000080000000 /* 1000 0000 0000 0000 */

/* Shared memory */
#define DATA_FILE_PATH "/viper.data.%d"
#define GLOBAL_DATA_FILE_PATH "/viper.global.data.%d"

/* Semaphores */
#define VP_SEM_FILE_PATH	  "/viper.sem.vp.%d" /* Viper handshake */
#define TPL_SEM_FILE_PATH	  "/viper.sem.tpl.%d" /* Trampoline handshake */
#define IT_ID_SEM_FILE_PATH	  "/viper.sem.it_id.%d" /* Access to interruption id area */
#define FIFO_FULL_SEM_FILE_PATH	  "/viper.sem.fifo.full.%d" /* Can't add when fifo is full */
#define FIFO_EMPTY_SEM_FILE_PATH  "/viper.sem.fifo.empty%d" /* We can't read when fifo is empty */
#define GLOBAL_SEM_FILE_PATH  "/viper.sem.global.%d"

/* Semaphore to access registers */
#ifdef READER_WRITER_SEM
  #define WRITER_SEM_FILE_PATH "/viper.sem.writer.%d"
  #define READER_SEM_FILE_PATH "/viper.sem.reader.%d"
  #define MUTEX_SEM_FILE_PATH  "/viper.sem.mutex.%d"
#else
  #define REG_SEM_FILE_PATH    "/viper.sem.reg_write.%d" /* Access to reg array */
#endif

/* Useful for FIFO container */
typedef int64_t reg_id_t;
typedef int64_t mask_t;
typedef int64_t time_tt;
typedef struct st_modified_reg_t
{
    reg_id_t dev;
    mask_t reg_mask; 
    time_tt time;
}modified_reg_t;

#endif
