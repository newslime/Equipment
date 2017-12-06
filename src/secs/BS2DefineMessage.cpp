// $Id$

//=============================================================================
/**
 *  @file   BS2DefineMessage.cpp
 *
 *  @author Fukasawa Mitsuo
 *
 *
 *    Copyright (C) 2004 BEE Co.,Ltd. All rights reserved.
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

#define BEE_BUILD_DLL

#include "BS2MessageDictionary.h"
#include "BS2ItemDictionary.h"
#include "BS2MessageType.h"
#include "SECSXmlParser.h"

static int make_itemtype();
static int make_itemtype_reference();

int make_itemtype()
{
    BS2ItemDictionary::instance()->add("ABS", 0x2, ATOM_BINARY);
    BS2ItemDictionary::instance()->add("ACCESSMODE", 0x1000, ATOM_UINT1);
    BS2ItemDictionary::instance()->add("ACDS", 0x2200, ATOM_INT2);
    BS2ItemDictionary::instance()->add("ACKA", 0x4, ATOM_BOOLEAN);
    BS2ItemDictionary::instance()->add("ACKC10", 0x2, ATOM_BINARY);
    BS2ItemDictionary::instance()->add("ACKC13", 0x2, ATOM_BINARY);
    BS2ItemDictionary::instance()->add("ACKC3", 0x2, ATOM_BINARY);
    BS2ItemDictionary::instance()->add("ACKC5", 0x2, ATOM_BINARY);
    BS2ItemDictionary::instance()->add("ACKC6", 0x2, ATOM_BINARY);
    BS2ItemDictionary::instance()->add("ACKC7", 0x2, ATOM_BINARY);
    BS2ItemDictionary::instance()->add("ACKC7A", 0x1100, ATOM_INT1);
    BS2ItemDictionary::instance()->add("AGENT", 0x10, ATOM_ASCII);
    //BS2ItemDictionary::instance()->add("ALCD", 0x2, ATOM_BINARY);
	BS2ItemDictionary::instance()->add("ALCD", 0x1002, ATOM_BINARY);
    BS2ItemDictionary::instance()->add("ALED", 0x2, ATOM_BINARY);
    BS2ItemDictionary::instance()->add("ALID", 0xFF00, ATOM_UINT4);
	BS2ItemDictionary::instance()->add("ALIDARRAY", 0xFF00, ATOM_ARRAY); //stone
    BS2ItemDictionary::instance()->add("ALTX", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("ATTRDATA", 0xCFF17, ATOM_ASCII, true);
    BS2ItemDictionary::instance()->add("ATTRID", 0xF010, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("ATTRRELN", 0x1000, ATOM_UINT1);
    BS2ItemDictionary::instance()->add("BCDS", 0x2200, ATOM_UINT2);
    BS2ItemDictionary::instance()->add("BCEQU", 0x1010, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("BINLT", 0x1010, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("BLKDEF", 0x1100, ATOM_UINT1);
    BS2ItemDictionary::instance()->add("BPD", 0x2, ATOM_BINARY);
    BS2ItemDictionary::instance()->add("BYTMAX", 0xFF00, ATOM_UINT4);
    BS2ItemDictionary::instance()->add("CAACK", 0x1000, ATOM_UINT1);
    BS2ItemDictionary::instance()->add("CARRIERACTION", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("CARRIERID", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("CARRIERSPEC", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("CATTRDATA", 0xCFF17, ATOM_ASCII, true);
    BS2ItemDictionary::instance()->add("CATTRID", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("CCODE", 0x2200, ATOM_UINT2);
    BS2ItemDictionary::instance()->add("CEED", 0x4, ATOM_BOOLEAN);
    BS2ItemDictionary::instance()->add("CEID", 0xFF10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("CEPACK", 0x1001, ATOM_UINT1, true);
    BS2ItemDictionary::instance()->add("CEPVAL", 0xCFF17, ATOM_ASCII, true);
    BS2ItemDictionary::instance()->add("CKPNT", 0x4000, ATOM_UINT4);
    BS2ItemDictionary::instance()->add("CMDA", 0x1100, ATOM_UINT1);
    BS2ItemDictionary::instance()->add("CMDMAX", 0xFF00, ATOM_UINT4);
    BS2ItemDictionary::instance()->add("CNAME", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("COLCT", 0xF000, ATOM_UINT4);
    BS2ItemDictionary::instance()->add("COLHDR", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("COMMACK", 0x2, ATOM_BINARY);
    BS2ItemDictionary::instance()->add("CPACK", 0x2, ATOM_BINARY);
    BS2ItemDictionary::instance()->add("CPNAME", 0xFF10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("CPVAL", 0xFF16, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("CSAACK", 0x2, ATOM_BINARY);
    BS2ItemDictionary::instance()->add("CTLJOBCMD", 0x1000, ATOM_UINT1);
    BS2ItemDictionary::instance()->add("CTLJOBID", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("DATA", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("DATAACK", 0x2, ATOM_BINARY);
    BS2ItemDictionary::instance()->add("DATAID", 0xFF10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("DATALENGTH", 0xFF00, ATOM_UINT4);
    BS2ItemDictionary::instance()->add("DATASEG", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("DATASRC", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("DATLC", 0x1000, ATOM_UINT1);
    BS2ItemDictionary::instance()->add("DRACK", 0x2, ATOM_BINARY);
    BS2ItemDictionary::instance()->add("DSID", 0xFF10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("DSNAME", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("DSPER", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("DUTMS", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("DVNAME", 0xFF10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("DVVAL", 0xCFF17, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("EAC", 0x2, ATOM_BINARY);
    BS2ItemDictionary::instance()->add("ECDEF", 0xCFF16, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("ECID", 0xFF10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("ECMAX", 0xCFF16, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("ECMIN", 0xCFF16, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("ECNAME", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("ECV", 0xCFF16, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("EDID", 0xFF12, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("EMID", 0x12, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("EPD", 0x2, ATOM_BINARY);
    BS2ItemDictionary::instance()->add("EQNAME", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("ERACK", 0x2, ATOM_BINARY);
    BS2ItemDictionary::instance()->add("ERRCODE", 0xF000, ATOM_UINT4);
    BS2ItemDictionary::instance()->add("ERRTEXT", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("ERRW7", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("EVNTSRC", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("EXID", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("EXMESSAGE", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("EXRECVRA", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("EXTYPE", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("FCNID", 0x1000, ATOM_UINT1);
    BS2ItemDictionary::instance()->add("FFROT", 0x2000, ATOM_UINT2);
    BS2ItemDictionary::instance()->add("FILDAT", 0x12, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("FNLOC", 0x2000, ATOM_UINT2);
    BS2ItemDictionary::instance()->add("FRMLEN", 0xFF00, ATOM_UINT4);
    BS2ItemDictionary::instance()->add("GRANT", 0x2, ATOM_BINARY);
    BS2ItemDictionary::instance()->add("GRANT6", 0x2, ATOM_BINARY);
    BS2ItemDictionary::instance()->add("GRNT1", 0x2, ATOM_BINARY);
    BS2ItemDictionary::instance()->add("HANDLE", 0xFF00, ATOM_UINT4);
    BS2ItemDictionary::instance()->add("HCACK", 0x2, ATOM_BINARY);
    BS2ItemDictionary::instance()->add("HOACK", 0x4, ATOM_BOOLEAN);
    BS2ItemDictionary::instance()->add("HOCANCELACK", 0x1000, ATOM_UINT1);
    BS2ItemDictionary::instance()->add("HOCMDNAME", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("HOHALTACK", 0x1000, ATOM_UINT1);
    BS2ItemDictionary::instance()->add("IACDS", 0x2200, ATOM_UINT2);
    BS2ItemDictionary::instance()->add("IBCDS", 0x2200, ATOM_UINT2);
    BS2ItemDictionary::instance()->add("IDTYP", 0x2, ATOM_BINARY);
    BS2ItemDictionary::instance()->add("INPTN", 0x1002, ATOM_BINARY);
    BS2ItemDictionary::instance()->add("JOBACTION", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("LENGTH", 0xFF00, ATOM_UINT4);
    BS2ItemDictionary::instance()->add("LIMITACK", 0x2, ATOM_BINARY);
    BS2ItemDictionary::instance()->add("LIMITID", 0x2, ATOM_BINARY);
    BS2ItemDictionary::instance()->add("LIMITMAX", 0xCFF12, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("LIMITMIN", 0xCFF12, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("LINKID", 0x4000, ATOM_UINT4);
    BS2ItemDictionary::instance()->add("LLIM", 0xCFF00, ATOM_UINT4);
    BS2ItemDictionary::instance()->add("LOC", 0x2, ATOM_BINARY);
    BS2ItemDictionary::instance()->add("LOCID", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("LOWERDB", 0xCFF14, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("LRACK", 0x2, ATOM_BINARY);
    BS2ItemDictionary::instance()->add("LVACK", 0x2, ATOM_BINARY);
    BS2ItemDictionary::instance()->add("MAPER", 0x2, ATOM_BINARY);
    BS2ItemDictionary::instance()->add("MAPFT", 0x2, ATOM_BINARY);
    BS2ItemDictionary::instance()->add("MCINDEX", 0xF000, ATOM_UINT4);
    BS2ItemDictionary::instance()->add("MDACK", 0x2, ATOM_BINARY);
    BS2ItemDictionary::instance()->add("MDLN", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("MEXP", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("MF", 0x10, ATOM_ASCII);
    //BS2ItemDictionary::instance()->add("MHEAD", 0x2, ATOM_BINARY);
	BS2ItemDictionary::instance()->add("MHEAD", 0x12, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("MID", 0x12, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("MIDAC", 0x2, ATOM_BINARY);
    BS2ItemDictionary::instance()->add("MIDRA", 0x2, ATOM_BINARY);
    BS2ItemDictionary::instance()->add("MLCL", 0xF000, ATOM_UINT4);
    BS2ItemDictionary::instance()->add("MMODE", 0x2, ATOM_BINARY);
    BS2ItemDictionary::instance()->add("NACDS", 0x2200, ATOM_UINT2);
    BS2ItemDictionary::instance()->add("NBCDS", 0x2200, ATOM_UINT2);
    BS2ItemDictionary::instance()->add("NULBC", 0x1010, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("OBJACK", 0x1000, ATOM_UINT1);
    BS2ItemDictionary::instance()->add("OBJCMD", 0x1000, ATOM_UINT1);
    BS2ItemDictionary::instance()->add("OBJID", 0xF010, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("OBJSPEC", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("OBJTOKEN", 0x4000, ATOM_UINT4);
    BS2ItemDictionary::instance()->add("OBJTYPE", 0xF010, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("OFLACK", 0x2, ATOM_BINARY);
    BS2ItemDictionary::instance()->add("ONLACK", 0x2, ATOM_BINARY);
    BS2ItemDictionary::instance()->add("OPID", 0xF000, ATOM_UINT4);
    BS2ItemDictionary::instance()->add("ORLOC", 0x2, ATOM_BINARY);
    BS2ItemDictionary::instance()->add("OUTPTN", 0x1002, ATOM_BINARY);
    BS2ItemDictionary::instance()->add("PARAMNAME", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("PARAMVAL", 0xCFF17, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("PDFLT", 0xCFF14, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("PFCD", 0x2, ATOM_BINARY);
    BS2ItemDictionary::instance()->add("PGRPACTION", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("PMAX", 0xFF00, ATOM_UINT4);
    BS2ItemDictionary::instance()->add("PNAME", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("PODID", 0xF010, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("PORTACTION", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("PORTGRPNAME", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("PPARM", 0xCFF14, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("PPBODY", 0xFF12, ATOM_BINARY);
    BS2ItemDictionary::instance()->add("PPGNT", 0x2, ATOM_BINARY);
    BS2ItemDictionary::instance()->add("PPID", 0x12, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("PRAXI", 0x2, ATOM_BINARY);
    BS2ItemDictionary::instance()->add("PRCMDNAME", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("PRDCT", 0xF000, ATOM_UINT4);
    BS2ItemDictionary::instance()->add("PREVENTID", 0xF000, ATOM_UINT4);
    BS2ItemDictionary::instance()->add("PRJOBID", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("PRJOBMILESTONE", 0xF000, ATOM_UINT4);
    BS2ItemDictionary::instance()->add("PRJOBSPACE", 0x2000, ATOM_UINT2);
    BS2ItemDictionary::instance()->add("PRMTRLORDER", 0x1000, ATOM_UINT1);
    BS2ItemDictionary::instance()->add("PRPAUSEEVENT", 0x1, ATOM_LIST);
    BS2ItemDictionary::instance()->add("PRPROCESSSTART", 0x4, ATOM_BOOLEAN);
    BS2ItemDictionary::instance()->add("PRRECIPEMETHOD", 0xF000, ATOM_UINT4);
    BS2ItemDictionary::instance()->add("PRSTATE", 0x1000, ATOM_UINT1);
    BS2ItemDictionary::instance()->add("PTN", 0x2, ATOM_BINARY);
    BS2ItemDictionary::instance()->add("QUA", 0x2, ATOM_BINARY);
    BS2ItemDictionary::instance()->add("RAC", 0x1100, ATOM_UINT1);
    BS2ItemDictionary::instance()->add("RCMD", 0x1110, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("RCPATTRDATA", 0xCFF17, ATOM_ASCII, true);
    BS2ItemDictionary::instance()->add("RCPATTRID", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("RCPBODY", 0xFF12, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("RCPCLASS", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("RCPCMD", 0x1000, ATOM_UINT1);
    BS2ItemDictionary::instance()->add("RCPDEL", 0x1000, ATOM_UINT1);
    BS2ItemDictionary::instance()->add("RCPDESCLTH", 0xF000, ATOM_UINT4);
    BS2ItemDictionary::instance()->add("RCPDESCNM", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("RCPDESCTIME", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("RCPID", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("RCPNAME", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("RCPNEWID", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("RCPOWCODE", 0x4, ATOM_BOOLEAN);
    BS2ItemDictionary::instance()->add("RCPPARNM", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("RCPPARRULE", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("RCPPARVAL", 0xCFF16, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("RCPRENAME", 0x4, ATOM_BOOLEAN);
    BS2ItemDictionary::instance()->add("RCPSECCODE", 0x2, ATOM_BINARY);
    BS2ItemDictionary::instance()->add("RCPSECNM", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("RCPSPEC", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("RCPSTAT", 0x1000, ATOM_UINT1);
    BS2ItemDictionary::instance()->add("RCPUPDT", 0x4, ATOM_BOOLEAN);
    BS2ItemDictionary::instance()->add("RCPVERS", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("READLN", 0xFF00, ATOM_UINT4);
    BS2ItemDictionary::instance()->add("RECLEN", 0xFF00, ATOM_UINT4);
    BS2ItemDictionary::instance()->add("REFP", 0xF00, ATOM_INT4);
    BS2ItemDictionary::instance()->add("REPGSZ", 0xFF10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("RESC", 0x1100, ATOM_UINT1);
    BS2ItemDictionary::instance()->add("RESPEC", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("RESV", 0xCFF00, ATOM_UINT4);
    BS2ItemDictionary::instance()->add("RETICLEID", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("RETPLACEINSTR", 0x1000, ATOM_UINT1);
    BS2ItemDictionary::instance()->add("RETREMOVERINSTR", 0x1000, ATOM_UINT1);
    BS2ItemDictionary::instance()->add("RIC", 0x1100, ATOM_UINT1);
    BS2ItemDictionary::instance()->add("RMACK", 0x1000, ATOM_UINT1);
    BS2ItemDictionary::instance()->add("RMCHGSTAT", 0xF000, ATOM_UINT4);
    BS2ItemDictionary::instance()->add("RMCHGTYPE", 0xF000, ATOM_UINT4);
    BS2ItemDictionary::instance()->add("RMDATASIZE", 0xF000, ATOM_UINT4);
    BS2ItemDictionary::instance()->add("RMGRNT", 0x2, ATOM_BINARY);
    BS2ItemDictionary::instance()->add("RMNEWNS", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("RMNSCMD", 0x1000, ATOM_UINT1);
    BS2ItemDictionary::instance()->add("RMNSSPEC", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("RMRECSPEC", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("RMREQUESTOR", 0x4, ATOM_BOOLEAN);
    BS2ItemDictionary::instance()->add("RMSEGSPEC", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("RMSPACE", 0xF000, ATOM_UINT4);
    BS2ItemDictionary::instance()->add("ROWCT", 0xF000, ATOM_UINT4);
    BS2ItemDictionary::instance()->add("RPMACK", 0x1000, ATOM_UINT1);
    BS2ItemDictionary::instance()->add("RPMDESTLOC", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("RPMSOURTLOC", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("RPSEL", 0x1000, ATOM_UINT1);
    BS2ItemDictionary::instance()->add("RPTID", 0xFF10, ATOM_UINT4);
    BS2ItemDictionary::instance()->add("RPTOC", 0x4, ATOM_BOOLEAN);
    BS2ItemDictionary::instance()->add("RQCMD", 0x4, ATOM_BOOLEAN);
    BS2ItemDictionary::instance()->add("RQPAR", 0x4, ATOM_BOOLEAN);
    BS2ItemDictionary::instance()->add("RRACK", 0x2, ATOM_BINARY);
    BS2ItemDictionary::instance()->add("RSACK", 0x2, ATOM_BINARY);
    BS2ItemDictionary::instance()->add("RSDA", 0x2, ATOM_BINARY);
    BS2ItemDictionary::instance()->add("RSDC", 0x1000, ATOM_UINT1);
    BS2ItemDictionary::instance()->add("RSINF", 0xF00, ATOM_INT4);
    BS2ItemDictionary::instance()->add("RSPACK", 0x2, ATOM_BINARY);
    BS2ItemDictionary::instance()->add("RTYPE", 0xFF00, ATOM_UINT4);
    BS2ItemDictionary::instance()->add("SDACK", 0x2, ATOM_BINARY);
    BS2ItemDictionary::instance()->add("SDBIN", 0x2, ATOM_BINARY);
    BS2ItemDictionary::instance()->add("SEQNUM", 0xFF00, ATOM_UINT4);
    BS2ItemDictionary::instance()->add("SFCD", 0x2, ATOM_BINARY);
    BS2ItemDictionary::instance()->add("SHEAD", 0x2, ATOM_BINARY);
    BS2ItemDictionary::instance()->add("SLOTID", 0x1000, ATOM_UINT1);
    BS2ItemDictionary::instance()->add("SMPLN", 0xFF00, ATOM_UINT4);
    BS2ItemDictionary::instance()->add("SOFTREV", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("SPAACK", 0x2, ATOM_BINARY);
    BS2ItemDictionary::instance()->add("SPD", 0x2, ATOM_BINARY);
    BS2ItemDictionary::instance()->add("SPID", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("SPNAME", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("SPR", 0xCFF16, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("SPVAL", 0xCFF17, ATOM_ASCII, true);
    BS2ItemDictionary::instance()->add("SSACK", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("SSCMD", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("STATUS", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("STATUSLIST", 0x1, ATOM_LIST);
    BS2ItemDictionary::instance()->add("STEMP", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("STIME", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("STRACK", 0x2, ATOM_BINARY);
    BS2ItemDictionary::instance()->add("STRID", 0x1000, ATOM_UINT1);
    BS2ItemDictionary::instance()->add("STRP", 0xF000, ATOM_UINT4);
    BS2ItemDictionary::instance()->add("SV", 0xCFF17, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("SVCACK", 0x2, ATOM_BINARY);
    BS2ItemDictionary::instance()->add("SVCNAME", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("SVID", 0xFF10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("SVNAME", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("TARGETID", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("TARGETSPEC", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("TBLACK", 0x1000, ATOM_UINT1);
    BS2ItemDictionary::instance()->add("TBLCMD", 0x1000, ATOM_UINT1);
    BS2ItemDictionary::instance()->add("TBLELT", 0xCFF17, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("TBLID", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("TBLTYP", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("TEXT", 0xFF12, ATOM_ASCII);
    //BS2ItemDictionary::instance()->add("TIAACK", 0x2, ATOM_BINARY);
	BS2ItemDictionary::instance()->add("TIAACK", 0x1002, ATOM_BINARY);
    BS2ItemDictionary::instance()->add("TIACK", 0x2, ATOM_BINARY);
    BS2ItemDictionary::instance()->add("TID", 0x2, ATOM_BINARY);
    BS2ItemDictionary::instance()->add("TIME", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("TIMEFORMAT", 0xF000, ATOM_UINT4);
    BS2ItemDictionary::instance()->add("TIMESTAMP", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("TOTSMP", 0xFF10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("TRACK", 0x4, ATOM_BOOLEAN);
    BS2ItemDictionary::instance()->add("TRATOMICID", 0xF000, ATOM_UINT4);
    BS2ItemDictionary::instance()->add("TRAUTOD", 0x4, ATOM_BOOLEAN);
    BS2ItemDictionary::instance()->add("TRAUTOSTART", 0x4, ATOM_BOOLEAN);
    BS2ItemDictionary::instance()->add("TRCMDNAME", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("TRDIR", 0x1000, ATOM_UINT1);
    BS2ItemDictionary::instance()->add("TRID", 0xFF10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("TRJOBID", 0x2, ATOM_BINARY);
    BS2ItemDictionary::instance()->add("TRJOBMS", 0x1000, ATOM_UINT1);
    BS2ItemDictionary::instance()->add("TRJOBNAME", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("TRLINK", 0xF000, ATOM_UINT4);
    BS2ItemDictionary::instance()->add("TRLOCATION", 0xF000, ATOM_UINT4);
    BS2ItemDictionary::instance()->add("TROBJNAME", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("TROBJTYPE", 0xF000, ATOM_UINT4);
    BS2ItemDictionary::instance()->add("TRPORT", 0xF000, ATOM_UINT4);
    BS2ItemDictionary::instance()->add("TRPTNR", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("TRPTPORT", 0xF000, ATOM_UINT4);
    BS2ItemDictionary::instance()->add("TRRCP", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("TRROLE", 0x1000, ATOM_UINT1);
    BS2ItemDictionary::instance()->add("TRSPER", 0xC0000, ATOM_FLOAT8);
    BS2ItemDictionary::instance()->add("TRTYPE", 0x1000, ATOM_UINT1);
    BS2ItemDictionary::instance()->add("TSIP", 0x2, ATOM_BINARY);
    BS2ItemDictionary::instance()->add("TSOP", 0x2, ATOM_BINARY);
    BS2ItemDictionary::instance()->add("TTC", 0xFF00, ATOM_UINT4);
    BS2ItemDictionary::instance()->add("ULIM", 0xCFF00, ATOM_UINT4);
    BS2ItemDictionary::instance()->add("UNFLEN", 0xFF00, ATOM_UINT4);
    BS2ItemDictionary::instance()->add("UNITS", 0x10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("UPPERDB", 0xCFF14, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("V", 0xCFF17, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("VID", 0xFF10, ATOM_ASCII);
    BS2ItemDictionary::instance()->add("VLAACK", 0x2, ATOM_BINARY);
    BS2ItemDictionary::instance()->add("XDIES", 0xCF000, ATOM_UINT4);
    BS2ItemDictionary::instance()->add("XYPOS", 0xF00, ATOM_INT4);
    BS2ItemDictionary::instance()->add("YDIES", 0xCF000, ATOM_UINT4);
    return 0;
}

#define SET_OBJECTITEM(n)                                                      \
do {                                                                           \
    if ((itemType = const_cast<BS2ItemType *>(itemTable->find(n))) == NULL)    \
    {                                                                          \
        TRACE_ERROR((_TX("The item was not found (%s).\n"), (n)));             \
        return -1;                                                             \
    }                                                                          \
    if (! itemType->isList())                                                  \
    {                                                                          \
        TRACE_ERROR((_TX("The item was not list (%s).\n"), (n)));              \
        return -1;                                                             \
    }                                                                          \
    objList = reinterpret_cast<BS2Itemtypes *>(itemType);                      \
} while (0)

#define ADD_REFITEM(n)                                                         \
do {                                                                           \
    if ((refItem = itemTable->find(n)) == NULL)                                \
    {                                                                          \
        TRACE_ERROR((_TX("The ref-item was not found (%s).\n"), (n)));         \
        return -1;                                                             \
    }                                                                          \
    objList->add(refItem);                                                     \
} while (0)

//add in itemType of secs.xml -> <format>list
int make_itemtype_reference()
{
    TRACE_FUNCTION(TRL_LOW, "make_itemtype_reference");

    BS2ItemDictionary * itemTable = BS2ItemDictionary::instance();
    BS2ItemType *  itemType;
    BS2Itemtypes * objList;
    BS2ItemType *  refItem;

    SET_OBJECTITEM("ATTRDATA");
    ADD_REFITEM("ATTRID");
    ADD_REFITEM("ATTRDATA");

    SET_OBJECTITEM("CATTRDATA");
    ADD_REFITEM("CATTRID");
    ADD_REFITEM("CATTRDATA");

    SET_OBJECTITEM("CEPACK");
    ADD_REFITEM("CPNAME");
    ADD_REFITEM("CEPVAL");

    SET_OBJECTITEM("CEPVAL");
    ADD_REFITEM("CPNAME");
    ADD_REFITEM("CEPVAL");

    SET_OBJECTITEM("RCPATTRDATA");
    ADD_REFITEM("RCPATTRID");
    ADD_REFITEM("RCPATTRDATA");

    SET_OBJECTITEM("SPVAL");
    ADD_REFITEM("SPNAME");
    ADD_REFITEM("SPVAL");

    return 0;
}

int make_msgtype()
{
    BS2MessageType * msgtype;

    if (make_itemtype() < 0)
    {
        return -1;
    }

    if (make_itemtype_reference() < 0)
    {
        return -1;
    }

    // S1F0
    msgtype = new BS2MessageType(SFCODE(1,0), BOTH_DIR, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    BS2MessageDictionary::instance()->add(msgtype);

    // S1F1
    msgtype = new BS2MessageType(SFCODE(1,1), BOTH_DIR, REPLY_REQUIRED, "CONTROL", SINGLE_BLOCK_MESSAGE);
    BS2MessageDictionary::instance()->add(msgtype);

    // S1F2
    msgtype = new BS2MessageType(SFCODE(1,2), BOTH_DIR, REPLY_NONE, "CONTROL", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("MDLN"));
        listitem_1->add(BS2ItemDictionary::instance()->find("SOFTREV"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S1F3
    msgtype = new BS2MessageType(SFCODE(1,3), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "INFO", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("SVID"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S1F4
    msgtype = new BS2MessageType(SFCODE(1,4), EQUIPMENT_TO_HOST, REPLY_NONE, "INFO", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("SV"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S1F5
    msgtype = new BS2MessageType(SFCODE(1,5), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("SFCD"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S1F6
    msgtype = new BS2MessageType(SFCODE(1,6), EQUIPMENT_TO_HOST, REPLY_NONE, "", MULTI_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("SFCD"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S1F7
    msgtype = new BS2MessageType(SFCODE(1,7), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("SFCD"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S1F8
    msgtype = new BS2MessageType(SFCODE(1,8), EQUIPMENT_TO_HOST, REPLY_NONE, "", MULTI_BLOCK_MESSAGE);
    BS2MessageDictionary::instance()->add(msgtype);

    // S1F9
    msgtype = new BS2MessageType(SFCODE(1,9), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    BS2MessageDictionary::instance()->add(msgtype);

    // S1F10
    msgtype = new BS2MessageType(SFCODE(1,10), EQUIPMENT_TO_HOST, REPLY_NONE, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("TSIP"));
        listitem_1->add(BS2ItemDictionary::instance()->find("TSOP"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S1F11
    msgtype = new BS2MessageType(SFCODE(1,11), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "INFO", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("SVID"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S1F12
    msgtype = new BS2MessageType(SFCODE(1,12), EQUIPMENT_TO_HOST, REPLY_NONE, "INFO", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("SVID"));
            listitem_2->add(BS2ItemDictionary::instance()->find("SVNAME"));
            listitem_2->add(BS2ItemDictionary::instance()->find("UNITS"));
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S1F13
    msgtype = new BS2MessageType(SFCODE(1,13), BOTH_DIR, REPLY_REQUIRED, "COMMUNICATION", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("MDLN"));
        listitem_1->add(BS2ItemDictionary::instance()->find("SOFTREV"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S1F14
    msgtype = new BS2MessageType(SFCODE(1,14), BOTH_DIR, REPLY_NONE, "COMMUNICATION", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("COMMACK"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("MDLN"));
            listitem_2->add(BS2ItemDictionary::instance()->find("SOFTREV"));
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S1F15
    msgtype = new BS2MessageType(SFCODE(1,15), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "CONTROL", SINGLE_BLOCK_MESSAGE);
    BS2MessageDictionary::instance()->add(msgtype);

    // S1F16
    msgtype = new BS2MessageType(SFCODE(1,16), EQUIPMENT_TO_HOST, REPLY_NONE, "CONTROL", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("OFLACK"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S1F17
    msgtype = new BS2MessageType(SFCODE(1,17), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "CONTROL", SINGLE_BLOCK_MESSAGE);
    BS2MessageDictionary::instance()->add(msgtype);

    // S1F18
    msgtype = new BS2MessageType(SFCODE(1,18), EQUIPMENT_TO_HOST, REPLY_NONE, "CONTROL", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("ONLACK"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S1F19
    msgtype = new BS2MessageType(SFCODE(1,19), BOTH_DIR, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("OBJTYPE"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("OBJID"));
        }
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("ATTRID"));
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S1F20
    msgtype = new BS2MessageType(SFCODE(1,20), BOTH_DIR, REPLY_NONE, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("ATTRDATA"));
            }
        }
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                listitem_3->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S2F0
    msgtype = new BS2MessageType(SFCODE(2,0), BOTH_DIR, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    BS2MessageDictionary::instance()->add(msgtype);

    // S2F1
    msgtype = new BS2MessageType(SFCODE(2,1), BOTH_DIR, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("SPID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("LENGTH"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S2F2
    msgtype = new BS2MessageType(SFCODE(2,2), BOTH_DIR, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("GRANT"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S2F3
    msgtype = new BS2MessageType(SFCODE(2,3), BOTH_DIR, REPLY_REQUIRED, "", MULTI_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("SPD"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S2F4
    msgtype = new BS2MessageType(SFCODE(2,4), BOTH_DIR, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("SPAACK"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S2F5
    msgtype = new BS2MessageType(SFCODE(2,5), BOTH_DIR, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("SPID"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S2F6
    msgtype = new BS2MessageType(SFCODE(2,6), BOTH_DIR, REPLY_NONE, "", MULTI_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("SPD"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S2F7
    msgtype = new BS2MessageType(SFCODE(2,7), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("SPID"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S2F8
    msgtype = new BS2MessageType(SFCODE(2,8), EQUIPMENT_TO_HOST, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("CSAACK"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S2F9
    msgtype = new BS2MessageType(SFCODE(2,9), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("SPID"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S2F10
    msgtype = new BS2MessageType(SFCODE(2,10), EQUIPMENT_TO_HOST, REPLY_NONE, "", MULTI_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("SPR"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S2F11
    msgtype = new BS2MessageType(SFCODE(2,11), BOTH_DIR, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    BS2MessageDictionary::instance()->add(msgtype);

    // S2F12
    msgtype = new BS2MessageType(SFCODE(2,12), BOTH_DIR, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("SPID"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S2F13
    msgtype = new BS2MessageType(SFCODE(2,13), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "INFO", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("ECID"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S2F14
    msgtype = new BS2MessageType(SFCODE(2,14), EQUIPMENT_TO_HOST, REPLY_NONE, "INFO", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("ECV"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S2F15
    msgtype = new BS2MessageType(SFCODE(2,15), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "INFO", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("ECID"));
            listitem_2->add(BS2ItemDictionary::instance()->find("ECV"));
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S2F16
    msgtype = new BS2MessageType(SFCODE(2,16), EQUIPMENT_TO_HOST, REPLY_NONE, "INFO", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("EAC"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S2F17
    msgtype = new BS2MessageType(SFCODE(2,17), BOTH_DIR, REPLY_REQUIRED, "INFO", SINGLE_BLOCK_MESSAGE);
    BS2MessageDictionary::instance()->add(msgtype);

    // S2F18
    msgtype = new BS2MessageType(SFCODE(2,18), BOTH_DIR, REPLY_NONE, "INFO", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("TIME"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S2F19
    msgtype = new BS2MessageType(SFCODE(2,19), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("RIC"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S2F20
    msgtype = new BS2MessageType(SFCODE(2,20), EQUIPMENT_TO_HOST, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("RAC"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S2F21
    msgtype = new BS2MessageType(SFCODE(2,21), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("RCMD"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S2F22
    msgtype = new BS2MessageType(SFCODE(2,22), EQUIPMENT_TO_HOST, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("CMDA"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S2F23
    msgtype = new BS2MessageType(SFCODE(2,23), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "TRACE", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("TRID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("DSPER"));
        listitem_1->add(BS2ItemDictionary::instance()->find("TOTSMP"));
        listitem_1->add(BS2ItemDictionary::instance()->find("REPGSZ"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("SVID"));
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S2F24
    msgtype = new BS2MessageType(SFCODE(2,24), EQUIPMENT_TO_HOST, REPLY_NONE, "TRACE", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("TIAACK"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S2F25
    msgtype = new BS2MessageType(SFCODE(2,25), BOTH_DIR, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("ABS"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S2F26
    msgtype = new BS2MessageType(SFCODE(2,26), BOTH_DIR, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("ABS"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S2F27
    msgtype = new BS2MessageType(SFCODE(2,27), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("LOC"));
        listitem_1->add(BS2ItemDictionary::instance()->find("PPID"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("MID"));
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S2F28
    msgtype = new BS2MessageType(SFCODE(2,28), EQUIPMENT_TO_HOST, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("CMDA"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S2F29
    msgtype = new BS2MessageType(SFCODE(2,29), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "INFO", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("ECID"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S2F30
    msgtype = new BS2MessageType(SFCODE(2,30), EQUIPMENT_TO_HOST, REPLY_NONE, "INFO", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("ECID"));
            listitem_2->add(BS2ItemDictionary::instance()->find("ECNAME"));
            listitem_2->add(BS2ItemDictionary::instance()->find("ECMIN"));
            listitem_2->add(BS2ItemDictionary::instance()->find("ECMAX"));
            listitem_2->add(BS2ItemDictionary::instance()->find("ECDEF"));
            listitem_2->add(BS2ItemDictionary::instance()->find("UNITS"));
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S2F31
    msgtype = new BS2MessageType(SFCODE(2,31), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "INFO", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("TIME"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S2F32
    msgtype = new BS2MessageType(SFCODE(2,32), EQUIPMENT_TO_HOST, REPLY_NONE, "INFO", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("TIACK"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S2F33
    msgtype = new BS2MessageType(SFCODE(2,33), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "EVENT", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("DATAID"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("RPTID"));
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    listitem_4->add(BS2ItemDictionary::instance()->find("VID"));
                }
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S2F34
    msgtype = new BS2MessageType(SFCODE(2,34), EQUIPMENT_TO_HOST, REPLY_NONE, "EVENT", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("DRACK"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S2F35
    msgtype = new BS2MessageType(SFCODE(2,35), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "EVENT", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("DATAID"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("CEID"));
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    listitem_4->add(BS2ItemDictionary::instance()->find("RPTID"));
                }
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S2F36
    msgtype = new BS2MessageType(SFCODE(2,36), EQUIPMENT_TO_HOST, REPLY_NONE, "EVENT", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("LRACK"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S2F37
    msgtype = new BS2MessageType(SFCODE(2,37), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "EVENT", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("CEED"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("CEID"));
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S2F38
    msgtype = new BS2MessageType(SFCODE(2,38), EQUIPMENT_TO_HOST, REPLY_NONE, "EVENT", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("ERACK"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S2F39
    msgtype = new BS2MessageType(SFCODE(2,39), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "INFO", INQUIRE_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("DATAID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("DATALENGTH"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S2F40
    msgtype = new BS2MessageType(SFCODE(2,40), EQUIPMENT_TO_HOST, REPLY_NONE, "INFO", GRANT_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("GRANT"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S2F41
    msgtype = new BS2MessageType(SFCODE(2,41), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "ACCESS", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("RCMD"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("CPNAME"));
                listitem_3->add(BS2ItemDictionary::instance()->find("CPVAL"));
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S2F42
    msgtype = new BS2MessageType(SFCODE(2,42), EQUIPMENT_TO_HOST, REPLY_NONE, "ACCESS", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("HCACK"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("CPNAME"));
                listitem_3->add(BS2ItemDictionary::instance()->find("CPACK"));
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S2F43
    msgtype = new BS2MessageType(SFCODE(2,43), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "SPOOL", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("STRID"));
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("FCNID"));
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S2F44
    msgtype = new BS2MessageType(SFCODE(2,44), EQUIPMENT_TO_HOST, REPLY_NONE, "SPOOL", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("RSPACK"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("STRID"));
                listitem_3->add(BS2ItemDictionary::instance()->find("STRACK"));
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    listitem_4->add(BS2ItemDictionary::instance()->find("FCNID"));
                }
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S2F45
    msgtype = new BS2MessageType(SFCODE(2,45), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "LIMIT", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("DATAID"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("VID"));
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    {
                        BS2Itemtypes * listitem_5 = new BS2Itemtypes();
                        listitem_4->add(listitem_5);
                        listitem_5->add(BS2ItemDictionary::instance()->find("LIMITID"));
                        {
                            BS2Itemtypes * listitem_6 = new BS2Itemtypes();
                            listitem_5->add(listitem_6);
                            listitem_6->add(BS2ItemDictionary::instance()->find("UPPERDB"));
                            listitem_6->add(BS2ItemDictionary::instance()->find("LOWERDB"));
                        }
                    }
                }
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S2F46
    msgtype = new BS2MessageType(SFCODE(2,46), EQUIPMENT_TO_HOST, REPLY_NONE, "LIMIT", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("VLAACK"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("VID"));
                listitem_3->add(BS2ItemDictionary::instance()->find("LVACK"));
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    listitem_4->add(BS2ItemDictionary::instance()->find("LIMITID"));
                    listitem_4->add(BS2ItemDictionary::instance()->find("LIMITACK"));
                }
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S2F47
    msgtype = new BS2MessageType(SFCODE(2,47), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "LIMIT", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("VID"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S2F48
    msgtype = new BS2MessageType(SFCODE(2,48), EQUIPMENT_TO_HOST, REPLY_NONE, "LIMIT", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("VID"));
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("UNITS"));
                listitem_3->add(BS2ItemDictionary::instance()->find("LIMITMIN"));
                listitem_3->add(BS2ItemDictionary::instance()->find("LIMITMAX"));
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    {
                        BS2Itemtypes * listitem_5 = new BS2Itemtypes();
                        listitem_4->add(listitem_5);
                        listitem_5->add(BS2ItemDictionary::instance()->find("LIMITID"));
                        listitem_5->add(BS2ItemDictionary::instance()->find("UPPERDB"));
                        listitem_5->add(BS2ItemDictionary::instance()->find("LOWERDB"));
                    }
                }
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S2F49
    msgtype = new BS2MessageType(SFCODE(2,49), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "ACCESS", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("DATAID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("OBJSPEC"));
        listitem_1->add(BS2ItemDictionary::instance()->find("RCMD"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("CPNAME"));
                listitem_3->add(BS2ItemDictionary::instance()->find("CEPVAL"));
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S2F50
    msgtype = new BS2MessageType(SFCODE(2,50), EQUIPMENT_TO_HOST, REPLY_NONE, "ACCESS", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("HCACK"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("CPNAME"));
                listitem_3->add(BS2ItemDictionary::instance()->find("CEPACK"));
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S3F0
    msgtype = new BS2MessageType(SFCODE(3,0), BOTH_DIR, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    BS2MessageDictionary::instance()->add(msgtype);

    // S3F1
    msgtype = new BS2MessageType(SFCODE(3,1), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    BS2MessageDictionary::instance()->add(msgtype);

    // S3F2
    msgtype = new BS2MessageType(SFCODE(3,2), EQUIPMENT_TO_HOST, REPLY_NONE, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("MF"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("LOC"));
                listitem_3->add(BS2ItemDictionary::instance()->find("QUA"));
                listitem_3->add(BS2ItemDictionary::instance()->find("MID"));
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S3F3
    msgtype = new BS2MessageType(SFCODE(3,3), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    BS2MessageDictionary::instance()->add(msgtype);

    // S3F4
    msgtype = new BS2MessageType(SFCODE(3,4), EQUIPMENT_TO_HOST, REPLY_NONE, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("MF"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("TTC"));
                listitem_3->add(BS2ItemDictionary::instance()->find("QUA"));
                listitem_3->add(BS2ItemDictionary::instance()->find("MID"));
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S3F5
    msgtype = new BS2MessageType(SFCODE(3,5), EQUIPMENT_TO_HOST, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("MF"));
        listitem_1->add(BS2ItemDictionary::instance()->find("QUA"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S3F6
    msgtype = new BS2MessageType(SFCODE(3,6), HOST_TO_EQUIPMENT, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("ACKC3"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S3F7
    msgtype = new BS2MessageType(SFCODE(3,7), EQUIPMENT_TO_HOST, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("MF"));
        listitem_1->add(BS2ItemDictionary::instance()->find("QUA"));
        listitem_1->add(BS2ItemDictionary::instance()->find("MID"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S3F8
    msgtype = new BS2MessageType(SFCODE(3,8), HOST_TO_EQUIPMENT, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("ACKC3"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S3F9
    msgtype = new BS2MessageType(SFCODE(3,9), EQUIPMENT_TO_HOST, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("MID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("EMID"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S3F10
    msgtype = new BS2MessageType(SFCODE(3,10), HOST_TO_EQUIPMENT, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("ACKC3"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S3F11
    msgtype = new BS2MessageType(SFCODE(3,11), EQUIPMENT_TO_HOST, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("PTN"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S3F12
    msgtype = new BS2MessageType(SFCODE(3,12), HOST_TO_EQUIPMENT, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("PTN"));
        listitem_1->add(BS2ItemDictionary::instance()->find("MIDRA"));
        listitem_1->add(BS2ItemDictionary::instance()->find("MID"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S3F13
    msgtype = new BS2MessageType(SFCODE(3,13), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("PTN"));
        listitem_1->add(BS2ItemDictionary::instance()->find("MID"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S3F14
    msgtype = new BS2MessageType(SFCODE(3,14), EQUIPMENT_TO_HOST, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("MIDAC"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S3F15
    msgtype = new BS2MessageType(SFCODE(3,15), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "", INQUIRE_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("DATAID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("DATALENGTH"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S3F16
    msgtype = new BS2MessageType(SFCODE(3,16), EQUIPMENT_TO_HOST, REPLY_NONE, "", GRANT_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("GRANT"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S3F17
    msgtype = new BS2MessageType(SFCODE(3,17), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("DATAID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("CARRIERACTION"));
        listitem_1->add(BS2ItemDictionary::instance()->find("CARRIERID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("PTN"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("CATTRID"));
                listitem_3->add(BS2ItemDictionary::instance()->find("CATTRDATA"));
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S3F18
    msgtype = new BS2MessageType(SFCODE(3,18), EQUIPMENT_TO_HOST, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("CAACK"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                listitem_3->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S3F19
    msgtype = new BS2MessageType(SFCODE(3,19), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    BS2MessageDictionary::instance()->add(msgtype);

    // S3F20
    msgtype = new BS2MessageType(SFCODE(3,20), EQUIPMENT_TO_HOST, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("CAACK"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                listitem_3->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S3F21
    msgtype = new BS2MessageType(SFCODE(3,21), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("PORTGRPNAME"));
        listitem_1->add(BS2ItemDictionary::instance()->find("ACCESSMODE"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("PTN"));
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S3F22
    msgtype = new BS2MessageType(SFCODE(3,22), EQUIPMENT_TO_HOST, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("CAACK"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                listitem_3->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S3F23
    msgtype = new BS2MessageType(SFCODE(3,23), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("PGRPACTION"));
        listitem_1->add(BS2ItemDictionary::instance()->find("PORTGRPNAME"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("PARAMNAME"));
                listitem_3->add(BS2ItemDictionary::instance()->find("PARAMVAL"));
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S3F24
    msgtype = new BS2MessageType(SFCODE(3,24), EQUIPMENT_TO_HOST, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("CAACK"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                listitem_3->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S3F25
    msgtype = new BS2MessageType(SFCODE(3,25), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("PORTACTION"));
        listitem_1->add(BS2ItemDictionary::instance()->find("PTN"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("PARAMNAME"));
                listitem_3->add(BS2ItemDictionary::instance()->find("PARAMVAL"));
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S3F26
    msgtype = new BS2MessageType(SFCODE(3,26), EQUIPMENT_TO_HOST, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("CAACK"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                listitem_3->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S3F27
    msgtype = new BS2MessageType(SFCODE(3,27), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("ACCESSMODE"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("PTN"));
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S3F28
    msgtype = new BS2MessageType(SFCODE(3,28), EQUIPMENT_TO_HOST, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("CAACK"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("PTN"));
                listitem_3->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                listitem_3->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S3F29
    msgtype = new BS2MessageType(SFCODE(3,29), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("LOCID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("CARRIERSPEC"));
        listitem_1->add(BS2ItemDictionary::instance()->find("DATASEG"));
        listitem_1->add(BS2ItemDictionary::instance()->find("DATALENGTH"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S3F30
    msgtype = new BS2MessageType(SFCODE(3,30), EQUIPMENT_TO_HOST, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("DATA"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("CAACK"));
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
                }
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S3F31
    msgtype = new BS2MessageType(SFCODE(3,31), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("LOCID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("CARRIERSPEC"));
        listitem_1->add(BS2ItemDictionary::instance()->find("DATASEG"));
        listitem_1->add(BS2ItemDictionary::instance()->find("DATALENGTH"));
        listitem_1->add(BS2ItemDictionary::instance()->find("DATA"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S3F32
    msgtype = new BS2MessageType(SFCODE(3,32), EQUIPMENT_TO_HOST, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("CAACK"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                listitem_3->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S3F33
    msgtype = new BS2MessageType(SFCODE(3,33), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    BS2MessageDictionary::instance()->add(msgtype);

    // S3F34
    msgtype = new BS2MessageType(SFCODE(3,34), EQUIPMENT_TO_HOST, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("CAACK"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                listitem_3->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S3F35
    msgtype = new BS2MessageType(SFCODE(3,35), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("JOBACTION"));
        listitem_1->add(BS2ItemDictionary::instance()->find("PODID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("INPTN"));
        listitem_1->add(BS2ItemDictionary::instance()->find("OUTPTN"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("ATTRID"));
                listitem_3->add(BS2ItemDictionary::instance()->find("ATTRDATA"));
            }
        }
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("RETICLEID"));
                listitem_3->add(BS2ItemDictionary::instance()->find("RETREMOVERINSTR"));
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    {
                        BS2Itemtypes * listitem_5 = new BS2Itemtypes();
                        listitem_4->add(listitem_5);
                        listitem_5->add(BS2ItemDictionary::instance()->find("ATTRID"));
                        listitem_5->add(BS2ItemDictionary::instance()->find("ATTRDATA"));
                    }
                }
            }
        }
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("RETICLEID"));
                listitem_3->add(BS2ItemDictionary::instance()->find("RETPLACEINSTR"));
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S3F36
    msgtype = new BS2MessageType(SFCODE(3,36), EQUIPMENT_TO_HOST, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("RPMACK"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                listitem_3->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S4F0
    msgtype = new BS2MessageType(SFCODE(4,0), HOST_TO_EQUIPMENT, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    BS2MessageDictionary::instance()->add(msgtype);

    // S4F1
    msgtype = new BS2MessageType(SFCODE(4,1), BOTH_DIR, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("PTN"));
        listitem_1->add(BS2ItemDictionary::instance()->find("MID"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S4F2
    msgtype = new BS2MessageType(SFCODE(4,2), BOTH_DIR, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("RSACK"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S4F3
    msgtype = new BS2MessageType(SFCODE(4,3), BOTH_DIR, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("PTN"));
        listitem_1->add(BS2ItemDictionary::instance()->find("MID"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S4F5
    msgtype = new BS2MessageType(SFCODE(4,5), BOTH_DIR, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("PTN"));
        listitem_1->add(BS2ItemDictionary::instance()->find("MID"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S4F7
    msgtype = new BS2MessageType(SFCODE(4,7), BOTH_DIR, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("PTN"));
        listitem_1->add(BS2ItemDictionary::instance()->find("MID"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S4F9
    msgtype = new BS2MessageType(SFCODE(4,9), EQUIPMENT_TO_HOST, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("PTN"));
        listitem_1->add(BS2ItemDictionary::instance()->find("MID"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S4F11
    msgtype = new BS2MessageType(SFCODE(4,11), EQUIPMENT_TO_HOST, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("PTN"));
        listitem_1->add(BS2ItemDictionary::instance()->find("MID"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S4F13
    msgtype = new BS2MessageType(SFCODE(4,13), EQUIPMENT_TO_HOST, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("PTN"));
        listitem_1->add(BS2ItemDictionary::instance()->find("MID"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S4F15
    msgtype = new BS2MessageType(SFCODE(4,15), EQUIPMENT_TO_HOST, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("PTN"));
        listitem_1->add(BS2ItemDictionary::instance()->find("MID"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S4F17
    msgtype = new BS2MessageType(SFCODE(4,17), BOTH_DIR, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("PTN"));
        listitem_1->add(BS2ItemDictionary::instance()->find("MID"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S4F18
    msgtype = new BS2MessageType(SFCODE(4,18), BOTH_DIR, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("RRACK"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S4F19
    msgtype = new BS2MessageType(SFCODE(4,19), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("DATAID"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("TRJOBNAME"));
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    listitem_4->add(BS2ItemDictionary::instance()->find("TRLINK"));
                    listitem_4->add(BS2ItemDictionary::instance()->find("TRPORT"));
                    listitem_4->add(BS2ItemDictionary::instance()->find("TROBJNAME"));
                    listitem_4->add(BS2ItemDictionary::instance()->find("TROBJTYPE"));
                    listitem_4->add(BS2ItemDictionary::instance()->find("TRROLE"));
                    listitem_4->add(BS2ItemDictionary::instance()->find("TRRCP"));
                    listitem_4->add(BS2ItemDictionary::instance()->find("TRPTNR"));
                    listitem_4->add(BS2ItemDictionary::instance()->find("TRPTPORT"));
                    listitem_4->add(BS2ItemDictionary::instance()->find("TRDIR"));
                    listitem_4->add(BS2ItemDictionary::instance()->find("TRTYPE"));
                    listitem_4->add(BS2ItemDictionary::instance()->find("TRLOCATION"));
                    listitem_4->add(BS2ItemDictionary::instance()->find("TRAUTOSTART"));
                }
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S4F20
    msgtype = new BS2MessageType(SFCODE(4,20), EQUIPMENT_TO_HOST, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("TRJOBID"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("TRATOMICID"));
        }
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("TRACK"));
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
                }
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S4F21
    msgtype = new BS2MessageType(SFCODE(4,21), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("TRJOBID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("TRCMDNAME"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("CPNAME"));
                listitem_3->add(BS2ItemDictionary::instance()->find("CPVAL"));
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S4F22
    msgtype = new BS2MessageType(SFCODE(4,22), EQUIPMENT_TO_HOST, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("TRACK"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                listitem_3->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S4F23
    msgtype = new BS2MessageType(SFCODE(4,23), EQUIPMENT_TO_HOST, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("TRJOBID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("TRJOBNAME"));
        listitem_1->add(BS2ItemDictionary::instance()->find("TRJOBMS"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("TRACK"));
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
                }
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S4F24
    msgtype = new BS2MessageType(SFCODE(4,24), HOST_TO_EQUIPMENT, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    BS2MessageDictionary::instance()->add(msgtype);

    // S4F25
    msgtype = new BS2MessageType(SFCODE(4,25), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "", INQUIRE_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("DATAID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("DATALENGTH"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S4F26
    msgtype = new BS2MessageType(SFCODE(4,26), EQUIPMENT_TO_HOST, REPLY_NONE, "", GRANT_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("GRANT"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S4F27
    msgtype = new BS2MessageType(SFCODE(4,27), BOTH_DIR, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("EQNAME"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("TRLINK"));
            listitem_2->add(BS2ItemDictionary::instance()->find("TRPORT"));
            listitem_2->add(BS2ItemDictionary::instance()->find("TROBJNAME"));
            listitem_2->add(BS2ItemDictionary::instance()->find("TROBJTYPE"));
            listitem_2->add(BS2ItemDictionary::instance()->find("TRROLE"));
            listitem_2->add(BS2ItemDictionary::instance()->find("TRPTNR"));
            listitem_2->add(BS2ItemDictionary::instance()->find("TRPTPORT"));
            listitem_2->add(BS2ItemDictionary::instance()->find("TRDIR"));
            listitem_2->add(BS2ItemDictionary::instance()->find("TRTYPE"));
            listitem_2->add(BS2ItemDictionary::instance()->find("TRLOCATION"));
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S4F29
    msgtype = new BS2MessageType(SFCODE(4,29), BOTH_DIR, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("TRLINK"));
        listitem_1->add(BS2ItemDictionary::instance()->find("MCINDEX"));
        listitem_1->add(BS2ItemDictionary::instance()->find("HOCMDNAME"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("CPNAME"));
                listitem_3->add(BS2ItemDictionary::instance()->find("CPVAL"));
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S4F31
    msgtype = new BS2MessageType(SFCODE(4,31), BOTH_DIR, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("TRLINK"));
        listitem_1->add(BS2ItemDictionary::instance()->find("MCINDEX"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("HOACK"));
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
                }
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S4F33
    msgtype = new BS2MessageType(SFCODE(4,33), BOTH_DIR, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("TRLINK"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("HOACK"));
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
                }
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S4F35
    msgtype = new BS2MessageType(SFCODE(4,35), BOTH_DIR, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("TRLINK"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S4F37
    msgtype = new BS2MessageType(SFCODE(4,37), BOTH_DIR, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("TRLINK"));
        listitem_1->add(BS2ItemDictionary::instance()->find("HOCANCELACK"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S4F39
    msgtype = new BS2MessageType(SFCODE(4,39), BOTH_DIR, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("TRLINK"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S4F41
    msgtype = new BS2MessageType(SFCODE(4,41), BOTH_DIR, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("TRLINK"));
        listitem_1->add(BS2ItemDictionary::instance()->find("HOHALTACK"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S5F0
    msgtype = new BS2MessageType(SFCODE(5,0), BOTH_DIR, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    BS2MessageDictionary::instance()->add(msgtype);

    // S5F1
    msgtype = new BS2MessageType(SFCODE(5,1), EQUIPMENT_TO_HOST, REPLY_REQUIRED, "ALARM", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("ALCD"));
        listitem_1->add(BS2ItemDictionary::instance()->find("ALID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("ALTX"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S5F2
    msgtype = new BS2MessageType(SFCODE(5,2), HOST_TO_EQUIPMENT, REPLY_NONE, "ALARM", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("ACKC5"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S5F3
    msgtype = new BS2MessageType(SFCODE(5,3), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "ALARM", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("ALED"));
        listitem_1->add(BS2ItemDictionary::instance()->find("ALID"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S5F4
    msgtype = new BS2MessageType(SFCODE(5,4), EQUIPMENT_TO_HOST, REPLY_NONE, "ALARM", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("ACKC5"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S5F5
    msgtype = new BS2MessageType(SFCODE(5,5), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "ALARM", SINGLE_BLOCK_MESSAGE);
    //msgtype->add(BS2ItemDictionary::instance()->find("ALID"));
	msgtype->add(BS2ItemDictionary::instance()->find("ALIDARRAY")); //stone
    BS2MessageDictionary::instance()->add(msgtype);

    // S5F6
    msgtype = new BS2MessageType(SFCODE(5,6), EQUIPMENT_TO_HOST, REPLY_NONE, "ALARM", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("ALCD"));
            listitem_2->add(BS2ItemDictionary::instance()->find("ALID"));
            listitem_2->add(BS2ItemDictionary::instance()->find("ALTX"));
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S5F7
    msgtype = new BS2MessageType(SFCODE(5,7), HOST_TO_EQUIPMENT, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    BS2MessageDictionary::instance()->add(msgtype);

    // S5F8
    msgtype = new BS2MessageType(SFCODE(5,8), EQUIPMENT_TO_HOST, REPLY_NONE, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("ALCD"));
            listitem_2->add(BS2ItemDictionary::instance()->find("ALID"));
            listitem_2->add(BS2ItemDictionary::instance()->find("ALTX"));
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S5F9
    msgtype = new BS2MessageType(SFCODE(5,9), EQUIPMENT_TO_HOST, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("TIMESTAMP"));
        listitem_1->add(BS2ItemDictionary::instance()->find("EXID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("EXTYPE"));
        listitem_1->add(BS2ItemDictionary::instance()->find("EXMESSAGE"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("EXRECVRA"));
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S5F10
    msgtype = new BS2MessageType(SFCODE(5,10), HOST_TO_EQUIPMENT, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    BS2MessageDictionary::instance()->add(msgtype);

    // S5F11
    msgtype = new BS2MessageType(SFCODE(5,11), EQUIPMENT_TO_HOST, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("TIMESTAMP"));
        listitem_1->add(BS2ItemDictionary::instance()->find("EXID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("EXTYPE"));
        listitem_1->add(BS2ItemDictionary::instance()->find("EXMESSAGE"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S5F12
    msgtype = new BS2MessageType(SFCODE(5,12), HOST_TO_EQUIPMENT, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    BS2MessageDictionary::instance()->add(msgtype);

    // S5F13
    msgtype = new BS2MessageType(SFCODE(5,13), EQUIPMENT_TO_HOST, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("EXID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("EXRECVRA"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S5F14
    msgtype = new BS2MessageType(SFCODE(5,14), HOST_TO_EQUIPMENT, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("EXID"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("ACKA"));
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                listitem_3->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S5F15
    msgtype = new BS2MessageType(SFCODE(5,15), EQUIPMENT_TO_HOST, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("TIMESTAMP"));
        listitem_1->add(BS2ItemDictionary::instance()->find("EXID"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("ACKA"));
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                listitem_3->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S5F16
    msgtype = new BS2MessageType(SFCODE(5,16), HOST_TO_EQUIPMENT, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    BS2MessageDictionary::instance()->add(msgtype);

    // S5F17
    msgtype = new BS2MessageType(SFCODE(5,17), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("EXID"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S5F18
    msgtype = new BS2MessageType(SFCODE(5,18), EQUIPMENT_TO_HOST, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("EXID"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("ACKA"));
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                listitem_3->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S6F0
    msgtype = new BS2MessageType(SFCODE(6,0), BOTH_DIR, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    BS2MessageDictionary::instance()->add(msgtype);

    // S6F1
    msgtype = new BS2MessageType(SFCODE(6,1), EQUIPMENT_TO_HOST, REPLY_REQUIRED, "TRACE", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("TRID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("SMPLN"));
        listitem_1->add(BS2ItemDictionary::instance()->find("STIME"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("SV"));
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S6F2
    msgtype = new BS2MessageType(SFCODE(6,2), HOST_TO_EQUIPMENT, REPLY_NONE, "TRACE", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("ACKC6"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S6F3
    msgtype = new BS2MessageType(SFCODE(6,3), EQUIPMENT_TO_HOST, REPLY_REQUIRED, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("DATAID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("CEID"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("DSID"));
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    {
                        BS2Itemtypes * listitem_5 = new BS2Itemtypes();
                        listitem_4->add(listitem_5);
                        listitem_5->add(BS2ItemDictionary::instance()->find("DVNAME"));
                        listitem_5->add(BS2ItemDictionary::instance()->find("DVVAL"));
                    }
                }
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S6F4
    msgtype = new BS2MessageType(SFCODE(6,4), HOST_TO_EQUIPMENT, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("ACKC6"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S6F5
    msgtype = new BS2MessageType(SFCODE(6,5), EQUIPMENT_TO_HOST, REPLY_REQUIRED, "INFO", INQUIRE_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("DATAID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("DATALENGTH"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S6F6
    msgtype = new BS2MessageType(SFCODE(6,6), HOST_TO_EQUIPMENT, REPLY_NONE, "INFO", GRANT_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("GRANT6"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S6F7
    msgtype = new BS2MessageType(SFCODE(6,7), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("DATAID"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S6F8
    msgtype = new BS2MessageType(SFCODE(6,8), EQUIPMENT_TO_HOST, REPLY_NONE, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("DATAID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("CEID"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("DSID"));
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    {
                        BS2Itemtypes * listitem_5 = new BS2Itemtypes();
                        listitem_4->add(listitem_5);
                        listitem_5->add(BS2ItemDictionary::instance()->find("DVNAME"));
                        listitem_5->add(BS2ItemDictionary::instance()->find("DVVAL"));
                    }
                }
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S6F9
    msgtype = new BS2MessageType(SFCODE(6,9), EQUIPMENT_TO_HOST, REPLY_REQUIRED, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("PFCD"));
        listitem_1->add(BS2ItemDictionary::instance()->find("DATAID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("CEID"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("DSID"));
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    listitem_4->add(BS2ItemDictionary::instance()->find("DVVAL"));
                }
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S6F10
    msgtype = new BS2MessageType(SFCODE(6,10), HOST_TO_EQUIPMENT, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("ACKC6"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S6F11
    msgtype = new BS2MessageType(SFCODE(6,11), EQUIPMENT_TO_HOST, REPLY_REQUIRED, "EVENT", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("DATAID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("CEID"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("RPTID"));
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    listitem_4->add(BS2ItemDictionary::instance()->find("V"));
                }
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S6F12
    msgtype = new BS2MessageType(SFCODE(6,12), HOST_TO_EQUIPMENT, REPLY_NONE, "EVENT", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("ACKC6"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S6F13
    msgtype = new BS2MessageType(SFCODE(6,13), EQUIPMENT_TO_HOST, REPLY_REQUIRED, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("DATAID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("CEID"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("RPTID"));
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    {
                        BS2Itemtypes * listitem_5 = new BS2Itemtypes();
                        listitem_4->add(listitem_5);
                        listitem_5->add(BS2ItemDictionary::instance()->find("VID"));
                        listitem_5->add(BS2ItemDictionary::instance()->find("V"));
                    }
                }
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S6F14
    msgtype = new BS2MessageType(SFCODE(6,14), HOST_TO_EQUIPMENT, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("ACKC6"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S6F15
    msgtype = new BS2MessageType(SFCODE(6,15), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "EVENT", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("CEID"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S6F16
    msgtype = new BS2MessageType(SFCODE(6,16), EQUIPMENT_TO_HOST, REPLY_NONE, "EVENT", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("DATAID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("CEID"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("RPTID"));
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    listitem_4->add(BS2ItemDictionary::instance()->find("V"));
                }
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S6F17
    msgtype = new BS2MessageType(SFCODE(6,17), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("CEID"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S6F18
    msgtype = new BS2MessageType(SFCODE(6,18), EQUIPMENT_TO_HOST, REPLY_NONE, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("DATAID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("CEID"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("RPTID"));
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    {
                        BS2Itemtypes * listitem_5 = new BS2Itemtypes();
                        listitem_4->add(listitem_5);
                        listitem_5->add(BS2ItemDictionary::instance()->find("VID"));
                        listitem_5->add(BS2ItemDictionary::instance()->find("V"));
                    }
                }
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S6F19
    msgtype = new BS2MessageType(SFCODE(6,19), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "EVENT", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("RPTID"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S6F20
    msgtype = new BS2MessageType(SFCODE(6,20), EQUIPMENT_TO_HOST, REPLY_NONE, "EVENT", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("V"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S6F21
    msgtype = new BS2MessageType(SFCODE(6,21), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("RPTID"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S6F22
    msgtype = new BS2MessageType(SFCODE(6,22), EQUIPMENT_TO_HOST, REPLY_NONE, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("VID"));
            listitem_2->add(BS2ItemDictionary::instance()->find("V"));
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S6F23
    msgtype = new BS2MessageType(SFCODE(6,23), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "SPOOL", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("RSDC"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S6F24
    msgtype = new BS2MessageType(SFCODE(6,24), EQUIPMENT_TO_HOST, REPLY_NONE, "SPOOL", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("RSDA"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S6F25
    msgtype = new BS2MessageType(SFCODE(6,25), BOTH_DIR, REPLY_REQUIRED, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("DATAID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("OPID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("LINKID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("RCPSPEC"));
        listitem_1->add(BS2ItemDictionary::instance()->find("RMCHGSTAT"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("RCPATTRID"));
                listitem_3->add(BS2ItemDictionary::instance()->find("RCPATTRDATA"));
            }
        }
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("RMACK"));
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
                }
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S6F26
    msgtype = new BS2MessageType(SFCODE(6,26), BOTH_DIR, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("ACKC6"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S6F27
    msgtype = new BS2MessageType(SFCODE(6,27), EQUIPMENT_TO_HOST, REPLY_REQUIRED, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("DATAID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("TRID"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("RPTID"));
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    listitem_4->add(BS2ItemDictionary::instance()->find("V"));
                }
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S6F28
    msgtype = new BS2MessageType(SFCODE(6,28), HOST_TO_EQUIPMENT, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("TRID"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S6F29
    msgtype = new BS2MessageType(SFCODE(6,29), HOST_TO_EQUIPMENT, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("TRID"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S6F30
    msgtype = new BS2MessageType(SFCODE(6,30), EQUIPMENT_TO_HOST, REPLY_NONE, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("TRID"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("RPTID"));
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    listitem_4->add(BS2ItemDictionary::instance()->find("V"));
                }
            }
        }
        listitem_1->add(BS2ItemDictionary::instance()->find("ERRCODE"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S7F0
    msgtype = new BS2MessageType(SFCODE(7,0), BOTH_DIR, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    BS2MessageDictionary::instance()->add(msgtype);

    // S7F1
    msgtype = new BS2MessageType(SFCODE(7,1), BOTH_DIR, REPLY_REQUIRED, "PROCPROG", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("PPID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("LENGTH"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S7F2
    msgtype = new BS2MessageType(SFCODE(7,2), BOTH_DIR, REPLY_NONE, "PROCPROG", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("PPGNT"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S7F3
    msgtype = new BS2MessageType(SFCODE(7,3), BOTH_DIR, REPLY_REQUIRED, "PROCPROG", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("PPID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("PPBODY"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S7F4
    msgtype = new BS2MessageType(SFCODE(7,4), BOTH_DIR, REPLY_NONE, "PROCPROG", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("ACKC7"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S7F5
    msgtype = new BS2MessageType(SFCODE(7,5), BOTH_DIR, REPLY_REQUIRED, "PROCPROG", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("PPID"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S7F6
    msgtype = new BS2MessageType(SFCODE(7,6), BOTH_DIR, REPLY_NONE, "PROCPROG", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("PPID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("PPBODY"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S7F7
    msgtype = new BS2MessageType(SFCODE(7,7), EQUIPMENT_TO_HOST, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("MID"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S7F8
    msgtype = new BS2MessageType(SFCODE(7,8), HOST_TO_EQUIPMENT, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("PPID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("MID"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S7F9
    msgtype = new BS2MessageType(SFCODE(7,9), BOTH_DIR, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    BS2MessageDictionary::instance()->add(msgtype);

    // S7F10
    msgtype = new BS2MessageType(SFCODE(7,10), BOTH_DIR, REPLY_NONE, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("PPID"));
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("MID"));
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S7F11
    msgtype = new BS2MessageType(SFCODE(7,11), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("PPID"));
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("MID"));
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S7F12
    msgtype = new BS2MessageType(SFCODE(7,12), EQUIPMENT_TO_HOST, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("ACKC7"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S7F13
    msgtype = new BS2MessageType(SFCODE(7,13), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("PPID"));
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("MID"));
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S7F14
    msgtype = new BS2MessageType(SFCODE(7,14), EQUIPMENT_TO_HOST, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("ACKC7"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S7F15
    msgtype = new BS2MessageType(SFCODE(7,15), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("MMODE"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S7F16
    msgtype = new BS2MessageType(SFCODE(7,16), EQUIPMENT_TO_HOST, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("ACKC7"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S7F17
    msgtype = new BS2MessageType(SFCODE(7,17), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "PROCPROG", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("PPID"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S7F18
    msgtype = new BS2MessageType(SFCODE(7,18), EQUIPMENT_TO_HOST, REPLY_NONE, "PROCPROG", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("ACKC7"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S7F19
    msgtype = new BS2MessageType(SFCODE(7,19), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "PROCPROG", SINGLE_BLOCK_MESSAGE);
    BS2MessageDictionary::instance()->add(msgtype);

    // S7F20
    msgtype = new BS2MessageType(SFCODE(7,20), EQUIPMENT_TO_HOST, REPLY_NONE, "PROCPROG", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("PPID"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S7F21
    msgtype = new BS2MessageType(SFCODE(7,21), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    BS2MessageDictionary::instance()->add(msgtype);

    // S7F22
    msgtype = new BS2MessageType(SFCODE(7,22), EQUIPMENT_TO_HOST, REPLY_NONE, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("MDLN"));
        listitem_1->add(BS2ItemDictionary::instance()->find("SOFTREV"));
        listitem_1->add(BS2ItemDictionary::instance()->find("CMDMAX"));
        listitem_1->add(BS2ItemDictionary::instance()->find("BYTMAX"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("CCODE"));
                listitem_3->add(BS2ItemDictionary::instance()->find("CNAME"));
                listitem_3->add(BS2ItemDictionary::instance()->find("RQCMD"));
                listitem_3->add(BS2ItemDictionary::instance()->find("BLKDEF"));
                listitem_3->add(BS2ItemDictionary::instance()->find("BCDS"));
                listitem_3->add(BS2ItemDictionary::instance()->find("IBCDS"));
                listitem_3->add(BS2ItemDictionary::instance()->find("NBCDS"));
                listitem_3->add(BS2ItemDictionary::instance()->find("ACDS"));
                listitem_3->add(BS2ItemDictionary::instance()->find("IACDS"));
                listitem_3->add(BS2ItemDictionary::instance()->find("NACDS"));
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    listitem_4->add(BS2ItemDictionary::instance()->find("PNAME"));
                    listitem_4->add(BS2ItemDictionary::instance()->find("RQPAR"));
                    listitem_4->add(BS2ItemDictionary::instance()->find("PDFLT"));
                    listitem_4->add(BS2ItemDictionary::instance()->find("PMAX"));
                    listitem_4->add(BS2ItemDictionary::instance()->find("LLIM"));
                    listitem_4->add(BS2ItemDictionary::instance()->find("ULIM"));
                    listitem_4->add(BS2ItemDictionary::instance()->find("UNITS"));
                    listitem_4->add(BS2ItemDictionary::instance()->find("RESC"));
                    listitem_4->add(BS2ItemDictionary::instance()->find("RESV"));
                }
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S7F23
    msgtype = new BS2MessageType(SFCODE(7,23), BOTH_DIR, REPLY_REQUIRED, "PROCPROG", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("PPID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("MDLN"));
        listitem_1->add(BS2ItemDictionary::instance()->find("SOFTREV"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("CCODE"));
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    listitem_4->add(BS2ItemDictionary::instance()->find("PPARM"));
                }
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S7F24
    msgtype = new BS2MessageType(SFCODE(7,24), BOTH_DIR, REPLY_NONE, "PROCPROG", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("ACKC7"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S7F25
    msgtype = new BS2MessageType(SFCODE(7,25), BOTH_DIR, REPLY_REQUIRED, "PROCPROG", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("PPID"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S7F26
    msgtype = new BS2MessageType(SFCODE(7,26), BOTH_DIR, REPLY_NONE, "PROCPROG", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("PPID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("MDLN"));
        listitem_1->add(BS2ItemDictionary::instance()->find("SOFTREV"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("CCODE"));
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    listitem_4->add(BS2ItemDictionary::instance()->find("PPARM"));
                }
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S7F27
    msgtype = new BS2MessageType(SFCODE(7,27), EQUIPMENT_TO_HOST, REPLY_REQUIRED, "PROCPROG", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("PPID"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("ACKC7A"));
                listitem_3->add(BS2ItemDictionary::instance()->find("SEQNUM"));
                listitem_3->add(BS2ItemDictionary::instance()->find("ERRW7"));
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S7F28
    msgtype = new BS2MessageType(SFCODE(7,28), HOST_TO_EQUIPMENT, REPLY_NONE, "PROCPROG", SINGLE_BLOCK_MESSAGE);
    BS2MessageDictionary::instance()->add(msgtype);

    // S7F29
    msgtype = new BS2MessageType(SFCODE(7,29), EQUIPMENT_TO_HOST, REPLY_REQUIRED, "PROCPROG", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("LENGTH"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S7F30
    msgtype = new BS2MessageType(SFCODE(7,30), HOST_TO_EQUIPMENT, REPLY_NONE, "PROCPROG", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("PPGNT"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S7F31
    msgtype = new BS2MessageType(SFCODE(7,31), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("PPID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("MDLN"));
        listitem_1->add(BS2ItemDictionary::instance()->find("SOFTREV"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("CCODE"));
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    listitem_4->add(BS2ItemDictionary::instance()->find("PPARM"));
                }
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S7F32
    msgtype = new BS2MessageType(SFCODE(7,32), EQUIPMENT_TO_HOST, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("ACKC7"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S7F33
    msgtype = new BS2MessageType(SFCODE(7,33), BOTH_DIR, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("PPID"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S7F34
    msgtype = new BS2MessageType(SFCODE(7,34), BOTH_DIR, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("PPID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("UNFLEN"));
        listitem_1->add(BS2ItemDictionary::instance()->find("FRMLEN"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S7F35
    msgtype = new BS2MessageType(SFCODE(7,35), BOTH_DIR, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("MID"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S7F36
    msgtype = new BS2MessageType(SFCODE(7,36), BOTH_DIR, REPLY_NONE, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("MID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("PPID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("PPBODY"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S8F0
    msgtype = new BS2MessageType(SFCODE(8,0), BOTH_DIR, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    BS2MessageDictionary::instance()->add(msgtype);

    // S8F1
    msgtype = new BS2MessageType(SFCODE(8,1), BOTH_DIR, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    BS2MessageDictionary::instance()->add(msgtype);

    // S8F2
    msgtype = new BS2MessageType(SFCODE(8,2), BOTH_DIR, REPLY_NONE, "", MULTI_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("BPD"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S8F3
    msgtype = new BS2MessageType(SFCODE(8,3), BOTH_DIR, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    BS2MessageDictionary::instance()->add(msgtype);

    // S8F4
    msgtype = new BS2MessageType(SFCODE(8,4), BOTH_DIR, REPLY_NONE, "", MULTI_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("EPD"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S9F0
    msgtype = new BS2MessageType(SFCODE(9,0), BOTH_DIR, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    BS2MessageDictionary::instance()->add(msgtype);

    // S9F1
    msgtype = new BS2MessageType(SFCODE(9,1), EQUIPMENT_TO_HOST, REPLY_NONE, "COMMUNICATION", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("MHEAD"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S9F3
    msgtype = new BS2MessageType(SFCODE(9,3), EQUIPMENT_TO_HOST, REPLY_NONE, "COMMUNICATION", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("MHEAD"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S9F5
    msgtype = new BS2MessageType(SFCODE(9,5), EQUIPMENT_TO_HOST, REPLY_NONE, "COMMUNICATION", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("MHEAD"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S9F7
    msgtype = new BS2MessageType(SFCODE(9,7), EQUIPMENT_TO_HOST, REPLY_NONE, "COMMUNICATION", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("MHEAD"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S9F9
    msgtype = new BS2MessageType(SFCODE(9,9), EQUIPMENT_TO_HOST, REPLY_NONE, "COMMUNICATION", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("SHEAD"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S9F11
    msgtype = new BS2MessageType(SFCODE(9,11), EQUIPMENT_TO_HOST, REPLY_NONE, "COMMUNICATION", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("MHEAD"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S9F13
    msgtype = new BS2MessageType(SFCODE(9,13), EQUIPMENT_TO_HOST, REPLY_NONE, "COMMUNICATION", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("MEXP"));
        listitem_1->add(BS2ItemDictionary::instance()->find("EDID"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S10F0
    msgtype = new BS2MessageType(SFCODE(10,0), BOTH_DIR, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    BS2MessageDictionary::instance()->add(msgtype);

    // S10F1
    msgtype = new BS2MessageType(SFCODE(10,1), EQUIPMENT_TO_HOST, REPLY_REQUIRED, "TERMINAL", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("TID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("TEXT"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S10F2
    msgtype = new BS2MessageType(SFCODE(10,2), HOST_TO_EQUIPMENT, REPLY_NONE, "TERMINAL", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("ACKC10"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S10F3
    msgtype = new BS2MessageType(SFCODE(10,3), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "TERMINAL", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("TID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("TEXT"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S10F4
    msgtype = new BS2MessageType(SFCODE(10,4), EQUIPMENT_TO_HOST, REPLY_NONE, "TERMINAL", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("ACKC10"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S10F5
    msgtype = new BS2MessageType(SFCODE(10,5), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "TERMINAL", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("TID"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("TEXT"));
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S10F6
    msgtype = new BS2MessageType(SFCODE(10,6), EQUIPMENT_TO_HOST, REPLY_NONE, "TERMINAL", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("ACKC10"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S10F7
    msgtype = new BS2MessageType(SFCODE(10,7), EQUIPMENT_TO_HOST, REPLY_REQUIRED, "TERMINAL", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("TID"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S10F9
    msgtype = new BS2MessageType(SFCODE(10,9), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("TEXT"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S10F10
    msgtype = new BS2MessageType(SFCODE(10,10), EQUIPMENT_TO_HOST, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("ACKC10"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S11F0
    msgtype = new BS2MessageType(SFCODE(11,0), BOTH_DIR, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    BS2MessageDictionary::instance()->add(msgtype);

    // S12F0
    msgtype = new BS2MessageType(SFCODE(12,0), BOTH_DIR, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    BS2MessageDictionary::instance()->add(msgtype);

    // S12F1
    msgtype = new BS2MessageType(SFCODE(12,1), EQUIPMENT_TO_HOST, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("MID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("IDTYP"));
        listitem_1->add(BS2ItemDictionary::instance()->find("FNLOC"));
        listitem_1->add(BS2ItemDictionary::instance()->find("FFROT"));
        listitem_1->add(BS2ItemDictionary::instance()->find("ORLOC"));
        listitem_1->add(BS2ItemDictionary::instance()->find("RPSEL"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("REFP"));
        }
        listitem_1->add(BS2ItemDictionary::instance()->find("DUTMS"));
        listitem_1->add(BS2ItemDictionary::instance()->find("XDIES"));
        listitem_1->add(BS2ItemDictionary::instance()->find("YDIES"));
        listitem_1->add(BS2ItemDictionary::instance()->find("ROWCT"));
        listitem_1->add(BS2ItemDictionary::instance()->find("COLCT"));
        listitem_1->add(BS2ItemDictionary::instance()->find("NULBC"));
        listitem_1->add(BS2ItemDictionary::instance()->find("PRDCT"));
        listitem_1->add(BS2ItemDictionary::instance()->find("PRAXI"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S12F2
    msgtype = new BS2MessageType(SFCODE(12,2), HOST_TO_EQUIPMENT, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("SDACK"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S12F3
    msgtype = new BS2MessageType(SFCODE(12,3), EQUIPMENT_TO_HOST, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("MID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("IDTYP"));
        listitem_1->add(BS2ItemDictionary::instance()->find("MAPFT"));
        listitem_1->add(BS2ItemDictionary::instance()->find("FNLOC"));
        listitem_1->add(BS2ItemDictionary::instance()->find("FFROT"));
        listitem_1->add(BS2ItemDictionary::instance()->find("ORLOC"));
        listitem_1->add(BS2ItemDictionary::instance()->find("PRAXI"));
        listitem_1->add(BS2ItemDictionary::instance()->find("BCEQU"));
        listitem_1->add(BS2ItemDictionary::instance()->find("NULBC"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S12F4
    msgtype = new BS2MessageType(SFCODE(12,4), HOST_TO_EQUIPMENT, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("MID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("IDTYP"));
        listitem_1->add(BS2ItemDictionary::instance()->find("FNLOC"));
        listitem_1->add(BS2ItemDictionary::instance()->find("ORLOC"));
        listitem_1->add(BS2ItemDictionary::instance()->find("RPSEL"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("REFP"));
        }
        listitem_1->add(BS2ItemDictionary::instance()->find("DUTMS"));
        listitem_1->add(BS2ItemDictionary::instance()->find("XDIES"));
        listitem_1->add(BS2ItemDictionary::instance()->find("YDIES"));
        listitem_1->add(BS2ItemDictionary::instance()->find("ROWCT"));
        listitem_1->add(BS2ItemDictionary::instance()->find("COLCT"));
        listitem_1->add(BS2ItemDictionary::instance()->find("PRDCT"));
        listitem_1->add(BS2ItemDictionary::instance()->find("BCEQU"));
        listitem_1->add(BS2ItemDictionary::instance()->find("NULBC"));
        listitem_1->add(BS2ItemDictionary::instance()->find("MLCL"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S12F5
    msgtype = new BS2MessageType(SFCODE(12,5), EQUIPMENT_TO_HOST, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("MID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("IDTYP"));
        listitem_1->add(BS2ItemDictionary::instance()->find("MAPFT"));
        listitem_1->add(BS2ItemDictionary::instance()->find("MLCL"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S12F6
    msgtype = new BS2MessageType(SFCODE(12,6), HOST_TO_EQUIPMENT, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("GRNT1"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S12F7
    msgtype = new BS2MessageType(SFCODE(12,7), EQUIPMENT_TO_HOST, REPLY_REQUIRED, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("MID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("IDTYP"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("RSINF"));
                listitem_3->add(BS2ItemDictionary::instance()->find("BINLT"));
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S12F8
    msgtype = new BS2MessageType(SFCODE(12,8), HOST_TO_EQUIPMENT, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("MDACK"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S12F9
    msgtype = new BS2MessageType(SFCODE(12,9), EQUIPMENT_TO_HOST, REPLY_REQUIRED, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("MID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("IDTYP"));
        listitem_1->add(BS2ItemDictionary::instance()->find("STRP"));
        listitem_1->add(BS2ItemDictionary::instance()->find("BINLT"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S12F10
    msgtype = new BS2MessageType(SFCODE(12,10), HOST_TO_EQUIPMENT, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("MDACK"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S12F11
    msgtype = new BS2MessageType(SFCODE(12,11), EQUIPMENT_TO_HOST, REPLY_REQUIRED, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("MID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("IDTYP"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("XYPOS"));
                listitem_3->add(BS2ItemDictionary::instance()->find("BINLT"));
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S12F12
    msgtype = new BS2MessageType(SFCODE(12,12), HOST_TO_EQUIPMENT, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("MDACK"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S12F13
    msgtype = new BS2MessageType(SFCODE(12,13), EQUIPMENT_TO_HOST, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("MID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("IDTYP"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S12F14
    msgtype = new BS2MessageType(SFCODE(12,14), HOST_TO_EQUIPMENT, REPLY_NONE, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("MID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("IDTYP"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("RSINF"));
                listitem_3->add(BS2ItemDictionary::instance()->find("BINLT"));
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S12F15
    msgtype = new BS2MessageType(SFCODE(12,15), EQUIPMENT_TO_HOST, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("MID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("IDTYP"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S12F16
    msgtype = new BS2MessageType(SFCODE(12,16), HOST_TO_EQUIPMENT, REPLY_NONE, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("MID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("IDTYP"));
        listitem_1->add(BS2ItemDictionary::instance()->find("STRP"));
        listitem_1->add(BS2ItemDictionary::instance()->find("BINLT"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S12F17
    msgtype = new BS2MessageType(SFCODE(12,17), EQUIPMENT_TO_HOST, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("MID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("IDTYP"));
        listitem_1->add(BS2ItemDictionary::instance()->find("SDBIN"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S12F18
    msgtype = new BS2MessageType(SFCODE(12,18), HOST_TO_EQUIPMENT, REPLY_NONE, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("MID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("IDTYP"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("RSINF"));
                listitem_3->add(BS2ItemDictionary::instance()->find("BINLT"));
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S12F19
    msgtype = new BS2MessageType(SFCODE(12,19), BOTH_DIR, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("MAPER"));
        listitem_1->add(BS2ItemDictionary::instance()->find("DATLC"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S13F0
    msgtype = new BS2MessageType(SFCODE(13,0), BOTH_DIR, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    BS2MessageDictionary::instance()->add(msgtype);

    // S13F1
    msgtype = new BS2MessageType(SFCODE(13,1), BOTH_DIR, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("DSNAME"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S13F2
    msgtype = new BS2MessageType(SFCODE(13,2), BOTH_DIR, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("DSNAME"));
        listitem_1->add(BS2ItemDictionary::instance()->find("ACKC13"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S13F3
    msgtype = new BS2MessageType(SFCODE(13,3), BOTH_DIR, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("HANDLE"));
        listitem_1->add(BS2ItemDictionary::instance()->find("DSNAME"));
        listitem_1->add(BS2ItemDictionary::instance()->find("CKPNT"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S13F4
    msgtype = new BS2MessageType(SFCODE(13,4), BOTH_DIR, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("HANDLE"));
        listitem_1->add(BS2ItemDictionary::instance()->find("DSNAME"));
        listitem_1->add(BS2ItemDictionary::instance()->find("ACKC13"));
        listitem_1->add(BS2ItemDictionary::instance()->find("RTYPE"));
        listitem_1->add(BS2ItemDictionary::instance()->find("RECLEN"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S13F5
    msgtype = new BS2MessageType(SFCODE(13,5), BOTH_DIR, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("HANDLE"));
        listitem_1->add(BS2ItemDictionary::instance()->find("READLN"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S13F6
    msgtype = new BS2MessageType(SFCODE(13,6), BOTH_DIR, REPLY_NONE, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("HANDLE"));
        listitem_1->add(BS2ItemDictionary::instance()->find("ACKC13"));
        listitem_1->add(BS2ItemDictionary::instance()->find("CKPNT"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("FILDAT"));
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S13F7
    msgtype = new BS2MessageType(SFCODE(13,7), BOTH_DIR, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("HANDLE"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S13F8
    msgtype = new BS2MessageType(SFCODE(13,8), BOTH_DIR, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("HANDLE"));
        listitem_1->add(BS2ItemDictionary::instance()->find("ACKC13"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S13F9
    msgtype = new BS2MessageType(SFCODE(13,9), BOTH_DIR, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    BS2MessageDictionary::instance()->add(msgtype);

    // S13F10
    msgtype = new BS2MessageType(SFCODE(13,10), BOTH_DIR, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    BS2MessageDictionary::instance()->add(msgtype);

    // S13F11
    msgtype = new BS2MessageType(SFCODE(13,11), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("DATAID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("OBJSPEC"));
        listitem_1->add(BS2ItemDictionary::instance()->find("DATALENGTH"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S13F12
    msgtype = new BS2MessageType(SFCODE(13,12), BOTH_DIR, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("GRANT"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S13F13
    msgtype = new BS2MessageType(SFCODE(13,13), BOTH_DIR, REPLY_REQUIRED, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("DATAID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("OBJSPEC"));
        listitem_1->add(BS2ItemDictionary::instance()->find("TBLTYP"));
        listitem_1->add(BS2ItemDictionary::instance()->find("TBLID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("TBLCMD"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("ATTRID"));
                listitem_3->add(BS2ItemDictionary::instance()->find("ATTRDATA"));
            }
        }
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("COLHDR"));
        }
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("TBLELT"));
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S13F14
    msgtype = new BS2MessageType(SFCODE(13,14), BOTH_DIR, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("TBLACK"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                listitem_3->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S13F15
    msgtype = new BS2MessageType(SFCODE(13,15), BOTH_DIR, REPLY_REQUIRED, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("DATAID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("OBJSPEC"));
        listitem_1->add(BS2ItemDictionary::instance()->find("TBLTYP"));
        listitem_1->add(BS2ItemDictionary::instance()->find("TBLID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("TBLCMD"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("COLHDR"));
        }
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("TBLELT"));
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S13F16
    msgtype = new BS2MessageType(SFCODE(13,16), BOTH_DIR, REPLY_NONE, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("TBLTYP"));
        listitem_1->add(BS2ItemDictionary::instance()->find("TBLID"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("ATTRID"));
                listitem_3->add(BS2ItemDictionary::instance()->find("ATTRDATA"));
            }
        }
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("COLHDR"));
        }
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("TBLELT"));
            }
        }
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("TBLACK"));
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
                }
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S14F0
    msgtype = new BS2MessageType(SFCODE(14,0), BOTH_DIR, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    BS2MessageDictionary::instance()->add(msgtype);

    // S14F1
    msgtype = new BS2MessageType(SFCODE(14,1), BOTH_DIR, REPLY_NONE, "OBJECT", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("OBJSPEC"));
        listitem_1->add(BS2ItemDictionary::instance()->find("OBJTYPE"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("OBJID"));
        }
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("ATTRID"));
                listitem_3->add(BS2ItemDictionary::instance()->find("ATTRDATA"));
                listitem_3->add(BS2ItemDictionary::instance()->find("ATTRRELN"));
            }
        }
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("ATTRID"));
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S14F2
    msgtype = new BS2MessageType(SFCODE(14,2), BOTH_DIR, REPLY_NONE, "OBJECT", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("OBJID"));
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    {
                        BS2Itemtypes * listitem_5 = new BS2Itemtypes();
                        listitem_4->add(listitem_5);
                        listitem_5->add(BS2ItemDictionary::instance()->find("ATTRID"));
                        listitem_5->add(BS2ItemDictionary::instance()->find("ATTRDATA"));
                    }
                }
            }
        }
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("OBJACK"));
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
                }
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S14F3
    msgtype = new BS2MessageType(SFCODE(14,3), BOTH_DIR, REPLY_REQUIRED, "OBJECT", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("OBJSPEC"));
        listitem_1->add(BS2ItemDictionary::instance()->find("OBJTYPE"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("OBJID"));
        }
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("ATTRID"));
                listitem_3->add(BS2ItemDictionary::instance()->find("ATTRDATA"));
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S14F4
    msgtype = new BS2MessageType(SFCODE(14,4), BOTH_DIR, REPLY_NONE, "OBJECT", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("OBJID"));
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    {
                        BS2Itemtypes * listitem_5 = new BS2Itemtypes();
                        listitem_4->add(listitem_5);
                        listitem_5->add(BS2ItemDictionary::instance()->find("ATTRID"));
                        listitem_5->add(BS2ItemDictionary::instance()->find("ATTRDATA"));
                    }
                }
            }
        }
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("OBJACK"));
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
                }
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S14F5
    msgtype = new BS2MessageType(SFCODE(14,5), BOTH_DIR, REPLY_NONE, "OBJECT", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("OBJSPEC"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S14F6
    msgtype = new BS2MessageType(SFCODE(14,6), EQUIPMENT_TO_HOST, REPLY_NONE, "OBJECT", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("OBJTYPE"));
        }
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("OBJACK"));
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
                }
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S14F7
    msgtype = new BS2MessageType(SFCODE(14,7), BOTH_DIR, REPLY_NONE, "OBJECT", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("OBJSPEC"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("OBJTYPE"));
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S14F8
    msgtype = new BS2MessageType(SFCODE(14,8), BOTH_DIR, REPLY_NONE, "OBJECT", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("OBJTYPE"));
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    listitem_4->add(BS2ItemDictionary::instance()->find("ATTRID"));
                }
            }
        }
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("OBJACK"));
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
                }
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S14F9
    msgtype = new BS2MessageType(SFCODE(14,9), BOTH_DIR, REPLY_REQUIRED, "OBJECT", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("OBJSPEC"));
        listitem_1->add(BS2ItemDictionary::instance()->find("OBJTYPE"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("ATTRID"));
                listitem_3->add(BS2ItemDictionary::instance()->find("ATTRDATA"));
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S14F10
    msgtype = new BS2MessageType(SFCODE(14,10), BOTH_DIR, REPLY_NONE, "OBJECT", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("OBJSPEC"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("ATTRID"));
                listitem_3->add(BS2ItemDictionary::instance()->find("ATTRDATA"));
            }
        }
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("OBJACK"));
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
                }
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S14F11
    msgtype = new BS2MessageType(SFCODE(14,11), BOTH_DIR, REPLY_REQUIRED, "OBJECT", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("OBJSPEC"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("ATTRID"));
                listitem_3->add(BS2ItemDictionary::instance()->find("ATTRDATA"));
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S14F12
    msgtype = new BS2MessageType(SFCODE(14,12), BOTH_DIR, REPLY_NONE, "OBJECT", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("ATTRID"));
                listitem_3->add(BS2ItemDictionary::instance()->find("ATTRDATA"));
            }
        }
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("OBJACK"));
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
                }
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S14F13
    msgtype = new BS2MessageType(SFCODE(14,13), BOTH_DIR, REPLY_REQUIRED, "OBJECT", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("OBJSPEC"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("ATTRID"));
                listitem_3->add(BS2ItemDictionary::instance()->find("ATTRDATA"));
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S14F14
    msgtype = new BS2MessageType(SFCODE(14,14), BOTH_DIR, REPLY_NONE, "OBJECT", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("OBJTOKEN"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("ATTRID"));
                listitem_3->add(BS2ItemDictionary::instance()->find("ATTRDATA"));
            }
        }
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("OBJACK"));
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
                }
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S14F15
    msgtype = new BS2MessageType(SFCODE(14,15), BOTH_DIR, REPLY_REQUIRED, "OBJECT", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("OBJSPEC"));
        listitem_1->add(BS2ItemDictionary::instance()->find("OBJCMD"));
        listitem_1->add(BS2ItemDictionary::instance()->find("OBJTOKEN"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("ATTRID"));
                listitem_3->add(BS2ItemDictionary::instance()->find("ATTRDATA"));
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S14F16
    msgtype = new BS2MessageType(SFCODE(14,16), BOTH_DIR, REPLY_NONE, "OBJECT", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("ATTRID"));
                listitem_3->add(BS2ItemDictionary::instance()->find("ATTRDATA"));
            }
        }
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("OBJACK"));
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
                }
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S14F17
    msgtype = new BS2MessageType(SFCODE(14,17), BOTH_DIR, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("OBJSPEC"));
        listitem_1->add(BS2ItemDictionary::instance()->find("OBJCMD"));
        listitem_1->add(BS2ItemDictionary::instance()->find("TARGETSPEC"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("ATTRID"));
                listitem_3->add(BS2ItemDictionary::instance()->find("ATTRDATA"));
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S14F18
    msgtype = new BS2MessageType(SFCODE(14,18), BOTH_DIR, REPLY_NONE, "OBJECT", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("ATTRID"));
                listitem_3->add(BS2ItemDictionary::instance()->find("ATTRDATA"));
            }
        }
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("OBJACK"));
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
                }
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S14F19
    msgtype = new BS2MessageType(SFCODE(14,19), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("DATAID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("OPID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("OBJSPEC"));
        listitem_1->add(BS2ItemDictionary::instance()->find("SVCNAME"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("SPNAME"));
                listitem_3->add(BS2ItemDictionary::instance()->find("SPVAL"));
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S14F20
    msgtype = new BS2MessageType(SFCODE(14,20), EQUIPMENT_TO_HOST, REPLY_NONE, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("SVCACK"));
        listitem_1->add(BS2ItemDictionary::instance()->find("LINKID"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("SPNAME"));
                listitem_3->add(BS2ItemDictionary::instance()->find("SPVAL"));
            }
        }
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("OBJACK"));
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
                }
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S14F21
    msgtype = new BS2MessageType(SFCODE(14,21), EQUIPMENT_TO_HOST, REPLY_REQUIRED, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("DATAID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("OPID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("LINKID"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("SPNAME"));
                listitem_3->add(BS2ItemDictionary::instance()->find("SPVAL"));
            }
        }
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("OBJACK"));
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
                }
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S14F22
    msgtype = new BS2MessageType(SFCODE(14,22), HOST_TO_EQUIPMENT, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("DATAACK"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S14F23
    msgtype = new BS2MessageType(SFCODE(14,23), BOTH_DIR, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("DATAID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("DATALENGTH"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S14F24
    msgtype = new BS2MessageType(SFCODE(14,24), BOTH_DIR, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("GRANT"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S14F25
    msgtype = new BS2MessageType(SFCODE(14,25), HOST_TO_EQUIPMENT, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("OBJSPEC"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("OBJTYPE"));
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S14F26
    msgtype = new BS2MessageType(SFCODE(14,26), EQUIPMENT_TO_HOST, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("OBJTYPE"));
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    listitem_4->add(BS2ItemDictionary::instance()->find("SVCNAME"));
                }
            }
        }
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("OBJACK"));
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
                }
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S14F27
    msgtype = new BS2MessageType(SFCODE(14,27), HOST_TO_EQUIPMENT, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("OBJSPEC"));
        listitem_1->add(BS2ItemDictionary::instance()->find("OBJTYPE"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("SVCNAME"));
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S14F28
    msgtype = new BS2MessageType(SFCODE(14,28), EQUIPMENT_TO_HOST, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("SVCNAME"));
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    listitem_4->add(BS2ItemDictionary::instance()->find("SPNAME"));
                }
            }
        }
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("OBJACK"));
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
                }
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S15F0
    msgtype = new BS2MessageType(SFCODE(15,0), BOTH_DIR, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    BS2MessageDictionary::instance()->add(msgtype);

    // S15F1
    msgtype = new BS2MessageType(SFCODE(15,1), BOTH_DIR, REPLY_REQUIRED, "RECIPE", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("DATAID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("RCPSPEC"));
        listitem_1->add(BS2ItemDictionary::instance()->find("RMDATASIZE"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S15F2
    msgtype = new BS2MessageType(SFCODE(15,2), BOTH_DIR, REPLY_NONE, "RECIPE", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("RMGRNT"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S15F3
    msgtype = new BS2MessageType(SFCODE(15,3), BOTH_DIR, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("RMNSSPEC"));
        listitem_1->add(BS2ItemDictionary::instance()->find("RMNSCMD"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S15F4
    msgtype = new BS2MessageType(SFCODE(15,4), BOTH_DIR, REPLY_NONE, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("RMACK"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                listitem_3->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S15F5
    msgtype = new BS2MessageType(SFCODE(15,5), BOTH_DIR, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("RMNSSPEC"));
        listitem_1->add(BS2ItemDictionary::instance()->find("RMNEWNS"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S15F6
    msgtype = new BS2MessageType(SFCODE(15,6), BOTH_DIR, REPLY_NONE, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("RMACK"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                listitem_3->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S15F7
    msgtype = new BS2MessageType(SFCODE(15,7), BOTH_DIR, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("OBJSPEC"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S15F8
    msgtype = new BS2MessageType(SFCODE(15,8), BOTH_DIR, REPLY_NONE, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("RMSPACE"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("RMACK"));
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
                }
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S15F9
    msgtype = new BS2MessageType(SFCODE(15,9), BOTH_DIR, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("RCPSPEC"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S15F10
    msgtype = new BS2MessageType(SFCODE(15,10), BOTH_DIR, REPLY_NONE, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("RCPSTAT"));
        listitem_1->add(BS2ItemDictionary::instance()->find("RCPVERS"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("RMACK"));
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
                }
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S15F11
    msgtype = new BS2MessageType(SFCODE(15,11), BOTH_DIR, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("RMNSSPEC"));
        listitem_1->add(BS2ItemDictionary::instance()->find("RCPCLASS"));
        listitem_1->add(BS2ItemDictionary::instance()->find("RCPNAME"));
        listitem_1->add(BS2ItemDictionary::instance()->find("AGENT"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S15F12
    msgtype = new BS2MessageType(SFCODE(15,12), BOTH_DIR, REPLY_NONE, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("AGENT"));
        listitem_1->add(BS2ItemDictionary::instance()->find("RCPVERS"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("RMACK"));
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
                }
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S15F13
    msgtype = new BS2MessageType(SFCODE(15,13), BOTH_DIR, REPLY_REQUIRED, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("DATAID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("RCPUPDT"));
        listitem_1->add(BS2ItemDictionary::instance()->find("RCPSPEC"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("RCPATTRID"));
                listitem_3->add(BS2ItemDictionary::instance()->find("RCPATTRDATA"));
            }
        }
        listitem_1->add(BS2ItemDictionary::instance()->find("RCPBODY"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S15F14
    msgtype = new BS2MessageType(SFCODE(15,14), BOTH_DIR, REPLY_NONE, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("RMACK"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                listitem_3->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S15F15
    msgtype = new BS2MessageType(SFCODE(15,15), BOTH_DIR, REPLY_REQUIRED, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("DATAID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("RCPSPEC"));
        listitem_1->add(BS2ItemDictionary::instance()->find("RCPSECCODE"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("RCPSECNM"));
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    {
                        BS2Itemtypes * listitem_5 = new BS2Itemtypes();
                        listitem_4->add(listitem_5);
                        listitem_5->add(BS2ItemDictionary::instance()->find("RCPATTRID"));
                        listitem_5->add(BS2ItemDictionary::instance()->find("RCPATTRDATA"));
                    }
                }
            }
            listitem_2->add(BS2ItemDictionary::instance()->find("RCPBODY"));
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    listitem_4->add(BS2ItemDictionary::instance()->find("RCPSECNM"));
                    {
                        BS2Itemtypes * listitem_5 = new BS2Itemtypes();
                        listitem_4->add(listitem_5);
                        {
                            BS2Itemtypes * listitem_6 = new BS2Itemtypes();
                            listitem_5->add(listitem_6);
                            listitem_6->add(BS2ItemDictionary::instance()->find("RCPATTRID"));
                            listitem_6->add(BS2ItemDictionary::instance()->find("RCPATTRDATA"));
                        }
                    }
                }
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S15F16
    msgtype = new BS2MessageType(SFCODE(15,16), BOTH_DIR, REPLY_NONE, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("RCPSECCODE"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("RMACK"));
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
                }
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S15F17
    msgtype = new BS2MessageType(SFCODE(15,17), BOTH_DIR, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("RCPSPEC"));
        listitem_1->add(BS2ItemDictionary::instance()->find("RCPSECCODE"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S15F18
    msgtype = new BS2MessageType(SFCODE(15,18), BOTH_DIR, REPLY_NONE, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("RCPSECNM"));
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    {
                        BS2Itemtypes * listitem_5 = new BS2Itemtypes();
                        listitem_4->add(listitem_5);
                        listitem_5->add(BS2ItemDictionary::instance()->find("RCPATTRID"));
                        listitem_5->add(BS2ItemDictionary::instance()->find("RCPATTRDATA"));
                    }
                }
            }
            listitem_2->add(BS2ItemDictionary::instance()->find("RCPBODY"));
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    listitem_4->add(BS2ItemDictionary::instance()->find("RCPSECNM"));
                    {
                        BS2Itemtypes * listitem_5 = new BS2Itemtypes();
                        listitem_4->add(listitem_5);
                        {
                            BS2Itemtypes * listitem_6 = new BS2Itemtypes();
                            listitem_5->add(listitem_6);
                            listitem_6->add(BS2ItemDictionary::instance()->find("RCPATTRID"));
                            listitem_6->add(BS2ItemDictionary::instance()->find("RCPATTRDATA"));
                        }
                    }
                }
            }
        }
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("RMACK"));
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
                }
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S15F19
    msgtype = new BS2MessageType(SFCODE(15,19), BOTH_DIR, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("RCPSPEC"));
        listitem_1->add(BS2ItemDictionary::instance()->find("RCPRENAME"));
        listitem_1->add(BS2ItemDictionary::instance()->find("RCPNEWID"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S15F20
    msgtype = new BS2MessageType(SFCODE(15,20), BOTH_DIR, REPLY_NONE, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("RMACK"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                listitem_3->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S15F21
    msgtype = new BS2MessageType(SFCODE(15,21), BOTH_DIR, REPLY_REQUIRED, "RECIPE", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("DATAID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("RCPCMD"));
        listitem_1->add(BS2ItemDictionary::instance()->find("RMNSSPEC"));
        listitem_1->add(BS2ItemDictionary::instance()->find("OPID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("AGENT"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("RCPID"));
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S15F22
    msgtype = new BS2MessageType(SFCODE(15,22), BOTH_DIR, REPLY_NONE, "RECIPE", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("AGENT"));
        listitem_1->add(BS2ItemDictionary::instance()->find("LINKID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("RCPCMD"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("RMACK"));
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
                }
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S15F23
    msgtype = new BS2MessageType(SFCODE(15,23), BOTH_DIR, REPLY_REQUIRED, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("DATAID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("OBJSPEC"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("RCPID"));
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S15F24
    msgtype = new BS2MessageType(SFCODE(15,24), BOTH_DIR, REPLY_NONE, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    listitem_4->add(BS2ItemDictionary::instance()->find("RCPDESCNM"));
                    listitem_4->add(BS2ItemDictionary::instance()->find("RCPDESCTIME"));
                    listitem_4->add(BS2ItemDictionary::instance()->find("RCPDESCLTH"));
                }
            }
        }
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("RMACK"));
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
                }
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S15F25
    msgtype = new BS2MessageType(SFCODE(15,25), BOTH_DIR, REPLY_REQUIRED, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("DATAID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("RMNSSPEC"));
        listitem_1->add(BS2ItemDictionary::instance()->find("AGENT"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("RCPPARNM"));
                listitem_3->add(BS2ItemDictionary::instance()->find("RCPPARVAL"));
                listitem_3->add(BS2ItemDictionary::instance()->find("RCPPARRULE"));
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S15F26
    msgtype = new BS2MessageType(SFCODE(15,26), BOTH_DIR, REPLY_NONE, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("RMACK"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                listitem_3->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S15F27
    msgtype = new BS2MessageType(SFCODE(15,27), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "RECIPE", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("DATAID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("RCPOWCODE"));
        listitem_1->add(BS2ItemDictionary::instance()->find("RCPSPEC"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("RCPATTRID"));
                listitem_3->add(BS2ItemDictionary::instance()->find("RCPATTRDATA"));
            }
        }
        listitem_1->add(BS2ItemDictionary::instance()->find("RCPBODY"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S15F28
    msgtype = new BS2MessageType(SFCODE(15,28), EQUIPMENT_TO_HOST, REPLY_NONE, "RECIPE", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("RCPID"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("RCPATTRID"));
                listitem_3->add(BS2ItemDictionary::instance()->find("RCPATTRDATA"));
            }
        }
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("RMACK"));
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
                }
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S15F29
    msgtype = new BS2MessageType(SFCODE(15,29), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "RECIPE", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("DATAID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("OPID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("RCPSPEC"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("RCPID"));
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S15F30
    msgtype = new BS2MessageType(SFCODE(15,30), EQUIPMENT_TO_HOST, REPLY_NONE, "RECIPE", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("OPID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("LINKID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("RCPID"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("RCPATTRID"));
                listitem_3->add(BS2ItemDictionary::instance()->find("RCPATTRDATA"));
            }
        }
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("RMACK"));
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
                }
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S15F31
    msgtype = new BS2MessageType(SFCODE(15,31), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "RECIPE", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("RCPSPEC"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S15F32
    msgtype = new BS2MessageType(SFCODE(15,32), EQUIPMENT_TO_HOST, REPLY_NONE, "RECIPE", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("RCPSPEC"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("RCPATTRID"));
                listitem_3->add(BS2ItemDictionary::instance()->find("RCPATTRDATA"));
            }
        }
        listitem_1->add(BS2ItemDictionary::instance()->find("RCPBODY"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("RMACK"));
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
                }
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S15F33
    msgtype = new BS2MessageType(SFCODE(15,33), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("DATAID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("RESPEC"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("RCPID"));
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    {
                        BS2Itemtypes * listitem_5 = new BS2Itemtypes();
                        listitem_4->add(listitem_5);
                        listitem_5->add(BS2ItemDictionary::instance()->find("RCPPARNM"));
                        listitem_5->add(BS2ItemDictionary::instance()->find("RCPPARVAL"));
                    }
                }
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S15F34
    msgtype = new BS2MessageType(SFCODE(15,34), EQUIPMENT_TO_HOST, REPLY_NONE, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("RMACK"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                listitem_3->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S15F35
    msgtype = new BS2MessageType(SFCODE(15,35), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "RECIPE", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("DATAID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("RESPEC"));
        listitem_1->add(BS2ItemDictionary::instance()->find("RCPDEL"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("RCPID"));
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S15F36
    msgtype = new BS2MessageType(SFCODE(15,36), EQUIPMENT_TO_HOST, REPLY_NONE, "RECIPE", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("RMACK"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                listitem_3->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S15F37
    msgtype = new BS2MessageType(SFCODE(15,37), BOTH_DIR, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("RMSEGSPEC"));
        listitem_1->add(BS2ItemDictionary::instance()->find("OBJTOKEN"));
        listitem_1->add(BS2ItemDictionary::instance()->find("RMGRNT"));
        listitem_1->add(BS2ItemDictionary::instance()->find("OPID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("RCPID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("RMCHGTYPE"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S15F38
    msgtype = new BS2MessageType(SFCODE(15,38), BOTH_DIR, REPLY_NONE, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("RMACK"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                listitem_3->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S15F39
    msgtype = new BS2MessageType(SFCODE(15,39), BOTH_DIR, REPLY_REQUIRED, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("DATAID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("RMNSCMD"));
        listitem_1->add(BS2ItemDictionary::instance()->find("RMRECSPEC"));
        listitem_1->add(BS2ItemDictionary::instance()->find("RMSEGSPEC"));
        listitem_1->add(BS2ItemDictionary::instance()->find("OBJTOKEN"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S15F40
    msgtype = new BS2MessageType(SFCODE(15,40), BOTH_DIR, REPLY_NONE, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("RMACK"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                listitem_3->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S15F41
    msgtype = new BS2MessageType(SFCODE(15,41), BOTH_DIR, REPLY_REQUIRED, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("DATAID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("RMRECSPEC"));
        listitem_1->add(BS2ItemDictionary::instance()->find("OBJTOKEN"));
        listitem_1->add(BS2ItemDictionary::instance()->find("RMNSCMD"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("RCPID"));
            listitem_2->add(BS2ItemDictionary::instance()->find("RCPNEWID"));
            listitem_2->add(BS2ItemDictionary::instance()->find("RMSEGSPEC"));
            listitem_2->add(BS2ItemDictionary::instance()->find("RMCHGTYPE"));
            listitem_2->add(BS2ItemDictionary::instance()->find("OPID"));
            listitem_2->add(BS2ItemDictionary::instance()->find("TIMESTAMP"));
            listitem_2->add(BS2ItemDictionary::instance()->find("RMREQUESTOR"));
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S15F42
    msgtype = new BS2MessageType(SFCODE(15,42), BOTH_DIR, REPLY_NONE, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("RMACK"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                listitem_3->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S15F43
    msgtype = new BS2MessageType(SFCODE(15,43), BOTH_DIR, REPLY_REQUIRED, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("DATAID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("OBJSPEC"));
        listitem_1->add(BS2ItemDictionary::instance()->find("TARGETSPEC"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S15F44
    msgtype = new BS2MessageType(SFCODE(15,44), BOTH_DIR, REPLY_NONE, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("RCPID"));
                listitem_3->add(BS2ItemDictionary::instance()->find("RCPNEWID"));
                listitem_3->add(BS2ItemDictionary::instance()->find("RMSEGSPEC"));
                listitem_3->add(BS2ItemDictionary::instance()->find("RMCHGTYPE"));
                listitem_3->add(BS2ItemDictionary::instance()->find("OPID"));
                listitem_3->add(BS2ItemDictionary::instance()->find("TIMESTAMP"));
                listitem_3->add(BS2ItemDictionary::instance()->find("RMREQUESTOR"));
            }
        }
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("RMACK"));
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
                }
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S15F45
    msgtype = new BS2MessageType(SFCODE(15,45), BOTH_DIR, REPLY_REQUIRED, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("DATAID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("RCPSPEC"));
        listitem_1->add(BS2ItemDictionary::instance()->find("RCPNEWID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("RMCHGTYPE"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S15F46
    msgtype = new BS2MessageType(SFCODE(15,46), BOTH_DIR, REPLY_NONE, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("RMCHGTYPE"));
        listitem_1->add(BS2ItemDictionary::instance()->find("RMGRNT"));
        listitem_1->add(BS2ItemDictionary::instance()->find("OPID"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S15F47
    msgtype = new BS2MessageType(SFCODE(15,47), BOTH_DIR, REPLY_REQUIRED, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("DATAID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("OBJSPEC"));
        listitem_1->add(BS2ItemDictionary::instance()->find("RMNSSPEC"));
        listitem_1->add(BS2ItemDictionary::instance()->find("RMRECSPEC"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("RMSEGSPEC"));
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S15F48
    msgtype = new BS2MessageType(SFCODE(15,48), BOTH_DIR, REPLY_NONE, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("RMACK"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                listitem_3->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S16F0
    msgtype = new BS2MessageType(SFCODE(16,0), BOTH_DIR, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    BS2MessageDictionary::instance()->add(msgtype);

    // S16F1
    msgtype = new BS2MessageType(SFCODE(16,1), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("DATAID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("DATALENGTH"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S16F2
    msgtype = new BS2MessageType(SFCODE(16,2), EQUIPMENT_TO_HOST, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("GRANT"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S16F3
    msgtype = new BS2MessageType(SFCODE(16,3), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("DATAID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("MF"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("MID"));
        }
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("PRRECIPEMETHOD"));
            listitem_2->add(BS2ItemDictionary::instance()->find("RCPSPEC"));
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    listitem_4->add(BS2ItemDictionary::instance()->find("RCPPARNM"));
                    listitem_4->add(BS2ItemDictionary::instance()->find("RCPPARVAL"));
                }
            }
        }
        listitem_1->add(BS2ItemDictionary::instance()->find("PRPROCESSSTART"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S16F4
    msgtype = new BS2MessageType(SFCODE(16,4), EQUIPMENT_TO_HOST, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("PRJOBID"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("ACKA"));
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
                }
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S16F5
    msgtype = new BS2MessageType(SFCODE(16,5), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("DATAID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("PRJOBID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("PRCMDNAME"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("CPNAME"));
                listitem_3->add(BS2ItemDictionary::instance()->find("CPVAL"));
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S16F6
    msgtype = new BS2MessageType(SFCODE(16,6), EQUIPMENT_TO_HOST, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("PRJOBID"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("ACKA"));
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
                }
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S16F7
    msgtype = new BS2MessageType(SFCODE(16,7), EQUIPMENT_TO_HOST, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("TIMESTAMP"));
        listitem_1->add(BS2ItemDictionary::instance()->find("PRJOBID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("PRJOBMILESTONE"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("ACKA"));
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
                }
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S16F8
    msgtype = new BS2MessageType(SFCODE(16,8), HOST_TO_EQUIPMENT, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    BS2MessageDictionary::instance()->add(msgtype);

    // S16F9
    msgtype = new BS2MessageType(SFCODE(16,9), EQUIPMENT_TO_HOST, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("PREVENTID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("TIMESTAMP"));
        listitem_1->add(BS2ItemDictionary::instance()->find("PRJOBID"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("VID"));
                listitem_3->add(BS2ItemDictionary::instance()->find("V"));
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S16F10
    msgtype = new BS2MessageType(SFCODE(16,10), HOST_TO_EQUIPMENT, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    BS2MessageDictionary::instance()->add(msgtype);

    // S16F11
    msgtype = new BS2MessageType(SFCODE(16,11), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("DATAID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("PRJOBID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("MF"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("CARRIERID"));
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    listitem_4->add(BS2ItemDictionary::instance()->find("SLOTID"));
                }
            }
        }
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("PRRECIPEMETHOD"));
            listitem_2->add(BS2ItemDictionary::instance()->find("RCPSPEC"));
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    listitem_4->add(BS2ItemDictionary::instance()->find("RCPPARNM"));
                    listitem_4->add(BS2ItemDictionary::instance()->find("RCPPARVAL"));
                }
            }
        }
        listitem_1->add(BS2ItemDictionary::instance()->find("PRPROCESSSTART"));
        listitem_1->add(BS2ItemDictionary::instance()->find("PRPAUSEEVENT"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S16F12
    msgtype = new BS2MessageType(SFCODE(16,12), EQUIPMENT_TO_HOST, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("PRJOBID"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("ACKA"));
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
                }
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S16F13
    msgtype = new BS2MessageType(SFCODE(16,13), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("DATAID"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("PRJOBID"));
                listitem_3->add(BS2ItemDictionary::instance()->find("MF"));
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    {
                        BS2Itemtypes * listitem_5 = new BS2Itemtypes();
                        listitem_4->add(listitem_5);
                        listitem_5->add(BS2ItemDictionary::instance()->find("CARRIERID"));
                        {
                            BS2Itemtypes * listitem_6 = new BS2Itemtypes();
                            listitem_5->add(listitem_6);
                            listitem_6->add(BS2ItemDictionary::instance()->find("SLOTID"));
                        }
                    }
                }
            }
        }
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("PRRECIPEMETHOD"));
            listitem_2->add(BS2ItemDictionary::instance()->find("RCPSPEC"));
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    listitem_4->add(BS2ItemDictionary::instance()->find("RCPPARNM"));
                    listitem_4->add(BS2ItemDictionary::instance()->find("RCPPARVAL"));
                }
            }
        }
        listitem_1->add(BS2ItemDictionary::instance()->find("PRPROCESSSTART"));
        listitem_1->add(BS2ItemDictionary::instance()->find("PRPAUSEEVENT"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S16F14
    msgtype = new BS2MessageType(SFCODE(16,14), EQUIPMENT_TO_HOST, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("PRJOBID"));
        }
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("ACKA"));
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
                }
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S16F15
    msgtype = new BS2MessageType(SFCODE(16,15), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("DATAID"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("PRJOBID"));
                listitem_3->add(BS2ItemDictionary::instance()->find("MF"));
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    {
                        BS2Itemtypes * listitem_5 = new BS2Itemtypes();
                        listitem_4->add(listitem_5);
                        listitem_5->add(BS2ItemDictionary::instance()->find("CARRIERID"));
                        {
                            BS2Itemtypes * listitem_6 = new BS2Itemtypes();
                            listitem_5->add(listitem_6);
                            listitem_6->add(BS2ItemDictionary::instance()->find("SLOTID"));
                        }
                    }
                }
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    listitem_4->add(BS2ItemDictionary::instance()->find("PRRECIPEMETHOD"));
                    listitem_4->add(BS2ItemDictionary::instance()->find("RCPSPEC"));
                    {
                        BS2Itemtypes * listitem_5 = new BS2Itemtypes();
                        listitem_4->add(listitem_5);
                        {
                            BS2Itemtypes * listitem_6 = new BS2Itemtypes();
                            listitem_5->add(listitem_6);
                            listitem_6->add(BS2ItemDictionary::instance()->find("RCPPARNM"));
                            listitem_6->add(BS2ItemDictionary::instance()->find("RCPPARVAL"));
                        }
                    }
                }
                listitem_3->add(BS2ItemDictionary::instance()->find("PRPROCESSSTART"));
                listitem_3->add(BS2ItemDictionary::instance()->find("PRPAUSEEVENT"));
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S16F16
    msgtype = new BS2MessageType(SFCODE(16,16), EQUIPMENT_TO_HOST, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("PRJOBID"));
        }
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("ACKA"));
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
                }
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S16F17
    msgtype = new BS2MessageType(SFCODE(16,17), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("PRJOBID"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S16F18
    msgtype = new BS2MessageType(SFCODE(16,18), EQUIPMENT_TO_HOST, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("PRJOBID"));
        }
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("ACKA"));
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
                }
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S16F19
    msgtype = new BS2MessageType(SFCODE(16,19), HOST_TO_EQUIPMENT, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    BS2MessageDictionary::instance()->add(msgtype);

    // S16F20
    msgtype = new BS2MessageType(SFCODE(16,20), EQUIPMENT_TO_HOST, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("PRJOBID"));
            listitem_2->add(BS2ItemDictionary::instance()->find("PRSTATE"));
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S16F21
    msgtype = new BS2MessageType(SFCODE(16,21), HOST_TO_EQUIPMENT, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    BS2MessageDictionary::instance()->add(msgtype);

    // S16F22
    msgtype = new BS2MessageType(SFCODE(16,22), EQUIPMENT_TO_HOST, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("PRJOBSPACE"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S16F23
    msgtype = new BS2MessageType(SFCODE(16,23), HOST_TO_EQUIPMENT, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("PRJOBID"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("RCPPARNM"));
                listitem_3->add(BS2ItemDictionary::instance()->find("RCPPARVAL"));
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S16F24
    msgtype = new BS2MessageType(SFCODE(16,24), EQUIPMENT_TO_HOST, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("ACKA"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                listitem_3->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S16F25
    msgtype = new BS2MessageType(SFCODE(16,25), HOST_TO_EQUIPMENT, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("PRJOBID"));
        }
        listitem_1->add(BS2ItemDictionary::instance()->find("PRPROCESSSTART"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S16F26
    msgtype = new BS2MessageType(SFCODE(16,26), EQUIPMENT_TO_HOST, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("PRJOBID"));
        }
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("ACKA"));
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                {
                    BS2Itemtypes * listitem_4 = new BS2Itemtypes();
                    listitem_3->add(listitem_4);
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                    listitem_4->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
                }
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S16F27
    msgtype = new BS2MessageType(SFCODE(16,27), HOST_TO_EQUIPMENT, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("CTLJOBID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("CTLJOBCMD"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("CPNAME"));
            listitem_2->add(BS2ItemDictionary::instance()->find("CPVAL"));
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S16F28
    msgtype = new BS2MessageType(SFCODE(16,28), EQUIPMENT_TO_HOST, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("ACKA"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("ERRCODE"));
            listitem_2->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S16F29
    msgtype = new BS2MessageType(SFCODE(16,29), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("PRMTRLORDER"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S16F30
    msgtype = new BS2MessageType(SFCODE(16,30), EQUIPMENT_TO_HOST, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("ACKA"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S17F0
    msgtype = new BS2MessageType(SFCODE(17,0), BOTH_DIR, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    BS2MessageDictionary::instance()->add(msgtype);

    // S17F1
    msgtype = new BS2MessageType(SFCODE(17,1), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("DATAID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("RPTID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("DATASRC"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("VID"));
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S17F2
    msgtype = new BS2MessageType(SFCODE(17,2), EQUIPMENT_TO_HOST, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("RPTID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("ERRCODE"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S17F3
    msgtype = new BS2MessageType(SFCODE(17,3), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("RPTID"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S17F4
    msgtype = new BS2MessageType(SFCODE(17,4), EQUIPMENT_TO_HOST, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("ACKA"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("RPTID"));
                listitem_3->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                listitem_3->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S17F5
    msgtype = new BS2MessageType(SFCODE(17,5), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("DATAID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("TRID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("CEED"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("RPTID"));
        }
        listitem_1->add(BS2ItemDictionary::instance()->find("TRSPER"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("TOTSMP"));
            listitem_2->add(BS2ItemDictionary::instance()->find("REPGSZ"));
            listitem_2->add(BS2ItemDictionary::instance()->find("EVNTSRC"));
            listitem_2->add(BS2ItemDictionary::instance()->find("CEID"));
            listitem_2->add(BS2ItemDictionary::instance()->find("EVNTSRC"));
            listitem_2->add(BS2ItemDictionary::instance()->find("CEID"));
            listitem_2->add(BS2ItemDictionary::instance()->find("TRAUTOD"));
            listitem_2->add(BS2ItemDictionary::instance()->find("RPTOC"));
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S17F6
    msgtype = new BS2MessageType(SFCODE(17,6), EQUIPMENT_TO_HOST, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("TRID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("ERRCODE"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S17F7
    msgtype = new BS2MessageType(SFCODE(17,7), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("TRID"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S17F8
    msgtype = new BS2MessageType(SFCODE(17,8), EQUIPMENT_TO_HOST, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("ACKA"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("TRID"));
                listitem_3->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                listitem_3->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S17F9
    msgtype = new BS2MessageType(SFCODE(17,9), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "", MULTI_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("DATAID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("EVNTSRC"));
        listitem_1->add(BS2ItemDictionary::instance()->find("CEID"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("RPTID"));
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S17F10
    msgtype = new BS2MessageType(SFCODE(17,10), EQUIPMENT_TO_HOST, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("EVNTSRC"));
        listitem_1->add(BS2ItemDictionary::instance()->find("CEID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("ERRCODE"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S17F11
    msgtype = new BS2MessageType(SFCODE(17,11), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("EVNTSRC"));
        listitem_1->add(BS2ItemDictionary::instance()->find("CEID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("ERRCODE"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S17F12
    msgtype = new BS2MessageType(SFCODE(17,12), EQUIPMENT_TO_HOST, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("EVNTSRC"));
        listitem_1->add(BS2ItemDictionary::instance()->find("CEID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("RPTID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("ERRCODE"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S17F13
    msgtype = new BS2MessageType(SFCODE(17,13), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("TRID"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S17F14
    msgtype = new BS2MessageType(SFCODE(17,14), EQUIPMENT_TO_HOST, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("ACKA"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("TRID"));
                listitem_3->add(BS2ItemDictionary::instance()->find("ERRCODE"));
                listitem_3->add(BS2ItemDictionary::instance()->find("ERRTEXT"));
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S18F0
    msgtype = new BS2MessageType(SFCODE(18,0), BOTH_DIR, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    BS2MessageDictionary::instance()->add(msgtype);

    // S18F1
    msgtype = new BS2MessageType(SFCODE(18,1), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("TARGETID"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("ATTRID"));
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S18F2
    msgtype = new BS2MessageType(SFCODE(18,2), EQUIPMENT_TO_HOST, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("TARGETID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("SSACK"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("ATTRDATA"));
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S18F3
    msgtype = new BS2MessageType(SFCODE(18,3), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("TARGETID"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            {
                BS2Itemtypes * listitem_3 = new BS2Itemtypes();
                listitem_2->add(listitem_3);
                listitem_3->add(BS2ItemDictionary::instance()->find("ATTRID"));
                listitem_3->add(BS2ItemDictionary::instance()->find("ATTRDATA"));
            }
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S18F4
    msgtype = new BS2MessageType(SFCODE(18,4), EQUIPMENT_TO_HOST, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("TARGETID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("SSACK"));
        listitem_1->add(BS2ItemDictionary::instance()->find("STATUSLIST"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S18F5
    msgtype = new BS2MessageType(SFCODE(18,5), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("TARGETID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("DATASEG"));
        listitem_1->add(BS2ItemDictionary::instance()->find("DATALENGTH"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S18F6
    msgtype = new BS2MessageType(SFCODE(18,6), EQUIPMENT_TO_HOST, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("TARGETID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("SSACK"));
        listitem_1->add(BS2ItemDictionary::instance()->find("DATA"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S18F7
    msgtype = new BS2MessageType(SFCODE(18,7), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("TARGETID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("DATASEG"));
        listitem_1->add(BS2ItemDictionary::instance()->find("DATALENGTH"));
        listitem_1->add(BS2ItemDictionary::instance()->find("DATA"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S18F8
    msgtype = new BS2MessageType(SFCODE(18,8), EQUIPMENT_TO_HOST, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("TARGETID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("SSACK"));
        listitem_1->add(BS2ItemDictionary::instance()->find("STATUSLIST"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S18F9
    msgtype = new BS2MessageType(SFCODE(18,9), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    msgtype->add(BS2ItemDictionary::instance()->find("TARGETID"));
    BS2MessageDictionary::instance()->add(msgtype);

    // S18F10
    msgtype = new BS2MessageType(SFCODE(18,10), EQUIPMENT_TO_HOST, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("TARGETID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("SSACK"));
        listitem_1->add(BS2ItemDictionary::instance()->find("MID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("STATUSLIST"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S18F11
    msgtype = new BS2MessageType(SFCODE(18,11), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("TARGETID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("MID"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S18F12
    msgtype = new BS2MessageType(SFCODE(18,12), EQUIPMENT_TO_HOST, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("TARGETID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("SSACK"));
        listitem_1->add(BS2ItemDictionary::instance()->find("STATUSLIST"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S18F13
    msgtype = new BS2MessageType(SFCODE(18,13), HOST_TO_EQUIPMENT, REPLY_REQUIRED, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("TARGETID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("SSCMD"));
        {
            BS2Itemtypes * listitem_2 = new BS2Itemtypes();
            listitem_1->add(listitem_2);
            listitem_2->add(BS2ItemDictionary::instance()->find("CPVAL"));
        }
    }
    BS2MessageDictionary::instance()->add(msgtype);

    // S18F14
    msgtype = new BS2MessageType(SFCODE(18,14), EQUIPMENT_TO_HOST, REPLY_NONE, "", SINGLE_BLOCK_MESSAGE);
    {
        BS2Itemtypes * listitem_1 = new BS2Itemtypes();
        msgtype->add(listitem_1);
        listitem_1->add(BS2ItemDictionary::instance()->find("TARGETID"));
        listitem_1->add(BS2ItemDictionary::instance()->find("SSACK"));
        listitem_1->add(BS2ItemDictionary::instance()->find("STATUSLIST"));
    }
    BS2MessageDictionary::instance()->add(msgtype);

    return 0;
}

void close_msgtype()
{
	BS2MessageDictionary::instance()->eraseAll();
	BS2MessageDictionary::close_singleton();

	BS2ItemDictionary::instance()->eraseAll();
	BS2ItemDictionary::close_singleton();

	SECSXmlParser::close_singleton();
}
