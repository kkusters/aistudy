/*
 * CANopen Library V 4.3 
 * CANopen Design Tool 2.1.13
 *
 * Automatically generated C config: don't edit
 * 09-22-2008 12:02PM
 */

#ifndef __CAL_CONF_H
#define __CAL_CONF_H
#define CONFIG_DESIGNTOOL_VERSION 0x0201

/* active configuration : 0 */
#define CONFIG_USE_TARGET_0 1


/*
 * General Settings
 */
#define CONFIG_SLAVE                   	1
#define CONFIG_CAN_ERROR_HANDLING      	1
#define CONFIG_NON_VOLATILE_MEM        	1
#define CONFIG_SLAVE_PLUS              	1

/*
 * Hardware configuration 0: 0
 */


/*
 * Code Maturity Level Options
 */


/*
 * CPU Setup
 */
#ifdef CONFIG_USE_TARGET_0
#define CONFIG_SIZE_POOL                    	8182 // 8184 //8192
#define CONFIG_CPU_FAMILY_XC166             	1
#define CONFIG_COLIB_MALLOC                 	1
#define CONFIG_COLIB_TIMER                  	1
#define CONFIG_TIMER_ISR_REGISTERBANK       	using t4regs
#define CONFIG_TIMER_ISR_NUMBER             	interrupt 0x24
#define CONFIG_TIMER_INC                    	250
#define CONFIG_CPU_TYPE_XC164               	1

# ifdef DEF_HW_PART
#  include <cpu_166.h>
# endif /* DEF_HW_PART */

#endif /* CONFIG_USE_TARGET_0 */


/*
 * CAN Controller Setup
 */
#ifdef CONFIG_USE_TARGET_0
#define CONFIG_CAN_FAMILY_TWINCAN           	1
#define CONFIG_COLIB_BUFFER                 	1
#define CONFIG_COLIB_FLUSHMBOX              	1
#define CONFIG_RX_BUFFER_SIZE               	100
#define CONFIG_TX_BUFFER_SIZE               	100
#define CONFIG_CAN_START_TYPE               	1
#define CONFIG_CAN_ACCESS_MEM_MAP           	1
#define CONFIG_FULLCAN                       	1
#define CONFIG_CAN_FULLCAN_SOFT_RTR         	1
#define CONFIG_CAN_ISR_NUMBER               	interrupt 0x54
#define CONFIG_CAN_ISR_REGISTERBANK         	using canregs
#define CONFIG_CAN_REGISTER_OFFSET          	1
#define CONFIG_CAN_TYPE_XC164               	1
#define CONFIG_CAN_T_CLK                    	40
#define CONFIG_CAN_USE_MEMCPY               	1
#define CONFIG_ONLY_ONE_TRANSMIT_CHANNEL    	1
#define CONFIG_STANDARD_IDENTIFIER          	1
#define CONFIG_CAN_ADDR                     	{(void *) 0x200200}
#endif /* CONFIG_USE_TARGET_0 */


/*
 * Compiler Setup
 */
#ifdef CONFIG_USE_TARGET_0
#define CONFIG_ALIGNMENT                    	2
#define CONFIG_COMPILER_TASKING_C166        	1
#  include <co_tasking.h>
#endif /* CONFIG_USE_TARGET_0 */




/*
 * CANopen Services
 */
#define CONFIG_HEARTBEAT_PRODUCER      	1
#define CONFIG_SDO_SERVER              	1
#define CONFIG_SEG_SDO                 	1
#define CONFIG_PDO_EVENTTIMER          	1
#define CONFIG_PDO_CONSUMER            	9
#define CONFIG_DYN_PDO_MAPPING         	1
#define CONFIG_EMCY_PRODUCER           	1
#define CONFIG_HEARTBEAT_CONSUMER      	1
#define CONFIG_PDO_PRODUCER            	11
#define CONFIG_MAX_DYN_MAP_ENTRIES     	8
#define CONFIG_MAPPING_CNT             	92
#define CONFIG_SYNC_CONSUMER           	1
/*
 * Additional CANopen Services
 */

/*
 * Optimization for CANopen 
 */
#define CONFIG_CONST_OBJDIR            	1
/* 
 * user specific part - will not be changed
 */
#define CONFIG_USER_SPEC_START

#define CONFIG_USER_SPEC_END
#endif /* __CAL_CONF_H */
