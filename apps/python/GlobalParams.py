class GlobalParams(object):
    import getpass
    import os
    import sys

    username=getpass.getuser()

    MYHOST=os.environ.get('MYHOST','tryton')
    app=os.environ.get('ROMS_APPLICATION','baltic')
    xcpu=os.environ.get('ROMSXCPU',6)
    ycpu=os.environ.get('ROMSYCPU',3)
    cicexcpu=os.environ.get('CICEXCPU',3)
    ciceycpu=os.environ.get('CICEYCPU',2)
    METROMSDIR=os.environ.get('METROMS_BASEDIR','/users/work/mmuzyka/roms_iopan/metroms-0.3')
    tmpdir=os.environ.get('METROMS_CASEDIR','/users/work/'+username+'/CSDIR/metroms_jj4')
    RUNDIR=tmpdir+'/run'
    

#    COMMONPATH="/disk1/"+username+"/metroms/apps/common"
    COMMONPATH=METROMSDIR+"/apps"
    COMMONORIGPATH=METROMSDIR+"/apps/origfiles"

    ########################################################################
    # Internal files:
    ########################################################################
    FELT2NC_CONFIG=COMMONORIGPATH+"/felt2nc.xml"
    IN_CLMFILE="ocean_clm_in.nc"
    CLMFILE="ocean_clm.nc"
    INIFILE="ocean_ini.nc"
