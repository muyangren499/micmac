#include "cfeatheringandmosaicking.h"



cFeatheringAndMosaicOrtho::cFeatheringAndMosaicOrtho(int argc,char ** argv):lut_w(Im1D_REAL4(1,1))
{
    mDist=100; // distance chamfer de 100 pour le feathering/estompage
    mLambda=0.4;
    mDebug=1;
    mLabel="Label.tif";// from tawny tool
    mNameMosaicOut="MosaicFeathering.tif";
    mTmpDir="Tmp-MM-Dir/";

    ElInitArgMain
            (
                argc,argv,
                LArgMain()   << EAMC(mFullDir,"ortho pattern", eSAM_IsPatFile)
                ,
                LArgMain()  << EAM(mNameMosaicOut,"Out",true, "Name of resulting map")
                << EAM(mDist,"Dist",true, "Distance for seamline feathering blending, in chamfer 32 distance" )
                << EAM(mLambda,"Lambda",true, "lambda value for gaussian distance weighting, def 0.4." )
                << EAM(mLabel,"Label",true, "label/index map (tif) previously computed by Tawny. If not provided, generate a new label map based on incidence value" )
                << EAM(mDebug,"Debug",true, "Write intermediate results for debug purpose." )
                << EAM(mTmpDir,"TmpDir",true, "Directory for intermediate results generated in debug mode." )
                );

    if (!MMVisualMode)
    {

        checkParam();

        mDir="./";
        if (mDebug & !ELISE_fp::IsDirectory(mTmpDir)) ELISE_fp::MkDir(mTmpDir);

        mICNM = cInterfChantierNameManipulateur::BasicAlloc(mDir);
        mLFile = mICNM->StdGetListOfFile(mFullDir);

        MTD = StdGetFromPCP("MTDOrtho.xml",FileOriMnt);
        Box2dr boxMosaic(Pt2dr(MTD.OriginePlani().x,MTD.OriginePlani().y+MTD.ResolutionPlani().y*MTD.NombrePixels().y),Pt2dr(MTD.OriginePlani().x+MTD.ResolutionPlani().x*MTD.NombrePixels().x,MTD.OriginePlani().y));
        sz= Pt2di(MTD.NombrePixels().x,MTD.NombrePixels().y);
        aCorner=Pt2dr(boxMosaic._p0.x,boxMosaic._p1.y); // xmin, ymax;

        mosaic=Im2D_REAL4(sz.x,sz.y);
        NbIm=Im2D_REAL4(sz.x,sz.y);

        // either use Label already created (by tawny for eg) or generate itself the Table of label
        if (!EAMIsInit(&mLabel))
        {
           GenLabelTable();
        }
        if (ELISE_fp::exist_file(mLabel)) { label=Im2D_U_INT1::FromFileStd(mLabel); } else { std::cout << "Cannot load images label table ; " << mLabel << "\n";}

        mSumDistExt=Im2D_INT4(sz.x,sz.y);
        mSumDistInter= Im2D_INT4(sz.x,sz.y);
        PondInterne=Im2D_REAL4(sz.x,sz.y);
        mSumWeighting=Im2D_REAL4(sz.x,sz.y);
        std::string aName;

        // look up table of weight
        lut_w=Im1D_REAL4(mDist+1);
        ELISE_COPY
                (
                    lut_w.all_pts(),
                    pow(0.5,pow((FX/((double)mDist-FX+1)),2*mLambda)),
                    lut_w.out()
                    );
        // replace by 1 distance over the threshold of mDist
        ELISE_COPY(select(lut_w.all_pts(),lut_w.in()>1),1,lut_w.out());

         if (mDebug) {
        std::cout << "Look up table of weight depending on the distance from seamline\n";
        std::cout << "Blending of two images i and j in the feather buffer is done as follow: in the internal area of image i, weight of image i is 1/2 + 1/2 * Feathering weight display below. \n";
        for (unsigned int i(0); i<mDist;i++)
        {
            std::cout << "Feathering weight at Chamfer Dist=" << i << " is equal to " << lut_w.At(i) << " * O.5 + 0.5\n";
        }
        }
        // compute chamfer distance from seamline and determine the number of images for each xy location
        std::cout << "Compute Chamfer Distance from seamline for each ortho\n";
        // this is also during this stage that images are loaded
        ChamferDist4AllOrt();

        if (mDebug)
        {
            // save the map of number of image
            aName="NbIm.tif";
            std::cout << "Save map of number of ortho used for each location , file " << aName << "\n";
            SaveTiff(mTmpDir+aName,& NbIm);
            writeTFW(mTmpDir+aName, MTD.ResolutionPlani(),aCorner);
            aName="SumDistInterne.tif";
            std::cout << "Save map of sum of chamfer distance inside enveloppe for each location , file " << aName << "\n";
            SaveTiff(mTmpDir+aName,& mSumDistInter);
            writeTFW(mTmpDir+aName, MTD.ResolutionPlani(),aCorner);
            aName="SumDistExt.tif";
            std::cout << "Save map of sum of chamfer distance outside enveloppe for each location , file " << aName << "\n";
            SaveTiff(mTmpDir+aName,& mSumDistExt);
            writeTFW(mTmpDir+aName, MTD.ResolutionPlani(),aCorner);
        }

        std::cout << "Compute weighting for each orthoimage in blending zone with 2 images.\n";
        // compute the weighting for area where number of images is 1 or 2
        WeightingNbIm1and2();

        // Particular attention for area of 3 images overlapping.
        std::cout << "Start monitoring of areas of 3 orthoimages blending\n";
        WeightingNbIm3();

        if (mDebug)
        {
            // save the map of number of images
            aName="PondInterne.tif";
            std::cout << "Save map of number of orthoimage used for each location , file " << aName << "\n";
            SaveTiff(mTmpDir+aName,& PondInterne);
            writeTFW(mTmpDir+aName, MTD.ResolutionPlani(),aCorner);
        }

        std::cout << "Compute mosaic by multipling orthoimage value by weighting map.\n";
        ComputeMosaic();

        if (mDebug)
        {
            aName="SumWeighting.tif";
            std::cout << "Save map of sum of weighting, should be equal to 1 everywhere , file " << aName << "\n";
            SaveTiff(mTmpDir+aName,& mSumWeighting);
            writeTFW(mTmpDir+aName, MTD.ResolutionPlani(),aCorner);
        }


        SaveTiff(mNameMosaicOut, & mosaic);
        writeTFW(mNameMosaicOut, MTD.ResolutionPlani(),aCorner);

        banniereFeathering();

    }
}




// Chamfer::d32.im_dist compute chamfer distance from border of the image, i do not like that at all. this function add a border prior to compute chanfer distance and remove it afterward
void cFeatheringAndMosaicOrtho::ChamferNoBorder(Im2D<U_INT1,INT> i2d) const
{
    int border(200);
    Im2D_U_INT1 tmp(i2d.sz().x+2*border,i2d.sz().y+2*border,1);
    ELISE_COPY(select(tmp.all_pts(),trans(i2d.in(1),-Pt2di(border,border))==0),0,tmp.out());
    Chamfer::d32.im_dist(tmp);
    ELISE_COPY(i2d.all_pts(),trans(tmp.in(255),Pt2di(border,border)),i2d.oclip());
}


void cFeatheringAndMosaicOrtho::ChamferDist4AllOrt()
{
    unsigned int i(0); // i is the label of the image - and the key
    for (auto &im : mLFile)
    {
        if (mDebug) std::cout << "Image num " << i << " is " << im <<" : loading and computing feathering buffer.\n";
        // open orthos
        mIms[i]= new cImGeo(mDir+im);
        mIm2Ds[i]=  mIms[i]->toRAM();
        mChamferDist[i]=Im2D_INT2(mIms[i]->Im().sz().x,mIms[i]->Im().sz().y,1);
        Pt2di sz(mIms[i]->Im().sz());
        Pt2di tr= -mIms[i]->computeTrans(aCorner);

        // la fonction chamfer fonctionne sur une image binaire et va calculer les distance à partir des pixels qui ont une valeur de 0.
        // la distance maximale est de 255

        //detect seam line for this image
        //1) translation of label to be in ortho geometry and set value to 0 for points that are not inside area of mosaicking for this image
        Im2D_U_INT1 tmp(sz.x,sz.y,1);
        ELISE_COPY(select(tmp.all_pts(), trans(label.in(),tr)!=(int)i),
                   //trans(label.in(),tr),
                   0,
                   tmp.out()
                   );

        // remove very small patch for wich we do not want to perform feathering because it is ugly otherwise
        Im2D_U_INT1 Ibin(tmp.sz().x,tmp.sz().y,0);
        ELISE_COPY(Ibin.all_pts(),tmp.in(),Ibin.out());
        // completely stupid but i have to ensure the border of the bin image is not ==1 otherwise I got the error out of bitmap in dilate spec Image
        // so the code may suffer weakness if a small patch is located near the border--ask marc
        ELISE_COPY(Ibin.border(2),0,Ibin.out());
        U_INT1 ** d = Ibin.data();
        Neighbourhood V8=Neighbourhood::v8();

        for (INT x=0; x < Ibin.sz().x; x++)
        {
            for (INT y=0; y < Ibin.sz().y; y++)
            {
                if (d[y][x] == 1)
                {
                    Liste_Pts_INT2 cc(2);
                    ELISE_COPY
                            (
                                // flux: composantes connexes du point.
                                conc
                                (
                                    Pt2di(x,y),
                                    Ibin.neigh_test_and_set(V8, 1, 0,  10) ), // on change la valeur des points sélectionné comme ça à la prochaine itération on ne sélectionne plus cette zone de composante connexe
                                2, // valeur bidonne, c'est juste le flux que je sauve dans cc
                                cc
                                );
                    // remove the patch
                    if (cc.card()<pow(mDist,2)) ELISE_COPY(cc.all_pts(),0,tmp.out());
                }
            }
        }

        // compute chamfer Distance d32
        ChamferNoBorder(tmp);

        // inverse value of distance because this is inside the enveloppe
        ELISE_COPY(mChamferDist[i].all_pts(),-tmp.in(),mChamferDist[i].out());

        // Initialise tmp again
        ELISE_COPY(tmp.all_pts(),1,tmp.out());
        ELISE_COPY(
                    select(mChamferDist[i].all_pts(),mChamferDist[i].in()==-2),// distance ==-2 are pixels on the seamline (more or less)
                    0,
                    tmp.out());
        // chamfer distance outside the enveloppe
        ChamferNoBorder(tmp);

        ELISE_COPY(
                    select(mChamferDist[i].all_pts(),mChamferDist[i].in()==0),
                    tmp.in(),
                    mChamferDist[i].out());

        // apply the hidden part masq

        //std::string aNameIm =  mICNM->Assoc2To1("Key-Assoc-OpIm2PC@",im,true).first;
        std::string aNamePC = "PC"+ im.substr(3,im.size()-2);
        Im2D_U_INT1 masq=Im2D_U_INT1::FromFileStd(aNamePC);

        // apply the mask of the ortho and the mask of the label
        ELISE_COPY(
                    select(mChamferDist[i].all_pts(),masq.in()==255) || select(mChamferDist[i].all_pts(), trans(label.in(),tr)==255),
                    255,
                    mChamferDist[i].out());

        // save chamfer map for checking purpose
        if (mDebug)
        {
            std::string aNameTmp="Chamfer_" + std::to_string(i) + ".tif";
            SaveTiff(mTmpDir+aNameTmp, &mChamferDist[i]);
            // save TFW file
            mIms[i]->writeTFW(mTmpDir+aNameTmp);
        }

        // comptage du nombre d'image a utiliser pour le blending (geométrie mosaic)
        ELISE_COPY(select(NbIm.all_pts(),trans(mChamferDist[i].in(mDist+1),-tr)<=mDist),
                   NbIm.in(0)+1,
                   NbIm.out()
                   );

        // Q? pourquoi je ne peux pas renseigner juste in() sans avoir une erreur genre  BITMAP :  out of domain while reading (RLE mode)

        // somme des distances de chamber dans les enveloppes externes  - pour gérer les cas de blending de 3 images
        ELISE_COPY(select(mSumDistExt.all_pts(),(trans(mChamferDist[i].in_proj(),-tr)<=mDist ) & ( trans(mChamferDist[i].in_proj(),-tr)>0 ) ),
                   mSumDistExt.in(0)+trans(mChamferDist[i].in(0),-tr),
                   mSumDistExt.out()
                   );
        // somme des distances de chamber dans les enveloppes inter  - également pour gérer les cas de blending de 3 images
        ELISE_COPY(select(mSumDistInter.all_pts(),(trans(mChamferDist[i].in_proj(),-tr)>=-mDist) & ( trans(mChamferDist[i].in_proj(),-tr)<0)),
                   mSumDistInter.in(0)+trans(mChamferDist[i].in(0),-tr),
                   mSumDistInter.out()
                   );

        i++;
    }

}

void cFeatheringAndMosaicOrtho::WeightingNbIm1and2()
{

    //  pondération contribution de l'image à l'intérieur de son enveloppe; je peux effectuer le calcul du facteur de pondération pour toutes les images
    //  partie fixe pondérée seulement par le nombre d'image

    ELISE_COPY(select(PondInterne.all_pts(),( NbIm.in()==1) | (NbIm.in()==2)),
               1-(NbIm.in()-1)/NbIm.in(0),
               PondInterne.out()
               );

    // featherling dans l'enveloppe interne
    ELISE_COPY(select(PondInterne.all_pts(), (NbIm.in()==2) && mSumDistInter.in() <=0 && mSumDistInter.in()>=-mDist),
               PondInterne.in()+ (1-(1/NbIm.in())) *  lut_w.in()[mDist+mSumDistInter.in()],
               PondInterne.out()
            );


    for (unsigned int i(0);  i < mIms.size();i++)
    {

        if (mDebug) std::cout << "Image" << i << ", computing weighting in overlap NbIm=1 and 2\n";

        Pt2di tr= mIms[i]->computeTrans(aCorner);
        mImWs[i]=Im2D_REAL4(mIms[i]->Im().sz().x,mIms[i]->Im().sz().y);
        // initialize to 0 because bug may appears otherwise and I do not like bugs
        ELISE_COPY(mImWs[i].all_pts(),0,mImWs[i].out());

        // internal enveloppe
        ELISE_COPY(select(mImWs[i].all_pts(),mChamferDist[i].in(0)<=0),
                   trans(PondInterne.in(0),-tr),
                   mImWs[i].out()
                   );
        // external enveloppe
        ELISE_COPY(select(mImWs[i].all_pts(),mChamferDist[i].in(0)>0  & mChamferDist[i].in(0)<=mDist & (trans(NbIm.in(),-tr)==1 |trans(NbIm.in(),-tr)==2)),
                   1-trans(PondInterne.in(0),-tr),
                   mImWs[i].out()
                   );


    }
}

void cFeatheringAndMosaicOrtho::WeightingNbIm3()
{

    Im2D_U_INT1 Ibin(sz.x,sz.y,0);
    ELISE_COPY(select(Ibin.all_pts(),NbIm.in()==3), 1,Ibin.out());
    U_INT1 ** d = Ibin.data();
    int count_O3(0); // counter for number of area of 3 images overlapping
    Neighbourhood V8=Neighbourhood::v8();


    for (INT x=0; x < Ibin.sz().x; x++)
    {
        for (INT y=0; y < Ibin.sz().y; y++)
        {
            if (d[y][x] == 1)
            {
                count_O3++;
                Liste_Pts_INT2 cc(2);
                ELISE_COPY
                        (
                            // flux: composantes connexes du point.
                            conc
                            (
                                Pt2di(x,y),
                                Ibin.neigh_test_and_set(V8, 1, 0,  20) ), // on change la valeur des points sélectionné comme ça à la prochaine itération on ne sélectionne plus cette zone de composante connexe
                            1, // valeur bidonne, c'est juste le flux que je sauve dans cc
                            cc
                            );

                // determine la boite englobante
                Pt2di pmax,pmin;
                // temporary map enabling a quick selection of these pixels
                Im2D_INT2    CurrentArea(sz.x,sz.y,0);
                ELISE_COPY
                        (
                            cc.all_pts(),
                            Virgule(FX,FY),
                            ((pmax.VMax())|(pmin.VMin())) );

                ELISE_COPY
                        (
                            cc.all_pts(),
                            1,
                            CurrentArea.out());

                unsigned int val;
                std::vector<int> labs;
                for (unsigned int x2(0);x2<cc.image().sz().x;x2++){
                    // cumbersome but only way I found to retrieve point position from list of points
                    Pt2di pt(cc.image().GetR(Pt2di(x2,0)),cc.image().GetR(Pt2di(x2,1)));
                    val = label.GetR(pt);
                    //test if value is in the labs array
                    if (std::find(std::begin(labs), std::end(labs), val) == std::end(labs) && val!=255)
                    {labs.push_back(val);}
                    // std::cout << "add label image " << val << "\n"       ;}
                }
                std::cout << "I got the 3 images for an ovelap area (id: " << count_O3 << "), images labels are " << labs <<"\n";

                // load 3 chamfer distance for ease of manipulation
                // I do not know why but i have to add 1 to the length
                Pt2di beSz(pmax.x-pmin.x+1,pmax.y-pmin.y+1);
                //std::cout << "size of the area : " << beSz << ", location on the global orthophoto : " << pmin << " and " << pmax <<"\n";
                Im2D_INT2 dist1(beSz.x,beSz.y),dist2(beSz.x,beSz.y),dist3(beSz.x,beSz.y);
                Im2D_REAL4 gateau12(beSz.x,beSz.y);
                Pt2di tr0 = mIms[labs[0]]->computeTrans(aCorner)+pmin;// double translation, une vers le système global, une vers la boite englobante de la zone
                ELISE_COPY(dist1.all_pts(),trans(mChamferDist[labs[0]].in(0),tr0),dist1.out());
                Pt2di tr1 = mIms[labs[1]]->computeTrans(aCorner)+pmin;
                ELISE_COPY(dist2.all_pts(),trans(mChamferDist[labs[1]].in(0),tr1),dist2.out());
                Pt2di tr2 = mIms[labs[2]]->computeTrans(aCorner)+pmin;
                ELISE_COPY(dist2.all_pts(),trans(mChamferDist[labs[2]].in(0),tr2),dist3.out());

                // strategy; firt im1 and im2 are blended, then im3 with the blend of im1 im2. but in fact, process first im3

                // feathering dans l'enveloppe interne de l'image numéro 3
                Pt2di tr = mIms[labs[2]]->computeTrans(aCorner);
                ELISE_COPY(select(mImWs[labs[2]].all_pts(),mChamferDist[labs[2]].in()<=0 & trans(CurrentArea.in(0),-tr)==1),
                           0.5+ 0.5 * lut_w.in()[mDist+mChamferDist[labs[2]].in()],
                            mImWs[labs[2]].out()
                            );
                // feathering dans l'enveloppe externe de l'image numéro 3
                ELISE_COPY(select(mImWs[labs[2]].all_pts(),mChamferDist[labs[2]].in()>0 & mChamferDist[labs[2]].in()<=mDist & trans(CurrentArea.in(0),-tr)==1),
                           1-(0.5+ 0.5 * lut_w.in()[mDist-mChamferDist[labs[2]].in()]),
                            mImWs[labs[2]].out()
                            );
                // copie de ceci dans PondInterne
                ELISE_COPY(select(PondInterne.all_pts(), CurrentArea.in()==1 & trans(mChamferDist[labs[2]].in(0),tr)<=0),
                           trans(mImWs[labs[2]].in(0),tr),
                           PondInterne.out()
                            );

                ELISE_COPY(gateau12.all_pts(),0,gateau12.out());
                ELISE_COPY(select(gateau12.all_pts(), trans(CurrentArea.in(),pmin)==1),
                           1-trans(mImWs[labs[2]].in(0),tr2),
                           gateau12.out()
                            );

                // ok, im1 with im2 now
                //redefinition of a seamline between the two images, seamline on the enveloppe of the 3th image
                Im2D_U_INT1  tmp(beSz.x,beSz.y,1);
                Im2D_INT2    NewDist(beSz.x,beSz.y,1);
                ELISE_COPY(select(tmp.all_pts(),dist1.in()==dist2.in()) || select(tmp.all_pts(),dist1.in()==-2 & dist2.in()<5),
                           0,
                           tmp.out()
                           );
                ChamferNoBorder(tmp);

                ELISE_COPY(select(dist1.all_pts(),dist1.in()>dist2.in()),tmp.in(),NewDist.out());
                ELISE_COPY(select(dist1.all_pts(),dist1.in()<=dist2.in()),-tmp.in(),NewDist.out());

                // Now I got my new seamline between 1 and 2 and my new chamfer distance, negative inside envelope, positive outside

                // env interne im 1
                Im2D_REAL4 w0(beSz.x,beSz.y),w1(beSz.x,beSz.y);
                ELISE_COPY(w0.all_pts(),0,w0.out());
                ELISE_COPY(select(NewDist.all_pts(),NewDist.in()<=0 & NewDist.in()>=-mDist),
                           gateau12.in() * (0.5+ 0.5*lut_w.in()[mDist+NewDist.in()]),
                           w0.out()
                            );
                ELISE_COPY(select(NewDist.all_pts(),NewDist.in()>0 & NewDist.in()<=mDist),
                           gateau12.in() * (1 - 0.5-0.5*lut_w.in()[mDist-NewDist.in()]),
                           w0.out()
                            );
                ELISE_COPY(w1.all_pts(),gateau12.in()-w0.in(),w1.out());

                //copy these weighting in the mImWs

                ELISE_COPY( mImWs[labs[0]].all_pts(),
                            mImWs[labs[0]].in() + trans(w0.in(0),-mIms[labs[0]]->computeTrans(aCorner)-pmin),
                            mImWs[labs[0]].out()
                            );

                ELISE_COPY( mImWs[labs[1]].all_pts(),
                            mImWs[labs[1]].in() + trans(w1.in(0),-mIms[labs[1]]->computeTrans(aCorner)-pmin),
                            mImWs[labs[1]].out()
                            );

                if (mDebug)
                  {
                   std::string aName;

                  aName="O3_Num" + ToString(count_O3) + "_part_12.tif";
                  SaveTiff(mTmpDir+aName,& gateau12);
                  // plani origin of this small area in order to generate the tfw file
                  Pt2dr corner(aCorner.x+pmin.x*MTD.ResolutionPlani().x,aCorner.y+pmin.y*MTD.ResolutionPlani().y);
                  writeTFW(mTmpDir+aName, MTD.ResolutionPlani(),corner);
                  aName="O3_Num" + ToString(count_O3) + "_Dist1.tif";
                  SaveTiff(mTmpDir+aName,& dist1);
                  writeTFW(mTmpDir+aName, MTD.ResolutionPlani(),corner);
                  aName="O3_Num" + ToString(count_O3) + "_Dist2.tif";
                  SaveTiff(mTmpDir+aName,& dist2);
                  writeTFW(mTmpDir+aName, MTD.ResolutionPlani(),corner);
                  aName="O3_Num" + ToString(count_O3) + "_W0.tif";
                  SaveTiff(mTmpDir+aName,& w0);
                  writeTFW(mTmpDir+aName, MTD.ResolutionPlani(),corner);
                  aName="O3_Num" + ToString(count_O3) + "_W1.tif";
                  SaveTiff(mTmpDir+aName,& w1);
                  writeTFW(mTmpDir+aName, MTD.ResolutionPlani(),corner);
                  aName="O3_Num" + ToString(count_O3) + "_NewDist.tif";
                  SaveTiff(mTmpDir+aName,& NewDist);
                  writeTFW(mTmpDir+aName, MTD.ResolutionPlani(),corner);
                }

            }
        }
    }
}



void cFeatheringAndMosaicOrtho::ComputeMosaic()
{
    for (unsigned int i(0);  i < mIms.size();i++)
    {
        Pt2di tr= mIms[i]->computeTrans(aCorner);
        // mosaic
        ELISE_COPY(select(mosaic.all_pts(),trans(mImWs[i].in(0),tr)>0),
                   mosaic.in()+trans(mIm2Ds[i].in()*mImWs[i].in(),tr) ,
                   mosaic.out()
                   );
        // sum of weighting, to check it is well equal to 1 everywhere
        if (mDebug)
        {
            ELISE_COPY(select(mSumWeighting.all_pts(),trans(mImWs[i].in(0),tr)>0),
                       mSumWeighting.in()+trans(mImWs[i].in(0),tr) ,
                       mSumWeighting.out()
                       );
            // individual weighting
            std::string aNameTmp="Weighting_" + std::to_string(i) + ".tif";
            SaveTiff(mTmpDir+aNameTmp, &mImWs[i]);
            mIms[i]->writeTFW(mTmpDir+mTmpDir+aNameTmp);
        }
    }
}

template <class T,class TB> void  cFeatheringAndMosaicOrtho::SaveTiff(string aName, Im2D<T,TB> * aIm)
{
    Tiff_Im  aTF
            (
                aName.c_str(),
                aIm->sz(),
                GenIm::real4,
                Tiff_Im::No_Compr,
                Tiff_Im::BlackIsZero
                );
    ELISE_COPY(aIm->all_pts(),aIm->in(),aTF.out());
}

void cFeatheringAndMosaicOrtho::GenLabelTable()
{
    mLabel="Label-Feathering.tif";

    std::cout << "Divide mosaic space between every ortho (label map generation) based on their incidence.\n ";
    Im2D_U_INT1 ImLabel(sz.x,sz.y,255);
    Im2D_REAL4 ImScoreIncid(sz.x,sz.y,100.0);

    int imLab(0);
    for (auto &im : mLFile)
    {

        std::string incidName=KeyAssocNameOrt2Incid(im);
        std::string TFWName=KeyAssocNameOrt2TFW(im);
        std::string pcName=KeyAssocNameOrt2PC(im);

        // load the incidence map and hidden part map
        // le tfw convient pour im masque PC mais par pour de l'image incid qui est sous resolue.
        cImGeo Masq(pcName,TFWName);
        Im2D_REAL4 imPC=Masq.toRAM();

        //cMetaDataPartiesCachees MTD = StdGetFromPCP(pcName.substr(0, im.size()-3)+"xml",MetaDataPartiesCachees);
        //Im2D_REAL4 imIncid2Scale=Im2D_REAL4::FromFileStd(incidName);
        //Im2D_REAL4 imIncid(imIncid2Scale.sz().x*aFact,imIncid2Scale.sz().y*aFact,0.0);
        //Im2D_REAL4 imIncid(imIncid2Scale.sz().x,imIncid2Scale.sz().y,0.0);
        //ELISE_COPY(imIncid.all_pts(),imIncid2Scale.in(3.14),imIncid.out());

        int aFact=10;
        Im2D_REAL4 imIncid=Im2D_REAL4::FromFileStd(incidName);
        TIm2D<REAL4,REAL> mTScLoc(imIncid);

        // faudrai que je stoque chacune des valeurs de translation dans un vecteur de pt2di indexé plutot que de recalculer par ci par la
        Pt2di tr = Masq.computeTrans(aCorner);
        Pt2di aP;

        /*
        // incidence map has not the same scale, require resampling prior to use OLD-- TIm2D<REAL4,REAL> perform kind of interpolation by it own if you provide pixel position as Pt2dr
        //int aFact=MTD.SsResolIncH();
        Fonc_Num aFIn = StdFoncChScale_BicubNonNeg
                     (
                           imIncid2Scale.in_proj(),
                           Pt2dr(0,0),
                           Pt2dr(1.0/aFact,1.0/aFact)
                     );
        // result of resized incid is quite wierd on the edge, cause the masq is applyied on the incid image and out of masq value are high. interpolation use these high value - ask MPD
        std::cout << " Resize Im incid\n";
        ELISE_COPY(imIncid.all_pts(),aFIn,imIncid.out());
        //std::string aNameInc="TestIncid_"+ToString(imLab)+".tif";
        //imIncid.Resize(Pt2di(imIncid2Scale.sz().x*aFact,imIncid2Scale.sz().y*aFact));
        */

        if (mDebug)
        {
            std::string aNameInc="Incid_"+ToString(imLab)+".tif";
            SaveTiff(mTmpDir+aNameInc,&imIncid);
            // tfw file; not same resol
            writeTFW(mTmpDir+aNameInc,Pt2dr(Masq.GSD()*aFact,-Masq.GSD()*aFact),Pt2dr(Masq.OriginePlani().x,Masq.OriginePlani().y));
        }


        for (aP.x =0 ; aP.x < Masq.SzUV().x ; aP.x++)
        {
             for (aP.y =0 ; aP.y < Masq.SzUV().y ; aP.y++)
             {
                   //std::cout << " masq value  for pos " << aP << " is " << imPC.GetR(aP) << "\n";
                  double aValIncid=mTScLoc.getr(Pt2dr(aP)/aFact,10e3);
                  if (imPC.GetR(aP) != 255 & aValIncid<3)
                  {
                     Pt2di aPinMosaic = aP-tr;
                     double aScore = ImScoreIncid.GetR(aPinMosaic);

                     //Pt2di aPincid=Pt2di(round_down(Pt2dr(aP)/aFact));
                     //std::cout << "position on the image " << imLab << " is " << aP << ", in mosaic pos= "<< aPinMosaic <<", value of incid is " << imIncid.GetR(aP) << "\n";
                     //std::cout << "position on the image " << imLab << " is " << aP << ", in incid image= "<< aPincid <<"\n";
                     if (aScore>aValIncid)// incid should be as low as possible
                     {
                          ImLabel.SetR(aPinMosaic,imLab);
                          ImScoreIncid.SetR(aPinMosaic,aValIncid);
                     }
                  }
             }
        }
        imLab++;
    }

    // save label map and tfw file
    SaveTiff(mLabel,&ImLabel);

    if (mDebug) {
        std::string aNameIncid("IncidScore.tif");
        SaveTiff(mTmpDir+aNameIncid,&ImScoreIncid);
        writeTFW(mTmpDir+aNameIncid, MTD.ResolutionPlani(),aCorner);
    }
}


// write tfw for all resulting map
int cFeatheringAndMosaicOrtho::writeTFW(std::string aNameTiffFile, Pt2dr aGSD, Pt2dr aXminYmax)
{
     std::string aNameTFW=KeyAssocNameOrt2TFW(aNameTiffFile);
     std::ofstream aTFW(aNameTFW.c_str());
     aTFW.precision(12);
     aTFW << aGSD.x << "\n" << 0 << "\n";
     aTFW << 0 << "\n" <<  aGSD.y << "\n";
     aTFW << aXminYmax.x << "\n" << aXminYmax.y << "\n";
     aTFW.close();
     return EXIT_SUCCESS;
}


std::string cFeatheringAndMosaicOrtho::KeyAssocNameOrt2PC(std::string aOrtName)
{
     std::string aKey= "Key-Assoc-OpIm2PC"  ;
     std::string Name = mICNM->Assoc1To1(aKey,aOrtName,true);
     return Name;
}

std::string cFeatheringAndMosaicOrtho::KeyAssocNameOrt2Incid(std::string aOrtName)
{
    std::string aKey= "Key-Assoc-OpIm2Incid"  ;
    std::string Name = mICNM->Assoc1To1(aKey,aOrtName,true);
    return Name;
}

std::string cFeatheringAndMosaicOrtho::KeyAssocNameOrt2TFW(std::string aOrtName)
{
     std::string TFWName=aOrtName.substr(0, aOrtName.size()-3)+"tfw";
     return TFWName;
}

void cFeatheringAndMosaicOrtho::banniereFeathering()
{
    std::cout <<  "\n";
    std::cout <<  " ************************************\n";
    std::cout <<  " *     No funcky name right now     *\n";
    std::cout <<  " *     Mosaiking of orthoimages     *\n";
    std::cout <<  " *     With feathering along        *\n";
    std::cout <<  " *     seamline                     *\n";
    std::cout <<  " ************************************\n\n";
    std::cout <<  "Lamda for gaussian distance (from seamline) weighting: " << mLambda << "\n";
    std::cout <<  "Chamfer 3-2 distance from seamline for feathering: " << mDist << "\n";
    std::cout <<  "Number of orthoimages in the mosaic: " << mIms.size() << "\n";
    std::cout <<  "Result file: " <<  mNameMosaicOut << "\n";
    std::cout <<  " ************************************\n\n";
}

void cFeatheringAndMosaicOrtho::checkParam()
{
    if (mDist>244 | mDist<0)
    {
       std::cout << "Warning, feathering distance have to be between [0-255[. I reset it to an appropriate value.\n";
       mDist=100;
    }

    if (mLambda<0 | mLambda > 1)
    {
       std::cout << "Warning, lambda have to be between [0-1]. I reset it to an appropriate value.\n";
       mLambda=0.4;
    }

    if (EAMIsInit(&mLabel) & !ELISE_fp::exist_file(mLabel))
    {
       std::cout << "Warning, you provide a label map (" << mLabel << ") but I can not find the file.\n";
    }
}


int main_featheringOrtho(int argc,char ** argv)
{
   cFeatheringAndMosaicOrtho(argc,argv);
   return EXIT_SUCCESS;
}

