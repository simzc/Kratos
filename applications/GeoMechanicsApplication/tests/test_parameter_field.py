import os
import shutil

import KratosMultiphysics as Kratos
import KratosMultiphysics.GeoMechanicsApplication as KratosGeo
import KratosMultiphysics.KratosUnittest as KratosUnittest
import test_helper


class KratosGeoMechanicsParameterFieldTests(KratosUnittest.TestCase):
    """
    This class contains tests which check if custom parameter fields are correctly added to the model
    """

    def test_parameter_field_with_function(self):
        """
        Test to check if values from a function defined parameter field are correctly added to each individual element

        """

        test_name = os.path.join("test_parameter_field", "parameter_field_input")
        file_path = test_helper.get_file_path(test_name)

        # run simulation
        simulation = test_helper.run_kratos(file_path)

        # get element centers
        elements = simulation._list_of_output_processes[0].model_part.Elements
        center_coords = [element.GetGeometry().Center() for element in elements]

        # Get test results
        results = test_helper.get_on_integration_points(simulation, Kratos.YOUNG_MODULUS)

        # assert
        for center_coord, res in zip(center_coords, results):
            expected_res = 20000 * center_coord[0] + 30000 * center_coord[1]
            self.assertAlmostEqual(expected_res, res[0])

    def test_parameter_field_with_python(self):
        """
        Test to check if values from a parameter field generated by a user defined python script are correctly added to
        each individual element

        """
        test_name = os.path.join("test_parameter_field", "parameter_field_python")
        file_path = test_helper.get_file_path(test_name)

        custom_script_name = "custom_field.py"
        custom_python_file = os.path.join(file_path, custom_script_name)

        # copy user defined python script to installation folder
        new_custom_script_path = os.path.join(os.path.dirname(KratosGeo.__file__), "user_defined_scripts")
        shutil.copy(custom_python_file,new_custom_script_path)

        # run simulation
        simulation = test_helper.run_kratos(file_path)

        # get element centers
        elements = simulation._list_of_output_processes[0].model_part.Elements
        center_coords = [element.GetGeometry().Center() for element in elements]

        # Get test results
        results = test_helper.get_on_integration_points(simulation, Kratos.YOUNG_MODULUS)

        # assert
        for center_coord, res in zip(center_coords, results):
            expected_res = 20000 * center_coord[0] + 30000 * center_coord[1]
            self.assertAlmostEqual(expected_res, res[0])

        # remove user defined python script from installation folder
        os.remove(os.path.join(new_custom_script_path, custom_script_name))

    def test_parameter_field_with_json(self):
        """
        Test to check if values from a parameter field stored in a json file are correctly added to
        each individual element

        """
        test_name = os.path.join("test_parameter_field", "parameter_field_json")
        file_path = test_helper.get_file_path(test_name)

        # run simulation
        simulation = test_helper.run_kratos(file_path)

        # Get element centers
        elements = simulation._list_of_output_processes[0].model_part.Elements
        center_coords = [element.GetGeometry().Center() for element in elements]

        # Get test results
        results = test_helper.get_on_integration_points(simulation, Kratos.YOUNG_MODULUS)

        # assert
        for center_coord, res in zip(center_coords, results):
            expected_res = 20000 * center_coord[0] + 30000 * center_coord[1]
            self.assertAlmostEqual(expected_res, res[0])

    def test_parameter_field_with_invalid_json(self):
        """
        Test to check if values from a parameter field stored in a json file are correctly added to
        each individual element

        """
        test_name = os.path.join("test_parameter_field", "invalid_parameter_field_json")
        file_path = test_helper.get_file_path(test_name)

        # run simulation and assert if correct exception is raised
        with self.assertRaises(RuntimeError) as cm:
            test_helper.run_kratos(file_path)

        self.assertTrue(r'Error: The parameter field: "invalid_parameter_field.json" does not have the same size as '
                        r'the amount of elements within the model part!' in str(cm.exception))


if __name__ == '__main__':
    KratosUnittest.main()
