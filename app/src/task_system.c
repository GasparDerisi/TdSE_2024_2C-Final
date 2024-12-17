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
 *
 * @file   : task_system.c
 * @date   : Set 26, 2023
 * @author : Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>
 * @version	v1.0.0
 */

/********************** inclusions *******************************************/
/* Project includes. */
#include "main.h"

/* Demo includes. */
#include "logger.h"
#include "dwt.h"

/* Application & Tasks includes. */
#include "board.h"
#include "app.h"
#include "task_system_attribute.h"
#include "task_system_interface.h"
#include "task_actuator_attribute.h"
#include "task_actuator_interface.h"

/********************** macros and definitions *******************************/
#define G_TASK_SYS_CNT_INI			0ul
#define G_TASK_SYS_TICK_CNT_INI		0ul

#define DEL_SYS_XX_MIN				0ul
#define DEL_SYS_XX_MED				50ul
#define DEL_SYS_XX_MAX				500ul

/********************** internal data declaration ****************************/
task_system_dta_t task_system_dta = {DEL_SYS_XX_MIN, ST_GATE_CLOSED, EV_SYS_BTN_A_IDLE, false};

task_system_st_t previous_state = ST_GATE_CLOSED;

#define SYSTEM_DTA_QTY	(sizeof(task_system_dta)/sizeof(task_system_dta_t))

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/
const char *p_task_system 		= "Task System (System Statechart)";
const char *p_task_system_ 		= "Non-Blocking & Update By Time Code";

/********************** external data declaration ****************************/
uint32_t g_task_system_cnt;
volatile uint32_t g_task_system_tick_cnt;

/********************** external functions definition ************************/
void task_system_init(void *parameters)
{
	task_system_dta_t 	*p_task_system_dta;
	task_system_st_t	state;
	task_system_ev_t	event;
	bool b_event;

	/* Print out: Task Initialized */
	LOGGER_LOG("  %s is running - %s\r\n", GET_NAME(task_system_init), p_task_system);
	LOGGER_LOG("  %s is a %s\r\n", GET_NAME(task_system), p_task_system_);

	g_task_system_cnt = G_TASK_SYS_CNT_INI;

	/* Print out: Task execution counter */
	LOGGER_LOG("   %s = %lu\r\n", GET_NAME(g_task_system_cnt), g_task_system_cnt);

	init_queue_event_task_system();

	/* Update Task Actuator Configuration & Data Pointer */
	p_task_system_dta = &task_system_dta;

	/* Print out: Task execution FSM */
	state = p_task_system_dta->state;
	LOGGER_LOG("   %s = %lu", GET_NAME(state), (uint32_t)state);

	event = p_task_system_dta->event;
	LOGGER_LOG("   %s = %lu", GET_NAME(event), (uint32_t)event);

	b_event = p_task_system_dta->flag;
	LOGGER_LOG("   %s = %s\r\n", GET_NAME(b_event), (b_event ? "true" : "false"));

	g_task_system_tick_cnt = G_TASK_SYS_TICK_CNT_INI;
}

void task_system_update(void *parameters)
{
	task_system_dta_t *p_task_system_dta;
	bool b_time_update_required = false;

	/* Update Task System Counter */
	g_task_system_cnt++;

	/* Protect shared resource (g_task_system_tick) */
	__asm("CPSID i");	/* disable interrupts*/
    if (G_TASK_SYS_TICK_CNT_INI < g_task_system_tick_cnt)
    {
    	g_task_system_tick_cnt--;
    	b_time_update_required = true;
    }
    __asm("CPSIE i");	/* enable interrupts*/

	while (b_time_update_required)
	{
		/* Protect shared resource (g_task_system_tick) */
		__asm("CPSID i");	/* disable interrupts*/
		if (G_TASK_SYS_TICK_CNT_INI < g_task_system_tick_cnt)
		{
			g_task_system_tick_cnt--;
			b_time_update_required = true;
		}
		else
		{
			b_time_update_required = false;
		}
		__asm("CPSIE i");	/* enable interrupts*/

		/* Update Task System Data Pointer */
		p_task_system_dta = &task_system_dta;

		if (true == any_event_task_system())
		{
			p_task_system_dta->flag = true;
			p_task_system_dta->event = get_event_task_system();
		}

		previous_state = p_task_system_dta->state;

		switch (p_task_system_dta->state)
		{
			case ST_GATE_OPEN:
				if ((p_task_system_dta->flag == true) && (p_task_system_dta->event == EV_SYS_BTN_A_ACTIVE)){ //Si aprietan el botón de control.
					p_task_system_dta->flag = false;
					put_event_task_actuator(EV_ACTUATOR_XX_BLINK, ID_LED_A); 	// Mueve el motor para cerrar el portón.
					put_event_task_actuator(EV_ACTUATOR_XX_ON, ID_LED_D); 		// Avisa que el portón no está completamente cerrado (LED)
					put_event_task_actuator(EV_ACTUATOR_XX_BLINK, ID_BUZ_A); 	// Avisa que el portón no está completamente cerrado (Chicharra)
					p_task_system_dta->state = ST_GATE_CLOSING;
				}
				break;

			case ST_GATE_CLOSING:

				if ((p_task_system_dta->flag == true) && (p_task_system_dta->event == EV_SYS_SWC_C_ACTIVE)){ // Si se detecta un objeto en el portón
					p_task_system_dta->flag = false;
					put_event_task_actuator(EV_ACTUATOR_XX_OFF, ID_LED_A); 		// Detiene el motor del portón
					p_task_system_dta->state = ST_GATE_STOPED;
				}

				else if ((p_task_system_dta->flag == true) && (p_task_system_dta->event == EV_SYS_BTN_A_ACTIVE)){ // Si se presiona nuevamente el control, se detiene el cierre del portón.
					p_task_system_dta->flag = false;
					put_event_task_actuator(EV_ACTUATOR_XX_OFF, ID_LED_A); 		// Detiene el motor del portón
					p_task_system_dta->state = ST_GATE_STOPED;
				}

				else if ((p_task_system_dta->flag == true) && (p_task_system_dta->event == EV_SYS_SWC_A_ACTIVE)){ // Si se detecta que el portón se terminó de cerrar, se detiene.
					p_task_system_dta->flag = false;
					put_event_task_actuator(EV_ACTUATOR_XX_OFF, ID_LED_A); 		// Detiene el portón
					put_event_task_actuator(EV_ACTUATOR_XX_ON, ID_LED_C);		// Indica que el portón está completamente cerrado.
					put_event_task_actuator(EV_ACTUATOR_XX_OFF, ID_LED_D); 		// Deja de avisar que el portón no está completamente cerrado (LED)
					put_event_task_actuator(EV_ACTUATOR_XX_OFF, ID_BUZ_A); 		// Deja de avisar que el portón no está completamente cerrado (Chicharra)
					p_task_system_dta->state = ST_GATE_CLOSED;
				}
				break;

			case ST_GATE_CLOSED:

				if ((p_task_system_dta->flag == true) && (p_task_system_dta->event == EV_SYS_BTN_A_ACTIVE)){ // Si se presiona el botón de control.
					p_task_system_dta->flag = false;
					put_event_task_actuator(EV_ACTUATOR_XX_BLINK, ID_LED_B);	// Activa el motor de apertura del portón.
					put_event_task_actuator(EV_ACTUATOR_XX_ON, ID_LED_D); 		// Avisa que el portón no está completamente cerrado (LED)
					put_event_task_actuator(EV_ACTUATOR_XX_BLINK, ID_BUZ_A); 	// Avisa que el portón no está completamente cerrado (Chicharra)
					p_task_system_dta->state = ST_GATE_OPENING;
				}
				break;

			case ST_GATE_OPENING:

				if ((p_task_system_dta->flag == true) && (p_task_system_dta->event == EV_SYS_BTN_A_ACTIVE)){ // Si se presiona nuevamente el control, se detiene la apertura del portón.
					p_task_system_dta->flag = false;
					put_event_task_actuator(EV_ACTUATOR_XX_OFF, ID_LED_B); 		// Detiene el motor del portón
					p_task_system_dta->state = ST_GATE_STOPED;
				}

				else if ((p_task_system_dta->flag == true) && (p_task_system_dta->event == EV_SYS_SWC_B_ACTIVE)){ // Si se detecta que el portón se terminó de abrir, se detiene.
					p_task_system_dta->flag = false;
					put_event_task_actuator(EV_ACTUATOR_XX_OFF, ID_LED_B); 		// Detiene el motor del portón
					put_event_task_actuator(EV_ACTUATOR_XX_OFF, ID_LED_D); 		// Deja de avisa que el portón no está completamente cerrado (LED)
					put_event_task_actuator(EV_ACTUATOR_XX_OFF, ID_BUZ_A); 		// Deja de avisa que el portón no está completamente cerrado (Chicharra)
					p_task_system_dta->state = ST_GATE_CLOSED;
				}
				break;

			case ST_GATE_STOPED:

				if ((p_task_system_dta->flag == true) && (p_task_system_dta->event == EV_SYS_BTN_A_ACTIVE) && (previous_state == ST_GATE_CLOSING)){ // Si antes de pararse, el portón se estaba cerrando y se presiona el botón de control, comienza a abrirse
					p_task_system_dta->flag = false;
					put_event_task_actuator(EV_ACTUATOR_XX_BLINK, ID_LED_B); 	// Activa el motor de apertura del portón.
					p_task_system_dta->state = ST_GATE_OPENING;
				}

				else if ((p_task_system_dta->flag == true) && (p_task_system_dta->event == EV_SYS_BTN_A_ACTIVE) && (previous_state == ST_GATE_OPENING)){ // Si antes de pararse, el portón se estaba abriendo y se presiona el botón de control, comienza a cerrarse
					p_task_system_dta->flag = false;
					put_event_task_actuator(EV_ACTUATOR_XX_BLINK, ID_LED_A); 	// Activa el motor de apertura del portón.
					p_task_system_dta->state = ST_GATE_CLOSING;
				}
				break;

			default:

				break;
		}
	}
}

/********************** end of file ******************************************/
