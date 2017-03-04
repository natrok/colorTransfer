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

static void mean(float* vec, float* means, int pxls)
{
  printf("%f %f %f \n", means[0], means[1], means[2]);
  for(int i = 0; i < pxls; i++)
  {
    means[0] += vec[i*3];
	means[1] += vec[i*3+1];
	means[2] += vec[i*3+2];
	//if(i > pxls/4+3600 && i < pxls/4 + 4600)
	//printf("%d %f %f %f \n", i, means[0], vec[i*3], means[2]);
  }
  printf("%f %f %f \n", means[0], means[1], means[2]);
	  
  means[0] = means[0]/pxls;
  means[1] = means[1]/pxls;
  means[2] = means[2]/pxls;
}

static void sd(float* vec, float* sds, float* means, int pxls)
{
  for(int i = 0; i < pxls; i++)
  {
	sds[0] += pow(vec[i*3] - means[0], 2);
	sds[1] += pow(vec[i*3+1] - means[1], 2);
	sds[2] += pow(vec[i*3+2] - means[2], 2);
  }
  sds[0] = sqrt(sds[0]/(pxls-1));
  sds[1] = sqrt(sds[1]/(pxls-1));
  sds[2] = sqrt(sds[2]/(pxls-1));
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

static void toLogSpace(float* img, int pxls)
{
  for(int i = 0; i < pxls*3; i++)
  {
    img[i] = log10(img[i]+1); //add 1 to avoid log(0)=inf
  }
}

static void toLinSpace(float* img, int pxls)
{
  for(int i = 0; i < pxls*3; i++)
  {
    img[i] = pow(10, img[i])-1; //substract the previously added 1
  }
}



static void process(char *ims, char *imt, char* imd){
  pnm img = pnm_load(ims);  
  int rows = pnm_get_height(img);
  int cols = pnm_get_width(img);
  
  pnm img2 = pnm_load(imt);  
  int rows2 = pnm_get_height(img2);
  int cols2 = pnm_get_width(img2);

	
  pnm newi = pnm_new(cols2, rows2, PnmRawPpm);

  unsigned short *imgSource = pnm_get_image(img);
  unsigned short *imgTarget = pnm_get_image(img2);
  unsigned short *result = pnm_get_image(newi);
  float *source = (float*)malloc(sizeof(float)*3*rows*cols);
  float *target = (float*)malloc(sizeof(float)*3*rows2*cols2);
  printf("a: %d %d %d \n",  imgSource[70071*3 + 0], imgSource[70071*3 + 1], imgSource[70071*3 + 2]);
  for(int i = 0; i < rows*cols*3; i++)
  {
	source[i] = (float)imgSource[i];
  }
  for(int i = 0; i < rows2*cols2*3; i++)
  {
	target[i] = (float)imgTarget[i];
  }

  //------transform to lab space------//
  transform(source, RGB2LMS, cols*rows);
  transform(target, RGB2LMS, cols2*rows2);
  printf("a: %f %f %f \n",  source[70071*3 + 0], source[70071*3 + 1], source[70071*3 + 2]);
  toLogSpace(source, cols*rows);
  toLogSpace(target, cols2*rows2);
  printf("log: %f %f %f \n",  source[70071*3 + 0], source[70071*3 + 1], source[70071*3 + 2]);
  transform(source, LMS2lab1, cols*rows);
  transform(target, LMS2lab1, cols2*rows2);
  printf("a: %f %f %f \n",  source[70071*3 + 0], source[70071*3 + 1], source[70071*3 + 2]);
  transform(source, LMS2lab2, cols*rows);
  transform(target, LMS2lab2, cols2*rows2);
  
  printf("a: %f %f %f \n",  source[70071*3 + 0], source[70071*3 + 1], source[70071*3 + 2]);
  
  //------color transfer------//
  
  float meanS[3] = {0};
  float meanT[3] = {0};
  float sdS[3] = {0};
  float sdT[3] = {0};
  float sdNorm[3] = {0};
  mean(source, meanS, rows*cols);
  mean(target, meanT, rows2*cols2);
  sd(source, sdS, meanS, rows*cols);
  sd(target, sdT, meanT, rows2*cols2);
  for(int c = 0; c < 3; c++)
  {
	  sdNorm[c] = sdT[c]/sdS[c]; 
	  //printf("%f %f %f \n", sdNorm[c], meanT[c], meanS[c]);
	  for(int i = 0; i < cols2*rows2; i++)
	  {
		target[i*3+c] = sdNorm[c]*(target[i*3+c] - meanT[c]) + meanS[c];
	  }
  }
  
  
  //------transform to RGB space------//
  transform(source, lab2LMS1, cols*rows);
  transform(target, lab2LMS1, cols2*rows2);
  transform(source, lab2LMS2, cols*rows);
  transform(target, lab2LMS2, cols2*rows2);
  toLinSpace(source, cols*rows);
  toLinSpace(target, cols2*rows2);
  transform(source, LMS2RGB, cols*rows);
  transform(target, LMS2RGB, cols2*rows2);

  /*float* a;
  a = (float*)malloc(6*sizeof(float));
  for(int i = 0; i < 6; i++)
   a[i]=1.0;
  transform(a, lab2LMS2, 2);
  transform(a, lab2LMS2, 2);*/
  //for(int i = 0; i < rows*cols; i++)
  //printf("%d, %d, %d, %f, %f, %f\n", imgSource[i*3], imgSource[i*3+1], imgSource[i*3+2], source[i*3], source[i*3+1], source[i*3+2]);
  
  for(int i = 0; i < rows2*cols2*3; i++)
  {
	result[i] = roundf(target[i]);
  }
  
  pnm_save(newi, PnmRawPpm, imd);  
  pnm_free(img);
  pnm_free(img2);
  pnm_free(newi);
  free(source);
  free(target);
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
