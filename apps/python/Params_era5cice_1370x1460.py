from GlobalParams import *
from Constants import *
import sys
from datetime import datetime, timedelta
import netCDF4

class Params(object):
    RUNPATH=None
    KEYWORDFILE=None
    ROMSINFILE=None
    FELT_CLMFILE=None
    KEYWORDLIST=None
    XCPU=None
    YCPU=None
    TSTEPS=None
    NRREC=None
    
    def __init__(self,app,xcpu,ycpu,start_date,end_date,nrrec=0,cicecpu=0,restart=False,copy=True):
        self.KEYWORDFILE=GlobalParams.METROMSDIR+"/apps/common/roms_in/roms_keyword.in"
        self.XCPU=xcpu
        self.YCPU=ycpu
        self.CICECPU=cicecpu
        self.FCLEN=(end_date-start_date).total_seconds()
        self.NRREC=nrrec
        self.TIMEREF=datetime(1968,5,23,0)
        self.RESTART=restart
        self.COPYFILES=copy
        if app=='baltic':
            ########################################################################
            # Name of roms.in keyword-file:
            ########################################################################
            self.RUNPATH=GlobalParams.RUNDIR+"/baltic"
            self.ROMSINFILE=self.RUNPATH+"/roms.in"
            self.CICERUNDIR=self.RUNPATH+'/cice/rundir'
            self.CICEINFILE=self.RUNPATH + "/ice_in"
            self.CICEKEYWORDFILE=self.RUNPATH + "/cice/ice_in"
            self.FELT_CLMFILE=self.RUNPATH+"/FOAM.felt"
            self.DELTAT=60
            self.CICEDELTAT=600
            self.COUPLINGTIME_I2O=600
            self.ROMSINFOLDER="/users/work/mmuzyka/CSDIR/input_1370x1460"
            self.ROMSFORCING="/users/work/mmuzyka/programs/ERA5"
            self.ROMSINIFILE = self.ROMSINFOLDER+"/ROMS_grid_05NM_015v2_filter_initial_full.nc"
#            self.ROMSINIFILE = self.ROMSINFOLDER+"/ROMS_grid_05NM_015v2_filter_initial_full_20161120.nc"
#            self.ROMSINIFILE = self.RUNPATH+"/ocean_rst_1999-11-27.nc"
            if restart == True:
                f = open(self.CICERUNDIR+'/restart/ice.restart_file', 'r')
                cice_restartfile = f.readline().strip()
                cice_rst_time = netCDF4.Dataset(cice_restartfile).istep1
                cicerst_truefalse = ".true."
                cice_runtype = "'continue'"
                cice_ic = "'default'"
                f = open(self.RUNPATH+'/roms.restart_file', 'r')
                self.ROMSINIRST = f.readline().strip()
            else:
                cice_rst_time = 0
                cicerst_truefalse = ".false."
                cice_runtype = "'initial'"
#                cice_ic = self.CICERUNDIR+"/restart/iced.1999-11-27-00000.nc"
                cice_ic = "'default'"
                self.ROMSINIRST = self.ROMSINIFILE
            ########################################################################
            # List of keywords:
            ########################################################################
            self.KEYWORDLIST=[
            ['APPTITLE',"ROMS 1087 - baltic 2.3km - Coupled ROMS-CICE"],
            ['MYAPPCPPNAME',"BALTIC"],
            ['VARFILE',GlobalParams.tmpdir+"/roms_src/ROMS/External/varinfo.dat"],
            ['XPOINTS',"1368"],  #Could read from grd-file?
            ['YPOINTS',"1458"],  #Could read from grd-file?
            ['NLEVELS',"26"],  #Could read from grd-file?
            ['GRDTHETAS',"4.0d0"],
            ['GRDTHETAB',"1.5d0"],
            ['GRDTCLINE',"20.0d0"],            
            ['_TNU2_',"0.0d2  0.0d2"],
            ['_TNU4_',"5.0d5  5.0d5"],
            ['_VISC2_',"0.0d0"],
            ['_VISC4_',"2.5d6"],
            ['XCPU',str(self.XCPU)],
            ['YCPU',str(self.YCPU)],
            ['TSTEPS',str(self.FCLEN/self.DELTAT-cice_rst_time*self.CICEDELTAT/self.DELTAT)],
            ['DELTAT',str(self.DELTAT)],
            ['RATIO',"30"],
            ['IRESTART',str(self.NRREC)],
            ['RSTSTEP',str(240*3600/int(self.DELTAT))],
            ['STASTEP',str(0*3600/int(self.DELTAT))],
            ['INFOSTEP',str(1*3600/int(self.DELTAT))],
            ['HISSTEPP',str(0*3600/int(self.DELTAT))],
            ['AVGSTEPP',str(6*3600/int(self.DELTAT))],
            ['STARTAVG',"1"],
            ['DEFAVGSTEP',str(24*3600/int(self.DELTAT))],  #if 0; all output in one avg-file
            ['STARTTIME',str((start_date-self.TIMEREF).total_seconds()/86400)],
            ['TIDEREF',str((start_date-self.TIMEREF).total_seconds()/86400)],
            ['DEFHISSTEP',str(0*3600/int(self.DELTAT))],
            ['TIMEREF',self.TIMEREF.strftime("%Y%m%d.00")],
            ['V_TRANS',"2"],
            ['_TNUDG_',"6.0d0 6.0d0"],
            ['V_STRETCH',"4"],
            ['OBCFAKTOR',"10.0"],
            ['NUDGZONEWIDTH',"10"],
            ['GRDFILE',self.ROMSINFOLDER+"/ROMS_grid_05NM_015v2_filter.nc"],
            ['INIFILE',self.ROMSINIRST],
            ['RUNDIR',self.RUNPATH],
            ['RIVERFILE',\
                self.ROMSINFOLDER+"/rivers_1470x1360x26_2016_vshape3.nc | \n"+\
                self.ROMSINFOLDER+"/rivers_1470x1360x26_2017_vshape3.nc | \n"+\
                self.ROMSINFOLDER+"/rivers_1470x1360x26_2018_vshape3.nc | \n"+\
                self.ROMSINFOLDER+"/rivers_1470x1360x26_2019_vshape3.nc"],
            ['_BRYNAME_',\
                self.ROMSINFOLDER+"/ROMS_grid_05NM_015v2_filter_bc_obs_zeta_2016.nc | \n"+\
                self.ROMSINFOLDER+"/ROMS_grid_05NM_015v2_filter_bc_obs_zeta_2017.nc | \n"+\
                self.ROMSINFOLDER+"/ROMS_grid_05NM_015v2_filter_bc_obs_zeta_2018.nc | \n"+\
                self.ROMSINFOLDER+"/ROMS_grid_05NM_015v2_filter_bc_obs_zeta_2019.nc"],
            ['TIDEDIR',self.RUNPATH],
            ['ATMDIR',\
                self.ROMSFORCING+"/baltic_lwrad_down_2016.nc | \n"+\
                self.ROMSFORCING+"/baltic_lwrad_down_2017.nc | \n"+\
                self.ROMSFORCING+"/baltic_lwrad_down_2018.nc | \n"+\
                self.ROMSFORCING+"/baltic_lwrad_down_2019.nc \ \n"+\
                self.ROMSFORCING+"/baltic_Pair_2016.nc | \n"+\
                self.ROMSFORCING+"/baltic_Pair_2017.nc | \n"+\
                self.ROMSFORCING+"/baltic_Pair_2018.nc | \n"+\
                self.ROMSFORCING+"/baltic_Pair_2019.nc \ \n"+\
                self.ROMSFORCING+"/baltic_Qair_2016.nc | \n"+\
                self.ROMSFORCING+"/baltic_Qair_2017.nc | \n"+\
                self.ROMSFORCING+"/baltic_Qair_2018.nc | \n"+\
                self.ROMSFORCING+"/baltic_Qair_2019.nc \ \n"+\
                self.ROMSFORCING+"/baltic_rain_2016.nc | \n"+\
                self.ROMSFORCING+"/baltic_rain_2017.nc | \n"+\
                self.ROMSFORCING+"/baltic_rain_2018.nc | \n"+\
                self.ROMSFORCING+"/baltic_rain_2019.nc \ \n"+\
                self.ROMSFORCING+"/baltic_swrad_2016.nc | \n"+\
                self.ROMSFORCING+"/baltic_swrad_2017.nc | \n"+\
                self.ROMSFORCING+"/baltic_swrad_2018.nc | \n"+\
                self.ROMSFORCING+"/baltic_swrad_2019.nc \ \n"+\
                self.ROMSFORCING+"/baltic_Tair_2016.nc | \n"+\
                self.ROMSFORCING+"/baltic_Tair_2017.nc | \n"+\
                self.ROMSFORCING+"/baltic_Tair_2018.nc | \n"+\
                self.ROMSFORCING+"/baltic_Tair_2019.nc \ \n"+\
                self.ROMSFORCING+"/baltic_wind_2016.nc | \n"+\
                self.ROMSFORCING+"/baltic_wind_2017.nc | \n"+\
                self.ROMSFORCING+"/baltic_wind_2018.nc | \n"+\
                self.ROMSFORCING+"/baltic_wind_2019.nc"],
            ['FORCEFILES',"7"], # The files should be specified here as well
            ['COUPLINGTIMEI2O',str(self.COUPLINGTIME_I2O)],
            ['ROMSINFILE', self.ROMSINFILE ],
            ['CICEINFILE', self.CICEINFILE ],
            ['NUMROMSCORES',str(int(self.XCPU)*int(self.YCPU))],
            ['NUMCICECORES',str(int(self.CICECPU))],
            ['NBCISTR', str(149) ],
            ['NBCIEND', str(210) ],
            ['NBCJ', str(492) ]
            ]
            ########################################################################
            # List of CICE keywords:
            ########################################################################
            self.CICEKEYWORDLIST=[
            ['CICEYEARSTART',start_date.strftime("%Y")],
            ['CICEMONTHSTART',start_date.strftime("%m")],
            ['CICEDAYSTART',start_date.strftime("%d")],
            ['CICESECSTART',start_date.strftime("%S")],
            ['CICEDELTAT',str(self.CICEDELTAT)],
            ['CICENPT',str(int((self.FCLEN/self.CICEDELTAT)-cice_rst_time))],   # minus diff restart og start_date
            ['CICERUNTYPE',cice_runtype],
            ['CICEIC',cice_ic],
            ['CICEREST',cicerst_truefalse],
            ['CICERSTTIME',cicerst_truefalse],
            ['CICEKTENS',"0.7"],
            ['CICEFYEAR',"1993"],
            ['CICEGRD',"CICE_grid_1370x1460v2.nc"],
            ['CICEKMT',"CICE_KMT_1370x1460v2.nc"],
            ['CICEBAT',"CICE_bathy_1370x1460v2.nc"],
            ]
            ########################################################################
            ########################################################################
        else:
            print('Unknown application, will exit now')
            sys.exit()


    def change_run_param(self,keyword,value):
        for keyvalue in self.KEYWORDLIST:
            if keyvalue[0]==keyword:
                keyvalue[1]=str(value)
