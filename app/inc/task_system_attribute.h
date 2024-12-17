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
 * @file   : task_system_attribute.h
 * @date   : Set 26, 2023
 * @author : Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>
 * @version	v1.0.0
 */

#ifndef TASK_INC_TASK_SYSTEM_ATTRIBUTE_H_
#define TASK_INC_TASK_SYSTEM_ATTRIBUTE_H_

/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/

/********************** macros ***********************************************/

/********************** typedef **********************************************/
/* System Statechart - State Transition Table */
/* 	------------------------+-----------------------+-----------------------+-----------------------+------------------------
 * 	| Current               | Event                 |                       | Next                  |                       |
 * 	| State                 | (Parameters)          | [Guard]               | State                 | Actions               |
 * 	|=======================+=======================+=======================+=======================+=======================|
 * 	| ST_GATE_OPEN          | EV_SYS_BTN_A_ACTIVE   |                       | ST_GATE_CLOSING       | - Turn on LED_D       |
 * 	|                       |                       |                       |                       | - BLINK BUZ_A         |
 * 	|                       |                       |                       |                       | - Turn on LED_A       |
 * 	|-----------------------+-----------------------+-----------------------+-----------------------+-----------------------|
 * 	| ST_GATE_CLOSING       | EV_SYS_BTN_A_ACTIVE   |                       | ST_GATE_STOPED        | - Turn on LED_D       |
 *  |                       |                       |                       |                       | - BLINK BUZ_A         |
 * 	|                       |                       |                       |                       | - Turn off LED_A      |
 * 	|                       +-----------------------+-----------------------+-----------------------+-----------------------|
 * 	|                       | EV_SYS_SWC_C_ACTIVE   |                       | ST_GATE_STOPED        | - Turn on LED_D       |
 * 	|                       |                       |                       |                       | - BLINK BUZ_A         |
 * 	|                       |                       |                       |                       | - Turn off LED_A      |
 * 	|                       +-----------------------+-----------------------+-----------------------+-----------------------|
 * 	|                       | EV_SYS_SWC_A_ACTIVE   |                       | ST_GATE_CLOSED        | - Turn off LED_D      |
 * 	|                       |                       |                       |                       | - Turn off BUZ_A      |
 * 	|                       |                       |                       |                       | - Turn off LED_A      |
 * 	|                       |                       |                       |                       | - Turn on LED_C       |
 * 	|-----------------------+-----------------------+-----------------------+-----------------------+-----------------------|
 * 	| ST_GATE_CLOSED        | EV_SYS_BTN_A_ACTIVE   |                       | ST_GATE_OPENING       | - Turn on LED_D       |
 * 	|                       |                       |                       |                       | - BLINK BUZ_A         |
 * 	|                       |                       |                       |                       | - Turn on LED_B       |
 * 	|-----------------------+-----------------------+-----------------------+-----------------------+-----------------------|
 * 	| ST_GATE_OPENING       | EV_SYS_BTN_A_ACTIVE   |                       | ST_GATE_STOPED        | - Turn off LED_B      |
 *  |                       |                       |                       |                       | - BLINK BUZ_A         |
 * 	|                       |                       |                       |                       | - Turn on LED_D       |
 * 	|                       +-----------------------+-----------------------+-----------------------+-----------------------|
 * 	|                       | EV_SYS_SWC_B_ACTIVE   |                       | ST_GATE_OPENED        | - Turn off LED_B      |
 * 	|                       |                       |                       |                       | - Turn off BUZ_A      |
 * 	|                       |                       |                       |                       | - Turn off LED_A      |
 * 	|-----------------------+-----------------------+-----------------------+-----------------------+-----------------------|
 * 	| ST_GATE_STOPED        | EV_SYS_BTN_A_ACTIVE   | previous_state ==     | ST_GATE_CLOSING       | - Turn on LED_A       |
 *  |                       |                       | ST_GATE_OPENING       |                       | - BLINK BUZ_A         |
 * 	|                       |                       |                       |                       | - Turn on LED_D       |
 * 	|                       |                       |-----------------------+-----------------------+-----------------------|
 * 	|                       |                       | previous_state ==     | ST_GATE_OPENING       | - Turn on LED_B       |
 * 	|                       |                       | ST_GATE_CLOSING       |                       | - Turn on BUZ_A       |
 * 	|                       |                       |                       |                       | - Turn on LED_A       |
 * 	|-----------------------+-----------------------+-----------------------+-----------------------+-----------------------|
 *
 */

/* Events to excite Task System */
typedef enum task_system_ev {EV_SYS_BTN_A_IDLE,							// Control del portón sin presionar
						 	 EV_SYS_BTN_A_ACTIVE,						// Control del portón presionado (cambiar de estado)
							 EV_SYS_SWC_A_IDLE,							// Portón no está completamente cerrado.
							 EV_SYS_SWC_A_ACTIVE,						// Portón está completamente cerrado.
							 EV_SYS_SWC_B_IDLE,							// Portón no está completamente abierto.
							 EV_SYS_SWC_B_ACTIVE,						// Portón está completamente abierto.
							 EV_SYS_SWC_C_IDLE,							// No se detecta ningún objeto en el portón.
							 EV_SYS_SWC_C_ACTIVE} task_system_ev_t;		// Se detecta un objeto en el portón.

/* State of Task System */
typedef enum task_system_st {ST_GATE_OPEN,
							 ST_GATE_OPENING,
							 ST_GATE_CLOSING,
							 ST_GATE_CLOSED,
							 ST_GATE_STOPED} task_system_st_t;

typedef struct
{
	uint32_t			tick;
	task_system_st_t	state;
	task_system_ev_t	event;
	bool				flag;
} task_system_dta_t;

/********************** external data declaration ****************************/
extern task_system_dta_t task_system_dta;

/********************** external functions declaration ***********************/

/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* TASK_INC_TASK_SYSTEM_ATTRIBUTE_H_ */

/********************** end of file ******************************************/
