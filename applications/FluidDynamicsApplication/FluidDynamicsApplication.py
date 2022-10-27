from KratosFluidDynamicsApplication import *

import KratosMultiphysics
from KratosMultiphysics import _ImportApplication
application = KratosFluidDynamicsApplication()
application_name = "KratosFluidDynamicsApplication"

_ImportApplication(application, application_name)

#TODO: We should add a RegistryProcess function to the python_registry.py to put the keyword in front (we don't want the user to do this)
KratosMultiphysics.Registry.AddItem(
    "Processes.KratosMultiphysics.FluidDynamicsApplication.ApplyInletProcess",
    {
        "ClassName" : "ApplyInletProcess",
        "ModuleName" : "KratosMultiphysics.FluidDynamicsApplication.apply_inlet_process"
    })
