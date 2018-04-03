#include <stdio.h>


void Roundkey(unsigned char* _key64, unsigned char *out){

	/////// 56 비트 선택 /////////
	int PC_1[56] = {
		57, 49, 41, 33, 25, 17, 9,
		1, 58, 50, 42, 34, 26, 18,
		10, 2, 59, 51, 43, 35, 27,
		19, 11, 3, 60, 52, 44, 36,
		63, 55, 47, 39, 31, 23, 15,
		7, 62, 54, 46, 38, 30, 22,
		14, 6, 61, 53, 45, 37, 29,
		21, 13, 5, 28, 20, 12, 4
	};
	unsigned char Lkey[4];
	unsigned char Rkey[4];
	int i, index, bit,make;
	unsigned char mask = 0x80; 

	for (i = 0; i < 7; i++)
		out[i] = 0x00;


	// index => 행
	// bit => 열
	for (i = 0; i < 56; i++) {
		index = (ip_t[i] - 1) / 8;
		bit = (ip_t[i] - 1) % 7;

		if (in[index] & (mask >> bit))
			out[i / 8] |= mask >> (i % 8);
	}
	

	/////// 28비트 씩 선택 /////////
	///// L K E Y /////
	for (i = 0; i < 3; i++){
		Lkey[i] = out[i];
	}
	Lkey[3] = out[3] & 0xF0;
	Rkey[0] = out[3] & 0x0F;
	///// R K E Y /////
	for (int i = 1; i < 3; i++){
		Rkey[i] = out[i+3];
    // Rkey[1] = out[i+3] << 4;
	//	Rkey[1] |= out[i+4] >> 4;
	}
	//Rkey[3] = out[7] << 4 & 0xFF;
	
	

	int roundno = 0; // roundno


	/* ////////////////////////////////////////
	//	Left Shift Ex when Bit rotated 2	//
	//	Lkey[0] = Lkey[0] << 2;				//
	//	Lkey[0] |= Lkey[1] >> 6;			//
	//	Lkey[1] = Lkey[1] << 2;				//
	//	Lkey[1] |= Lkey[2] >> 6;			//
	//	Lkey[2] = Lkey[2] << 2;				//
	//	Lkey[2] |= Lkey[3] >> 6;			//
	////////////////////////////////////////*/

	unsigned char temp;
	//// when Bit rotated 1 (Round no 1, 2, 9, 16)
	////// 1 Left Shift [ L ] //////
	////// 1 Left Shift [ R ] //////
	if (roundno == 1 || roundno == 2 || roundno == 9 || roundno == 16){
		temp = Lkey[0] & 0x80;
		for (i = 0; i < 3; i++){
			Lkey[i] = Lkey[i] << 1;
			Lkey[i] |= Lkey[i + 1] >> 7;
		}
		Lkey[3] |= temp >> 3;

		temp = Rkey[0] >> 3;
		for (i = 0; i < 3; i++){
			Rkey[i] = Rkey[i] << 1;
			Rkey[i] |= Rkey[i + 1] >> 7;
		}
		Rkey[3] = Rkey[3] << 2;
		Rkey[3] |= temp;
	}

	/* ///////////////////////////////////////
	//	Left Shift Ex when Bit rotated 2	//
	//	temp = Rkey[0] >> 2;				//
	//	Rkey[0] = Rkey[0] << 2;				//
	//	Reky[0] |= Rkey[1] >> 6;			//
	//	Rkey[1] = Rkey[1] << 2;				//
	//	Rkey[1] |= Rkey[2] >> 6;			//
	//	Rkey[2] = Rkey[2] << 2;				//
	//	Rkey[2] |= Rkey[3] >> 6;			//
	//	Rkey[3] = Rkey[3] << 2;				//
	//	Rkey[3] |= temp;					//
	////////////////////////////////////////*/

	/// when Bit rotated 2 (Round no 3, 4, 5, 6, 7, 8, 10, 11, 12, 13, 14, 15)
	////// 2 Left Shift [ L ] //////
	////// 2 Left Shift [ R ] //////
	else{
		temp = Lkey[0] & 0xC0;
		for (i = 0; i < 3; i++){
			Lkey[i] = Lkey[i] << 2;
			Lkey[i] |= Lkey[i + 1] >> 6;
		}
		Lkey[3] |= temp >> 2;

		temp = Rkey[0] >> 2;
		for (i = 0; i < 3; i++){
			Rkey[i] = Rkey[i] << 2;
			Rkey[i] |= Rkey[i + 1] >> 6;
		}
		Rkey[3] = Rkey[3] << 2;
		Rkey[3] |= temp;
	}


	///////////////////////////////////
	////// Combine Lkey and Rkey //////
	///////////////////////////////////

	



	///////////////////////////////////
	////////// extract 48 bit /////////
	///////////////////////////////////
	int PC_2[48] = {
		14, 17, 11, 24, 1, 5,
		3, 28, 15, 6, 21, 10,
		23, 19, 12, 4, 26, 8,
		16, 7, 27, 20, 13, 2,
		41, 52, 31, 37, 47, 55,
		30, 40, 51, 45, 33, 48,
		44, 49, 39, 56, 34, 53,
		46, 42, 50, 36, 29, 32,
	};



}


void RoundFunction(){

}


void ip(unsigned char *in, unsigned char *out) {
    int ip_t[64] = {
		58, 50, 42, 34, 26, 18, 10, 2,
		60, 52, 44, 36, 28, 20, 12, 4,
		62, 54, 46, 38, 30, 22, 14, 6,
		64, 56, 48, 40, 32, 24, 16, 8,
		57, 49, 41, 33, 25, 17, 9, 1,
		59, 51, 43, 35, 27, 19, 11, 3,
		61, 53, 45, 37, 29, 21, 13, 5,
		63, 55, 47, 39, 31, 23, 15, 7
	};

		  
	int i, index, bit;
	unsigned char mask = 0x80; 
	for (i = 0; i < 8; i++)  
		out[i] = 0x00;

	for (i = 0; i < 64; i++) {
		index = (ip_t[i]-1) / 8;
		bit = (ip_t[i] -1) % 8;

		if (in[index] & (mask >> bit));
			out[i/8] |= mask >> (i%8);
	}
}

void ip_r(unsigned char *in, unsigned char *out) {
    unsigned char ip_rt[64] = {
		40, 8, 48, 16, 56, 24, 64, 32,
		39, 7, 47, 15, 55, 23, 63, 31,
		38, 6, 46, 14, 54, 22, 62, 30,
		37, 5, 45, 13, 53, 21, 61, 29,
		36, 4, 44, 12, 52, 20, 60, 28,
		35, 3, 43, 11, 51, 19, 59, 27,
		34, 2, 42, 10, 50, 18, 58, 26,
		33, 1, 41, 9, 49, 17, 57, 25
	};

	int i, index, bit;
	unsigned char  mask = 0x80;

	for (i = 0; i < 8; i++)
		out[i] = 0x00;

	for (i = 0; i < 64; i++) {
		index = (ip_rt[i]-1) / 8;
		bit = (ip_rt[i] -1) % 8;

		if (in[index] & (mask >> bit))
			out[i/8] |= mask >> (i%8);
	}
}



int main() {
    unsigned char key64[8] = {0x6B, 0x68, 0x6E, 0x6B, 0x79, 0x34, 0x18, 0x20};

    unsigned char block[8] = {0x20, 0x18, 0x4C, 0x4F, 0x56, 0x45, 0x40, 0x52};

    unsigned char deckey64[8] = {0x6B, 0x68, 0x6E, 0x6B, 0x79, 0x34, 0x18, 0x20};   // same as key64 - encryption key

	printf("original text: %x %x %x %x %x %x %x %x\n", block[0], block[1], block[2], block[3], block[4], block[5], block[6], block[7]);

    printf("=======================================================\n");

    feistel_enc(block, key64);
	printf("cipher text: %x %x %x %x %x %x %x %x\n", block[0], block[1], block[2], block[3], block[4], block[5], block[6], block[7]);

    printf("=======================================================\n");

    feistel_dec(block, deckey64);
	printf("decrypted text: %x %x %x %x %x %x %x %x\n", block[0], block[1], block[2], block[3], block[4], block[5], block[6], block[7]);
}
