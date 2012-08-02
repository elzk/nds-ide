#include <stdio.h>

#include "tpl_os.h"
#include "tpl_machine.h"

#include "vp_ipc_devices.h"

#include <unistd.h>

int motor_speed = 20;

int main(void)
{
    StartOS(OSDEFAULTAPPMODE);
    return 0;
}


void PreTaskHook(void)
{
    TaskType id;
    GetTaskID(&id);
    printf("[TPL] Before %d\n",id);
}

void PostTaskHook(void)
{
    TaskType id;
    GetTaskID(&id);
    printf("[TPL] After %d\n",id);
}

void StartupHook(void)
{
    printf("[TPL] Startup !\n");
}

void ShutdownHook(StatusType error)
{
    printf("[TPL] Shutdown\n");
}

DeclareTask(init);
DeclareTask(motor);
DeclareTask(startMotor);

TASK(init)
{
    ActivateTask(startMotor);
    TerminateTask();
}

TASK(startMotor)
{
    printf("[TPL](MOTOR) Wanted speed : %d\n", motor_speed);

    vp_ipc_write_reg(&viper, MOTOR0_MOTOR0_CONTROL, (reg_t)motor_speed);
    vp_ipc_signal_update(&viper, &global_shared_memory, MOTOR0, MOTOR0_CONTROL);
    
    TerminateTask();
}

TASK(motor)
{
    int real_speed = (int)vp_ipc_read_reg(&viper, MOTOR0_MOTOR0_SENSOR);

    printf("[TPL](MOTOR) Motor position is : %d\n", real_speed);
    
    vp_ipc_write_reg(&viper, MOTOR0_MOTOR0_CONTROL, (reg_t)motor_speed);
    vp_ipc_signal_update(&viper, &global_shared_memory, MOTOR0, MOTOR0_CONTROL);

    TerminateTask();
}
