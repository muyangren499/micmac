// File Automatically generated by eLiSe
#include "StdAfx.h"


class cEqAppui_PTInc_M2CFour19x2: public cElCompiledFonc
{
   public :

      cEqAppui_PTInc_M2CFour19x2();
      void ComputeVal();
      void ComputeValDeriv();
      void ComputeValDerivHessian();
      double * AdrVarLocFromString(const std::string &);
      void SetFour19x2_State_0_0(double);
      void SetFour19x2_State_1_0(double);
      void SetFour19x2_State_2_0(double);
      void SetScNorm(double);
      void SetXIm(double);
      void SetYIm(double);


      static cAutoAddEntry  mTheAuto;
      static cElCompiledFonc *  Alloc();
   private :

      double mLocFour19x2_State_0_0;
      double mLocFour19x2_State_1_0;
      double mLocFour19x2_State_2_0;
      double mLocScNorm;
      double mLocXIm;
      double mLocYIm;
};
