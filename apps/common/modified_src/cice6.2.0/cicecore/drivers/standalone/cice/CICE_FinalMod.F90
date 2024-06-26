!=======================================================================
!
!  This module contains routines for the final exit of the CICE model,
!  including final output and clean exit from any message passing
!  environments and frameworks.
!
!  authors: Philip W. Jones, LANL
!  2006: Converted to free source form (F90) by Elizabeth Hunke
!  2008: E. Hunke moved ESMF code to its own driver

      module CICE_FinalMod

      use ice_kinds_mod
      use ice_communicate, only: my_task, master_task
      use ice_exit, only: end_run, abort_ice
      use ice_fileunits, only: nu_diag, release_all_fileunits
      use icepack_intfc, only: icepack_warnings_flush, icepack_warnings_aborted

      implicit none
      private
      public :: CICE_Finalize

!=======================================================================

      contains

!=======================================================================
!
!  This routine shuts down CICE by exiting all relevent environments.

      subroutine CICE_Finalize

      use ice_restart_shared, only: runid
      use ice_timers, only: ice_timer_stop, ice_timer_print_all, timer_total
#ifdef ROMSCOUPLED
      use CICE_MCT, only: finalize_mct_coupling
#endif

      character(len=*), parameter :: subname = '(CICE_Finalize)'

   !-------------------------------------------------------------------
   ! stop timers and print timer info
   !-------------------------------------------------------------------

      call ice_timer_stop(timer_total)        ! stop timing entire run
      call ice_timer_print_all(stats=.false.) ! print timing information

      call icepack_warnings_flush(nu_diag)
      if (icepack_warnings_aborted()) call abort_ice(error_message=subname, &
          file=__FILE__,line= __LINE__)

      if (my_task == master_task) then
         write(nu_diag, *) " "
         write(nu_diag, *) "CICE COMPLETED SUCCESSFULLY "
         write(nu_diag, *) " "
      endif

!echmod      if (nu_diag /= 6) close (nu_diag) ! diagnostic output
      call release_all_fileunits

   !-------------------------------------------------------------------
   ! write 'finished' file if needed
   !-------------------------------------------------------------------

      if (runid == 'bering') call writeout_finished_file()

   !-------------------------------------------------------------------
   ! quit MPI
   !-------------------------------------------------------------------

#ifdef ROMSCOUPLED
!jd Deallocate MCT infrastructure
    call finalize_mct_coupling
#else
      call end_run       ! quit MPI
#endif

      end subroutine CICE_Finalize

!=======================================================================
!
! Write a file indicating that this run finished cleanly.  This is
! needed only for runs on machine 'bering' (set using runid = 'bering').
!
!  author: Adrian Turner, LANL

      subroutine writeout_finished_file()
      
      use ice_restart_shared, only: restart_dir

      character(len=char_len_long) :: filename
      character(len=*), parameter :: subname = '(writeout_finished_file)'

      if (my_task == master_task) then
           
         filename = trim(restart_dir)//"finished"
         open(510,file=filename)
         write(510,*) "finished"
         close(510)

      endif

      end subroutine writeout_finished_file

!=======================================================================

      end module CICE_FinalMod

!=======================================================================
