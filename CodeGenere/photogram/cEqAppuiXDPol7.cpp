/*Header-MicMac-eLiSe-25/06/2007

    MicMac : Multi Image Correspondances par Methodes Automatiques de Correlation
    eLiSe  : ELements of an Image Software Environnement

    www.micmac.ign.fr

   
    Copyright : Institut Geographique National
    Author : Marc Pierrot Deseilligny
    Contributors : Gregoire Maillet, Didier Boldo.

[1] M. Pierrot-Deseilligny, N. Paparoditis.
    "A multiresolution and optimization-based image matching approach:
    An application to surface reconstruction from SPOT5-HRS stereo imagery."
    In IAPRS vol XXXVI-1/W41 in ISPRS Workshop On Topographic Mapping From Space
    (With Special Emphasis on Small Satellites), Ankara, Turquie, 02-2006.

[2] M. Pierrot-Deseilligny, "MicMac, un lociel de mise en correspondance
    d'images, adapte au contexte geograhique" to appears in 
    Bulletin d'information de l'Institut Geographique National, 2007.

Francais :

   MicMac est un logiciel de mise en correspondance d'image adapte 
   au contexte de recherche en information geographique. Il s'appuie sur
   la bibliotheque de manipulation d'image eLiSe. Il est distibue sous la
   licences Cecill-B.  Voir en bas de fichier et  http://www.cecill.info.


English :

    MicMac is an open source software specialized in image matching
    for research in geographic information. MicMac is built on the
    eLiSe image library. MicMac is governed by the  "Cecill-B licence".
    See below and http://www.cecill.info.

Header-MicMac-eLiSe-25/06/2007*/
// File Automatically generated by eLiSe

#include "general/all.h"
#include "private/all.h"
#include "cEqAppuiXDPol7.h"


cEqAppuiXDPol7::cEqAppuiXDPol7():
    cElCompiledFonc(1)
{
   AddIntRef (cIncIntervale("Intr",0,72));
   AddIntRef (cIncIntervale("Orient",72,78));
   Close(false);
}



void cEqAppuiXDPol7::ComputeVal()
{
   double tmp0_ = ElSquare(mLocYIm);
   double tmp1_ = ElSquare(mLocXIm);
   double tmp2_ = VCube(mLocYIm);
   double tmp3_ = VCube(mLocXIm);
   double tmp4_ = VPow4(mLocYIm);
   double tmp5_ = VPow4(mLocXIm);
   double tmp6_ = VPow5(mLocYIm);
   double tmp7_ = VPow5(mLocXIm);
   double tmp8_ = VPow6(mLocYIm);
   double tmp9_ = VPow6(mLocXIm);
   double tmp10_ = mCompCoord[72];
   double tmp11_ = mCompCoord[73];
   double tmp12_ = cos(tmp11_);
   double tmp13_ = sin(tmp10_);
   double tmp14_ = cos(tmp10_);
   double tmp15_ = sin(tmp11_);
   double tmp16_ = mCompCoord[74];
   double tmp17_ = mCompCoord[75];
   double tmp18_ = mLocXTer-tmp17_;
   double tmp19_ = sin(tmp16_);
   double tmp20_ = -(tmp19_);
   double tmp21_ = -(tmp15_);
   double tmp22_ = cos(tmp16_);
   double tmp23_ = mCompCoord[76];
   double tmp24_ = mLocYTer-tmp23_;
   double tmp25_ = mCompCoord[77];
   double tmp26_ = mLocZTer-tmp25_;

  mVal[0] = (mCompCoord[0]+mCompCoord[1]*mLocYIm+mCompCoord[2]*mLocXIm+mCompCoord[3]*tmp0_+mCompCoord[4]*mLocXIm*mLocYIm+mCompCoord[5]*tmp1_+mCompCoord[6]*tmp2_+mCompCoord[7]*mLocXIm*tmp0_+mCompCoord[8]*tmp1_*mLocYIm+mCompCoord[9]*tmp3_+mCompCoord[10]*tmp4_+mCompCoord[11]*mLocXIm*tmp2_+mCompCoord[12]*tmp1_*tmp0_+mCompCoord[13]*tmp3_*mLocYIm+mCompCoord[14]*tmp5_+mCompCoord[15]*tmp6_+mCompCoord[16]*mLocXIm*tmp4_+mCompCoord[17]*tmp1_*tmp2_+mCompCoord[18]*tmp3_*tmp0_+mCompCoord[19]*tmp5_*mLocYIm+mCompCoord[20]*tmp7_+mCompCoord[21]*tmp8_+mCompCoord[22]*mLocXIm*tmp6_+mCompCoord[23]*tmp1_*tmp4_+mCompCoord[24]*tmp3_*tmp2_+mCompCoord[25]*tmp5_*tmp0_+mCompCoord[26]*tmp7_*mLocYIm+mCompCoord[27]*tmp9_+mCompCoord[28]*VPow7(mLocYIm)+mCompCoord[29]*mLocXIm*tmp8_+mCompCoord[30]*tmp1_*tmp6_+mCompCoord[31]*tmp3_*tmp4_+mCompCoord[32]*tmp5_*tmp2_+mCompCoord[33]*tmp7_*tmp0_+mCompCoord[34]*tmp9_*mLocYIm+mCompCoord[35]*VPow7(mLocXIm))-(tmp14_*tmp12_*(tmp18_)+tmp13_*tmp12_*(tmp24_)+tmp15_*(tmp26_))/((-(tmp13_)*tmp20_+tmp14_*tmp21_*tmp22_)*(tmp18_)+(tmp14_*tmp20_+tmp13_*tmp21_*tmp22_)*(tmp24_)+tmp12_*tmp22_*(tmp26_));

}


void cEqAppuiXDPol7::ComputeValDeriv()
{
   double tmp0_ = ElSquare(mLocYIm);
   double tmp1_ = ElSquare(mLocXIm);
   double tmp2_ = VCube(mLocYIm);
   double tmp3_ = VCube(mLocXIm);
   double tmp4_ = VPow4(mLocYIm);
   double tmp5_ = VPow4(mLocXIm);
   double tmp6_ = VPow5(mLocYIm);
   double tmp7_ = VPow5(mLocXIm);
   double tmp8_ = VPow6(mLocYIm);
   double tmp9_ = VPow6(mLocXIm);
   double tmp10_ = mCompCoord[72];
   double tmp11_ = mCompCoord[73];
   double tmp12_ = cos(tmp11_);
   double tmp13_ = sin(tmp10_);
   double tmp14_ = cos(tmp10_);
   double tmp15_ = sin(tmp11_);
   double tmp16_ = mCompCoord[74];
   double tmp17_ = mCompCoord[75];
   double tmp18_ = mLocXTer-tmp17_;
   double tmp19_ = sin(tmp16_);
   double tmp20_ = -(tmp19_);
   double tmp21_ = -(tmp15_);
   double tmp22_ = cos(tmp16_);
   double tmp23_ = mCompCoord[76];
   double tmp24_ = mLocYTer-tmp23_;
   double tmp25_ = mCompCoord[77];
   double tmp26_ = mLocZTer-tmp25_;
   double tmp27_ = VPow7(mLocYIm);
   double tmp28_ = VPow7(mLocXIm);
   double tmp29_ = tmp14_*tmp12_;
   double tmp30_ = -(tmp13_);
   double tmp31_ = tmp30_*tmp20_;
   double tmp32_ = tmp14_*tmp21_;
   double tmp33_ = tmp32_*tmp22_;
   double tmp34_ = tmp31_+tmp33_;
   double tmp35_ = (tmp34_)*(tmp18_);
   double tmp36_ = tmp14_*tmp20_;
   double tmp37_ = tmp13_*tmp21_;
   double tmp38_ = tmp37_*tmp22_;
   double tmp39_ = tmp36_+tmp38_;
   double tmp40_ = (tmp39_)*(tmp24_);
   double tmp41_ = tmp35_+tmp40_;
   double tmp42_ = tmp12_*tmp22_;
   double tmp43_ = tmp42_*(tmp26_);
   double tmp44_ = tmp41_+tmp43_;
   double tmp45_ = tmp29_*(tmp18_);
   double tmp46_ = tmp13_*tmp12_;
   double tmp47_ = tmp46_*(tmp24_);
   double tmp48_ = tmp45_+tmp47_;
   double tmp49_ = tmp15_*(tmp26_);
   double tmp50_ = tmp48_+tmp49_;
   double tmp51_ = -(1);
   double tmp52_ = tmp51_*tmp13_;
   double tmp53_ = tmp51_*tmp15_;
   double tmp54_ = -(tmp12_);
   double tmp55_ = ElSquare(tmp44_);
   double tmp56_ = -(tmp22_);
   double tmp57_ = tmp51_*tmp19_;

  mVal[0] = (mCompCoord[0]+mCompCoord[1]*mLocYIm+mCompCoord[2]*mLocXIm+mCompCoord[3]*tmp0_+mCompCoord[4]*mLocXIm*mLocYIm+mCompCoord[5]*tmp1_+mCompCoord[6]*tmp2_+mCompCoord[7]*mLocXIm*tmp0_+mCompCoord[8]*tmp1_*mLocYIm+mCompCoord[9]*tmp3_+mCompCoord[10]*tmp4_+mCompCoord[11]*mLocXIm*tmp2_+mCompCoord[12]*tmp1_*tmp0_+mCompCoord[13]*tmp3_*mLocYIm+mCompCoord[14]*tmp5_+mCompCoord[15]*tmp6_+mCompCoord[16]*mLocXIm*tmp4_+mCompCoord[17]*tmp1_*tmp2_+mCompCoord[18]*tmp3_*tmp0_+mCompCoord[19]*tmp5_*mLocYIm+mCompCoord[20]*tmp7_+mCompCoord[21]*tmp8_+mCompCoord[22]*mLocXIm*tmp6_+mCompCoord[23]*tmp1_*tmp4_+mCompCoord[24]*tmp3_*tmp2_+mCompCoord[25]*tmp5_*tmp0_+mCompCoord[26]*tmp7_*mLocYIm+mCompCoord[27]*tmp9_+mCompCoord[28]*tmp27_+mCompCoord[29]*mLocXIm*tmp8_+mCompCoord[30]*tmp1_*tmp6_+mCompCoord[31]*tmp3_*tmp4_+mCompCoord[32]*tmp5_*tmp2_+mCompCoord[33]*tmp7_*tmp0_+mCompCoord[34]*tmp9_*mLocYIm+mCompCoord[35]*tmp28_)-(tmp50_)/(tmp44_);

  mCompDer[0][0] = 1;
  mCompDer[0][1] = mLocYIm;
  mCompDer[0][2] = mLocXIm;
  mCompDer[0][3] = tmp0_;
  mCompDer[0][4] = mLocXIm*mLocYIm;
  mCompDer[0][5] = tmp1_;
  mCompDer[0][6] = tmp2_;
  mCompDer[0][7] = mLocXIm*tmp0_;
  mCompDer[0][8] = tmp1_*mLocYIm;
  mCompDer[0][9] = tmp3_;
  mCompDer[0][10] = tmp4_;
  mCompDer[0][11] = mLocXIm*tmp2_;
  mCompDer[0][12] = tmp1_*tmp0_;
  mCompDer[0][13] = tmp3_*mLocYIm;
  mCompDer[0][14] = tmp5_;
  mCompDer[0][15] = tmp6_;
  mCompDer[0][16] = mLocXIm*tmp4_;
  mCompDer[0][17] = tmp1_*tmp2_;
  mCompDer[0][18] = tmp3_*tmp0_;
  mCompDer[0][19] = tmp5_*mLocYIm;
  mCompDer[0][20] = tmp7_;
  mCompDer[0][21] = tmp8_;
  mCompDer[0][22] = mLocXIm*tmp6_;
  mCompDer[0][23] = tmp1_*tmp4_;
  mCompDer[0][24] = tmp3_*tmp2_;
  mCompDer[0][25] = tmp5_*tmp0_;
  mCompDer[0][26] = tmp7_*mLocYIm;
  mCompDer[0][27] = tmp9_;
  mCompDer[0][28] = tmp27_;
  mCompDer[0][29] = mLocXIm*tmp8_;
  mCompDer[0][30] = tmp1_*tmp6_;
  mCompDer[0][31] = tmp3_*tmp4_;
  mCompDer[0][32] = tmp5_*tmp2_;
  mCompDer[0][33] = tmp7_*tmp0_;
  mCompDer[0][34] = tmp9_*mLocYIm;
  mCompDer[0][35] = tmp28_;
  mCompDer[0][36] = 0;
  mCompDer[0][37] = 0;
  mCompDer[0][38] = 0;
  mCompDer[0][39] = 0;
  mCompDer[0][40] = 0;
  mCompDer[0][41] = 0;
  mCompDer[0][42] = 0;
  mCompDer[0][43] = 0;
  mCompDer[0][44] = 0;
  mCompDer[0][45] = 0;
  mCompDer[0][46] = 0;
  mCompDer[0][47] = 0;
  mCompDer[0][48] = 0;
  mCompDer[0][49] = 0;
  mCompDer[0][50] = 0;
  mCompDer[0][51] = 0;
  mCompDer[0][52] = 0;
  mCompDer[0][53] = 0;
  mCompDer[0][54] = 0;
  mCompDer[0][55] = 0;
  mCompDer[0][56] = 0;
  mCompDer[0][57] = 0;
  mCompDer[0][58] = 0;
  mCompDer[0][59] = 0;
  mCompDer[0][60] = 0;
  mCompDer[0][61] = 0;
  mCompDer[0][62] = 0;
  mCompDer[0][63] = 0;
  mCompDer[0][64] = 0;
  mCompDer[0][65] = 0;
  mCompDer[0][66] = 0;
  mCompDer[0][67] = 0;
  mCompDer[0][68] = 0;
  mCompDer[0][69] = 0;
  mCompDer[0][70] = 0;
  mCompDer[0][71] = 0;
  mCompDer[0][72] = -(((tmp52_*tmp12_*(tmp18_)+tmp29_*(tmp24_))*(tmp44_)-(tmp50_)*((-(tmp14_)*tmp20_+tmp52_*tmp21_*tmp22_)*(tmp18_)+(tmp52_*tmp20_+tmp33_)*(tmp24_)))/tmp55_);
  mCompDer[0][73] = -(((tmp53_*tmp14_*(tmp18_)+tmp53_*tmp13_*(tmp24_)+tmp12_*(tmp26_))*(tmp44_)-(tmp50_)*(tmp54_*tmp14_*tmp22_*(tmp18_)+tmp54_*tmp13_*tmp22_*(tmp24_)+tmp53_*tmp22_*(tmp26_)))/tmp55_);
  mCompDer[0][74] = -(-((tmp50_)*((tmp56_*tmp30_+tmp57_*tmp32_)*(tmp18_)+(tmp56_*tmp14_+tmp57_*tmp37_)*(tmp24_)+tmp57_*tmp12_*(tmp26_)))/tmp55_);
  mCompDer[0][75] = -((tmp51_*tmp29_*(tmp44_)-(tmp50_)*tmp51_*(tmp34_))/tmp55_);
  mCompDer[0][76] = -((tmp51_*tmp46_*(tmp44_)-(tmp50_)*tmp51_*(tmp39_))/tmp55_);
  mCompDer[0][77] = -((tmp53_*(tmp44_)-(tmp50_)*tmp51_*tmp42_)/tmp55_);
}


void cEqAppuiXDPol7::ComputeValDerivHessian()
{
  ELISE_ASSERT(false,"Foncteur cEqAppuiXDPol7 Has no Der Sec");
}

void cEqAppuiXDPol7::SetXIm(double aVal){ mLocXIm = aVal;}
void cEqAppuiXDPol7::SetXTer(double aVal){ mLocXTer = aVal;}
void cEqAppuiXDPol7::SetYIm(double aVal){ mLocYIm = aVal;}
void cEqAppuiXDPol7::SetYTer(double aVal){ mLocYTer = aVal;}
void cEqAppuiXDPol7::SetZTer(double aVal){ mLocZTer = aVal;}



double * cEqAppuiXDPol7::AdrVarLocFromString(const std::string & aName)
{
   if (aName == "XIm") return & mLocXIm;
   if (aName == "XTer") return & mLocXTer;
   if (aName == "YIm") return & mLocYIm;
   if (aName == "YTer") return & mLocYTer;
   if (aName == "ZTer") return & mLocZTer;
   return 0;
}


cElCompiledFonc::cAutoAddEntry cEqAppuiXDPol7::mTheAuto("cEqAppuiXDPol7",cEqAppuiXDPol7::Alloc);


cElCompiledFonc *  cEqAppuiXDPol7::Alloc()
{  return new cEqAppuiXDPol7();
}



/*Footer-MicMac-eLiSe-25/06/2007

Ce logiciel est un programme informatique servant à la mise en
correspondances d'images pour la reconstruction du relief.

Ce logiciel est régi par la licence CeCILL-B soumise au droit français et
respectant les principes de diffusion des logiciels libres. Vous pouvez
utiliser, modifier et/ou redistribuer ce programme sous les conditions
de la licence CeCILL-B telle que diffusée par le CEA, le CNRS et l'INRIA 
sur le site "http://www.cecill.info".

En contrepartie de l'accessibilité au code source et des droits de copie,
de modification et de redistribution accordés par cette licence, il n'est
offert aux utilisateurs qu'une garantie limitée.  Pour les mêmes raisons,
seule une responsabilité restreinte pèse sur l'auteur du programme,  le
titulaire des droits patrimoniaux et les concédants successifs.

A cet égard  l'attention de l'utilisateur est attirée sur les risques
associés au chargement,  à l'utilisation,  à la modification et/ou au
développement et à la reproduction du logiciel par l'utilisateur étant 
donné sa spécificité de logiciel libre, qui peut le rendre complexe à 
manipuler et qui le réserve donc à des développeurs et des professionnels
avertis possédant  des  connaissances  informatiques approfondies.  Les
utilisateurs sont donc invités à charger  et  tester  l'adéquation  du
logiciel à leurs besoins dans des conditions permettant d'assurer la
sécurité de leurs systèmes et ou de leurs données et, plus généralement, 
à l'utiliser et l'exploiter dans les mêmes conditions de sécurité. 

Le fait que vous puissiez accéder à cet en-tête signifie que vous avez 
pris connaissance de la licence CeCILL-B, et que vous en avez accepté les
termes.
Footer-MicMac-eLiSe-25/06/2007*/
