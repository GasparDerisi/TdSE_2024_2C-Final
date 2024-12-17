/*
 * Copyright (c) 2023 Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * @file   : task_actuator_attribute.h
 * @date   : Set 26, 2023
 * @author : Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>
 * @version	v1.0.0
 */

#ifndef TASK_INC_TASK_ACTUATOR_ATTRIBUTESH_
#define TASK_INC_TASK_ACTUATOR_ATTRIBUTE_H_

/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/

/********************** macros ***********************************************/

/********************** typedef **********************************************/
/* Actuator Statechart - State Transition Table */
/* 	------------------------+-----------------------+-----------------------+--------------------------+------------------------
 * 	| Current               | Event                 |                       | Next                     |                       |
 * 	| State                 | (Parameters)          | [Guard]               | State                    | Actions               |
 * 	|=======================+=======================+=======================+==========================+=======================|
 * 	| ST_ACTUATOR_XX_OFF    | EV_ACTUATOR_XX_OFF    |                       | ST_ACTUATOR_XX_OFF       |                       |
 * 	|                       |-----------------------+-----------------------+--------------------------+-----------------------|
 * 	|                       | EV_ACTUATOR_XX_ON     |                       | ST_ACTUATOR_XX_ON	       | actuator = ON         |
 * 	|                       |-----------------------+-----------------------+--------------------------+-----------------------|
 * 	|                       | EV_ACTUATOR_XX_BLINK  |                       | ST_ACTUATOR_XX_BLINK_ON  | tick = tick_max       |
 * 	|                       |                       |                       |                          | actuator = ON         |
 * 	|                       |-----------------------+-----------------------+--------------------------+-----------------------|
 * 	|                       | EV_ACTUATOR_XX_PULSE  |                       | ST_ACTUATOR_XX_PULSE     | tick = tick_max       |
 * 	|                       |                       |                       |                          | actuator = ON		   |
 * 	|-----------------------+-----------------------+-----------------------+--------------------------+-----------------------|
 * 	| ST_ACTUATOR_XX_ON     | EV_ACTUATOR_XX_OFF    |                       | ST_ACTUATOR_XX_OFF	   | actuator = OFF        |
 * 	|                       +-----------------------+-----------------------+--------------------------+-----------------------|
 * 	|                       | EV_ACTUATOR_XX_ON     |                       | ST_ACTUATOR_XX_ON        |                       |
 * 	|-----------------------+-----------------------+-----------------------+--------------------------+-----------------------|
 * 	| ST_ACTUATOR_XX_       | EV_ACTUATOR_XX_OFF    |                       | ST_ACTUATOR_XX_OFF       | actuator = OFF        |
 * 	| BLINK_ON              +-----------------------+-----------------------+--------------------------+-----------------------|
 * 	|                       | EV_ACTUATOR_XX_ON     |                       | ST_ACTUATOR_XX_ON		   | actuator = ON         |
 * 	|                       |-----------------------+-----------------------+--------------------------+-----------------------|
 * 	|                       | EV_ACTUATOR_XX_NOT_BLINK|                     | ST_ACTUATOR_XX_OFF       | actuator = OFF        |
 * 	|                       +-----------------------+-----------------------+--------------------------+-----------------------|
 * 	|                       | EV_ACTUATOR_XX_BLINK  |                       | ST_ACTUATOR_XX_BLINK_ON  |                       |
 * 	|                       +-----------------------+-----------------------+--------------------------+-----------------------|
 * 	|                       |                       | [tick >  0]           | ST_ACTUATOR_XX_BLINK_ON  | tick--                |
 * 	|                       +-----------------------+-----------------------+--------------------------+-----------------------|
 * 	|                       |                       | [tick == 0]           | ST_ACTUATOR_XX_BLINK_OFF | tick = tick_max       |
 * 	|                       |                       |                       |                          | actuator = OFF        |
 * 	|-----------------------+-----------------------+-----------------------+--------------------------+-----------------------|
 * 	| ST_ACTUATOR_XX_       | EV_ACTUATOR_XX_OFF    |                       | ST_ACTUATOR_XX_OFF       | actuator = OFF        |
 * 	| BLINK_OFF             +-----------------------+-----------------------+--------------------------+-----------------------|
 * 	|                       | EV_ACTUATOR_XX_ON     |                       | ST_ACTUATOR_XX_ON		   | actuator = ON         |
 * 	|                       |-----------------------+-----------------------+--------------------------+-----------------------|
 * 	|                       | EV_ACTUATOR_XX_NOT_BLINK|                     | ST_ACTUATOR_XX_OFF       | actuator = OFF        |
 * 	|                       +-----------------------+-----------------------+--------------------------+-----------------------|
 * 	|                       | EV_ACTUATOR_XX_BLINK  |                       | ST_ACTUATOR_XX_BLINK_OFF |                       |
 * 	|                       +-----------------------+-----------------------+--------------------------+-----------------------|
 * 	|                       |                       | [tick >  0]           | ST_ACTUATOR_XX_BLINK_OFF | tick--                |
 * 	|                       +-----------------------+-----------------------+--------------------------+-----------------------|
 * 	|                       |                       | [tick == 0]           | ST_ACTUATOR_XX_BLINK_ON  | tick = tick_max       |
 * 	|                       |                       |                       |                          | actuator = ON         |
 * 	|-----------------------+-----------------------+-----------------------+--------------------------+-----------------------|
 * 	| ST_ACTUATOR_XX_PULSE  | EV_ACTUATOR_XX_OFF    |                       | ST_ACTUATOR_XX_OFF       | actuator = OFF        |
 * 	|                       +-----------------------+-----------------------+--------------------------+-----------------------|
 * 	|                       | EV_ACTUATOR_XX_ON     |                       | ST_ACTUATOR_XX_ON        | actuator = ON         |
 * 	|                       |-----------------------+-----------------------+--------------------------+-----------------------|
 * 	|                       | EV_ACTUATOR_XX_PULSE  | [tick >  0]           | ST_ACTUATOR_XX_PULSE     | tick--                |
 * 	|                       +-----------------------+-----------------------+--------------------------+-----------------------|
 * 	|                       | EV_ACTUATOR_XX_PULSE  | [tick == 0]           | ST_ACTUATOR_XX_OFF       | tick = tick_max       |
 * 	|                       |                       |                       |                          | actuator = OFF        |
 * 	------------------------+-----------------------+-----------------------+--------------------------+------------------------
 */

/* Events to excite Task Actuator */
typedef enum task_actuator_ev {EV_ACTUATOR_XX_OFF,
							   EV_ACTUATOR_XX_ON,
							   EV_ACTUATOR_XX_NOT_BLINK,
							   EV_ACTUATOR_XX_BLINK,
							   EV_ACTUATOR_XX_PULSE} task_actuator_ev_t;

/* States of Task Actuator */
typedef enum task_actuator_st {ST_ACTUATOR_XX_OFF,
							   ST_ACTUATOR_XX_ON,
							   ST_ACTUATOR_XX_BLINK_ON,
							   ST_ACTUATOR_XX_BLINK_OFF,
							   ST_ACTUATOR_XX_PULSE} task_actuator_st_t;

/* Identifier of Task Actuator */
typedef enum task_actuator_id {ID_LED_A, 						// Indica si el portón se está cerrando
							   ID_LED_B,						// Indica si el portón se está abriendo
							   ID_LED_C,						// Indica que el portón está completamente cerrado
							   ID_LED_D,						// Indica que el portón no está completamente cerrado
							   ID_BUZ_A} task_actuator_id_t;	// Indica que el portón no está completamente cerrado

typedef struct
{
	task_actuator_id_t	identifier;
	GPIO_TypeDef *		gpio_port;
	uint16_t			pin;
	GPIO_PinState		actuator_on;
	GPIO_PinState		actuator_off;
	uint32_t			tick_blink;
	uint32_t			tick_pulse;
} task_actuator_cfg_t;

typedef struct
{
	uint32_t			tick;
	task_actuator_st_t	state;
	task_actuator_ev_t	event;
	bool				flag;
} task_actuator_dta_t;

/********************** external data declaration ****************************/
extern task_actuator_dta_t task_actuator_dta_list[];

/********************** external functions declaration ***********************/

/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* TASK_INC_TASK_ACTUATOR_ATTRIBUTE_H_ */

/********************** end of file ******************************************/
