#ifndef __RDWR_H__
#define __RDWR_H__

#include "cyu3system.h"
#include "handlers.h"

extern io_handler_t io_handlers[];

typedef struct {
  rdwr_data_header_t header;   // current command
  io_handler_t *handler;       // current io_handler
  uint16_t ep_buffer_size;     // usb end point buffer size
  uint8_t done;                // has this command been handled?
  uint32_t transfered_so_far;  // used by handler to know how much data it has transfered so far
} rdwr_cmd_t;

/**
 * To start a rdwr_tranaction, gRdwrCmd must be initialized
 * This function allows customization of how that happens.
 **/
typedef CyU3PReturnStatus_t (*rdwr_setup_handler)();

/**
 * term_hint = a hint as to which term the setup handler will set.
 * length_hint a hint as to the transfer length... length&0xffff is fine. 
 *   (This is so slfifo can setup with the right auto or manual mode )
 *
 * The reason to use hints and wait until the other handler is initialized
 * is that sending the ack before the new handler starts allows the 
 * driver to start sending data before the handler is completely initialized.
 * The most common problem associated with this is a lost packet or two of data
 * for a write transaction. (Because the handlers flush the ep buffers.)
 *
 * NOTE apple support in firmware uses new endpoint instead of ep0.  Perhaps
 * A good reason to switch NITRO to a new interface and do away with ep0 
 * vendor command.
 **/
CyU3PReturnStatus_t start_rdwr(uint16_t term_hint, uint16_t length_hint, rdwr_setup_handler); // start a transaction internally

CyBool_t handle_vendor_cmd(uint8_t  bRequest, uint8_t bReqType,
			   uint8_t  bType, uint8_t bTarget,
			   uint16_t wValue, uint16_t wIndex, 
			   uint16_t wLength);

void rdwr_teardown();


#endif
