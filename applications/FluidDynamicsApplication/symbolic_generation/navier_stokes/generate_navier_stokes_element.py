from sympy import *
from KratosMultiphysics import *
from KratosMultiphysics.sympy_fe_utilities import *

## Settings explanation
# DIMENSION TO COMPUTE:
# This symbolic generator is valid for both 2D and 3D cases. Since the element has been programed with a dimension template in Kratos,
# it is advised to set the dim_to_compute flag as "Both". In this case the generated .cpp file will contain both 2D and 3D implementations.
# LINEARISATION SETTINGS:
# FullNR considers the convective velocity as "v-vmesh", hence v is taken into account in the derivation of the LHS and RHS.
# Picard (a.k.a. QuasiNR) considers the convective velocity as "a", thus it is considered as a constant in the derivation of the LHS and RHS.
# DIVIDE BY RHO:
# If set to true, divides the mass conservation equation by rho in order to have a better conditioned matrix. Otherwise the original form is kept.
# ARTIFICIAL COMPRESSIBILITY:
# If set to true, the time derivative of the density is introduced in the mass conservation equation together with the state equation
# dp/drho=c^2 (being c the sound velocity). Besides, the velocity divergence is not considered to be 0. These assumptions add some extra terms
# to the usual Navier-Stokes equations that are intended to act as a soft artificial compressibility, which is controlled by the value of "c".
# In the derivation of the residual equations the space variations of the density are considered to be close to 0.
# CONVECTIVE TERM:
# If set to true, the convective term is taken into account in the calculation of the variational form. This allows generating both 
# Navier-Stokes and Stokes elements.

## Symbolic generation settings
mode = "c"
do_simplifications = False
dim_to_compute = "Both"             # Spatial dimensions to compute. Options:  "2D","3D","Both"
divide_by_rho = True                # Divide the mass conservation equation by rho
ASGS_stabilization = True           # Consider ASGS stabilization terms
formulation = "NavierStokes"        # Element type. Options: "NavierStokes", "Stokes"

if formulation == "NavierStokes":
    convective_term = True
    artificial_compressibility = True
    linearisation = "Picard" # Convective term linearisation type. Options: "Picard", "FullNR"
    output_filename = "symbolic_navier_stokes.cpp"
    template_filename = "symbolic_navier_stokes_cpp_template.cpp"
elif formulation == "Stokes":
    artificial_compressibility = False
    convective_term = False
    output_filename = "symbolic_stokes.cpp"
    template_filename = "symbolic_stokes_cpp_template.cpp"
else:
    err_msg = "Wrong formulation. Given \'" + formulation + "\'. Available options are \'NavierStokes\' and \'Stokes\'."
    raise Exception(err_msg)

info_msg = "\n"
info_msg += "Element generator settings:\n"
info_msg += "\t - Element type: " + formulation + "\n"
info_msg += "\t - Dimension: " + dim_to_compute + "\n"
info_msg += "\t - ASGS stabilization: " + str(ASGS_stabilization) + "\n"
info_msg += "\t - Pseudo-compressibility: " + str(artificial_compressibility) + "\n"
info_msg += "\t - Divide mass conservation by rho: " + str(divide_by_rho) + "\n"
print(info_msg)

if (dim_to_compute == "2D"):
    dim_vector = [2]
elif (dim_to_compute == "3D"):
    dim_vector = [3]
elif (dim_to_compute == "Both"):
    dim_vector = [2,3]

## Initialize the outstring to be filled with the template .cpp file
print("Reading template file \'"+ template_filename + "\'\n")
templatefile = open(template_filename)
outstring = templatefile.read()

for dim in dim_vector:

    if(dim == 2):
        nnodes = 3
        strain_size = 3
    elif(dim == 3):
        nnodes = 4
        strain_size = 6

    impose_partion_of_unity = False
    N,DN = DefineShapeFunctions(nnodes, dim, impose_partion_of_unity)

    ## Unknown fields definition
    v = DefineMatrix('v',nnodes,dim)            # Current step velocity (v(i,j) refers to velocity of node i component j)
    vn = DefineMatrix('vn',nnodes,dim)          # Previous step velocity
    vnn = DefineMatrix('vnn',nnodes,dim)        # 2 previous step velocity
    p = DefineVector('p',nnodes)                # Pressure
    pn = DefineVector('pn',nnodes)              # Previous step pressure
    pnn = DefineVector('pnn',nnodes)            # 2 previous step pressure

    ## Test functions definition
    w = DefineMatrix('w',nnodes,dim)            # Velocity field test function
    q = DefineVector('q',nnodes)                # Pressure field test function

    ## Other data definitions
    f = DefineMatrix('f',nnodes,dim)            # Forcing term

    ## Constitutive matrix definition
    C = DefineSymmetricMatrix('C',strain_size,strain_size)

    ## Stress vector definition
    stress = DefineVector('stress',strain_size)

    ## Other simbols definition
    c   = Symbol('c',positive= True)            # Wave length number
    dt  = Symbol('dt', positive = True)         # Time increment
    rho = Symbol('rho', positive = True)        # Density
    nu  = Symbol('nu', positive = True)         # Kinematic viscosity (mu/rho)
    mu  = Symbol('mu', positive = True)         # Dynamic viscosity
    h = Symbol('h', positive = True)
    dyn_tau = Symbol('dyn_tau', positive = True)
    stab_c1 = Symbol('stab_c1', positive = True)
    stab_c2 = Symbol('stab_c2', positive = True)

    ## Backward differences coefficients
    bdf0 = Symbol('bdf0')
    bdf1 = Symbol('bdf1')
    bdf2 = Symbol('bdf2')

    ## Data interpolation to the Gauss points
    f_gauss = f.transpose()*N
    v_gauss = v.transpose()*N

    ## Convective velocity definition
    if convective_term:
        if (linearisation == "Picard"):
            vconv = DefineMatrix('vconv',nnodes,dim)    # Convective velocity defined a symbol
        elif (linearisation == "FullNR"):
            vmesh = DefineMatrix('vmesh',nnodes,dim)    # Mesh velocity
            vconv = v - vmesh                           # Convective velocity defined as a velocity dependent variable
        else:
            raise Exception("Wrong linearisation \'" + linearisation + "\' selected. Available options are \'Picard\' and \'FullNR\'.")
        vconv_gauss = vconv.transpose()*N

    ## Compute the stabilization parameters
    stab_norm_v = 0.0
    if convective_term:
        for i in range(0, dim):
            stab_norm_v += vconv_gauss[i]**2
    else:
        # Note that in the Stokes case we define an auxiliary velocity for the stabilization constants calculation
        # This is intentionally done to avoid introducing the norm of velocity in the residual linearisation
        v_aux = DefineVector('v_aux', dim)
        for i in range(0, dim):
            stab_norm_v += v_aux[i]**2
    stab_norm_v = sqrt(stab_norm_v)

    tau1 = 1.0/((rho*dyn_tau)/dt + (stab_c2*rho*stab_norm_v)/h + (stab_c1*mu)/(h*h))   # Stabilization parameter 1
    tau2 = mu + (stab_c2*rho*stab_norm_v*h)/stab_c1                                    # Stabilization parameter 2

    ## Compute the rest of magnitudes at the Gauss points
    accel_gauss = (bdf0*v + bdf1*vn + bdf2*vnn).transpose()*N

    p_gauss = p.transpose()*N
    if artificial_compressibility:
        pder_gauss = (bdf0*p + bdf1*pn + bdf2*pnn).transpose()*N

    w_gauss = w.transpose()*N
    q_gauss = q.transpose()*N

    ## Gradients computation (fluid dynamics gradient)
    grad_w = DfjDxi(DN,w)
    grad_q = DfjDxi(DN,q)
    grad_p = DfjDxi(DN,p)
    grad_v = DfjDxi(DN,v)

    div_w = div(DN,w)
    div_v = div(DN,v)
    if convective_term:
        div_vconv = div(DN,vconv)

    grad_sym_v = grad_sym_voigtform(DN,v)       # Symmetric gradient of v in Voigt notation
    grad_w_voigt = grad_sym_voigtform(DN,w)     # Symmetric gradient of w in Voigt notation
    # Recall that the grad(w):sigma contraction equals grad_sym(w)*sigma in Voigt notation since sigma is a symmetric tensor.

    # Convective term definition
    if convective_term:
        convective_term = (vconv_gauss.transpose()*grad_v)

    ## Compute galerkin functional
    # Navier-Stokes functional
    if (divide_by_rho):
        rv_galerkin = rho*w_gauss.transpose()*f_gauss - rho*w_gauss.transpose()*accel_gauss - grad_w_voigt.transpose()*stress + div_w*p_gauss - q_gauss*div_v
        if (artificial_compressibility):
            rv_galerkin -= (1/(rho*c*c))*q_gauss*pder_gauss
        if convective_term:
            rv_galerkin -= rho*w_gauss.transpose()*convective_term.transpose()
    else:
        rv_galerkin = rho*w_gauss.transpose()*f_gauss - rho*w_gauss.transpose()*accel_gauss  - grad_w_voigt.transpose()*stress + div_w*p_gauss - rho*q_gauss*div_v
        if (artificial_compressibility):
            rv_galerkin -= (1/(c*c))*q_gauss*pder_gauss
        if convective_term:
            rv_galerkin -= rho*w_gauss.transpose()*convective_term.transpose()

    ##  Stabilization functional terms
    # Momentum conservation residual
    # Note that the viscous stress term is dropped since linear elements are used
    vel_residual = rho*f_gauss - rho*accel_gauss - grad_p
    if convective_term:
        vel_residual -= rho*convective_term.transpose()

    # Mass conservation residual
    if (divide_by_rho):
        mas_residual = -div_v
        if (artificial_compressibility):
            mas_residual -= (1/(rho*c*c))*pder_gauss
    else:
        mas_residual = -rho*div_v
        if (artificial_compressibility):
            mas_residual -= (1/(c*c))*pder_gauss

    vel_subscale = tau1*vel_residual
    mas_subscale = tau2*mas_residual

    # Compute the ASGS stabilization terms using the momentum and mass conservation residuals above
    if (divide_by_rho):
        rv_stab = grad_q.transpose()*vel_subscale
    else:
        rv_stab = rho*grad_q.transpose()*vel_subscale
    if convective_term:
        rv_stab += rho*vconv_gauss.transpose()*grad_w*vel_subscale
        rv_stab += rho*div_vconv*w_gauss.transpose()*vel_subscale
    rv_stab += div_w*mas_subscale

    ## Add the stabilization terms to the original residual terms
    if (ASGS_stabilization):
        rv = rv_galerkin + rv_stab
    else:
        rv = rv_galerkin

    ## Define DOFs and test function vectors
    dofs = Matrix( zeros(nnodes*(dim+1), 1) )
    testfunc = Matrix( zeros(nnodes*(dim+1), 1) )

    for i in range(0,nnodes):

        # Velocity DOFs and test functions
        for k in range(0,dim):
            dofs[i*(dim+1)+k] = v[i,k]
            testfunc[i*(dim+1)+k] = w[i,k]

        # Pressure DOFs and test functions
        dofs[i*(dim+1)+dim] = p[i,0]
        testfunc[i*(dim+1)+dim] = q[i,0]

    ## Compute LHS and RHS
    # For the RHS computation one wants the residual of the previous iteration (residual based formulation). By this reason the stress is
    # included as a symbolic variable, which is assumed to be passed as an argument from the previous iteration database.
    print("Computing " + str(dim) + "D RHS Gauss point contribution\n")
    rhs = Compute_RHS(rv.copy(), testfunc, do_simplifications)
    rhs_out = OutputVector_CollectingFactors(rhs, "rhs", mode)

    # Compute LHS (RHS(residual) differenctiation w.r.t. the DOFs)
    # Note that the 'stress' (symbolic variable) is substituted by 'C*grad_sym_v' for the LHS differenctiation. Otherwise the velocity terms
    # within the velocity symmetryc gradient would not be considered in the differenctiation, meaning that the stress would be considered as
    # a velocity independent constant in the LHS.
    print("Computing " + str(dim) + "D LHS Gauss point contribution\n")
    SubstituteMatrixValue(rhs, stress, C*grad_sym_v)
    lhs = Compute_LHS(rhs, testfunc, dofs, do_simplifications) # Compute the LHS (considering stress as C*(B*v) to derive w.r.t. v)
    lhs_out = OutputMatrix_CollectingFactors(lhs, "lhs", mode)

    ## Replace the computed RHS and LHS in the template outstring
    if(dim == 2):
        outstring = outstring.replace("//substitute_lhs_2D", lhs_out)
        outstring = outstring.replace("//substitute_rhs_2D", rhs_out)
    elif(dim == 3):
        outstring = outstring.replace("//substitute_lhs_3D", lhs_out)
        outstring = outstring.replace("//substitute_rhs_3D", rhs_out)

## Write the modified template
print("Writing output file \'" + output_filename + "\'")
out = open(output_filename,'w')
out.write(outstring)
out.close()
