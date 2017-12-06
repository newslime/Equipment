
// -*- C++ -*-
// generate_export_file.pl,v 1.9 2002/05/08 21:54:10 nanbor Exp
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl -f BEE BEE
// ------------------------------
#ifndef BEE_EXPORT_H
#define BEE_EXPORT_H

#include "ace/config-all.h"

#if defined (BEE)
#  if !defined (BEE_HAS_DLL)
#    define BEE_HAS_DLL 0
#  endif /* ! BEE_HAS_DLL */
#else
#  if !defined (BEE_HAS_DLL)
#    define BEE_HAS_DLL 1
#  endif /* ! BEE_HAS_DLL */
#endif

#if defined (BEE_HAS_DLL) && (BEE_HAS_DLL == 1)
#  if defined (BEE_BUILD_DLL)
#    define BEE_Export ACE_Proper_Export_Flag
#    define BEE_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define BEE_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* BEE_BUILD_DLL */
#    define BEE_Export ACE_Proper_Import_Flag
#    define BEE_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define BEE_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* BEE_BUILD_DLL */
#else /* BEE_HAS_DLL == 1 */
#  define BEE_Export
#  define BEE_SINGLETON_DECLARATION(T)
#  define BEE_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* BEE_HAS_DLL == 1 */

#endif /* BEE_EXPORT_H */

// End of auto generated file.
