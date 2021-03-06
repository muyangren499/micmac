// File Automatically generated by eLiSe
#include "StdAfx.h"
#include "cRegCamConseq_Polyn0.h"


cRegCamConseq_Polyn0::cRegCamConseq_Polyn0():
    cElCompiledFonc(2)
{
   AddIntRef (cIncIntervale("Intr",0,3));
   AddIntRef (cIncIntervale("Intr2",3,6));
   Close(false);
}



void cRegCamConseq_Polyn0::ComputeVal()
{
   double tmp0_ = mCompCoord[0];
   double tmp1_ = tmp0_ / mLocRay_z;
   double tmp2_ = mCompCoord[3];
   double tmp3_ = tmp2_ / mLocRay_z;

  mVal[0] = (mLocRay_x * (tmp1_) + mCompCoord[1]) - (mLocRay_x * (tmp3_) + mCompCoord[4]);

  mVal[1] = (mLocRay_y * (tmp1_) + mCompCoord[2]) - (mLocRay_y * (tmp3_) + mCompCoord[5]);

}


void cRegCamConseq_Polyn0::ComputeValDeriv()
{
   double tmp0_ = ElSquare(mLocRay_z);
   double tmp1_ = mLocRay_z / tmp0_;
   double tmp2_ = (tmp1_) * mLocRay_x;
   double tmp3_ = mCompCoord[0];
   double tmp4_ = tmp3_ / mLocRay_z;
   double tmp5_ = mCompCoord[3];
   double tmp6_ = tmp5_ / mLocRay_z;
   double tmp7_ = (tmp1_) * mLocRay_y;
   double tmp8_ = -(1);

  mVal[0] = (mLocRay_x * (tmp4_) + mCompCoord[1]) - (mLocRay_x * (tmp6_) + mCompCoord[4]);

  mCompDer[0][0] = tmp2_;
  mCompDer[0][1] = 1;
  mCompDer[0][2] = 0;
  mCompDer[0][3] = -(tmp2_);
  mCompDer[0][4] = tmp8_;
  mCompDer[0][5] = 0;
  mVal[1] = (mLocRay_y * (tmp4_) + mCompCoord[2]) - (mLocRay_y * (tmp6_) + mCompCoord[5]);

  mCompDer[1][0] = tmp7_;
  mCompDer[1][1] = 0;
  mCompDer[1][2] = 1;
  mCompDer[1][3] = -(tmp7_);
  mCompDer[1][4] = 0;
  mCompDer[1][5] = tmp8_;
}


void cRegCamConseq_Polyn0::ComputeValDerivHessian()
{
  ELISE_ASSERT(false,"Foncteur cRegCamConseq_Polyn0 Has no Der Sec");
}

void cRegCamConseq_Polyn0::SetRay_x(double aVal){ mLocRay_x = aVal;}
void cRegCamConseq_Polyn0::SetRay_y(double aVal){ mLocRay_y = aVal;}
void cRegCamConseq_Polyn0::SetRay_z(double aVal){ mLocRay_z = aVal;}



double * cRegCamConseq_Polyn0::AdrVarLocFromString(const std::string & aName)
{
   if (aName == "Ray_x") return & mLocRay_x;
   if (aName == "Ray_y") return & mLocRay_y;
   if (aName == "Ray_z") return & mLocRay_z;
   return 0;
}


cElCompiledFonc::cAutoAddEntry cRegCamConseq_Polyn0::mTheAuto("cRegCamConseq_Polyn0",cRegCamConseq_Polyn0::Alloc);


cElCompiledFonc *  cRegCamConseq_Polyn0::Alloc()
{  return new cRegCamConseq_Polyn0();
}


