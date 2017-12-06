#ifndef GEMSDK_H
#define GEMSDK_H

#define GEMSDK_PROTECT

#include "secs/BS2DeclAtoms.h"
#include "secs/BS2SECSReceiver.h"
#include "secs/BS2Sender.h"
#include "secs/BS2Socket.h"
#include "secs/BS2Driver.h"
#include "secs/BS2Message.h"
#include "secs/BS2ErrorMessage.h"
#include "secs/BS2MessageInfo.h"
#include "secs/BS2MessageDictionary.h"
#include "secs/BS2ItemDictionary.h"
#include "secs/BS2Device.h"

#include "wxLocker.h"
#include "GemSender.h"
#include "GemReceiver.h"
#include "GemDevice.h"
#include "GemHsmsCall.h"

extern int	make_msgtype();
extern void close_msgtype(); 

#endif