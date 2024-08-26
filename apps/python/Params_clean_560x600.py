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
            self.DELTAT=30
            self.CICEDELTAT=900
            self.COUPLINGTIME_I2O=900
            self.ROMSINFOLDER="/users/project1/pt00625/CSDIR/input_dec"
            self.ROMSFORCING="/users/project1/pt00625/CSDIR/forcing_560x600_CERRA"
            self.ROMSINIFILE = self.ROMSINFOLDER+"/ROMS_grid_125NM_dec_mask2_19930223_initial_fullnew.nc"
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
            ['XPOINTS',"558"],  #Could read from grd-file?
            ['YPOINTS',"598"],  #Could read from grd-file?
            ['NLEVELS',"30"],  #Could read from grd-file?
            ['GRDTHETAS',"0.1d0"],
            ['GRDTHETAB',"1.0d0"],
            ['GRDTCLINE',"20.0d0"],            
            ['_TNU2_',"0.0d2  0.0d2"],
            ['_TNU4_',"3.0d7  3.0d7"],
            ['_VISC2_',"0.0d0"],
            ['_VISC4_',"3.0d9"],
            ['XCPU',str(self.XCPU)],
            ['YCPU',str(self.YCPU)],
            ['TSTEPS',str(self.FCLEN/self.DELTAT-cice_rst_time*self.CICEDELTAT/self.DELTAT)],
            ['DELTAT',str(self.DELTAT)],
            ['RATIO',"60"],
            ['IRESTART',str(self.NRREC)],
            ['RSTSTEP',str(240*3600/int(self.DELTAT))],
            ['STASTEP',str(0*3600/int(self.DELTAT))],
            ['INFOSTEP',str(1*3600/int(self.DELTAT))],
            ['HISSTEPP',str(0*3600/int(self.DELTAT))],
            ['AVGSTEPP',str(24*3600/int(self.DELTAT))],
            ['STARTAVG',"1"],
            ['DEFAVGSTEP',str(24*3600/int(self.DELTAT))],  #if 0; all output in one avg-file
            ['STARTTIME',str((start_date-self.TIMEREF).total_seconds()/86400)],
            ['TIDEREF',str((start_date-self.TIMEREF).total_seconds()/86400)],
            ['DEFHISSTEP',str(0*3600/int(self.DELTAT))],
            ['TIMEREF',self.TIMEREF.strftime("%Y%m%d.00")],
            ['V_TRANS',"2"],
            ['_TNUDG_',"1.1574d-3 1.1574d-3"],
            ['V_STRETCH',"4"],
            ['OBCFAKTOR',"1.0"],
            ['NUDGZONEWIDTH',"10"],
            ['GRDFILE',self.ROMSINFOLDER+"/ROMS_grid_125NM_dec_mask2new.nc"],
            ['INIFILE',self.ROMSINIRST],
            ['RUNDIR',self.RUNPATH],
            ['RIVERFILE',\
                self.ROMSINFOLDER+"/rivers_125NM_dec_vertical_1996_Luv.nc | \n"+\
                self.ROMSINFOLDER+"/rivers_125NM_dec_vertical_1997_Luv.nc"],
            ['_BRYNAME_',\
                self.ROMSINFOLDER+"/ROMS_grid_125NM_dec_bc_bar_spline_1996.nc | \n"+\
                self.ROMSINFOLDER+"/ROMS_grid_125NM_dec_bc_bar_spline_1997.nc"],
            ['_CLMNAME_',\
                self.ROMSINFOLDER+"/ROMS_clima_1996.nc | \n"+\
                self.ROMSINFOLDER+"/ROMS_clima_1997.nc"],
            ['_NUDNAME_',\
                self.ROMSINFOLDER+"/ROMS_nud60.nc"],
            ['TIDEDIR',self.RUNPATH],
            ['ATMDIR',\
                self.ROMSFORCING+"/baltic_lwrad_down_1996.nc | \n"+\
                self.ROMSFORCING+"/baltic_lwrad_down_1997.nc \ \n"+\
                self.ROMSFORCING+"/baltic_Pair_1996.nc | \n"+\
                self.ROMSFORCING+"/baltic_Pair_1997.nc \ \n"+\
                self.ROMSFORCING+"/baltic_Qair_1996.nc | \n"+\
                self.ROMSFORCING+"/baltic_Qair_1997.nc \ \n"+\
                self.ROMSFORCING+"/baltic_rain_1996.nc | \n"+\
                self.ROMSFORCING+"/baltic_rain_1997.nc \ \n"+\
                self.ROMSFORCING+"/baltic_Tair_1996.nc | \n"+\
                self.ROMSFORCING+"/baltic_Tair_1997.nc \ \n"+\
                self.ROMSFORCING+"/baltic_cloud_1996.nc | \n"+\
                self.ROMSFORCING+"/baltic_cloud_1997.nc \ \n"+\
                self.ROMSFORCING+"/baltic_wind_1996.nc | \n"+\
                self.ROMSFORCING+"/baltic_wind_1997.nc"],
            ['FORCEFILES',"7"], # The files should be specified here as well
            ['COUPLINGTIMEI2O',str(self.COUPLINGTIME_I2O)],
            ['ROMSINFILE', self.ROMSINFILE ],
            ['CICEINFILE', self.CICEINFILE ],
            ['NUMROMSCORES',str(int(self.XCPU)*int(self.YCPU))],
            ['NUMCICECORES',str(int(self.CICECPU))],
            ['NBCISTR', str(60) ],
            ['NBCIEND', str(88) ],
            ['NBCJ', str(205) ]
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
            ['CICEKTENS',"0.2"],
            ['CICEFYEAR',"1992"],
            ['CICEGRD',"CICE_grid_dec_mask2.nc"],
            ['CICEKMT',"CICE_KMT_dec_mask2.nc"],
            ['CICEBAT',"CICE_bathy_dec_mask2.nc"],
            ['CICEINTERP',"/users/project1/pt00625/CSDIR/input_october/out_1_25NM.nc"],
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
