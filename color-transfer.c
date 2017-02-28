/**
 * @file color-transfert
 * @brief transfert color from source image to target image.
 *        Method from Reinhard et al. : 
 *        Erik Reinhard, Michael Ashikhmin, Bruce Gooch and Peter Shirley, 
 *        'Color Transfer between Images', IEEE CGA special issue on 
 *        Applied Perception, Vol 21, No 5, pp 34-41, September - October 2001
 */

#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <bcl.h>

#define D 3

static float RGB2LMS[D][D] = {
  {0.3811, 0.5783, 0.0402}, 
  {0.1967, 0.7244, 0.0782},  
  {0.0241, 0.1288, 0.8444}
};

static float LMS2lab1[D][D] = {
  {1.0, 1.0, 1.0}, 
  {1.0, 1.0, -2.0},  
  {1.0, -1.0, 0.0}
};

static float LMS2lab2[D][D] = {
  {1.0/sqrt(3.0), 0.0, 0.0}, 
  {0.0, 1.0/sqrt(6.0), 0.0},  
  {0.0, 0.0, 1.0/sqrt(2.0)}
};

static float lab2LMS1[D][D] = {
  {sqrt(3.0)/3.0, 0.0, 0.0}, 
  {0.0, sqrt(6.0)/6.0, 0.0},  
  {0.0, 0.0, sqrt(2.0)/2.0}
};

static float lab2LMS2[D][D] = {
  {1.0, 1.0, 1.0}, 
  {1.0, 1.0, -1.0},  
  {1.0, -2.0, 0.0}
};

static float LMS2RGB[D][D] = {
  {4.4679, -3.587, 0.1193}, 
  {-1.2186, 2.3809, -0.1624},  
  {0.0497, -0.2439, 1.2045}
};

static float mean(float* vec, int pxls)
{
  float sum = 0.0;
  for(int i = 0; i < pxls; i++)
  {
    sum += vec[i];
  }
  return sum/pxls;
}

static float sd(float* vec, int pxls, float mean)
{
  float sum = 0.0;
  for(int i = 0; i < pxls; i++)
  {
    sum += pow(vec[i] - mean, 2);
  }
  return sqrt(sum/(pxls-1));
}

static void multiply(float* vec, float matr[D][D])
{
  float x1 = vec[0], x2 = vec[1], x3 = vec[2];
  vec[0] = matr[0][0]*x1 + matr[0][1]*x2 + matr[0][2]*x3;
  vec[1] = matr[1][0]*x1 + matr[1][1]*x2 + matr[1][2]*x3;
  vec[2] = matr[2][0]*x1 + matr[2][1]*x2 + matr[2][2]*x3;
}

static void transform(float* img, float matr[D][D], int pxls)
{
  float* addr = img;
  for(int i = 0; i < pxls; i++)
  {
    multiply(img, matr);
    img+=3;
  }
  img = addr;
}



static void process(char *ims, char *imt, char* imd){
  pnm img = pnm_load(ims);  
  int rows = pnm_get_height(img);
  int cols = pnm_get_width(img);
  
  pnm img2 = pnm_load(imt);  
  int rows2 = pnm_get_height(imt);
  int cols2 = pnm_get_width(imt);

	
  pnm newi = pnm_new(cols2, rows2, PnmRawPpm);

  unsigned short *imgSource = pnm_get_image(img);
  unsigned short *imgTarget = pnm_get_image(img2);

  for(int i = 0; i < rows2; i++)
		{
			for(int j = 0; j < cols2; j++)
			{}
	         }


  /*float* a;
  a = (float*)malloc(6*sizeof(float));
  for(int i = 0; i < 6; i++)
   a[i]=1.0;
  transform(a, lab2LMS2, 2);
  transform(a, lab2LMS2, 2);*/
  (void) ims;
  (void) imt;
  (void) imd;
  //printf("%f, %f, %f, %f, %f, %f", a[0], a[1], a[2], a[3], a[4], a[5]);
}

void usage (char *s){
  fprintf(stderr, "Usage: %s <ims> <imt> <imd> \n", s);
  exit(EXIT_FAILURE);
}


#define param 3
int main(int argc, char *argv[]){





  if (argc != param+1) 
    usage(argv[0]);
  process(argv[1], argv[2], argv[3]);
  return EXIT_SUCCESS;



}
