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
#define UV_COR
#define UV_LOGDRAG
#undef UV_VIS2
#define UV_VIS4
#define UV_SMAGORINSKY /*MACIEJ*/
#define SPLINES_VDIFF
#define SPLINES_VVISC
#define UV_U3HADVECTION
#define UV_C4VADVECTION
/*#define TS_U3HADVECTION
#undef TS_C4HADVECTION*/
#undef TS_DIF2
#define  TS_DIF4
/*#define LIMIT_VVISC
#define LIMIT_VDIFF*/
#define WIND_MINUS_CURRENT
#define MIX_S_UV
#define MIX_GEO_TS
#define SALINITY
#define NONLIN_EOS
#define DJ_GRADPS
#define MASKING
#define CURVGRID
#define VISC_GRID
#define DIFF_GRID
#define SPHERICAL



#define PARALLEL_IO 
#define HDF5
#define AVERAGES           /* define if writing out time-averaged data */
#define PERFECT_RESTART /*MACIEJ*/
#define NORTHBC

#define SOLVE3D
#define MY25_MIXING
#define BULK_FLUXES
#undef  LMD_MIXING
#undef  GLS_MIXING
#define ANA_BSFLUX
#define ANA_BTFLUX
#undef ANA_PERTURB

#ifdef LMD_MIXING
# define LMD_RIMIX
# define LMD_CONVEC
# define LMD_SKPP
# define LMD_NONLOCAL
/*# define LMD_SHAPIRO*/
# define LMD_SPLINES
# define LMD_BKPP
# define RI_SPLINES
#endif

#ifdef MY25_MIXING
# define RI_SPLINES
# define N2S2_HORAVG
# define KANTHA_CLAYSON
# define K_C4ADVECTION
#endif

#define FSOBC_REDUCED
#undef  RAMP_TIDES
#undef SSH_TIDES
#undef UV_TIDES
#ifdef SSH_TIDES
# undef  FSOBC_REDUCED
# define ADD_FSOBC
#endif
#if defined UV_TIDES || defined SSH_TIDES
# define ADD_M2OBC
#endif

#undef DIAGNOSTICS_TS
#undef DIAGNOSTICS_UV

#if defined BULK_FLUXES
# undef  SRELAXATION
# undef  SCORRECTION
# undef  QCORRECTION
# undef  LONGWAVE
# define LONGWAVE_OUT
# undef  CLOUDS
# define SOLAR_SOURCE
# define SPECIFIC_HUMIDITY /*MACIEJ*/
# define POP_WIND /*MACIEJ*/
# undef  DIURNAL_SRFLUX
# if ! defined SRELAXATION
#  define EMINUSP
# endif
#endif
