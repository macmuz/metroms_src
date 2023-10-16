#include "cppdefs.h"
      MODULE interp_coeff_mod

#ifdef INPUT_INTERP
      USE mod_parallel
      USE mod_iounits
      USE mod_param
      USE mod_kinds

      implicit none

      public :: get_interp_coeff, remove_interp_coeff, calc_interp
                
      integer(i4b), allocatable, public :: interp_idx(:,:,:,:)
      real(r4), allocatable, public :: interp_W(:,:,:)

      CONTAINS

      SUBROUTINE get_interp_coeff(ng)

      USE mod_scalars, ONLY: interp_file
      USE mod_netcdf
   
      integer, intent(in) :: ng 
      integer :: Imin, Imax, Jmin, Jmax, Ilen, Jlen
      integer :: ncid, varid

      Imin=IOBOUNDS(ng)%ILB_rho
      Imax=IOBOUNDS(ng)%IUB_rho
      Jmin=IOBOUNDS(ng)%JLB_rho
      Jmax=IOBOUNDS(ng)%JUB_rho

      Ilen=Imax-Imin+1
      Jlen=Jmax-Jmin+1

      allocate( interp_idx(Ilen,Jlen,4,2), interp_W(Ilen,Jlen,4) )

      CALL check(nf90_open(trim(interp_file), NF90_NOWRITE, ncid),110)
      CALL check(nf90_inq_varid(ncid, 'W', varid),111)
      CALL check(nf90_get_var(ncid, varid, interp_W),112)
      CALL check(nf90_inq_varid(ncid, 'idx', varid),113)
      CALL check(nf90_get_var(ncid, varid, interp_idx),114)
      CALL check(nf90_close( ncid ), 115 )

!      write(stdout,*) 'MACIEJ - interp_coeff OK X/Y/Myrank',Ilen,Jlen,&
!                        MyRank
!      write(stdout,*) 'idx MIN/MAX',minval(interp_idx),maxval(interp_idx)
!      write(stdout,*) 'W MIN/MAX',minval(interp_W),maxval(interp_W)

      END SUBROUTINE get_interp_coeff

      SUBROUTINE remove_interp_coeff()
      
      deallocate( interp_idx, interp_W )

      END SUBROUTINE remove_interp_coeff

      SUBROUTINE calc_interp(xin,yin,xout,yout,start,inarray,outarray)
        integer, intent(in) :: xin,yin,xout,yout,start(2)
        real(r8), dimension(xin,yin), intent(in) :: inarray
        real(r8), dimension(xout*yout), intent(out) :: outarray

        real(r8) :: tmp
        integer :: i,j,k,x,y

        do j = start(2),yout+start(2)-1
        do i = start(1),xout+start(1)-1
          tmp = 0
          do k = 1,4
            x = interp_idx(i,j,k,1)
            y = interp_idx(i,j,k,2)
            if (x.gt.0 .and. y.gt.0) then
              tmp = tmp+inarray(x,y)*interp_W(i,j,k)
            end if
          end do
          outarray(i+1-start(1)+(j-start(2))*xout) = tmp
        end do
        end do
      END SUBROUTINE calc_interp

      SUBROUTINE check(status,label)
      USE NETCDF, only: nf90_noerr, nf90_strerror
      implicit none
      INTEGER, INTENT(in) :: status , label

      IF (status .ne. nf90_noerr) then
          print*, trim(nf90_strerror(status)),'label=',label
          stop "Stopped"
      END IF
      END SUBROUTINE check

#endif
      END MODULE interp_coeff_mod
