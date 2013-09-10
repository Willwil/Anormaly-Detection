#ifndef _LIAN_GLCM_
#define _LIAN_GLCM_

class GLCM
{
	public:
		GLCM();

	public:
		float energy;   //sum_i{sum_j{c_ij * c_ij }}
		float entropy;   //sum_i{sum_j{log( c_ij ) * c_ij }}
		float constrast; //sum_i{sum_j{(i-j)^2 * c_ij }}
		float Homogeneity
};

#endif
