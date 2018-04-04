#include <stdio.h>


void ip(unsigned char *in, unsigned char *out);
void ip_r(unsigned char *in, unsigned char *out);
void feistel_enc(unsigned char* in, unsigned char *key);
feistel_dec(unsigned char* block, unsigned char *deckey64);
void Makekey(unsigned char* OneRkey, unsigned char*OneLkey, unsigned char* Rkey, unsigned char* Lkey, unsigned char* out, int roundno);
void Divkey(unsigned char* key64, unsigned char* OneRkey, unsigned char*OneLkey, unsigned char* Rkey, unsigned char* Lkey);


void Divkey(unsigned char* key64, unsigned char* OneRkey, unsigned char*OneLkey, unsigned char* Rkey, unsigned char* Lkey) {
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
	int i, index, bit;
	unsigned char mask = 0x80;
	unsigned char key56[7];

	for (i = 0; i < 7; i++)
		key56[i] = 0x00;
	// key -> 56개
	for (i = 0; i < 56; i++) {
		index = (PC_1[i] - 1) / 8;
		bit = (PC_1[i] - 1) % 7;

		if (key64[index] & (mask >> bit))
			key56[i / 8] |= mask >> (i % 8);
	}


	/////// 28비트 씩 선택 /////////
	///// L K E Y /////
	for (i = 0; i < 3; i++) {
		OneLkey[i] = key56[i];
	}
	OneLkey[3] = (key56[3] & 0xF0);
	///// R K E Y /////
	OneRkey[0] = (key56[3] & 0x0F);
	for (i = 1; i < 4; i++) {
		OneRkey[i] = key56[i + 3];
		// Rkey[1] = key[i+3] << 4;
		// Rkey[1] |= key[i+4] >> 4;
	}
	for (int i = 0; i < 4; i++) {
		Rkey[i] = OneRkey[i];
		Lkey[i] = OneLkey[i];
	}
}
void Makekey(unsigned char* OneRkey, unsigned char*OneLkey, unsigned char* Rkey, unsigned char* Lkey, unsigned char* out, int roundno) {
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
	int PC_2[48] = {
		50, 25, 3, 27, 5, 29,
		32, 7, 33, 9, 35, 11,
		37, 13, 40, 15, 17, 41,
		19, 43, 21, 54, 23, 48,
		49, 2, 26, 4, 28, 6,
		30, 52, 51, 34, 10, 36,
		12, 38, 14, 56, 53, 18,
		42, 20, 44, 22, 46, 1
	};
	int i, index, bit;
	unsigned char mask = 0x80;
	unsigned char key56[7];
	unsigned char combine[7] = {0,};
	unsigned char temp = 0x00;
	for (i = 0; i < 6; i++)
		out[i] = 0x00;

	//////////////////////////////////////////
	//Rkey[0] = Rkey[0] << 4;				//
	//Rkey[0] = Rkey[0] | Rkey[1] >> 4;		//
	//Rkey[1] = Rkey[1] << 4;				//
	//Rkey[1] = Rkey[1] | Rkey[2] >> 4;		//
	//Rkey[2] = Rkey[2] << 4;				//
	//Rkey[2] = Rkey[2] | Rkey[3] >> 4;		//
	//////////////////////////////////////////
	/*for (i = 0; i < 2; i++) {
	Rkey[i] = Rkey[i] << 4;
	Rkey[i] = Rkey[i] | Rkey[i + 1] >> 4;
	}
	Rkey[3] = Rkey[3] << 4;*/

	/* ////////////////////////////////////////
	//	Left Shift Ex when Bit rotated 2	//
	//	Lkey[0] = Lkey[0] << 2;				//
	//	Lkey[0] |= Lkey[1] >> 6;			//
	//	Lkey[1] = Lkey[1] << 2;				//
	//	Lkey[1] |= Lkey[2] >> 6;			//
	//	Lkey[2] = Lkey[2] << 2;				//
	//	Lkey[2] |= Lkey[3] >> 6;			//
	////////////////////////////////////////*/

	//// when Bit rotated 1 (Round no 1, 5, 9, 16)
	////// 1 Left Shift [ L ] //////
	////// 1 Left Shift [ R ] //////
	//for (roundno = 0; roundno < 16; roundno++) {
	if (roundno == 0 || roundno == 4 || roundno == 9 || roundno == 15) {
		temp = OneLkey[0] & 0x80;
		for (i = 0; i < 3; i++) {
			OneLkey[i] = OneLkey[i] << 1;
			OneLkey[i] |= OneLkey[i + 1] >> 7;
		}
		OneLkey[3] = OneLkey[3] << 1;
		OneLkey[3] = OneLkey[3] | (temp >> 3);

		temp = OneRkey[0] >> 3;
		for (i = 0; i < 3; i++) {
			OneRkey[i] = OneRkey[i] << 1;
			if (i == 0) {
				OneRkey[i] = OneRkey[i] & 0x0F;
			}
			OneRkey[i] |= OneRkey[i + 1] >> 7;
		}
		OneRkey[3] = OneRkey[3] << 1;
		OneRkey[3] |= temp;

		///////////////////////////////////
		////// Combine Lkey and Rkey //////
		///////////////////////////////////


		for (i = 0; i < 7; i++) {
			if (i < 3) {
				combine[i] = OneLkey[i];
			}
			else if (i == 3) {
				combine[i] = (OneLkey[i] | OneRkey[i - 3]);
			}
			else {
				combine[i] = OneRkey[i - 3];
			}
		}
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
	else {
		temp = Lkey[0] & 0xC0;
		for (i = 0; i < 3; i++) {
			Lkey[i] = Lkey[i] << 2;
			Lkey[i] |= Lkey[i + 1] >> 6;
		}
		Lkey[3] = Lkey[3] << 2;
		Lkey[3] |= temp >> 2;

		temp = Rkey[0] >> 2;
		for (i = 0; i < 3; i++) {
			Rkey[i] = Rkey[i] << 2;
			Rkey[i] |= Rkey[i + 1] >> 6;
		}
		Rkey[3] = Rkey[3] << 2;
		Rkey[3] |= temp;

		///////////////////////////////////
		////// Combine Lkey and Rkey //////
		///////////////////////////////////
		for (i = 0; i < 7; i++) {
			if (i < 3) {
				combine[i] = Lkey[i];
			}
			else if (i == 3) {
				combine[i] = (Lkey[i] | Rkey[i - 3]);
			}
			else {
				combine[i] = Rkey[i - 3];
			}
		}
	}
	///////////////////////////////////
	////////// extract 48 bit /////////
	///////////////////////////////////

	for (i = 0; i < 48; i++) {
		index = (PC_2[i] - 1) / 8;
		bit = (PC_2[i] - 1) % 6;
		if (combine[index] & (mask >> bit))
			out[i / 8] |= mask >> (i % 8);
	}
	//}
}
void feistel_enc(unsigned char* in, unsigned char *key) {

	int Expansion_t[] = {
		32, 1, 2, 3, 4, 5,
		4, 5, 6, 7, 8, 9,
		8, 9, 10, 11, 12, 13,
		12, 13, 14, 15, 16, 17,
		16, 17, 18, 19, 20, 21,
		20, 21, 22, 23, 24, 25,
		24, 25, 26, 27, 28, 29,
		28, 29, 30, 31, 32, 1
	};
	int substitution_t[] = {
		2, 8, 14, 20, 26, 32, 38, 44,
		3, 9, 15, 21, 27, 33, 39, 45,
		4, 10, 16, 22, 28, 34, 40, 46,
		5, 11, 17, 23, 29, 35, 41, 47
	};
	unsigned char OneLkey[4];
	unsigned char OneRkey[4];
	unsigned char Lkey[4];
	unsigned char Rkey[4];
	unsigned char Lp[4]; // 확장전 L
	unsigned char Rp[4]; // 확장전 R
	unsigned char pRp[6]; // 확장 후 R
	unsigned char pRst[6]; //key와 xor한 후 값
	unsigned char Rst[4]; // pRst 32bit 축소
	unsigned char fRst[4]; // L 과 R의 xor
	unsigned char Ciphertext[8];
	int i, j, index, bit, make;
	unsigned char mask = 0x80;
	unsigned char out[8] = { 0, };
	unsigned char exkey[6] = { 0, };
	ip(in, out);
	// 32비트 나누기
	for (i = 0; i < 8; i++) {
		if (i < 4) {
			Lp[i] = out[i];
		}
		else {
			Rp[i - 4] = out[i];
		}
	}
	// key 각각 나눠주기
	Divkey(key, OneRkey, OneLkey, Rkey, Lkey);
	for (j = 0; j < 16; j++) {
		Makekey(OneRkey, OneLkey, Rkey, Lkey, exkey, j);
		for (i = 0; i < 48; i++) {
			index = (Expansion_t[i] - 1) / 8;
			bit = (Expansion_t[i] - 1) % 6;

			//if (Lp[index] & (mask >> bit));
			//pLp[i / 8] |= mask >> (i % 8);
			if (Rp[index] & (mask >> bit));
			pRp[i / 8] |= mask >> (i % 8);
		} // Right 확장
		for (i = 0; i < 6; i++) {
			pRst[i] = pRp[i] ^ exkey[i];
		} // 확장한 R값과 key값을 xor
		for (i = 0; i < 32; i++) {
			index = (Expansion_t[i] - 1) / 8;
			bit = (Expansion_t[i] - 1) % 4;

			if (pRst[index] & (mask >> bit));
			Rst[i / 8] |= mask >> (i % 8);
		} // Right 축소
		if (j == 15) {
			for (i = 0; i < 4; i++){
				fRst[i] = Rp[i] ^ Rst[i];
				Rp[i] = Lp[i];
				Lp[i] = fRst[i];
			}
		}
		else {
			for (i = 0; i < 4; i++){
				fRst[i] = Lp[i] ^ Rst[i];
				Lp[i] = Rp[i];
				Rp[i] = fRst[i];
			}
		}
	} // 0~15까지 반복 16번
	for (i = 0; i < 8; i++) {
		Ciphertext[i] = Lp[i];
		Ciphertext[i+4] = Rp[i];
	}
	ip_r(Ciphertext, in);
}
feistel_dec(unsigned char* block, unsigned char *deckey64) {
	int Expansion_t[] = {
		32, 1, 2, 3, 4, 5,
		4, 5, 6, 7, 8, 9,
		8, 9, 10, 11, 12, 13,
		12, 13, 14, 15, 16, 17,
		16, 17, 18, 19, 20, 21,
		20, 21, 22, 23, 24, 25,
		24, 25, 26, 27, 28, 29,
		28, 29, 30, 31, 32, 1
	};
	int substitution_t[] = {
		2, 8, 14, 20, 26, 32, 38, 44,
		3, 9, 15, 21, 27, 33, 39, 45,
		4, 10, 16, 22, 28, 34, 40, 46,
		5, 11, 17, 23, 29, 35, 41, 47
	};
	unsigned char OneLkey[4];
	unsigned char OneRkey[4];
	unsigned char Lkey[4];
	unsigned char Rkey[4];
	unsigned char Lp[4];
	unsigned char Rp[4];
	unsigned char pLp[6];
	unsigned char pRp[6];
	unsigned char pRst[6];
	unsigned char Rst[4];
	unsigned char fRst[4];
	unsigned char Ciphertext[8];
	int i, j, index, bit, make;
	unsigned char mask = 0x80;
	unsigned char out[8] = { 0, };
	unsigned char exkey[6] = { 0, };
	ip(block, out);
	for (i = 0; i < 8; i++) {
		if (i < 4) {
			Lp[i] = out[i];
		}
		else {
			Rp[i - 4] = out[i];
		}
	} // 32비트 나누기
	Divkey(deckey64, OneLkey, OneRkey, Lkey, Rkey);
	for (j = 0; j < 16  ; j++) {
		Makekey(OneLkey, OneRkey, Lkey, Rkey, exkey, j);
		for (i = 0; i < 48; i++) {
			index = (Expansion_t[i] - 1) / 8;
			bit = (Expansion_t[i] - 1) % 6;

			//if (Lp[index] & (mask >> bit));
			//pLp[i / 8] |= mask >> (i % 8);
			if (Rp[index] & (mask >> bit));
			pRp[i / 8] |= mask >> (i % 8);
		} // Right 확장
		for (i = 0; i < 6; i++) {
			pRst[i] = pRp[i] ^ exkey[i];
		} // R값과 key값을 xor
		for (i = 0; i < 32; i++) {
			index = (Expansion_t[i] - 1) / 8;
			bit = (Expansion_t[i] - 1) % 4;

			if (pRst[index] & (mask >> bit));
			Rst[i / 8] |= mask >> (i % 8);
		} // Right 축소
		if (j == 0) {
			for (i = 0; i < 4; i++) {
				fRst[i] = Lp[i] ^ Rst[i];
				Lp[i] = Rp[i];
				Rp[i] = fRst[i];
			}
		}
		else {
			for (i = 0; i < 4; i++) {
				fRst[i] = Rp[i] ^ Rst[i];
				Rp[i] = Lp[i];
				Lp[i] = fRst[i];
			}
		}
	} // 0~15까지 반복 16번
	for (i = 0; i < 8; i++) {
		Ciphertext[i] = Lp[i];
		Ciphertext[i + 4] = Rp[i];
	}
	ip_r(Ciphertext, block);
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
		index = (ip_t[i] - 1) / 8;
		bit = (ip_t[i] - 1) % 8;

		if (in[index] & (mask >> bit));
		out[i / 8] |= mask >> (i % 8);
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
		index = (ip_rt[i] - 1) / 8;
		bit = (ip_rt[i] - 1) % 8;

		if (in[index] & (mask >> bit))
			out[i / 8] |= mask >> (i % 8);
	}
}



int main() {
	unsigned char key64[8] = { 0x6B, 0x68, 0x6E, 0x6B, 0x79, 0x34, 0x18, 0x20 };

	unsigned char block[8] = { 0x20, 0x18, 0x4C, 0x4F, 0x56, 0x45, 0x40, 0x52 };

	unsigned char deckey64[8] = { 0x6B, 0x68, 0x6E, 0x6B, 0x79, 0x34, 0x18, 0x20 };   // same as key64 - encryption key
	printf("20135185_황석진\n\n");
	printf("original text: %x %x %x %x %x %x %x %x\n", block[0], block[1], block[2], block[3], block[4], block[5], block[6], block[7]);

	printf("=======================================================\n");
	feistel_enc(block, key64);
	printf("cipher text: %x %x %x %x %x %x %x %x\n", block[0], block[1], block[2], block[3], block[4], block[5], block[6], block[7]);

	printf("=======================================================\n");

	feistel_dec(block, deckey64);
	printf("decrypted text: %x %x %x %x %x %x %x %x\n", block[0], block[1], block[2], block[3], block[4], block[5], block[6], block[7]);
	getchar();
}
