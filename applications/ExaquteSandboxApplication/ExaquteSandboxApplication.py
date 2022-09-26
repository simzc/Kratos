# makes KratosMultiphysics backward compatible with python 2.6 and 2.7

from KratosMultiphysics import _ImportApplication
import KratosMultiphysics.MeshingApplication
from KratosExaquteSandboxApplication import *
application = KratosExaquteSandboxApplication()
application_name = "KratosExaquteSandboxApplication"

_ImportApplication(application, application_name)