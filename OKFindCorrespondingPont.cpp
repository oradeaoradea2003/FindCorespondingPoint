/*
 ***************************************************************************
 *                                                                         *
 *                         Platform Independent                            *
 *                   Bitmap Image Reader Writer Library                    *
 *                                                                         *
 * Author: Arash Partow - 2002                                             *
 * URL: http://www.partow.net                                              *
 *                                                                         *
 * Copyright notice:                                                       *
 * Free use of the Platform Independent Bitmap Image Reader Writer Library *
 * is permitted under the guidelines and in accordance with the most       *
 * current version of the Common Public License.                           *
 * http://www.opensource.org/licenses/cpl1.0.php                           *
 *                                                                         *
 ***************************************************************************
*/
#ifdef  _MSC_VER
#define _CRT_SECURE_NO_WARNINGS 1
#endif
using namespace std;
#include <iostream>
#include <string>
#include <cstdlib>

#include <iostream>
#include <math.h>
#include <cstring>

#include "bitmap_image.hpp"

///////////////////////////////////////////////////////////////

/* system example : DIR */
#include <stdio.h>      /* printf */
#include <stdlib.h>     /* system, NULL, EXIT_FAILURE */
#include <cstdlib>
// SETAM ESENTIALU ***********************
string strInputImage = "image.bmp";
string strFindinImage = "findinimage.bmp";



double domainxy=4; //see next lines
// domeniul in care Mexican Hat 1D si 2D se afla
double xMin=-domainxy; //-2 e bun
double xMax=domainxy;   //2 e bun
double yMin=-domainxy; //-2 e bun
double yMax=domainxy;   //2 e bun
double deltax=0.001;  //saltul lui x in xMin xMax
double constY=0.0; // constanta Y [yMin yMax] in care se face plotul
// marimea array in care se formeaza MexicanHat
unsigned int Width=40; 
unsigned int Height=40;
double gama	= 0.5;//parametruMexican Hat  0.4 e BUN 
//cele doua situatii
float  gama1	= 0.4;//parametruMexican Hat  0.4 e BUN
float  gama2	= 0.6;//parametruMexican Hat  0.4 e BUN
unsigned int Width1=40; 
unsigned int Height1=40;
  
unsigned int Width2=60; //60 
unsigned int Height2=60;
 
 //the main outputs at xnCROSS ymCROSS
unsigned int xnCROSS;
unsigned int ynCROSS;

/// Valorile in pixelul ales
double GetValueMexHatAtCROSS1 =0.0;
double GetValueMexHatAtCROSS2 =0.0;
bool grey=true ; //do Mexican grey fill to make proper Calculation

//*********************
////////////////////////////////////////////////////////////////////////
// obtine valoarea MexicanHat


double pi=3.14159265;

double GetMexican(double x,double y, double gama)
{
/************** MATHEMATICA CODE mexicana
ro=0.4
Plot3D[(1/(3.14*ro*ro*ro*ro))*(1-0.5*(x*x+y*y)/(ro*ro))*Exp[-(x*x+y*y)/(2*ro*ro)],{x,-1,1},{y,-1,1}]
 
********************************/ 
 double z=0.0;

double ro;//BUN ASTA 0.5;
ro=gama;  // 4
z=1/((pi*ro*ro*ro*ro))*(1-0.5*(x*x+y*y)/(ro*ro))*exp(-(x*x+y*y)/(2*ro*ro));
//{x,-1,1},{y,-1,1}
return z; 
 }

////////////////////////////////////////////////////////////////////////

//AFLAM Zmin a mexicancei i 0=y
double GetZminMEXICACHAT(double gama, double  x1, double x2)
{
    int n = 6000;
    double Zmin = 9999999999999999.9999999;
    double delta = (x2 - x1) / n;
    for (int i = 0; i < n; i++)
    {
        double  z = GetMexican(x1 + delta, 0, gama);
        if (z < Zmin)Zmin = z;

    }//end for

    return (Zmin);
}//end GetZminMEXICACHAT( gama,x1,x2);}


//Create the full bitmap with the Hat  
void CreateMexicanHat(unsigned int BMPxMin, unsigned int  BMPxMax,
unsigned int BMPyMin, unsigned int  BMPyMax,
double HatxMin,double HatxMax, double HatyMin,double HatyMax,
double gama,string theHatBMP )
{
	//intregi pozitive
//unsigned int Width= BMPxMax - BMPxMin;
//unsigned int Height= BMPyMax - BMPyMin;
unsigned int xnBMP; //in (BMPxMax, BMPxMin)
unsigned int ynBMP;  //in (BMPyMax, BMPyMin)

// double reale
double xHat; // in (HatxMin,HatxMax)
double yHat; // in (HatyMin,HatyMax)

/*
double HatZ ;// [HatZminim, HatZmaxim] in care se plimba
double HatZminim; //max Z
double HatZmaxim;  // min Z
*/

unsigned int Vn; // in ( MinIntensityBMPn  , MaxIntensityBMPn )


//( x , y) si (xn, yn  )
//****************************
//xn - xnMin   =    
//     xnMax - xnMin
//= ****************  ( x - xMin)
 //    xMax - xMin
//*****************************



//xn - xnMin   =    
//     xnMax - xnMin
//= ****************  ( x - xMin)
 //    xMax - xMin

double z=0;
double Zmin=10000;
double Zmax=-10000;
double sum=0.0;
unsigned int MaxIntensityBMPn=255; //max of pixel value

unsigned int MinIntensityBMPn=0;

bitmap_image image3DHat(Width,Height);

rgb_store col ;
 
 
 //Calculate Zmin Zmax
for( xnBMP = BMPxMin; xnBMP < BMPxMax; ++xnBMP)
   {
      for(ynBMP = BMPyMin; ynBMP < BMPyMax; ++ynBMP)
      {
xHat=((HatxMax-HatxMin)/(BMPxMax-BMPxMin))*(xnBMP -BMPxMin) + HatxMin;
yHat=((HatyMax-HatyMin)/(BMPyMax-BMPyMin))*(ynBMP -BMPyMin) + HatyMin;

// gigi
z=GetMexican(xHat,yHat,  gama);


if(xnBMP == BMPxMin)
{
//cout << "z= " << z << " xHat= "<<xHat<< " yHat= "<<yHat << " gama= "<< gama<< endl ;
} 
     	

        if(z<Zmin)Zmin=z;     
        if(z>Zmax)Zmax=z; 
        sum+=z;    
     }
   }
//cout<<"am aflat Zmin si Zmax putem umple imaginea \n";
//MAX byte BMP 24bites is 255
MaxIntensityBMPn=255;//nZmax;
MinIntensityBMPn= 0;//nZmin;

// umplem cu setpixel tot

for( xnBMP = BMPxMin; xnBMP < BMPxMax; ++xnBMP)
   {
      for(ynBMP = BMPyMin; ynBMP < BMPyMax; ++ynBMP)
      {
        
xHat=((HatxMax-HatxMin)/(BMPxMax-BMPxMin))*(xnBMP -BMPxMin) + HatxMin;
yHat=((HatyMax-HatyMin)/(BMPyMax-BMPyMin))*(ynBMP -BMPyMin) + HatyMin;
//cout<<"am 1\n ";
z=GetMexican(xHat,yHat,  gama);
        
//****************************
//cout<<"am 2\n ";
//aici avem valoarea de pixelului
Vn=((MaxIntensityBMPn-MinIntensityBMPn)/(Zmax-Zmin))*(z -Zmin) + MinIntensityBMPn ;
if(grey==false)
{
col.red=0;
if(Vn<225)col.green=Vn;
else col.green=255;
col.blue=0;
///GREEN z pozitiv
}else
{
col.red=Vn;
col.green=Vn;
col.blue=Vn;
  
}
//cout<<"am 3\n ";
if(z>0)image3DHat.set_pixel(xnBMP,ynBMP,col.red,col.green,col.blue);

// PUT ZERO outside the DISK Kernel
unsigned int xn0=(unsigned int )Width/2.0;
unsigned int yn0=(unsigned int )Height/2.0;
unsigned int Rmax=(unsigned int )(Width/2.0);
unsigned int x=xnBMP;
unsigned int y=ynBMP;

//cout << "\n x-xn0 =\n "<<x-xn0; 
unsigned int r2=(x-xn0)*(x-xn0)+(y-yn0)*(y-yn0);
//cout << "\n r2  "<<r2 << "  Rmax*Rmax=  "<<Rmax*Rmax <<"\n";

if(r2> Rmax*Rmax)
{
 // cout << " a intrat \n ";
 // getchar();
col.red=0;
col.green=0;
col.blue=0;

image3DHat.set_pixel(xnBMP,ynBMP,col.red,col.green,col.blue);

}



//***************************

if(grey==false)
{
if(Vn<255)col.red=Vn;
else Vn=255;
col.green=0;
col.blue=0;
}else
{
col.red=Vn;
col.green=Vn;
col.blue=Vn;
  
}
// RED z negativ
if(z<0  && (r2 < Rmax*Rmax))
{
//cout << "  \n NEGATIV z = "<<z; 
image3DHat.set_pixel(xnBMP,ynBMP,col.red,col.green,col.blue);
}

      }//end for 
  }//end for
//cout << "\n save the imagine mexican3DHat.bmp\n ";
//cout << "MaxIntensityBMPn = "<< MaxIntensityBMPn << " \n MinIntensityBMPn = "<< MinIntensityBMPn<< endl;
//cout<<"Save  image ...\n ";
//getchar();
//image3DHat.save_image(theHatBMP);
image3DHat.save_image(theHatBMP);
//cout << "Zmin= " << Zmin <<   "\n Zmax= " << Zmax << "\n gama= "<< gama<< endl ;
//cout << "sum= " << sum << endl ;
//cout << "PRESS ENTER \n" ;
//getchar();
//cout << "END CreateMexicanHat sum= " << sum << endl ;
}//end CreateMexicanHat

//**************************************************************

////////////////////////////////////////////////////////////





class CImage{
public:
  bitmap_image *pbmp;
  unsigned int width;
  unsigned int height;
  char file_name[500];
  CImage(char * str)
  {
    strncpy(file_name,str,490);
    pbmp=new bitmap_image(file_name);
   width = pbmp->width();
   height = pbmp->height();
  
  }

  ~CImage()
  {
    delete pbmp; 
  }
  
  double get_pixel(unsigned int x,unsigned int y)
  {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    if(x>width-1 || y>width-1)return 0.0;
    pbmp->get_pixel(x,y,red,green,blue);
    return static_cast<double>(red);
  
  }

  void set_pixel(unsigned int x,unsigned int y,unsigned char val)
  {
    if(x>width-1 || y>width-1)return;
    pbmp->set_pixel(x,y,val,val,val);
  }

};// end class CImage

////////////////////////////////////////////////////////////////////////


/*
inline void ImageGreyscaleConvert()
{
   //std::string file_name = "image.bmp";
   bitmap_image image("image.bmp");
   image.convert_to_grayscale();
   image.save_image("grayscale_image.bmp");
}
*/


//***********************************************
/*
inline void flip()
{
   std::string file_name = "image.bmp";
   bitmap_image image("image.bmp");
   image.save_image("saved.bmp");

   image.vertical_flip();
   image.save_image("saved_vert_flip.bmp");
   image.vertical_flip();

   image.horizontal_flip();
   image.save_image("saved_horiz_flip.bmp");
}
*/
//****************************************************
/*
inline void test09()
{
   const unsigned int dim = 1000;
   bitmap_image image(dim,dim);
   for(unsigned int x = 0; x < dim; ++x)
   {
      for(unsigned int y = 0; y < dim; ++y)
      {
         rgb_store col = jet_colormap[(x + y) % dim];
         image.set_pixel(x,y,col.red,col.green,col.blue);
      }
   }
   image.save_image("color_map_image.bmp");
}
*/
/*
inline void mytest()
{
   std::string file_name = "image.bmp";
   bitmap_image image("image.bmp");
   bitmap_image image2(image.width(),image.height());

 unsigned char red;
 unsigned char green;
 unsigned char blue;
 unsigned int x;
 unsigned int y;
 int newred;
for(x = 0; x < image.width(); ++x)
   {
      for( y = 0; y < image.height(); ++y)
      {
       image.get_pixel(x,y,red,green,blue);
//cout<< " \n red="<<(int)red << " green="<<(int)green<< " blue="<<(int)blue;

       newred=(int)red -1;
       if(newred<0)newred=0;
       red=green=blue=static_cast<unsigned char>(newred);
       image2.set_pixel(x,y,red,green,blue);
      }
   }

image2.save_image("illumination-input.bmp");

//cout<< "red="<<(int)red << " green="<<(int)green<< " blue="<<(int)blue;

}

*/
//****************************************************



//write a CROSS in (xCROSS,yCROSS) of bitmap_image *pimg
void WriteCROSSinImage(unsigned int xCROSS,unsigned int yCROSS,
bitmap_image *pimage,string CROSSNameFile)
{
unsigned int x;
unsigned int y;
unsigned char red;
 unsigned char green;
 unsigned char blue;
 if(xCROSS> pimage->width() || yCROSS> pimage->height())
 {
 //cout << "Pont x y Not in the image \n";
   return;
 }
 
 //cout << "Pont x y OK \n";
for(x = 0; x < pimage->width(); ++x)
   {

 int newred=255;
       //red=green=blue=static_cast<unsigned char>(newred);;
       red=255; green=0;blue=0;
 pimage->set_pixel(x,yCROSS,red,green,blue);
       
   }
 
      for( y = 0; y < pimage->height(); ++y)
      {
       pimage->get_pixel(x,y,red,green,blue);
//cout<< " \n red="<<(int)red << " green="<<(int)green<< " blue="<<(int)blue;

int newred=255;

       //red=green=blue=static_cast<unsigned char>(newred);
       red=255; green=0;blue=0;
       pimage->set_pixel(xCROSS,y,red,green,blue);
 
      }
  
  cout <<"SAVE CROSS in file:...   . "<< CROSSNameFile << "\n";
  
  pimage->save_image(CROSSNameFile);
}

//***********************************************************
//Get the Value of Mexican hat in (x,y) of an given image
double GetValMexicanHatAt(unsigned int xnCROSS,unsigned int ynCROSS ,
 bitmap_image *pimage ,bitmap_image *pMexicanHat,
 bool printToGetValuseImageAt )
{

 if(xnCROSS> pimage->width() || ynCROSS> pimage->height())
 {
   ////cout << "Ponit x y Not in the image exit\n";
  //getchar();
   return(-90000000);
 }
 if( pMexicanHat->width() > pimage->width() || pMexicanHat->height() > pimage->height())
 {
   //cout << "Mexican Hat to big... \n"  << "MEXWIDTH= "  << pMexicanHat->width();
   //cout << "\n Mexican Hat to big... \n" << "image WIDTH= " << pimage->width();
   //cout << "Mexican Hat to big... \n" << "MEXheight= " << pMexicanHat->height();
   //cout << "\n Mexican Hat to big... \n" << "image WIDTH= " << pimage->height();

   //getchar();
   return(-90000000);
 }
  

unsigned char red_imag;
 unsigned char green_imag;
 unsigned char blue_imag;
 unsigned char red_mex;
 unsigned char green_mex;
 unsigned char blue_mex;


 //unsigned int xn_im;
//unsigned int yn_im;

 unsigned int xn;
unsigned int yn;

//unsigned int xn_mex;
//unsigned int yn_mex;
  
double vHatOut=0.0; //the value of Hat
unsigned int   wmex=pMexicanHat->width();
unsigned int   hmex=pMexicanHat->height() ;
unsigned int    wim=pimage->width();
unsigned int  him= pimage->height();

     for(unsigned int xn_im = 0; xn_im <wim; ++xn_im)
   {
      for(unsigned int yn_im = 0; yn_im < him; ++yn_im)
      {
         //in fiecare punct al; imaginii cautam sa vedem daca incape Hat ul
//xnCROSS ynCROSS
//vedem daca incape Hat toata in imagine cu centrul in xnCROSS ynCROSS

if(xn_im==xnCROSS && yn_im==ynCROSS)
{
  ////cout<< " cucu\n";
 // getchar();
	for(unsigned int xn_mex = 0; xn_mex <wmex; ++xn_mex)
	{
	  for(unsigned int yn_mex = 0; yn_mex < hmex; ++yn_mex)
	  {
	    
	  xn=xn_im-xn_mex+wmex/2;
	  yn=yn_im-yn_mex+hmex/2;
 	 if(xn>1 && yn>1 && xn<wim && yn<him)
	 {	
	 
	pimage->get_pixel(xn,yn,red_imag,green_imag,blue_imag);
	pMexicanHat->get_pixel(xn_mex,yn_mex,red_mex,green_mex,blue_mex);
	//check wehere is
	// //cout<< " set _pixel \n";
//SETAM MexicanHat pe imagine
	if(printToGetValuseImageAt==true) pimage->set_pixel(xn,yn,red_mex,green_mex,blue_mex);
	 // AICI E IMPORTANT SE PUNE SUMA LA MexicanHAT
   ////////////////////////////////////////////
	 vHatOut=vHatOut+  red_imag*red_mex; //rosu 
   /////////////////////////////////////////////////
	}else{cout << "CROSS e apropape de margine..xnCROSS= "<<xnCROSS << " \n ynCROSS= " << ynCROSS << "\n";
	  //getchar();
    vHatOut=-20000;
	  return(vHatOut);
	  }
	}
      }
 }//end if
      }
    }
      
      char memimagefile[200];
using namespace std;
 //sprintf (memimagefile, "GetValueImageAt_%d_%d.bmp", xnCROSS,ynCROSS);
if(printToGetValuseImageAt==true)
{
 sprintf (memimagefile, "GetValueImageAt_%d_%d.bmp", Width,Height);
  //  printf (" \n SAVE file ..... GetValueImageAt_%d_%d.bmp", xnCROSS,ynCROSS);  
    pimage->save_image(memimagefile);
 }     
      return(vHatOut);
      
   }//end function 

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////


// Se fac Graficul functiei MexicanHat in y=constY 777777777777777777777
void MexicanHatGraphic(double  xmin, double xmax,
    double constY, double deltax, double gama, string strGraph)
{
    //LUK HATGRAPHIC 777777777777777777777777

    cout << "Start in functie MexicanHatGraphic ... \n";

    rgb_store col;


    unsigned int n;
    unsigned int  xn;
    unsigned int  yn;
    double y;
    double x;
    double z;
    double Zmin = 1000000;//min Z of MexicanHatGraph
    double Zmax = -1000000;//max Z of MexicanHatGraph
    double sum = 0.0; //integral of MexicanHat
  //create the empty bitmap IS NOT NULL

    bitmap_image imageGraf(Width, Height);
    for (xn = 0; xn < Width; ++xn)
    {
        for (yn = 0; yn < Height; ++yn)
        {
            imageGraf.set_pixel(xn, yn, 0, 0, 0);
        }
    }

    //imageGraf.save_image(strGraph); //bubu

    //******************** cucu AICI sint setatrile care merg
    //double xMin=-2; //-2 e bun
    //double xMax=2;   //2 e bun



    //sample of MexicanHat
    unsigned  int nMax = (unsigned int)((xmax - xmin) / deltax);
    //cout << "LUK ENTER " << "aici nMax=" << nMax << "\n";


    //getchar();

    //*************************************
    x = xmin;//start the plot here
    for (n = 0; n < nMax; ++n)
    {
        x = x + deltax;
        z = GetMexican(x, constY, gama);
        //calculate the min max z
        if (z < Zmin)Zmin = z;
        if (z > Zmax)Zmax = z;
        //sum of z
        sum += z;
        //if(z>0.0)cout << "\n z = "<<z ;

        //cout << "\n sum = "<<sum ;
        //cout << "\n x = "<<x ;
        //cout << "\n Zmax = "<<Zmax ;
        //cout << "\n Zmin = "<<Zmin ;

        //cout << "\n n= "<<n ;

    }//end for n

  ///am gasit extremele Zmin Zmax
  //cout << "\n sum = "<<sum ;
    //cout << "\n Zmax = " << Zmax;
    //cout << "\n Zmin = " << Zmin << "\n";


    // incepem cu bitmaput
    unsigned int xnMin = 0;
    unsigned int xnMax = Width - 1;
    unsigned int ynMin = 0;
    unsigned int ynMax = Height - 1;

    //vom plota pe axa y valorile z
    double ymax = Zmax;
    double ymin = Zmin;


    x = xmin;
    //acum umplem bitmapul
    for (n = 0; n < nMax; ++n)
    {
        x = x + deltax;
        z = GetMexican(x, constY, gama);
        //aici e important
        y = z;

        ///calculam proportia
        xn = ((xnMax - xnMin) / (xMax - xMin)) * (x - xmin) + xnMin;
        yn = ((ynMax - ynMin) / (yMax - yMin)) * (y - ymin) + ynMin;
        if (xn<0 || xn >Width - 1)
        {
            cout << " xn trece de 0 si width \n";
            //getchar();
        }


        if (yn<0 || yn >Height - 1) {
            //cout << " ymin=  " << ymin << " ymax=   " << ymax;
            //cout << " \n yn trece de 0 si height \n";
            //cout << "\n ynMin = " << ynMin << " ynMax = " << ynMax << "   yn = " << yn;
            // getchar();
            continue;
        }



        col.red = 255;
        col.green = 0;
        col.blue = 0;

        //**********************************
        //main plot for negative y ROSU
        if (y < 0.0)imageGraf.set_pixel(xn, yn, col.red, col.green, col.blue);


        col.red = 0;
        col.green = 255;
        col.blue = 0;
        //main plot for pozitive y  VERDE
        if (y > 0.0)imageGraf.set_pixel(xn, yn, col.red, col.green, col.blue);

        //******************************************

        col.red = 0;
        col.green = 0;
        col.blue = 255;

        //plotam axa y=0; BLUE

        unsigned int ynZERO = ((ynMax - ynMin) / (yMax - yMin)) * (0.0 - ymin) + ynMin;
        //cout << "ynZERO= "<< ynZERO << "\n";
        //cout << "height= "<< Height << "\n";
        //cout << "ymin= "<< ymin << "\n";


        //plot the axe y=0
        imageGraf.set_pixel(xn, ynZERO, col.red, col.green, col.blue);

        //******************************************


        //******************************************
        col.red = 255;
        col.green = 255;
        col.blue = 255;
        //main plot for ZERO y=0  ALB= ZERO
        if ((y < 0.01) && (y > -0.01))imageGraf.set_pixel(xn, yn, col.red, col.green, col.blue);


    }///end FOR
  //am iesit din for


  //****************************************
  //plotam axa y cu x=0 in BLUE
    col.red = 0;
    col.green = 0;
    col.blue = 255;
    unsigned int xnZERO = ((xnMax - xnMin) / (xMax - xMin)) * (0.0 - xMin) + xnMin;

    for (yn = 0; yn < ynMax; ++yn)
    {
        //plot the axe x=0
        imageGraf.set_pixel(xnZERO, yn, col.red, col.green, col.blue);

    }
    //***********************************************

    //cout << "pozitive y  VERDE  "<<" negative y ROSU " <<" y=mic ALB \n";
    cout << "SAVE strGraph=" << strGraph << "\n";
    imageGraf.save_image(strGraph); //aici
    cout << "SAVEd.... strGraph=" << strGraph << "\n";
    //getchar();
   // if(strGraph  ==  "Mexican1Graph.bmp")
   // system("Mexican1Graph.bmp");
   // else //system("Mexican2Graph.bmp");


}//end function MexicanHatGraphic 7777777777777777777777777

//cout  <<  "end function MexicanHatGraphic::\ n " << strGraph << "\n";








//////////////////////////////////////////////////////////
//DO tHE main JOB find the point that corespond to the initioal

int GiveTheSamePixelinImage(unsigned int xnCROSSinput,
unsigned int ynCROSSinput,
bitmap_image *pimage,bitmap_image *pFindPixelImage,
string FindPixelNameFile)
{
//CREATE 2 MexicanHats different
 //////////////////////// run CreateMexicanHat()   /////////

/////////////////////// start SYSTEM /////////////////
int i;
  printf ("Checking if processor is available...");
  if (system(NULL)) puts ("Ok");
    else exit (EXIT_FAILURE);
  printf ("Executing command DIR...\n");
  i=system ("dir");
  printf ("The value returned was: %d.\n",i);
//return(0);
/////////////////////// end SYSTEM /////////////////
//domeniul BMP 
unsigned int BMPxMin=0; 
unsigned int  BMPxMax=Width;
unsigned int BMPyMin=0;
unsigned int  BMPyMax=Height;

//domeniul Mexican 
double HatxMin=xMin; 
double HatxMax=xMax;
double HatyMin=yMin;
double HatyMax=yMax;
string strtheHatBMP1="Mexican1.bmp";
string strtheHatGraphBMP1="Mexican1Graph.bmp"; 
string strtheHatBMP2="Mexican2.bmp";
string strtheHatGraphBMP2="Mexican2Graph.bmp";



cout << "Incepe MexicanGraphic... \n"<<  strtheHatGraphBMP1;

//  AICI PUNEM VARIABILE DIFERITE ******************

gama=gama1; Width=Width1; Height=Height1;
cout << "START MexicanGraphic..1\n";
MexicanHatGraphic(xMin,xMax,constY,deltax, gama,strtheHatGraphBMP1);


cout << "OUTSIDE MexicanGraphic..1.PRESS ENTER \n" ;
//getchar();

BMPxMin=0; 
BMPxMax=Width;
BMPyMin=0;
BMPyMax=Height;

cout << "CreateMexicanHat START \n " ;
 CreateMexicanHat( BMPxMin, BMPxMax,BMPyMin,  BMPyMax,
 HatxMin, HatxMax, HatyMin, HatyMax,
  gama,strtheHatBMP1);  
cout << "CreateMexicanHat END \n " ;


gama=gama2; Width=Width2; Height=Height2;
cout << "Incepe MexicanGraphic 2... \n" << strtheHatGraphBMP2;
MexicanHatGraphic(xMin,xMax,constY,deltax, gama,strtheHatGraphBMP2);

cout << "END MexicanGraphic. 2 ENTER.. \n" ;
//getchar();


BMPxMin=0; 
BMPxMax=Width;
BMPyMin=0;
BMPyMax=Height;
cout << "CreateMexicanHat START \n " ;
 CreateMexicanHat( BMPxMin, BMPxMax,BMPyMin,  BMPyMax,
 HatxMin, HatxMax, HatyMin, HatyMax,
  gama,strtheHatBMP2);  
cout << "CreateMexicanHat END \n " ;

/*//////////////////////// end al 2 SET ///////////////
*/


/////////////////////END RUN CreateMexicanHat() ////////
/////// cucu
//  AICI Calculam outul la GetValMexicanHatAt
// SETAM  VARIABILELE
gama=gama1; Width=Width1; Height=Height1;

bitmap_image MexicanHat1(strtheHatBMP1);//load the FILE must exist !!!!
bitmap_image *pMexicanHat1=&MexicanHat1;
double zHatOut1= GetValMexicanHatAt(xnCROSSinput,ynCROSSinput ,
pimage ,pMexicanHat1,false);
if(zHatOut1<0.0)
{
  cout << "\n zoutHat = " <<zHatOut1 << " PROBLEM \n";
getchar();
return(0);
}else
{
cout << " \n\n GetValMexicanHatAt   zoutHat = " <<zHatOut1 << "\n";
printf ("\n\n files out CROSS_%d_%d.bmp  \n", xnCROSSinput,ynCROSSinput );
cout << "\n\n Mexican Hat is "<<strtheHatBMP1<<"\n";

//getchar();
}


/*/////////////// a doua calculare */

gama=gama2; Width=Width2; Height=Height2;
bitmap_image MexicanHat2(strtheHatBMP2);//load the FILE must exist !!!!
bitmap_image *pMexicanHat2=&MexicanHat2;
double zHatOut2= GetValMexicanHatAt(xnCROSSinput,ynCROSSinput ,pimage ,
pMexicanHat2,false);
if(zHatOut2<0.0)
{
  cout << "\n zoutHat = " <<zHatOut2 << " PROBLEM \n";
//getchar();
return(0);
}else
{
cout << " \n\n GetValMexicanHatAt   zoutHat = " <<zHatOut2 << "\n";
printf ("\n\n files out CROSS_%d_%d.bmp  \n", xnCROSSinput,ynCROSSinput );
cout << "\n\n Mexican Hat is "<<strtheHatBMP2<<"\n";

//getchar();
}

cout << "\n \n zHatOut1=" << zHatOut1<< "\n\n zHatOut2=" << zHatOut2 <<"\n";
//PROBLEMA  AICI AVEM VALORILE DORITE DIN PIXELUL ALES
//GetValueMexHatAtCROSS1 =zHatOut1;
//GetValueMexHatAtCROSS2 =zHatOut2;
//getchar();

//////////////////////////////////////////////////

//Incep sa calculez outputul pentru diferitele pozitii
//ale imaginii in care se cauta CROSS x y
// incep sa calculez pentru fiecare pixel din imaginea de cautare
// pFindPixelImage care poate avea alte width height
unsigned int Width_SearchImage=pFindPixelImage->width();
unsigned int Height_SearchImage=pFindPixelImage->height();
unsigned int Height_Mexic1=pMexicanHat1->height();
unsigned int Height_Mexic2=pMexicanHat2->height();
unsigned int Height_Mexic_MAX;//the max of mexic we will ignore the pixels

if(Height_Mexic1>Height_Mexic2)Height_Mexic_MAX=Height_Mexic1;
else Height_Mexic_MAX=Height_Mexic2;

//cout << "Height_Mexic_MAX ="<< Height_Mexic_MAX << "\n";
//cout <<"Width_SearchImage="<< Width_SearchImage<< "\n";
//cout <<"Height_SearchImage="<< Height_SearchImage<< "\n";
//getchar();

//incepem sa calcul;am in fiecare punc al imaginii 
unsigned int xn;
unsigned int yn;
double zHatOutxnyn1;//the output 
double zHatOutxnyn2;

/*
 * EXPERIMNENTAM ************************
 * 
 */
//HERE WE GOT The values of input image
/*
gama=gama1; Width=Width1; Height=Height1;
GetValueMexHatAtCROSS1= GetValMexicanHatAt(xnCROSSinput,ynCROSSinput ,
pimage ,pMexicanHat1,false);
gama=gama2; Width=Width2; Height=Height2;
GetValueMexHatAtCROSS2= GetValMexicanHatAt(xnCROSSinput,ynCROSSinput ,
pimage ,pMexicanHat2,false);
*/


double deltaBIG=0;//aici calculam minimul diferentei MexicanHat
double deltaBIGold=99999999999999999999.0;
unsigned int xnFound=1; //aici punem pixelul gasit FFF important
unsigned int ynFound=1;
//double delta1;//calcul fabs de diferente;
//double delta2;

unsigned int xnmin=Height_Mexic_MAX;
unsigned int xnmax=Width_SearchImage-Height_Mexic_MAX;
unsigned int ynmin=Height_Mexic_MAX;
unsigned int ynmax=Height_SearchImage-Height_Mexic_MAX;
int  percent;
/*
unsigned int d=40;
xnmin= xnCROSSinput -d;
xnmax=xnCROSSinput+d;
ynmin=ynCROSSinput-d;
ynmax=ynCROSSinput+d;
*/



/// REINCARCAM Imaginile periculos
//cucu    MEREEEEEEEEEEEEEE E zero dif
//string strInputImage="image.bmp";
//string strFindinImage="findinimage.bmp";
// LOAD IMAGES FRESH ********** IMPORETANT
//cout << strFindinImage << endl;
bitmap_image imageinput(strInputImage);
// A doua imagine in care cautam punctul::::
bitmap_image FindinImage(strFindinImage);
bitmap_image *pimageNOU=&imageinput;
bitmap_image *pFindPixelImageNOU=&FindinImage;

/////////////////////////////////// TEST ////////////////
//cucu


//AICI E PROBLEMA La ACELEASI PARAM DA DOUA VAL DIF
//TRE false pus 

gama=gama1; Width=Width1; Height=Height1;
zHatOutxnyn1= GetValMexicanHatAt(xnCROSSinput,ynCROSSinput ,
pimageNOU ,pMexicanHat1,false);
gama=gama1; Width=Width1; Height=Height1;
GetValueMexHatAtCROSS1= GetValMexicanHatAt(xnCROSSinput,ynCROSSinput ,
pFindPixelImageNOU ,pMexicanHat1,false);
//cout << "\n \n\n zHatOutxnyn1 = "<<zHatOutxnyn1 <<"\n GetValueMexHatAtCROSS1 = "<<GetValueMexHatAtCROSS1<<"\n" ;
//cout << "\n \n\n zHatOutxnyn2 = "<<zHatOutxnyn2 <<"\n GetValueMexHatAtCROSS2 = "<<GetValueMexHatAtCROSS2<<"\n" ;

printf("\n \n\n zHatOutxnyn1 =%f   GetValueMexHatAtCROSS1 = %f delta=%f ",
zHatOutxnyn1,GetValueMexHatAtCROSS1,
zHatOutxnyn1-GetValueMexHatAtCROSS1);
/*
pFindPixelImage->save_image("pFindPixelImage.bmp");
pimage->save_image("pimage.bmp");


getchar();

///aici plotam imaginile:
/////////////////////// start SYSTEM /////////////////
int i1;
  printf ("Checking if processor is available...");
  if (system(NULL)) puts ("Ok");
    else exit (EXIT_FAILURE);
  printf ("Executing command mirage pimage.bmp ...\n");
  i1=system ("mirage pimage.bmp &");
  printf ("The value returned was: %d.\n",i1);
//return(0);
/////////////////////// end SYSTEM /////////////////




getchar();
//return(0);


///////////////////////////// END TEST////////////////////
*/

gama=gama1; Width=Width1; Height=Height1;
GetValueMexHatAtCROSS1= GetValMexicanHatAt(xnCROSSinput,ynCROSSinput ,
pimageNOU ,pMexicanHat1,false);
gama=gama2; Width=Width2; Height=Height2;
GetValueMexHatAtCROSS2= GetValMexicanHatAt(xnCROSSinput,ynCROSSinput ,
pimageNOU ,pMexicanHat2,false);

double deltaBIGBUN=100000000;
double count=0;
unsigned int NR_PixelsFound=0;
for(xn=xnmin;xn<xnmax; ++xn)
{
  for(yn=ynmin;yn<ynmax; ++yn)
  {
    
    count++;
    
percent=100.*count/((xnmax-xnmin)*(ynmax-ynmin));

//cout << "\n xn = "<<xn <<"yn = "<<yn << " percent= "<<percent <<"\n";



//AICI E PROBLEMA La ACELEASI PARAM DA DOUA VAL DIF
//cucu
gama=gama1; Width=Width1; Height=Height1;
zHatOutxnyn1= GetValMexicanHatAt(xn,yn ,
pFindPixelImageNOU ,pMexicanHat1,false);
gama=gama2; Width=Width2; Height=Height2;
zHatOutxnyn2= GetValMexicanHatAt(xn,yn,
pFindPixelImageNOU ,pMexicanHat2,false);
//cout << "\n \n\n zHatOutxnyn1 = "<<zHatOutxnyn1 <<"\n GetValueMexHatAtCROSS1 = "<<GetValueMexHatAtCROSS1<<"\n" ;
//cout << "\n \n\n zHatOutxnyn1 = "<<zHatOutxnyn2 <<"\n GetValueMexHatAtCROSS2 = "<<GetValueMexHatAtCROSS2<<"\n" ;
//getchar();




//cout << ".....\n";

if(zHatOutxnyn1>0.0 &&zHatOutxnyn2 >0.0)
{
//da diferenta intre valorile din CROSS si astea
double delta1=  fabs(zHatOutxnyn1-GetValueMexHatAtCROSS1); 
double delta2=  fabs(zHatOutxnyn2-GetValueMexHatAtCROSS2); 

//get the minimum to get the corespondimg points
deltaBIG=sqrt(delta1*delta1+delta2*delta2);
if (deltaBIG <=deltaBIGold ){
  xnFound=xn;
  ynFound=yn;
  deltaBIGBUN=deltaBIG;
  cout << "\n \n\n xnCROSSinput = "<<xnCROSSinput <<"  ynCROSSinput = "<<ynCROSSinput<<"\n" ;
cout << "\n xnFound = "<<xnFound <<"  ynFound= "<<ynFound <<"\n";
  cout<< "\n deltaBIG "<<deltaBIG<<"\n delta1 "<<delta1<<"\n delta2 "<<delta2;
//if(deltaBIG<0.1)  WriteCROSSinImage(xnFound,ynFound,pFindPixelImage,FindPixelNameFile);
 //NR_PixelsFound++;
 if(deltaBIG<0.1)getchar();
deltaBIGold=deltaBIG;
}//end if

}//end if out zozitiv
  }//end for ny
}//end for xn

cout << "\n deltaBIGBUN="<<deltaBIGBUN<<"\n";
cout << "\n \n\n xnCROSSinput = "<<xnCROSSinput <<"  ynCROSSinput = "<<ynCROSSinput<<"\n" ;
cout << "\n xnFound = "<<xnFound <<"  ynFound= "<<ynFound <<"\n";
cout <<"Wrote the OutFile with RESULT ......."<< FindPixelNameFile<< "\n";
WriteCROSSinImage(xnFound,ynFound,pFindPixelImage,FindPixelNameFile);
cout <<"COMPARA CU image.bmp input file\n\n  PRESS ENTER \n";
//cout << "\n\n\n NUMBER OF MATCHES = "<< NR_PixelsFound<< "\n";
//getchar();


  return 0;
}///end GiveTheSamePixelinImage
///////////////////////////////////////////////////
//******************************************************
// luk main 7777777777777777777 main 777777777777

int main( int argc, char*  argv[]  )
{

    if (argc <=1 )
    {
        cout << " no arguments..  \n  ";
      //  exit(1);
        xnCROSS =150 ;
        ynCROSS= 150;
        string strInputImage = "image.bmp";
        string strFindinImage = "findinimage.bmp";
        gama1 = 0.4;
        gama2 = 0.6;
       
    }
    if (argc == 3) {

        xnCROSS = atoi(argv[1]);
        ynCROSS = atoi(argv[2]);
        string strInputImage = "image.bmp";
        string strFindinImage = "findinimage.bmp";
        gama1 = 0.4;
        gama2 = 0.6;
        cout << "strInputImage=  " << strInputImage << endl;
        cout << " strFindinImage=  " << strFindinImage << endl;
        cout << "...........\n \n  .....PRESS ENTER....";

    }
    if (argc == 5) {
        xnCROSS = atoi(argv[1]);
        ynCROSS = atoi(argv[2]);
        strInputImage = argv[3];
        strFindinImage = argv[4];
        gama1 = 0.4;
        gama2 = 0.6;

    
    }
    if(argc==7){
        cout << "NEED ALL 6 ARGUMENTS... \n ";
           
          xnCROSS = atoi(argv[1]);
        ynCROSS = atoi(argv[2]);
        strInputImage = argv[3];
        strFindinImage = argv[4];
        gama1 = atof(argv[5]);
        gama2 = atof(argv[6]);


       
    }
    if(argc> 7){
        cout << "NEED only 6 ARGUMENTS... \n ";
        return 2;
    }


    cout << "  xnCROSS=  " << xnCROSS << endl;
    cout << "  ynCROSS=  " << ynCROSS << endl;

    cout << "strInputImage=  " << strInputImage << endl;

    cout << " strFindinImage=  " << strFindinImage << endl;
    cout << "gama1=  " << gama1 << endl;
    cout << "gama2=  " << gama2 << endl;
   
    
    cout << "...........\n \n  .....PRESS ENTER....";
//getchar();
// LOAD IMAGES********** IMPORETANT
bitmap_image imageinput(strInputImage);
// A doua imagine in care cautam punctul::::
bitmap_image FindinImage(strFindinImage);
if (imageinput.width() == 0 || FindinImage.width() == 0)
{
    cout << strInputImage  <<  endl;

    cout << strFindinImage << endl;
    cout << "  input image or find image  NOT FIND... not loaded....so  TO EXIT      ";
   // getchar();
    exit(1);


}
// vedem daca xnCROSS ynCROSS sint in zona buna
int fileWIDTH = imageinput.width();
int fileHEIGHT= imageinput.height();
if (xnCROSS <Width2 || xnCROSS> fileWIDTH - Width2 || ynCROSS <Height2 || ynCROSS> fileHEIGHT - Height2)
{
    cout << " xnCROSS <Width2 || xnCROSS> fileWIDTH - Width2 || ynCROSS <Height2 || ynCROSS  PROLEMA EXIT";
        exit(38);
}

//bitmap_image imageinput("image.bmp");
//bitmap_image CROSSoutImage("outputCROSS.bmp");

//imageinput.convert_to_grayscale();
//imageinput.save_image("grey1Enya.bmp");

// A doua imagine in care cautam punctul::::
//bitmap_image FindinImage("image.bmp");
//imageFindPoint.convert_to_grayscale();
//imageFindPoint.save_image("grey1.bmp");

//bitmap_image imageMexicanHat("mexican3DHat.bmp");
//imageMexicanHat.convert_to_grayscale();
//imageMexicanHat.save_image("grayscale_MexicanHat.bmp");

// LUK  AICI E FFFF IMPORTANT SE ALEGE PIXELUL
//xnCROSS = 150;//imageinput.width()/2;
//ynCROSS = 150;// imageinput.height() / 3;

bitmap_image *pimage=&imageinput;
bitmap_image *pFindinImage=&FindinImage;
//THE file where is The Result of finding the coresponding point
string FindPixelNameFile="OutPutCROSS.bmp";
 
 //////// The output file after CROSS
char memimagefile[200];
using namespace std;
//sprintf (memimagefile, "CROSS_input%d_%d.bmp", xnCROSS,ynCROSS);

// Scrie in pimage axele de CROSS x y
 //sprintf (memimagefile, "CROSS_input%d_%d.bmp", xnCROSS,ynCROSS);
 sprintf(memimagefile, "inputimageCROSS.bmp");
WriteCROSSinImage(xnCROSS,ynCROSS,pimage,memimagefile);


/*
Afla punctul din pFindinImage corespunzator 
* punctului xnCROSS, ynCROSS din pimage
* si il pune in fisierul cu numele FindPixelNameFile
* care tre sa aiba dimensiunea si continutul lui pFindinImage
*/ 
GiveTheSamePixelinImage( xnCROSS, ynCROSS,pimage,pFindinImage,
FindPixelNameFile);


return 0;
}

//end main


 /////////////////////////////// END /////////////////////



