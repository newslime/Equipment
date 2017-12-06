// $Id: BS2TTY_IO.h,v 1.6 2004/06/20 15:23:40 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2TTY_IO.h
 *
 *  @author Fukasawa Mitsuo
 *
 *
 *    Copyright (C) 1998-2004 BEE Co.,Ltd. All rights reserved.
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
//=============================================================================

#ifndef BS2TTY_IO_H
#define BS2TTY_IO_H

#include "beesecs.h"
#include "ace/TTY_IO.h"

/**
 *  @class   BS2TTY_IO
 *  @brief   Class definitions for platform specific TTY features.
 *  @note
 *  This class represents an example interface for a specific
 * device (a serial line) It extends the capability of the
 * underlying DEV_IO class by adding a control method that takes
 * a special structure (Serial_Params) as argument to allow a
 * comfortable user interface (away from that annoying termios
 * structure, which is very specific to UNIX).
 *
 */
class BEE_Export BS2TTY_IO : public ACE_TTY_IO
{
public:
    BS2TTY_IO() {}
    ~BS2TTY_IO() {}
  // Interface for reading/writing serial device parameters.
  int control (Control_Mode cmd, Serial_Params * arg) const;

};

#endif /* BS2TTY_IO_H */
