/* $Id: c_wrapper.h,v 1.1 2004/08/14 14:54:39 fukasawa Exp $ */

/*============================================================================*/
/**
 *  @file    c_wrapper.h
 *
 *  @author  Fukasawa Mitsuo
 *
 *
 *    Copyright (C) 2001-2004 BEE Co.,Ltd. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */
/*============================================================================*/

#ifndef C_WRAPPER_H
#define C_WRAPPER_H

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*fini_handler)(void);

int  ace_init(int argc, char * argv[]);
int  ace_run_loop();
void ace_fini();
void ace_error_log(const char * fmt, ...);
void ace_debug_log(const char * fmt, ...);
void ace_hexdump(const char * top, size_t size);
void ace_disable_debug_log();
void ace_enable_debug_log();
void ace_fini_handler(fini_handler fh);


#ifdef __cplusplus
}
#endif

#endif /* C_WRAPPER_H */
