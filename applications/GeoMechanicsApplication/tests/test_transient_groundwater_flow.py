import sys
import os

sys.path.append(os.path.join('..', '..', '..'))
sys.path.append(os.path.join('..', 'python_scripts'))

import KratosMultiphysics.KratosUnittest as KratosUnittest
import test_helper

class KratosGeoMechanicsTransientGroundWaterFlowTests(KratosUnittest.TestCase):
    """
    This class contains benchmark tests which are checked with the analytical solution
    """

    def setUp(self):
        # Code here will be placed BEFORE every test in this TestCase.
        pass

    def tearDown(self):
        # Code here will be placed AFTER every test in this TestCase.
        pass

    def test_Transient_Case_A1_2D3N(self):
        test_name = 'test_Transient_Case_A1_2D3N'
        file_path = test_helper.get_file_path(os.path.join('.', test_name + '.gid'))
        stages = test_helper.run_stages(file_path, 2)

        water_pressure = test_helper.get_water_pressure(stages[-1])

        p_height_1_72_m = water_pressure[32]

        self.assertAlmostEqual(3.7224838666844984, p_height_1_72_m)



if __name__ == '__main__':
    suites = KratosUnittest.KratosSuites
    smallSuite = suites['small'] # These tests are executed by the continuous integration tool
    smallSuite.addTests(KratosUnittest.TestLoader().loadTestsFromTestCases([KratosGeoMechanicsTransientGroundWaterFlowTests]))
    allSuite = suites['all']
    allSuite.addTests(smallSuite)
    KratosUnittest.runTests(suites)
