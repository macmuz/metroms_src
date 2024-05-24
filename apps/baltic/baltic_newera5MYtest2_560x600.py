########################################################################
# Python-modules:
########################################################################
import numpy as np
import os
import Constants
from datetime import datetime, timedelta
########################################################################
# METROMS-modules:
########################################################################
from GlobalParams import *
from Params import *
from ModelRun import *
########################################################################
########################################################################



#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
rst        = False
copyfiles  = True
#start_date = datetime(2003,10,27)
start_date = datetime(2021,1,2)
end_date   = datetime(2023,9,15)
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%



nr = -1 if rst == True else 0
app=GlobalParams.app
xcpu=int(GlobalParams.xcpu)
ycpu=int(GlobalParams.ycpu)
icecpu=int(GlobalParams.cicexcpu)*int(GlobalParams.ciceycpu)

a20params=Params(app,xcpu,ycpu,start_date,end_date,nrrec=nr,cicecpu=icecpu,restart=rst,copy=copyfiles)

modelrun=ModelRun(a20params)

modelrun.preprocess()
modelrun.run_roms(Constants.MPI,Constants.NODEBUG,Constants.TRYTON) #24h hindcast
modelrun.postprocess()
