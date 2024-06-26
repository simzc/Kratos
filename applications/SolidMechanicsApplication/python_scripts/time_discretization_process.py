#import kratos core and applications
import KratosMultiphysics
import KratosMultiphysics.SolidMechanicsApplication as KratosSolid

def Factory(custom_settings, Model):
    if( not isinstance(custom_settings,KratosMultiphysics.Parameters) ):
        raise Exception("expected input shall be a Parameters object, encapsulating a json string")
    return TimeDiscretizationProcess(Model, custom_settings["Parameters"])

## All the processes python should be derived from "Process"
class TimeDiscretizationProcess(KratosMultiphysics.Process):
    def __init__(self, Model, custom_settings ):
        KratosMultiphysics.Process.__init__(self)

        ##settings string in json format
        default_settings = KratosMultiphysics.Parameters("""
        {
                "model_part_name":"MODEL_PART_NAME",
                "start_time": 0,
                "end_time": 1,
                "time_step": 1,
                "prediction_level": -1,
                "increase_factor": 2,
                "decrease_factor": 2,
                "steps_update_delay": 4
        }
        """)

        ##overwrite the default settings with user-provided parameters
        self.settings = custom_settings
        self.settings.ValidateAndAssignDefaults(default_settings)

        self.model = Model


    def ExecuteInitialize(self):

        # set model part
        self.model_part = self.model[self.settings["model_part_name"].GetString()]

        self.TimeDiscretizationProcess = KratosSolid.TimeDiscretizationProcess(self.model_part, self.settings)

    def Execute(self):

        self.TimeDiscretizationProcess.Execute()
