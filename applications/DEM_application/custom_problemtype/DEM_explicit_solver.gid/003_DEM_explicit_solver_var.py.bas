# Model Type

ContinuumOption                  = "*GenData(Continuum)"
RotationOption                   = "*GenData(Rotation)"
HomogeneousMaterialOption        = "*GenData(Homogeneous_Material)"
ElementType                      = "*GenData(DEM_Element_Type)"

# Meshing Settings

CleanIndentationsOption          = "*GenData(Clean_Initial_Indentations)"

# General Settings

*format "%10.5e"
FinalTime                        = *GenData(Calculation_Time)
*format "%10.5e"
GravityX                         = *GenData(Gravity_x)
*format "%10.5e"
GravityY                         = *GenData(Gravity_y)
*format "%10.5e"
GravityZ                         = *GenData(Gravity_z)
BoundingBoxOption                = "*GenData(Bounding_Box_option)"
*format "%10.5e"
BoundingBoxEnlargementFactor     = *GenData(Bounding_Box_Enlargement_Factor)
Dimension                        = *GenData(Domain_Dimension)
OutputFileType                   = "*GenData(Output_file_type)"
Multifile                        = "*GenData(Multifile)"
PrintNeighbourLists              = "*GenData(Print_Neighbour_Lists)"
ModelDataInfo                    = "*GenData(Model_Data_Info)"

# Special features

VirtualMassOption                = "*GenData(Virtual_Mass_Option)"
*format "%10.5e"
VirtualMassCoefficient           = *GenData(Virtual_Mass_Coefficient)
*format "%10.5e"
MagicFactor                      = *GenData(Magic_Factor)
DeltaOption                      = "*GenData(Set_Initial_Indentation_To_Zero_Force)"
*format "%10.5e"
SearchRadiusExtension            = *GenData(Search_Radius_Extension)
*format "%10.5e"
AmplifiedSearchRadiusExtension   = *GenData(Amplified_Continuum_Search_Radius_Extension)
FixVelocitiesOption              = "*GenData(Fix_Velocities_At_Predetermined_Time)"
*format "%10.5e"
TotalTimePercentageFixVelocities = *GenData(Time_Step_Constrain_DOFs_Percentage)
TrihedronOption                  = "*GenData(Trihedron_On_Each_Ball)"
LimitSurfaceOption               = "*GenData(Limit_Surface)"
*format "%10.5e"
SurfaceNormalDirX                = *GenData(Surface_Normal_Dir_X)
*format "%10.5e"
SurfaceNormalDirY                = *GenData(Surface_Normal_Dir_Y)
*format "%10.5e"
SurfaceNormalDirZ                = *GenData(Surface_Normal_Dir_Z)
*format "%10.5e"
SurfacePointCoorX                = *GenData(Surface_Point_Coor_X)
*format "%10.5e"
SurfacePointCoorY                = *GenData(Surface_Point_Coor_Y)
*format "%10.5e"
SurfacePointCoorZ                = *GenData(Surface_Point_Coor_Z)
SurfaceFrictionAngle             = *GenData(Surface_Friction_Angle)
*format "%10.5e"
CylinderRadius                   = *GenData(Cylinder_Radius)

# Time Discretization Settings

IntegrationScheme               = "*GenData(Integration_Scheme)"
*format "%10.5e"
TimeStepsPerSearchStep           = *GenData(Time_Steps_Per_Search_Step)
AutoReductionOfTimeStepOption    = "*GenData(Automatic_Time_Step_Reduction_Option)"
*format "%10.5e"
DeltaTimeSafetyFactor            = *GenData(Time_Step_Safety_Factor)
*format "%10.5e"
MaxTimeStep                      = *GenData(Time_Step)
*format "%10.5e"
OutputTimeStep                   = *GenData(Output_Time_Step)
*format "%10.5e"
ControlTime                      = *GenData(Control_Time)

# Material Model

NormalForceCalculationType       = "*GenData(Normal_Force_Calculation)"
NormalDampingType                = "*GenData(Normal_Contact_Damp)"
TangentialDampingType            = "*GenData(Tangential_Contact_Damp)"
FailureCriterionType             = "*GenData(Failure_Criterion)"
*format "%10.5e"
TauZero                          = *GenData(Tau_Zero)
*format "%10.5e"
SigmaMax                         = *GenData(Sigma_Max)
*format "%10.5e"
SigmaMin                         = *GenData(Sigma_Min)
*format "%10.5e"
InternalFriction                 = *GenData(Internal_Friction)
NonLinearNormalElasticOption     = "*GenData(Non_Linear_Normal_Elastic_Model)"
*format "%10.5e"
C1                               = *GenData(C_1)
*format "%10.5e"
N1                               = *GenData(N_1)
*format "%10.5e"
C2                               = *GenData(C_2)
*format "%10.5e"
N2                               = *GenData(N_2)
RotationalSpringOption           = "*GenData(Rotation_Spring)"
RotaDampingType                  = "*GenData(Rota_Damp_Type)"

# Global Material Parameters

*format "%10.5e"
GeneralDensity                   = *GenData(General_Density)
*format "%10.5e"
GeneralYoungModulus              = *GenData(General_Young_Modulus)
*format "%10.5e"
GeneralPoissonRatio              = *GenData(General_Poisson_Ratio)
*format "%10.5e"
GeneralCohesion                  = *GenData(General_Cohesion)
*format "%10.5e"
GeneralRollingFriction           = *GenData(General_Rolling_Friction)
*format "%10.5e"
GeneralTension                   = *GenData(General_Tension)
*format "%10.5e"
GeneralRotaDampRatio             = *GenData(General_Rota_Damp_Ratio)
*format "%10.5e"
GeneralStaticFrictionCoef        = *GenData(General_Static_Friction_Coef)
*format "%10.5e"
GeneralDynamicFrictionCoef       = *GenData(General_Dynamic_Friction_Coef)
*format "%10.5e"
GeneralRestitutionCoef           = *GenData(General_Restitution_Coef)
*format "%10.5e"
GeneralColour                    = *GenData(General_Colour)
GlobalVariablesOption            = "*GenData(Globally_Specified_Variables)"
*format "%10.5e"
GlobalKn                         = *GenData(Global_KN)
*format "%10.5e"
GlobalKt                         = *GenData(Global_KT)
*format "%10.5e"
GlobalKr                         = *GenData(Global_KR)
*format "%10.5e"
GlobalRn                         = *GenData(Global_RN)
*format "%10.5e"
GlobalRT                         = *GenData(Global_RT)
*format "%10.5e"
GlobalRr                         = *GenData(Global_RR)
*format "%10.5e"
GlobalFrictionAngle              = *GenData(Global_FRI_ANG)

# Continuum Options

StressStrainOperationsOption     = "*GenData(Stress_Strain_Operations)"
ContactMeshOption                = "*GenData(Contact_Mesh)"
ConcreteTestOption               = "*GenData(Concrete_Test)"
RealTimeGraphOption              = "*GenData(Real_Time_Graph)"
TriaxialOption                   = "*GenData(Triaxial_Option)"
*format "%10.5e"
ConfinementPressure              = *GenData(Confinement_Pressure)
*format "%10.5e"
InitialPressureAplicationTime    = *GenData(Initial_Pressure_Aplication_Time)
*format "%10.5e"
TotalTimePercentAsForceAplTime   = *GenData(Total_Time_Percentage_As_Force_Aplication_Time)

#POSTPROCES

PostVelocity                     = "*GenData(VELOCITY)"
PostDisplacement                 = "*GenData(DISPLACEMENT)"
PostRadialDisplacement           = "*GenData(RADIAL_DISPLACEMENT)"
PostRHS                          = "*GenData(RHS)"
PostTotalForces                  = "*GenData(TOTAL_FORCES)"
PostDampForces                   = "*GenData(DAMP_FORCES)"
PostAppliedForces                = "*GenData(APPLIED_FORCES)"
PostRadius                       = "*GenData(RADIUS)"
PostParticleCohesion             = "*GenData(PARTICLE_COHESION)"
PostParticleTension              = "*GenData(PARTICLE_TENSION)"
PostGroupId                      = "*GenData(GROUP_ID)"
PostExportId                     = "*GenData(EXPORT_ID)"
PostExportParticleFailureId      = "*GenData(EXPORT_PARTICLE_FAILURE_ID)"
PostExportSkinSphere             = "*GenData(EXPORT_SKIN_SPHERE)"
PostLocalContactForceLow         = "*GenData(LOCAL_CONTACT_FORCE_LOW)"
PostLocalContactForceHigh        = "*GenData(LOCAL_CONTACT_FORCE_HIGH)"
PostFailureCriterionState        = "*GenData(FAILURE_CRITERION_STATE)"
PostContactFailure               = "*GenData(CONTACT_FAILURE)"
PostContactTau                   = "*GenData(CONTACT_TAU)"
PostContactSigma                 = "*GenData(CONTACT_SIGMA)"
PostAngularVelocity              = "*GenData(ANGULAR_VELOCITY)"
PostParticleMoment               = "*GenData(PARTICLE_MOMENT)"
PostEulerAngles                  = "*GenData(EULER_ANGLES)"
PostRepresentativeVolume         = "*GenData(REPRESENTATIVE_VOLUME)"
PostMeanContactArea              = "*GenData(MEAN_CONTACT_AREA)"
PostStressTensor                 = "*GenData(STRESS_TENSOR)"

#FROM CND:

*Set cond SET_SKIN_MANUALLY *elems
*Add cond INHERIT_SKIN_FROM_SURFACE *elems
*if(CondNumEntities(int))
PredefinedSkinOption             = "ON"
*else
PredefinedSkinOption             = "OFF"
*endif

TotalElementsVolume              = *tcl(DEM::Get_Mass_Elements)

# Declare Python Variables
