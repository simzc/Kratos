# Applications requiered
from KratosMultiphysics.DelaunayMeshingApplication import *

# Application dependent names and paths
from KratosMultiphysics import _ImportApplication
import KratosMultiphysics.KratosDelaunayMeshingApplication
from KratosContactMechanicsApplication import *
application = KratosContactMechanicsApplication()
application_name = "KratosContactMechanicsApplication"

_ImportApplication(application, application_name)
