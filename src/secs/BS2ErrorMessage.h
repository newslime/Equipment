// $Id: BS2ErrorMessage.h,v 1.6 2004/06/20 15:23:40 fukasawa Exp $

//=============================================================================
/**
 *  @file    BS2ErrorMessage.h
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

#ifndef BS2ERRORMESSAGE_H
#define BS2ERRORMESSAGE_H

#include "BS2Message.h"
#include "BS2BlockHeader.h"

/* ------------------------------------------------------------------------- */
class BEE_Export BS2ErrorMessage : public BS2Message
{
public:
    BS2ErrorMessage(int sfnum, BS2BlockHeader& header);
    BS2ErrorMessage(int sfnum, BS2Message * srcmsg);
    BS2ErrorMessage(BS2ErrorMessage const& rhs) : BS2Message(rhs.sf()) {
            m_wbit = false;
        }
    virtual ~BS2ErrorMessage() { }

    BS2ErrorMessage const& operator=(BS2ErrorMessage const& rhs);

//
private:
};

/* ------------------------------------------------------------------------- */
class BS2S9F1Message : public BS2ErrorMessage
{
public:
    BS2S9F1Message(BS2BlockHeader& header)
            : BS2ErrorMessage(SFCODE(9,1), header) { }
    BS2S9F1Message(BS2Message * srcmsg)
            : BS2ErrorMessage(SFCODE(9,1), srcmsg) { }
    virtual ~BS2S9F1Message() { }
};

/* ------------------------------------------------------------------------- */
class BS2S9F3Message : public BS2ErrorMessage
{
public:
    BS2S9F3Message(BS2BlockHeader& header)
            : BS2ErrorMessage(SFCODE(9,3), header) { }
    BS2S9F3Message(BS2Message * srcmsg)
            : BS2ErrorMessage(SFCODE(9,3), srcmsg) { }
    virtual ~BS2S9F3Message() { }
};

/* ------------------------------------------------------------------------- */
class BS2S9F5Message : public BS2ErrorMessage
{
public:
    BS2S9F5Message(BS2BlockHeader& header)
            : BS2ErrorMessage(SFCODE(9,5), header) { }
    BS2S9F5Message(BS2Message * srcmsg)
            : BS2ErrorMessage(SFCODE(9,5), srcmsg) { }
    virtual ~BS2S9F5Message() { }
};

/* ------------------------------------------------------------------------- */
class BS2S9F7Message : public BS2ErrorMessage
{
public:
    BS2S9F7Message(BS2BlockHeader& header)
            : BS2ErrorMessage(SFCODE(9,7), header) { }
    BS2S9F7Message(BS2Message * srcmsg)
            : BS2ErrorMessage(SFCODE(9,7), srcmsg) { }
    virtual ~BS2S9F7Message() { }
};

/* ------------------------------------------------------------------------- */
class BS2S9F9Message : public BS2ErrorMessage
{
public:
    BS2S9F9Message(BS2BlockHeader& header)
            : BS2ErrorMessage(SFCODE(9,9), header) { }
    BS2S9F9Message(BS2Message * srcmsg)
            : BS2ErrorMessage(SFCODE(9,9), srcmsg) { }
    virtual ~BS2S9F9Message() { }
};

/* ------------------------------------------------------------------------- */
class BS2S9F11Message : public BS2ErrorMessage
{
public:
    BS2S9F11Message(BS2BlockHeader& header)
            : BS2ErrorMessage(SFCODE(9,11), header) { }
    BS2S9F11Message(BS2Message * srcmsg)
            : BS2ErrorMessage(SFCODE(9,11), srcmsg) { }
    virtual ~BS2S9F11Message() { }
};

#endif /* BS2ERRORMESSAGE_H */
