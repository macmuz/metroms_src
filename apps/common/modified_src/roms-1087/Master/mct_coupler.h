#include "cppdefs.h"

#undef REPORT_COUPLING_ALL

      MODULE ocean_coupler_mod

#if defined MODEL_COUPLING && defined MCT_LIB
!
!git $Id$
!svn $Id: mct_coupler.h 1054 2021-03-06 19:47:12Z arango $
!==================================================== John C. Warner ===
!  Copyright (c) 2002-2021 The ROMS/TOMS Group      Hernan G. Arango   !
!    Licensed under a MIT/X style license                              !
!    See License_ROMS.txt                                              !
!=======================================================================
!                                                                      !
!  This module is used to communicate and exchange data between        !
!  ROMS/TOMS and other coupled model(s)  via the Model Coupling        !
!  Toolkit (MCT), developed at the Argonne National Laboratory.        !
!                                                                      !
!=======================================================================
!
!  Component Model Registry.
!
      USE m_MCTWorld, ONLY : MCTWorld_init => init
      USE m_MCTWorld, ONLY : MCTWorld_clean => clean
!
!  Domain Decomposition Descriptor DataType and associated methods.
!
      USE m_GlobalSegMap, ONLY : GlobalSegMap
      USE m_GlobalSegMap, ONLY : GlobalSegMap_init => init
      USE m_GlobalSegMap, ONLY : GlobalSegMap_lsize => lsize
      USE m_GlobalSegMap, ONLY : GlobalSegMap_clean => clean
      USE m_GlobalSegMap, ONLY : GlobalSegMap_Ordpnts => OrderedPoints
!
!  Field Storage DataType and associated methods.
!
      USE m_AttrVect, ONLY : AttrVect
      USE m_AttrVect, ONLY : AttrVect_init => init
      USE m_AttrVect, ONLY : AttrVect_zero => zero
      USE m_AttrVect, ONLY : AttrVect_lsize => lsize
      USE m_AttrVect, ONLY : AttrVect_clean => clean
      USE m_AttrVect, ONLY : AttrVect_importRAttr => importRAttr
      USE m_AttrVect, ONLY : AttrVect_exportRAttr => exportRAttr
!
!  Intercomponent communications scheduler.
!
      USE m_Router, ONLY : Router
      USE m_Router, ONLY : Router_init => init
      USE m_Router, ONLY : Router_clean => clean
!
!  Intercomponent transfer.
!
      USE m_Transfer, ONLY: MCT_Send => send
      USE m_Transfer, ONLY: MCT_Recv => recv
!
!  Sparse Matrix DataType and associated methods.
!
      USE m_SparseMatrix, ONLY : SparseMatrix
      USE m_SparseMatrix, ONLY : SparseMatrix_init => init
      USE m_SparseMatrix, ONLY : SparseMatrix_importGRowInd =>          &
     &                           importGlobalRowIndices
      USE m_SparseMatrix, ONLY : SparseMatrix_importGColInd =>          &
     &                           importGlobalColumnIndices
      USE m_SparseMatrix, ONLY : SparseMatrix_importMatrixElts =>       &
     &                           importMatrixElements
      USE m_SparseMatrixPlus, ONLY : SparseMatrixPlus
      USE m_SparseMatrixPlus, ONLY : SparseMatrixPlus_init => init
      USE m_SparseMatrixPlus, ONLY : SparseMatrixPlus_clean => clean
!
!  Decompose matrix by row.
!
      USE m_SparseMatrixPlus, ONLY : Xonly
!
!  Matrix-Vector multiply methods.
!
      USE m_MatAttrVectMul, ONLY : MCT_MatVecMul => sMatAvMult
!
      implicit none
!
      PRIVATE

# ifdef SWAN_COUPLING
      PUBLIC :: initialize_ocn2wav_coupling
      PUBLIC :: ocn2wav_coupling
      PUBLIC :: finalize_ocn2wav_coupling
# endif
# ifdef WRF_COUPLING
      PUBLIC :: initialize_ocn2atm_coupling
      PUBLIC :: ocn2atm_coupling
      PUBLIC :: finalize_ocn2atm_coupling
# endif
# ifdef CICE_COUPLING
      PUBLIC :: initialize_ocn2cice_coupling
      PUBLIC :: ocn2cice_coupling
      PUBLIC :: finalize_ocn2cice_coupling
# endif
!
!  Declarations.
!
# ifdef SWAN_COUPLING
      TYPE(AttrVect) :: wav2ocn_AV            ! AttrVect variables
      TYPE(AttrVect) :: ocn2wav_AV
      TYPE(Router)   :: ROMStoSWAN            ! Router variables
# endif
# ifdef WRF_COUPLING
      TYPE(AttrVect) :: atm2ocn_AV            ! AttrVect variables
      TYPE(AttrVect) :: ocn2atm_AV
      TYPE(Router)   :: ROMStoWRF             ! Router variables
# endif
# ifdef CICE_COUPLING
      TYPE(AttrVect) :: cice2ocn_AV           ! AttrVect variables
      TYPE(AttrVect) :: ocn2cice_AV
      TYPE(Router)   :: ROMStoCICE            ! Router variables
# endif
      TYPE(GlobalSegMap) :: GSMapROMS         ! GloabalSegMap variables

      CONTAINS
/*
************************************************************************
*  Include model specific communication routines.
************************************************************************
*/

# ifdef SWAN_COUPLING
#  include "mct_roms_swan.h"
# endif
# ifdef REFDIF_COUPLING
#  include "mct_roms_refdif.h"
# endif
# ifdef WRF_COUPLING
#  include "mct_roms_wrf.h"
# endif
# ifdef CICE_COUPLING

      SUBROUTINE initialize_ocn2cice_coupling (ng, tile)
      USE mod_param
      USE mod_parallel
      USE mod_coupler
      USE mod_forces
      USE mod_kinds
      USE mod_scalars
      USE mod_iounits, only: stdout

!
!  Imported variable definitions.
!
      integer, intent(in) :: ng, tile
!
!  Local variable declarations.
!
      integer :: Istr, Iend, Jstr, Jend
      integer :: IstrR, IendR, JstrR, JendR, IstrU, JstrV
      integer :: Asize, Jsize, MyError
      integer :: j, jc, nprocs

      integer, allocatable :: length(:)
      integer, allocatable :: start(:)

      character (len=*), parameter :: MyFile =                          &
     &  __FILE__

#ifdef PROFILE
      CALL wclock_on (ng, iNLM, 36, __LINE__, MyFile)
#endif

!
!-----------------------------------------------------------------------
!  Begin initialization phase.
!-----------------------------------------------------------------------
!
!  Get communicator local rank and size.
!
#ifdef REPORT_COUPLING_ALL
      WRITE (stdout,*) ' ROMS Ocean Coupler: mpi_comm_rank'
#endif
      CALL mpi_comm_rank (OCN_COMM_WORLD, MyRank, MyError)
#ifdef REPORT_COUPLING_ALL
      WRITE (stdout,*) ' ROMS Ocean Coupler: mpi_comm_size'
#endif
      CALL mpi_comm_size (OCN_COMM_WORLD, nprocs, MyError)
!
!  Initialize MCT coupled model registry.
!
#ifdef REPORT_COUPLING_ALL
      WRITE (stdout,*) ' ROMS Ocean Coupler: calling MCTWorld_init'
#endif
      CALL MCTWorld_init (Nmodels, MPI_COMM_WORLD, OCN_COMM_WORLD,      &
     &                    OCNid)
#ifdef REPORT_COUPLING_ALL
      WRITE(stdout,*) ' Done: ROMS Ocean Coupler: calling MCTWorld_init'
#elif !defined SILENT
      IF (Master)                                                       &
     &     WRITE(stdout,*)' ROMS Ocean Coupler: MCTWorld_init called'
#endif
!
!-----------------------------------------------------------------------
!  Compute lower and upper bounds over a particular domain partition or
!  tile for RHO-, U-, and V-variables. Notice that "set_bounds.h" is
!  not used here because of implementation of periodicity in other
!  models.
!-----------------------------------------------------------------------
!
      Istr=BOUNDS(ng)%Istr(tile)
      Iend=BOUNDS(ng)%Iend(tile)
      Jstr=BOUNDS(ng)%Jstr(tile)
      Jend=BOUNDS(ng)%Jend(tile)
!
      IF (DOMAIN(ng)%Western_Edge(tile)) THEN
        IstrR=BOUNDS(ng)%Istr(tile)-1
      ELSE
        IstrR=BOUNDS(ng)%Istr(tile)
      END IF
      IF (DOMAIN(ng)%Eastern_Edge(tile)) THEN
        IendR=BOUNDS(ng)%Iend(tile)+1
      ELSE
        IendR=BOUNDS(ng)%Iend(tile)
      END IF
      IF (DOMAIN(ng)%Southern_Edge(tile)) THEN
        JstrR=BOUNDS(ng)%Jstr(tile)-1
      ELSE
        JstrR=BOUNDS(ng)%Jstr(tile)
      END IF
      IF (DOMAIN(ng)%Northern_Edge(tile)) THEN
        JendR=BOUNDS(ng)%Jend(tile)+1
      ELSE
        JendR=BOUNDS(ng)%Jend(tile)
      END IF
!
!  Determine start and lengths for domain decomposition.
!
      Jsize=JendR-JstrR+1
#ifdef REPORT_COUPLING_ALL
      WRITE (stdout,*) ' ROMS Ocean Coupler: Jsize=', Jsize
#endif
      IF (.not.allocated(start)) THEN
        allocate ( start(Jsize) )
      END IF
      IF (.not.allocated(length)) THEN
        allocate ( length(Jsize) )
      END IF
      jc=0
      DO j=JstrR,JendR
        jc=jc+1
        start (jc)=j*(Lm(ng)+2)+IstrR+1
        length(jc)=(IendR-IstrR+1)
      END DO
#ifdef REPORT_COUPLING_ALL
      WRITE (stdout,*) ' ROMS Ocean Coupler: calling GlobalSegMap_init'
#elif !defined SILENT
      if (Master) WRITE (stdout,*)                                      &
     & ' ROMS Ocean Coupler: calling GlobalSegMap_init'
#endif
      CALL GlobalSegMap_init (GSMapROMS, start, length, 0,              &
     &     OCN_COMM_WORLD, OCNid)
!
!  Initialize attribute vector holding the export data code strings of
!  the cice model. The Asize is the number of grid point on this
!  processor.
!
      Asize=GlobalSegMap_lsize(GSMapROMS, OCN_COMM_WORLD)

#ifdef REPORT_COUPLING_ALL
      WRITE (stdout,*) ' ROMS Ocean Coupler: calling AttrVect_init.',   &
     &     ' Asize=', Asize
#endif

#if !defined SILENT
      if (master) write(stdout,*)                                       &
     &     'cice2ocn_AV, Exportlist ',trim(ExportList(Icice))
#endif

      CALL AttrVect_init (cice2ocn_AV, rList=TRIM(ExportList(Icice)),   &
     &     lsize=Asize)
!
!  Initialize attribute vector holding the export data code string of
!  the ocean model.
!
#if !defined SILENT
      if (master) write(stdout,*)                                       &
     &     'ocn2cice_AV, Exportlist ',trim(ExportList(Iocean))
#endif
      CALL AttrVect_init (ocn2cice_AV, rList=TRIM(ExportList(Iocean)),  &
     &     lsize=Asize)
      CALL AttrVect_zero (ocn2cice_AV)

#ifdef REPORT_COUPLING_ALL
      WRITE (stdout,*) ' ROMS Ocean Coupler: calling Router_init'
#endif
      CALL Router_init (CICEid, GSMapROMS, OCN_COMM_WORLD, ROMStoCICE)

#ifdef REPORT_COUPLING_ALL
      WRITE (stdout,*) ' ROMS Ocean Coupler: calling Router_init. Done'
#elif !defined SILENT
      if (master) write(stdout,*)                                       &
     &     ' ROMS Ocean Coupler: calling Router_init. Done'
#endif
!
!  Deallocate working arrays.
!
      IF (allocated(start)) THEN
        deallocate (start)
      END IF
      IF (allocated(length)) THEN
        deallocate (length)
      END IF

#ifdef PROFILE
      CALL wclock_off (ng, iNLM, 36, __LINE__, MyFile)
#endif

      RETURN
      END SUBROUTINE initialize_ocn2cice_coupling

      SUBROUTINE ocn2cice_coupling (ng, tile, ncouple)
      USE mod_param
      USE mod_parallel
      USE mod_coupler
      USE mod_forces
      USE mod_ocean
      USE mod_scalars
      USE mod_stepping
      USE mod_iounits, only: stdout
      USE mod_grid
!      USE distribute_mod, ONLY : mp_reduce
      USE ROMS_import_mod, ONLY : ROMS_import2d
      USE ROMS_export_mod, ONLY : ROMS_export2d
#  ifdef LMD_SKPP
      USE mod_mixing
#  endif
      USE bc_2d_mod
      USE exchange_2d_mod
# ifdef DISTRIBUTE
      USE mp_exchange_mod, ONLY : mp_exchange2d
# endif
      USE mod_ice
!jd
      USE frazil_ice_prod_mod, only : t_freeze
!jd

      implicit none

!
!  Imported variable declarations.
!
      integer, intent(in) :: ng, tile, ncouple
!
!  Local variable declarations.
!
      integer :: Istr, Iend, Jstr, Jend
      integer :: IstrR, IendR, JstrR, JendR, IstrU, JstrV
      integer :: Asize, Iimport, Iexport, MyError
      integer :: gtype, i, id, ifield, ij, j, status

      real(r8) :: add_offset, scale

      real(r8), pointer :: A(:)

      ! intermediates for ice stress interpolation
      real(r8) :: aice_psip, aice_psi, sap, sa

      character (len=3 ), dimension(2) :: op_handle
      character (len=40) :: code


      integer :: nbotu, nbotv, k
      integer, dimension(:,:), allocatable :: nbot,nbotvel
      real(r8), dimension(:,:), allocatable :: uw
      real(r8), dimension(:,:), allocatable :: vw
      real(r8), dimension(:,:), allocatable :: uwater
      real(r8), dimension(:,:), allocatable :: vwater
      real(r8), dimension(:,:), allocatable :: sst2d
      real(r8), dimension(:,:), allocatable :: sss2d
      real(r8), dimension(:,:), allocatable :: rho2d
      real(r8), dimension(:,:), allocatable :: ml2d
      real(r8), dimension(:,:,:), allocatable :: rho3d

      real(r8) :: mlio,mliovel
      real(r8) :: dml
      real(r8) :: totml
      real(r8) :: rho,sst,sss
#ifdef CPMAXSPEED
      real(r8) :: maxcurvel
#endif

      character (len=*), parameter :: MyFile =                          &
     &  __FILE__

#include "tile.h"

#ifdef PROFILE
      CALL wclock_on (ng, iNLM, 36, __LINE__, MyFile)
#endif


      allocate(nbot(IminS:ImaxS,JminS:JmaxS))
      allocate(nbotvel(IminS:ImaxS,JminS:JmaxS))
      allocate(uw(IminS:ImaxS,JminS:JmaxS))
      allocate(vw(IminS:ImaxS,JminS:JmaxS))
      allocate(uwater(LBi:UBi,LBj:UBj))
      allocate(vwater(LBi:UBi,LBj:UBj))
      allocate(sst2d(LBi:UBi,LBj:UBj))
      allocate(sss2d(LBi:UBi,LBj:UBj))
      allocate(rho2d(LBi:UBi,LBj:UBj))
      allocate(ml2d(LBi:UBi,LBj:UBj))
      allocate(rho3d(LBi:UBi,LBj:UBj,N(ng)))

      uw=0.0_r8
      vw=0.0_r8
      uwater=0.0_r8
      vwater=0.0_r8
      sst2d=0.0_r8
      sss2d=0.0_r8
      rho3d=1000.0_r8+OCEAN(ng)%pden

#if !defined SILENT
      IF (Master) THEN
          write(stdout,*) ' '
          write(stdout,*) ' ***************************************** '
          write(stdout,*) &
     &         '    Ocean - CICE: coupling routine called from ROMS'
          write(stdout,*) &
          '    Time : ', time_code, time
      END IF
#endif

!
!-----------------------------------------------------------------------
!  Compute lower and upper bounds over a particular domain partition or
!  tile for RHO-, U-, and V-variables. Notice that "set_bounds.h" is
!  not used here because of implementation of periodicity in other
!  models.
!-----------------------------------------------------------------------
!
      Istr=BOUNDS(ng)%Istr(tile)
      Iend=BOUNDS(ng)%Iend(tile)
      Jstr=BOUNDS(ng)%Jstr(tile)
      Jend=BOUNDS(ng)%Jend(tile)
      IstrU=BOUNDS(ng)%IstrU(tile)
      JstrV=BOUNDS(ng)%JstrV(tile)

!
      IF (DOMAIN(ng)%Western_Edge(tile)) THEN
        IstrR=BOUNDS(ng)%Istr(tile)-1
      ELSE
        IstrR=BOUNDS(ng)%Istr(tile)
      END IF
      IF (DOMAIN(ng)%Eastern_Edge(tile)) THEN
        IendR=BOUNDS(ng)%Iend(tile)+1
      ELSE
        IendR=BOUNDS(ng)%Iend(tile)
      END IF
      IF (DOMAIN(ng)%Southern_Edge(tile)) THEN
        JstrR=BOUNDS(ng)%Jstr(tile)-1
      ELSE
        JstrR=BOUNDS(ng)%Jstr(tile)
      END IF
      IF (DOMAIN(ng)%Northern_Edge(tile)) THEN
        JendR=BOUNDS(ng)%Jend(tile)+1
      ELSE
        JendR=BOUNDS(ng)%Jend(tile)
      END IF
!
!-----------------------------------------------------------------------
!  Allocate communications array.
!-----------------------------------------------------------------------
!
      Asize=GlobalSegMap_lsize(GSMapROMS, OCN_COMM_WORLD)
      allocate(A(Asize))
      A=0.0_r8

!
!  Schedule receiving fields from cice model.
!
      CALL mpi_comm_rank (OCN_COMM_WORLD, MyRank, MyError)

      CALL MCT_Recv (cice2ocn_AV, ROMStoCICE)
!
!  Receive fields from cice model.
!
      Iimport=0
      DO ifield=1,Nimport(Iocean)
        id=ImportID(Iocean)%val(ifield)
        code=ADJUSTL(Fields(id)%code)
        gtype=Fields(id)%GridType
        scale=Fields(id)%scale
        add_offset=Fields(id)%AddOffset

        SELECT CASE (TRIM(code))



        CASE ('AICE')           ! Sea ice concentration

           CALL AttrVect_exportRAttr (cice2ocn_AV, TRIM(code),A,Asize)
           Iimport=Iimport+1
#ifdef REPORT_COUPLING_ALL
           write(stdout,*) 'ROMS rank ', MyRank,                        &
     &          ' received ',trim(code),' field (max/min): ',           &
     &           maxval(A),' ',minval(A),' gtype : ', gtype
#endif
           scale=1.0_r8
           add_offset=0.0_r8
           CALL ROMS_import2d (ng, tile,                                &
     &                         id, gtype, scale, add_offset,            &
     &                         Asize, A,                                &
     &                         IstrR, IendR, JstrR, JendR,              &
     &                         LBi, UBi, LBj, UBj,                      &
     &                         Fields(id)%ImpMin, Fields(id)%ImpMax,    &
     &                         ICE(ng)%aice,                            &
     &                         status)

!jd Transfere to u- and v- points for weigthing with stress

           DO j=JstrR,JendR
              DO i=Istr,IendR
                 ICE(ng)%aice_u(i,j)=                                   &
     &                0.5_r8*(ICE(ng)%aice(i-1,j)+ICE(ng)%aice(i,j))
!# ifdef MASKING
!                 ICE(ng)%aice_u(i,j)=ICE(ng)%aice_u(i,j)*umask(i,j)
!# endif
              END DO
           END DO
           DO j=Jstr,JendR
              DO i=IstrR,IendR
                 ICE(ng)%aice_v(i,j)=                                   &
     &                0.5_r8*(ICE(ng)%aice(i,j-1)+ICE(ng)%aice(i,j))
!# ifdef MASKING
!                 ICE(ng)%aice_v(i,j)=ICE(ng)%aice_v(i,j)*vmask(i,j)
!# endif
              END DO
           END DO

           IF (EWperiodic(ng).or.NSperiodic(ng)) THEN
              CALL exchange_u2d_tile (ng, tile,                         &
     &             LBi, UBi, LBj, UBj,                                  &
     &             ICE(ng)%aice_u)
              CALL exchange_v2d_tile (ng, tile,                         &
     &             LBi, UBi, LBj, UBj,                                  &
     &             ICE(ng)%aice_v)
           END IF

#ifdef DISTRIBUTE
           CALL mp_exchange2d (ng, tile, iNLM, 2,                       &
     &          LBi, UBi, LBj, UBj,                                     &
     &          NghostPoints,                                           &
     &          EWperiodic(ng), NSperiodic(ng),                         &
     &          ICE(ng)%aice_u,ICE(ng)%aice_v )
#endif


        CASE ('freshAI')        ! Fresh water flux

           CALL AttrVect_exportRAttr (cice2ocn_AV, TRIM(code), A, Asize)
           Iimport=Iimport+1
#ifdef REPORT_COUPLING_ALL
           write(stdout,*) 'ROMS rank ', MyRank,                        &
     &          ' received ',trim(code),' field (max/min): ',           &
     &          maxval(A),' ',minval(A),' gtype : ', gtype
#endif
           scale=1.0_r8
           add_offset=0.0_r8
           CALL ROMS_import2d (ng, tile,                                &
     &                         id, gtype, scale, add_offset,            &
     &                         Asize, A,                                &
     &                         IstrR, IendR, JstrR, JendR,              &
     &                         LBi, UBi, LBj, UBj,                      &
     &                         Fields(id)%ImpMin, Fields(id)%ImpMax,    &
     &                         ICE(ng)%freshAI,                         &
     &                         status)

        CASE ('fsaltAI')        ! Salt flux

           CALL AttrVect_exportRAttr (cice2ocn_AV, TRIM(code), A, Asize)
           Iimport=Iimport+1
#ifdef REPORT_COUPLING_ALL
           write(stdout,*) 'ROMS rank ', MyRank,                        &
     &          ' received ',trim(code),' field (max/min): ',           &
     &          maxval(A),' ',minval(A),' gtype : ', gtype
#endif
           scale=1.0_r8
           add_offset=0.0_r8
           CALL ROMS_import2d (ng, tile,                                &
     &                         id, gtype, scale, add_offset,            &
     &                         Asize, A,                                &
     &                         IstrR, IendR, JstrR, JendR,              &
     &                         LBi, UBi, LBj, UBj,                      &
     &                         Fields(id)%ImpMin, Fields(id)%ImpMax,    &
     &                         ICE(ng)%fsaltAI,                         &
     &                         status)

        CASE ('fhocnAI')        ! Nonradiative heat flux

           CALL AttrVect_exportRAttr(cice2ocn_AV, TRIM(code), A, Asize)
           Iimport=Iimport+1
#ifdef REPORT_COUPLING_ALL
           write(stdout,*) 'ROMS rank ', MyRank,                        &
     &          ' received ',trim(code),' field (max/min): ',           &
     &          maxval(A),' ',minval(A),' gtype : ', gtype
#endif
           scale=1.0_r8
           add_offset=0.0_r8
           CALL ROMS_import2d (ng, tile,                                &
     &                         id, gtype, scale, add_offset,            &
     &                         Asize, A,                                &
     &                         IstrR, IendR, JstrR, JendR,              &
     &                         LBi, UBi, LBj, UBj,                      &
     &                         Fields(id)%ImpMin, Fields(id)%ImpMax,    &
     &                         ICE(ng)%fhocnAI,                         &
     &                         status)

        CASE ('fswthruAI')      ! Radiative heat flux through ice

           CALL AttrVect_exportRAttr (cice2ocn_AV, TRIM(code), A, Asize)
           Iimport=Iimport+1
#ifdef REPORT_COUPLING_ALL
           write(stdout,*) 'ROMS rank ', MyRank,                        &
     &          ' received ',trim(code),' field (max/min): ',           &
     &          maxval(A),' ',minval(A),' gtype : ', gtype
#endif
           scale=1.0_r8
           add_offset=0.0_r8
           CALL ROMS_import2d (ng, tile,                                &
     &                         id, gtype, scale, add_offset,            &
     &                         Asize, A,                                &
     &                         IstrR, IendR, JstrR, JendR,              &
     &                         LBi, UBi, LBj, UBj,                      &
     &                         Fields(id)%ImpMin, Fields(id)%ImpMax,    &
     &                         ICE(ng)%fswthruAI,                       &
     &                         status)

        CASE ('strocnx')        ! ice-ocean stress x-dir

           CALL AttrVect_exportRAttr (cice2ocn_AV, TRIM(code),A,Asize)
           Iimport=Iimport+1
#ifdef REPORT_COUPLING_ALL
           write(stdout,*) 'ROMS rank ', MyRank,                        &
     &          ' received strocnx field (max/min): ',                  &
     &          maxval(A), ' ', minval(A),' gtype : ', gtype
#endif
           scale=1.0_r8
           add_offset=0.0_r8

           CALL ROMS_import2d (ng, tile,                                &
     &                         id, gtype, scale, add_offset,            &
     &                         Asize, A,                                &
     &                         IstrR, IendR, JstrR, JendR,              &
     &                         LBi, UBi, LBj, UBj,                      &
     &                         Fields(id)%ImpMin, Fields(id)%ImpMax,    &
     &                         ICE(ng)%strx,                            &
     &                         status)


           DO j=JstrR,JendR
              DO i=Istr,IendR
                 ICE(ng)%stru(i,j) = 0.5_r8*                            &
     &                (ICE(ng)%strx(i-1,j) + ICE(ng)%strx(i,j))

!# ifdef MASKING
!                 ICE(ng)%stru(i,j)=ICE(ng)%stru(i,j)*umask(i,j)
!# endif
              END DO
           END DO

           IF (EWperiodic(ng).or.NSperiodic(ng)) THEN
              CALL exchange_u2d_tile (ng, tile,                         &
     &             LBi, UBi, LBj, UBj,                                  &
     &             ICE(ng)%stru)
           END IF
#ifdef DISTRIBUTE
           CALL mp_exchange2d (ng, tile, iNLM, 1,                       &
     &          LBi, UBi, LBj, UBj,                                     &
     &          NghostPoints,                                           &
     &          EWperiodic(ng), NSperiodic(ng),                         &
     &          ICE(ng)%stru )
#endif


        CASE ('strocny')        ! ice-ocean stress y-dir

           CALL AttrVect_exportRAttr (cice2ocn_AV, TRIM(code), A, Asize)
           Iimport=Iimport+1
#ifdef REPORT_COUPLING_ALL
           write(stdout,*) 'ROMS rank ', MyRank,                        &
     &          ' received strocny field (max/min): ',                  &
     &          maxval(A), ' ', minval(A),' gtype : ', gtype
#endif
           scale=1.0_r8
           add_offset=0.0_r8
           CALL ROMS_import2d (ng, tile,                                &
     &                         id, gtype, scale, add_offset,            &
     &                         Asize, A,                                &
     &                         IstrR, IendR, JstrR, JendR,              &
     &                         LBi, UBi, LBj, UBj,                      &
     &                         Fields(id)%ImpMin, Fields(id)%ImpMax,    &
     &                         ICE(ng)%stry,                            &
     &                         status)

           DO j=Jstr,JendR
              DO i=IstrR,IendR
                 ICE(ng)%strv(i,j)=0.5_r8*                              &
     &                (ICE(ng)%stry(i,j-1)+ICE(ng)%stry(i,j))
!# ifdef MASKING
!                 ICE(ng)%strv(i,j)=ICE(ng)%strv(i,j)*vmask(i,j)
!# endif
              END DO
           END DO
           IF (EWperiodic(ng).or.NSperiodic(ng)) THEN
              CALL exchange_v2d_tile (ng, tile,                         &
     &             LBi, UBi, LBj, UBj,                                  &
     &             ICE(ng)%strv)
           END IF
#ifdef DISTRIBUTE
           CALL mp_exchange2d (ng, tile, iNLM, 1,                       &
     &          LBi, UBi, LBj, UBj,                                     &
     &          NghostPoints,                                           &
     &          EWperiodic(ng), NSperiodic(ng),                         &
     &          ICE(ng)%strv )
#endif

        END SELECT
#if !defined SILENT
        IF (Master) write(stdout,*)                                     &
     &       'i2o: ',trim(code),' min/max', Fields(id)%ImpMin,        &
     &          Fields(id)%ImpMax
#endif

      END DO

!
!-----------------------------------------------------------------------
!  Export fields from ocean (ROMS) to sea ice (CICE) model.
!-----------------------------------------------------------------------
!

#ifdef CPMAXSPEED
   maxcurvel = 1.0
#endif

!  Prepare for depth-averaging (find k index just below mixed layer)

#  define I_RANGE MAX(Istr-2,0),MIN(Iend+2,Lm(ng)+1)
#  define J_RANGE MAX(Jstr-2,0),MIN(Jend+2,Mm(ng)+1)
      do j=J_RANGE
         do i=I_RANGE

#  ifdef LMD_SKPP
            mlio = min(MIXING(ng)%hsbl(i,j),-10._r8)
            mliovel = min(MIXING(ng)%hsbl(i,j),-10._r8)
#  else
            mlio = -5._r8
            mliovel = -5._r8
#  endif
            nbot(i,j) = 1
            do k=N(ng),1,-1
               if(GRID(ng)%z_r(i,j,k).lt.mlio) then
                  nbot(i,j) = min(k,N(ng))
                  nbot(i,j) = max(nbot(i,j),1)
                  goto 1111
               endif
            enddo
 1111       continue

            nbotvel(i,j) = 1
            do k=N(ng),1,-1
               if(GRID(ng)%z_r(i,j,k).lt.mliovel) then
                  nbotvel(i,j) = min(k,N(ng))
                  nbotvel(i,j) = max(nbotvel(i,j),1)
                  goto 1112
               endif
            enddo
 1112       continue
         enddo
      enddo
#undef I_RANGE
#undef J_RANGE

   
      do j=JstrR,JendR
       do i=IstrR,IendR
        totml = 0._r8
        sst = 0._r8
        sss = 0._r8
        rho = 0._r8
        do k=N(ng),nbot(i,j),-1
         dml = GRID(ng)%z_w(i,j,k)-GRID(ng)%z_w(i,j,k-1)
         totml = totml + dml

         sst = sst+OCEAN(ng)%t(i,j,k,NOUT,itemp)*dml
         sss = sss+OCEAN(ng)%t(i,j,k,NOUT,isalt)*dml
         rho = rho+rho3d(i,j,k)*dml 
        end do
        sst2d(i,j) = sst/totml 
        sss2d(i,j) = sss/totml
        rho2d(i,j) = rho/totml
        ml2d(i,j) = totml 
       end do
      end do

!  Schedule sending fields to the cice model.
!
      Iexport=0
      DO ifield=1,Nexport(Iocean)
         id=ExportID(Iocean)%val(ifield)
         code=ADJUSTL(Fields(id)%code)
         gtype=Fields(id)%GridType
         scale=Fields(id)%scale
         add_offset=Fields(id)%AddOffset

         SELECT CASE (TRIM(code))

         CASE ('SST')

            CALL ROMS_export2d (ng, tile,                               &
     &                          id, gtype, scale, add_offset,           &
     &                          LBi, UBi, LBj, UBj,                     &
!     &                          OCEAN(ng)%t(:,:,N(ng),NOUT,itemp),      &
     &                          sst2d,                                  &
     &                          Fields(id)%ExpMin, Fields(id)%ExpMax,   &
     &                          Asize, A,                               &
     &                          status)
#ifdef REPORT_COUPLING_ALL
            write(stdout,*)                                             &
     &           'ROMS rank ', MyRank, ' sending sst field (max/min): ',&
     &           maxval(A), ' ', minval(A),' gtype : ', gtype
#endif
            CALL AttrVect_importRAttr(ocn2cice_AV, TRIM(code), A, Asize)
            Iexport=Iexport+1

         CASE ('SSS')

            CALL ROMS_export2d (ng, tile,                               &
     &                          id, gtype, scale, add_offset,           &
     &                          LBi, UBi, LBj, UBj,                     &
!     &                          OCEAN(ng)%t(:,:,N(ng),NOUT,isalt),      &
     &                          sss2d,                                  &
     &                          Fields(id)%ExpMin, Fields(id)%ExpMax,   &
     &                          Asize, A,                               &
     &                          status)
#ifdef REPORT_COUPLING_ALL
            write(stdout,*) &
     &           'ROMS rank ', MyRank, ' sending sss field (max/min): ',&
     &           maxval(A), ' ', minval(A),' gtype : ', gtype
#endif
            CALL AttrVect_importRAttr(ocn2cice_AV, TRIM(code), A, Asize)
            Iexport=Iexport+1

         CASE ('FRZMLT')

           do j=JstrR,JendR
            do i=IstrR,IendR
             if (ICE(ng)%qfraz_accum(i,j).gt.0.0_r8) then
              ICE(ng)%qfraz_accum(i,j)=                                 &
     &               ICE(ng)%qfraz_accum(i,j)/ncouple
             else
!jd Assume 5-meter thickness of layer. Here melt potential should be a proper
!jd weight over a typical mixed (?)/ near-ice layer.
!             ICE(ng)%qfraz_accum(i,j) = rho0*Cp *                       &
!     &        min(t_freeze(OCEAN(ng)%t(i,j,N(ng),NOUT,isalt),0.0_r8)    &
!     &        - OCEAN(ng)%t(i,j,N(ng),NOUT,itemp), 0.0_r8 )             &
!     &        * 5.0_r8 / (ncouple*dt(ng))

              ICE(ng)%qfraz_accum(i,j) = rho2d(i,j) * Cp *               &
      &        min(t_freeze(max(0._r8,sss2d(i,j)),0.0_r8)-               &
      &        sst2d(i,j), 0.0_r8 ) * ml2d(i,j) / (ncouple*dt(ng))
             end if
            end do
           end do

           CALL ROMS_export2d (ng, tile,                                &
     &                         id, gtype, scale, add_offset,            &
     &                         LBi, UBi, LBj, UBj,                      &
     &                         ICE(ng)%qfraz_accum(:,:),                &
     &                         Fields(id)%ExpMin, Fields(id)%ExpMax,    &
     &                         Asize, A,                                &
     &                         status)
#ifdef REPORT_COUPLING_ALL
           write(stdout,*) &
     &          'ROMS rank ',MyRank,' sending frzmlt field (max/min):', &
     &          maxval(A), ' ', minval(A),' gtype : ', gtype
#endif
           CALL AttrVect_importRAttr(ocn2cice_AV, TRIM(code), A, Asize)
           Iexport=Iexport+1

!jd Reset accumulation array
           ICE(ng)%qfraz_accum(:,:) = 0


        CASE ('u')
#  define I_RANGE MAX(Istr-1,1),Iend+1
           do j=Jstr,Jend
              do i=I_RANGE
                 nbotu = NINT(0.5_r8*(nbotvel(i-1,j)+nbotvel(i,j)))
                 nbotu = max(min(nbotu,N(ng)),1)
                 uw(i,j) = 0._r8
                 totml = 0._r8
                 do k=N(ng),nbotu,-1
                    dml = 0.5_r8*(GRID(ng)%z_w(i-1,j,k)                 &
     &                   -GRID(ng)%z_w(i-1,j,k-1)                       &
     &                   + GRID(ng)%z_w(i,j,k)-GRID(ng)%z_w(i,j,k-1))
                    uw(i,j) = uw(i,j) + OCEAN(ng)%u(i,j,k,NOUT)*dml
                    totml = totml + dml
                 enddo
                 uw(i,j) = uw(i,j)/totml
#ifdef CPMAXSPEED
                 uw(i,j) = min(maxcurvel,abs(uw(i,j)))*SIGN(1.0,uw(i,j))
#endif
              enddo
           enddo
#  undef I_RANGE
           do j=Jstr,Jend
              do i=IstrU,Iend
                 uwater(i,j) = uw(i,j)
              enddo
           enddo
!  Apply boundary conditions.
           CALL bc_u2d_tile (ng, tile,                                  &
     &          LBi, UBi, LBj, UBj,                                     &
     &          uwater)
#ifdef DISTRIBUTE
           CALL mp_exchange2d (ng, tile, iNLM, 1,                       &
     &          LBi, UBi, LBj, UBj,                                     &
     &          NghostPoints, EWperiodic(ng), NSperiodic(ng),           &
     &          uwater)
#endif
!            write(stdout,*) TRIM(code),id,gtype
           CALL ROMS_export2d (ng, tile,                                &
     &                         id, gtype, scale, add_offset,            &
     &                         LBi, UBi, LBj, UBj,                      &
     &                         uwater,                                  &
!     &                         OCEAN(ng)%u(:,:,N(ng),NOUT),            &
     &                         Fields(id)%ExpMin, Fields(id)%ExpMax,    &
     &                         Asize, A,                                &
     &                         status)

#ifdef REPORT_COUPLING_ALL
           write(stdout,*)                                              &
     &          'ROMS rank ',MyRank,' sending u field (max/min):',      &
     &          maxval(A), ' ', minval(A),' gtype : ', gtype
#endif
           CALL AttrVect_importRAttr(ocn2cice_AV, TRIM(code), A, Asize)
           Iexport=Iexport+1

        CASE ('v')
#  define J_RANGE MAX(Jstr-1,1),Jend+1
           do j=J_RANGE
              do i=Istr,Iend
                 nbotv = NINT(0.5_r8*(nbotvel(i,j-1)+nbotvel(i,j)))
                 nbotv = max(min(nbotv,N(ng)),1)
                 vw(i,j) = 0._r8
                 totml = 0._r8
                 do k=N(ng),nbotv,-1
                    dml = 0.5_r8*(GRID(ng)%z_w(i,j-1,k)-GRID(ng)%z_w(i,j-1,k-1) + GRID(ng)%z_w(i,j,k)-GRID(ng)%z_w(i,j,k-1))
                    vw(i,j) = vw(i,j) + OCEAN(ng)%v(i,j,k,NOUT)*dml
                    totml = totml + dml
                 enddo
                 vw(i,j) = vw(i,j)/totml
#ifdef CPMAXSPEED
                 vw(i,j) = min(maxcurvel,abs(vw(i,j)))*SIGN(1.0,vw(i,j))
#endif
              enddo
           enddo
#  undef J_RANGE
           do j=JstrV,Jend
              do i=Istr,Iend
                 vwater(i,j) = vw(i,j)
              enddo
             enddo
!  Apply boundary conditions.
             CALL bc_v2d_tile (ng, tile,                                &
     &            LBi, UBi, LBj, UBj,                                   &
     &            vwater)
#ifdef DISTRIBUTE
             CALL mp_exchange2d (ng, tile, iNLM, 1,                     &
     &            LBi, UBi, LBj, UBj,                                   &
     &            NghostPoints, EWperiodic(ng), NSperiodic(ng),         &
     &            vwater)
#endif
             CALL ROMS_export2d (ng, tile,                              &
     &                           id, gtype, scale, add_offset,          &
     &                           LBi, UBi, LBj, UBj,                    &
     &                           vwater,                                &
!     &                          OCEAN(ng)%v(:,:,N(ng),NOUT),            &
     &                           Fields(id)%ExpMin, Fields(id)%ExpMax,  &
     &                           Asize, A,                              &
     &                           status)
#ifdef REPORT_COUPLING_ALL
             write(stdout,*)                                            &
     &            'ROMS rank ',MyRank,' sending v field (max/min):',    &
     &            maxval(A), ' ', minval(A),' gtype : ', gtype
#endif
             CALL AttrVect_importRAttr(ocn2cice_AV, TRIM(code),A,Asize)
             Iexport=Iexport+1

          CASE ('SSH')

             CALL ROMS_export2d (ng, tile,                              &
     &                          id, gtype, scale, add_offset,           &
     &                          LBi, UBi, LBj, UBj,                     &
     &                          OCEAN(ng)%zeta(:,:,KOUT),               &
     &                          Fields(id)%ExpMin, Fields(id)%ExpMax,   &
     &                          Asize, A,                               &
     &                          status)
#ifdef REPORT_COUPLING_ALL
             write(stdout,*)                                            &
     &           'ROMS rank ',MyRank,' sending SSH field (max/min):',   &
     &            maxval(A), ' ', minval(A),' gtype : ', gtype
#endif
             CALL AttrVect_importRAttr(ocn2cice_AV,TRIM(code),A,Asize)
             Iexport=Iexport+1


        END SELECT
#if !defined SILENT
        IF (Master) write(stdout,*)                                     &
     &       'o2i: ',trim(code),' min/max', Fields(id)%ExpMin,        &
     &          Fields(id)%ExpMax
#endif


      END DO

!
!  Send ocean fields
!
      IF (Iexport.gt.0) THEN

#ifdef REPORT_COUPLING_ALL
         if (master)                                                    &
     &        write(stdout,*)'ROMS sends', iexport,' fields to CICE'
#endif
         CALL MCT_Send(ocn2cice_AV, ROMStoCICE)
      END IF

!
!  Deallocate communication arrays.
!
      deallocate (A,nbot,nbotvel,uw,vw,uwater,vwater)
      deallocate (sst2d,sss2d,rho2d,ml2d,rho3d)

#ifdef PROFILE
      CALL wclock_off (ng, iNLM, 36, __LINE__, MyFile)
#endif

      RETURN
      END SUBROUTINE ocn2cice_coupling

      SUBROUTINE finalize_ocn2cice_coupling
!
!========================================================================
!                                                                       !
!  This routine finalizes ocean and ice models coupling data streams.  !
!                                                                       !
!========================================================================
!
!  Local variable declarations.
!
      integer :: MyError

!
!-----------------------------------------------------------------------
!  Deallocate MCT environment.
!-----------------------------------------------------------------------
!
      CALL Router_clean (ROMStoCICE, MyError)
      CALL AttrVect_clean (ocn2cice_AV, MyError)
      CALL GlobalSegMap_clean (GSMapROMS, MyError)

      RETURN

      END SUBROUTINE finalize_ocn2cice_coupling

# endif

#endif
      END MODULE ocean_coupler_mod
