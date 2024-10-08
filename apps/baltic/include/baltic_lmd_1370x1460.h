/*
** svn $Id: upwelling.h 1001 2020-01-10 22:41:16Z arango $
*******************************************************************************
** Copyright (c) 2002-2020 The ROMS/TOMS Group                               **
**   Licensed under a MIT/X style license                                    **
**   See License_ROMS.txt                                                    **
*******************************************************************************
**
** Options for Upwelling Test.
**
** Application flag:   BALTIC
** Input script:       roms_baltic.in
*/

#define UV_ADV
#define DJ_GRADPS
#define UV_COR
#define UV_QDRAG
#undef UV_VIS2
#define UV_VIS4
#define UV_SMAGORINSKY 
#define MIX_S_UV
#define MIX_GEO_TS
#undef  MIX_GEO_UV
#define SPLINES_VDIFF
#define SPLINES_VVISC
#undef TS_DIF2
#define  TS_DIF4
#undef  MIX_S_TS
#define NONLIN_EOS
#define CURVGRID
#define PROFILE
#define SPHERICAL
#define MASKING
/*(#define WIND_MINUS_CURRENT*/

#define PARALLEL_IO 
#define HDF5

#define NORTHBC
#define FSOBC_REDUCED
#define UV_COR

#define TS_U3HADVECTION
#undef TS_C4HADVECTION

#define SALINITY
#define SOLVE3D
#define AVERAGES           /* define if writing out time-averaged data */

#undef DIAGNOSTICS_TS
#undef DIAGNOSTICS_UV

#define INPUT_INTERP
#define BULK_FLUXES
#define SOLAR_SOURCE
#define LONGWAVE_OUT
#define EMINUSP
#define SPECIFIC_HUMIDITY /*MACIEJ*/
#define PERFECT_RESTART /*MACIEJ*/
#define POP_WIND /*MACIEJ*/
#define INLINE_2DIO

#ifdef SOLVE3D
# define LMD_MIXING
# ifdef LMD_MIXING
#   define LMD_RIMIX
#   define LMD_CONVEC
#   define LMD_SKPP
#   define LMD_BKPP
#   define LMD_NONLOCAL
#   define LMD_SHAPIRO
#   define LMD_DDMIX
# endif
# undef GLS_MIXING
# undef MY25_MIXING

#endif

#define ANA_BSFLUX
#define ANA_BTFLUX
