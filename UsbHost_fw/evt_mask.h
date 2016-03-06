/*
 * evt_mask.h
 *
 *  Created on: Apr 12, 2013
 *      Author: g.kruglov
 */

#pragma once

// Event masks
#define EVT_UART_NEW_CMD     EVENT_MASK(1)

#define EVT_RADIO_NEW_CMD    EVENT_MASK(2)
#define EVT_RADIO_OK         EVENT_MASK(3)
#define EVT_RADIO_TIMEOUT    EVENT_MASK(4)

#define EVT_USB_NEW_CMD      EVENT_MASK(9)
#define EVT_USB_READY        EVENT_MASK(10)
#define EVT_USB_SUSPEND      EVENT_MASK(11)
