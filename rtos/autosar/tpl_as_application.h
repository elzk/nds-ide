/**
 *  @file tpl_as_application.h
 *  
 *  @section desc File description
 *
 *  Header of Autosar OS Application API
 *
 *  @section copyright Copyright
 *
 *  Trampoline OS
 *
 *  Trampoline is copyright (c) IRCCyN 2005-2007
 *  Autosar extension is copyright (c) IRCCyN and ESEO 2007
 *  Trampoline and its Autosar extension are protected by the
 *  French intellectual property law.
 *
 *  This software is distributed under a dual licencing scheme
 *  1 - The Lesser GNU Public Licence v2
 *  2 - The BSD Licence
 *
 *  @section infos File informations
 *
 *  $Date$
 *  $Rev$
 *  $Author$
 *  $URL$
 */

#ifndef TPL_AS_APPLICATION_H
#define TPL_AS_APPLICATION_H

#include "tpl_os_custom_types.h"
#include "tpl_os_types.h"

/**
 *  @typedef  ObjectTypeType
 *
 *  type of object. It can be one of the following
 *
 *  @see  #OBJECT_TASK
 *  @see  #OBJECT_ISR
 *  @see  #OBJECT_ALARM
 *  @see  #OBJECT_RESOURCE
 *  @see  #OBJECT_COUNTER
 *  @see  #OBJECT_SCHEDULETABLE 
 */
typedef u8  ObjectTypeType;

/**
 *  @typedef  ApplicationType
 *
 *  Id of an OS Application
 */
typedef tpl_app_id  ApplicationType;

/**
 *  @typedef ObjectAccessType
 *
 *  Return type used by CheckObjectAccess. @see #CheckObjectAccess
 */
typedef u8  ObjectAccessType;

/**
 *  @typedef RestartType
 *
 *  Type used to specifies how TerminateApplication should behave
 */
typedef u8  RestartType;
  
/**
 * @def DeclareApplication
 *
 * Defines an application
 *
 * @param app_id application's C identifier
 *
 */
#define DeclareApplication(app_id)  \
	extern CONST(ApplicationType, OS_APPL_CONST) app_id

/**
 *  Get the application ID to which the current process belongs to
 *
 *  @retval   the application ID (OS261) or INVALID_OSAPPLICATION (OS262)
 */
FUNC(ApplicationType, OS_CODE) GetApplicationID(void);

/**
 *  Get the application ID to which the object belongs to
 *
 *  @param    obj_type  the type of object. @see #ObjectTypeType
 *  @param    obj_id    the id of the object. @see #tpl_generic_id
 *
 *  @retval   the application ID (OS273) or INVALID_OSAPPLICATION (OS274, OS319)
 */
FUNC(ApplicationType, OS_CODE) CheckObjectOwnership(
  ObjectTypeType  obj_type,
  tpl_generic_id  obj_id);

/**
 *  Check an Application may access an object
 *
 *  @param    app_id    the id of the application
 *  @param    obj_type  the type of object. @see #ObjectTypeType
 *  @param    obj_id    the id of the object. @see #tpl_generic_id
 *
 *  @retval   the application ID (OS273) or INVALID_OSAPPLICATION (OS274, OS319)
 */
FUNC(ObjectAccessType, OS_CODE) CheckObjectAccess(
  ApplicationType app_id, 
  ObjectTypeType  obj_type,
  tpl_generic_id  obj_id);

/**
 *  Terminate an OS Application. All running processes/alarms/schedule tables
 *  are killed
 *
 *  @param    restart_opt   indicates if the OS Application should be restarted.
 *                          @see #RestartType
 *
 *  @retval   E_OK          called from allowed context (OS287).
 *  @retval   E_OS_CALLEVEL wrong context (OS288)
 *  @retval   E_OS_VALUE    invalid restart_opt (OS459)
 */
FUNC(StatusType, OS_CODE) TerminateApplication(RestartType restart_opt);

/*  TPL_AS_APPLICATION_H  */
#endif
/*  End of file tpl_as_application.h  */
