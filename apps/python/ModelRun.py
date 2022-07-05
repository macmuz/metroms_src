import os
import netCDF4
import Constants
from GlobalParams import *
from datetime import datetime
from shutil import copyfile

class ModelRun(object):
    _params=None
    _clmfileoption=None
    _atmfileoption=None

    def __init__(self,params,clmoption=Constants.NC,atmoption=Constants.NC):
        self._params=params
        self._clmfileoption=clmoption
        self._atmfileoption=atmoption

    def run_roms(self,runoption=Constants.SERIAL,debugoption=Constants.NODEBUG,
                 architecture=Constants.TRYTON):
        """
        About this...
        """
        print("Running ROMS in directory: "+self._params.RUNPATH+"\n\n")
        os.chdir(self._params.RUNPATH)
        # Prepare roms input-file, replace keywords:
        if(self._params.COPYFILES == True):
            self._replace_keywords_roms_in()
            self._replace_keywords_cice_in()
        # Check to see if roms ini- and cice ini-file have same timestamp:
        if (self._params.RESTART == True):
            self._check_starttime()
        self._run(runoption,debugoption,architecture)
        # Should check output-files to verify a successful run?
        # Output to std.out that model has finished:
        print("\nROMS run finished")

    def preprocess(self):
        print(self._params.ROMSINIRST)
        """
        Contains collection of preprocessors common for all models.
        """
        os.chdir(self._params.RUNPATH)
        self._make_OBC()
        self._tpxo2romstide(None,None,None)
        self._make_atm_force()
        if (self._params.RESTART == True):
            print("Model is restarting from previuos solution...")

    def postprocess(self):
        """
        """
        os.chdir(self._params.RUNPATH)
        #pass

########################################################################
# PRIVATE METHODS:
########################################################################

    def _replace_keywords_roms_in(self):
        """
        This function will replace the keywords in the given 
        keyword.in-file.
        """
        with open(self._params.KEYWORDFILE, 'r') as f:
            newlines = f.read()
        for key,value in self._params.KEYWORDLIST:
            newlines = newlines.replace(key,value)
        with open(self._params.ROMSINFILE, 'w') as f:
            f.write(newlines)

    def _replace_keywords_cice_in(self):
        """
        This function will replace the keywords in the given 
        cice-keyword.in-file.
        """
        with open(self._params.CICEKEYWORDFILE, 'r') as f:
            newlines = f.read()
        for key,value in self._params.CICEKEYWORDLIST:
            newlines = newlines.replace(key,value)
        with open(self._params.CICEINFILE, 'w') as f:
            f.write(newlines)

    def _execute_roms_mpi(self,ncpus,infile,debugoption=Constants.NODEBUG,architecture=Constants.TRYTON):
        """
        Execute the ROMS model itself using MPI.
        """
        if debugoption==Constants.DEBUG:
            executable="romsG"
        else:
            executable="romsM"

        if architecture==Constants.TRYTON:
            os.system("mpirun -np "+str(ncpus)+" ./"+executable+" "+infile)


    def _execute_roms_openmp(self,ncpus,infile,debugoption=Constants.NODEBUG):
        """
        Execute the ROMS model itself using OpenMP.
        """
        executable="oceanO" if debugoption==Constants.NODEBUG else "oceanG"
        os.environ['OMP_NUM_THREADS'] = str(ncpus)
        os.system("./"+executable+" < "+infile)

    def _execute_roms_serial(self,infile,debugoption=Constants.NODEBUG):
        """
        Execute the ROMS model itself in serial mode.
        """
        executable="oceanS" if debugoption==Constants.NODEBUG else "oceanG"
        os.system("./"+executable+" < "+infile)

    def _fimex_hor_interp(self,ncfilein,ncfileout):
        """
        """
        #Denne funksjonen maa ha info om input og ouput grid
        #og configfiler..?
        print("fimex_hor_interp")

    def _fimex_felt2nc(self,infile,outfile,configfile):
        """
        """
        #fimex --input.file=fil.flt --input.type=felt --output.file=fil.nc --input.config=config.cfg
        print("fimex_felt2nc")

    def _verticalinterp(self,ncfilein,ncfileout):
        """
        """
        print("verticalinterp")

    def _bry_from_clm(self,clmfile,bryfile):
        print("bry_from_clm")

    def _ini_from_clm(self,clmfile,inifile):
        print("ini_from_clm")

    def _tpxo2romstide(self,grdfile,tpxofilein,tidefileout):
        """
        Create tide.nc-file from any TPXO nc-file.
        """
        print("tpxo2romstide")

    def _dew2spec(self,ncfile):
        print("dew2spec")

    def _make_atm_force(self):  #tHIS METHOD CAN VARY BETWEEN MODELS...
        print("make_atm_force start")
        self._fimex_felt2nc(None,None,None)
        self._dew2spec(None)
        print("make_atm_force end")

    def _make_OBC(self):
        self._verticalinterp(self._get_clmfile(),GlobalParams.CLMFILE)
        self._bry_from_clm(GlobalParams.CLMFILE,None)
        self._ini_from_clm(GlobalParams.CLMFILE,None)

    def _get_clmfile(self): 
        if self._clmfileoption==Constants.FELT:
            self._fimex_felt2nc(self._params.FELT_CLMFILE,GlobalParams.IN_CLMFILE,GlobalParams.FELT2NC_CONFIG)
        elif self._clmfileoption==Constants.NC:
            self._fimex_hor_interp(None, None)
        else:
            print("Illegal clmfileoption.")
            exit(1)
        return GlobalParams.IN_CLMFILE

    def _run(self,runoption=Constants.SERIAL,debugoption=Constants.NODEBUG,architecture=Constants.TRYTON):
        """
        """
        # Run the ROMS model:
        if architecture==Constants.TRYTON:
            if runoption==Constants.MPI:
                self._execute_roms_mpi((int(self._params.XCPU)*int(self._params.YCPU))+
                                       int(self._params.CICECPU),
                                       self._params.ROMSINFILE,debugoption,architecture)
            elif runoption==Constants.SERIAL:
                if self._params.CICECPU != 0:
                    print("MetROMS is currently not handling CICE coupling in serial")
                    exit(1)
                self._execute_roms_serial(self._params.ROMSINFILE,debugoption)
            elif runoption==Constants.DRY:
                pass
            else:
                print("No valid runoption!")
                exit(1)

        else:
            print("Unsupported architecture...")
            exit(1)

    def _check_starttime(self):
        try:
            roms_ini = netCDF4.num2date(netCDF4.Dataset(self._params.ROMSINIRST).variables['ocean_time'][:], 
                                        netCDF4.Dataset(self._params.ROMSINIRST).variables['ocean_time'].units)
        except:
            roms_ini = netCDF4.num2date(netCDF4.Dataset(self._params.ROMSINIRST).variables['ocean_time'],
                                        netCDF4.Dataset(self._params.ROMSINIRST).variables['ocean_time'].units)            
        f = open(self._params.CICERUNDIR+'/restart/ice.restart_file', 'r')
        cice_restartfile = f.readline().strip()
        cice_rst_day = netCDF4.Dataset(cice_restartfile).mday
        if (cice_rst_day == roms_ini[self._params.NRREC].day) :
            # Dates seem ok
            pass
        else:
            if (self._params.RESTART == False):
                print("There seems to be a problem with matching dates in ROMS and CICE, but will continue since this is not a restart")
            else:
                print("There seems to be a problem with matching dates in ROMS and CICE. Will exit...")
                print("ROMS: "+str(roms_ini[self._params.NRREC].day))
                print("CICE: "+str(cice_rst_day))
                exit(1)
