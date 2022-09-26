# makes KratosMultiphysics backward compatible with python 2.6 and 2.7

# Application dependent names and paths
from KratosMultiphysics import _ImportApplication
import KratosMultiphysics.FluidDynamicsApplication
from KratosFluidTransportApplication import *
application = KratosFluidTransportApplication()
application_name = "KratosFluidTransportApplication"

_ImportApplication(application, application_name)