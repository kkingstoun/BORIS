#pragma once

#include "Boris_Enums_Defs.h"
#if COMPILECUDA == 1

#include "BorisCUDALib.h"
#include "ErrorHandler.h"

class MeshCUDA;
class ManagedMeshCUDA;
class ManagedMeshPolicyCUDA;
class ExchangeBase;

//cannot include BorisLib.h (or even just VEC_VC.h and VEC.h) since it contains C++14 (ProgramState and Introspection in particular) code which nvcc doesn't compile.
//Since this header file inevitably gets included in .cu files (compiled with nvcc) then must forward declare VEC and VEC_VC here instead.
//To avoid incomplete type error then we must only declare pointers to these types in the class definition - otherwise how can the compiler know how much memory to allocate on object creation?
template <typename VType> class VEC;

class ExchangeBaseCUDA {

protected:

	//CMBND contacts between this mesh and other ferromagnetic meshes (we do not require other ferromagnetic meshes to have an exchange module enabled, just this one).
	std::vector<mCMBNDInfoCUDA> CMBNDcontactsCUDA;
	//...and we also need a cpu-memory version
	std::vector<CMBNDInfoCUDA> CMBNDcontacts;

	//vector of pointers to all ferromagnetic meshes in managed mesh cuda form
	std::vector<mcu_obj<ManagedMeshCUDA, ManagedMeshPolicyCUDA>*> pContactingManagedMeshes;

	//as above but this vector contains the MeshCUDA pointers
	std::vector<MeshCUDA*> pContactingMeshes;

	//pointer to CUDA version of mesh object holding the effective field module holding this CUDA module (the primary mesh)
	MeshCUDA* pMeshCUDA;

	//pointer to cpu version of this (base for exchange-type module)
	ExchangeBase* pExchBase;

protected:

	//this is overloaded by inheriting Exchange-type modules. Need this to be virtual so if for any reason a base pointer is used, the overloaded method is called instead.
	BError Initialize(void);

	//protected constructor - this class should not be instantiated by itself, but only used as a base for an exchange-type module for purposes of code reuse
	ExchangeBaseCUDA(MeshCUDA* pMeshCUDA_, ExchangeBase* pExchBase_);

	virtual ~ExchangeBaseCUDA() {}

	virtual void CalculateExchangeCoupling(mcu_val<cuBReal>& energy) = 0;
};

#endif

