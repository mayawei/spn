#include<iostream>
#include<stdio.h>
#include<math.h>
#include<windows.h>
#include<string>
#include<openssl/bn.h>
using namespace std;
int c[16][16];
unsigned short getbit(unsigned short a, int m)
{
	if ((a & (1 << m)) == 0) return 0;
	else return 1;
}
void bitcopy(unsigned short *a, unsigned short *b, int m, int n)
{
	if ((*a & (1 << m)) == 0) { (*b) &= (~(1 << n)); }
	else (*b) |= (1 << n);
};
void bitcopy_plus (unsigned char *a,unsigned char *b, int m, int n)
{
	if ((*a & (1 << m)) == 0) { (*b) &= (~(1 << n)); }
	else (*b) |= (1 << n);
};

void shif(unsigned char * input)
{
	unsigned char t = input[0];
	input[0] = input[1];
	input[1] = input[2];
	input[2] = input[3];
	input[3] = t;
}


unsigned short pis[] = { 14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7 };
unsigned char pisp[] = {147,29,42,61,77,81,103,125,138,0,162,190,196,219,236,250,14,24,32,55,70,90,109,120,134,159,172,187,194,216,225,255,7,26,41,48,68,93,110,123,137,144,164,178,193,220,234,244,11,17,46,57,73,80,99,117,128,150,161,188,203,215,239,242,10,19,38,51,72,86,96,118,140,154,167,180,202,209,233,247,4,31,35,52,79,89,102,127,139,145,170,183,205,222,226,252,13,20,47,54,67,88,105,112,135,157,171,189,199,210,230,249,1,18,37,58,64,87,106,115,141,152,166,177,200,221,232,245,5,27,33,50,75,84,97,116,143,153,168,181,207,214,224,246,8,22,45,56,65,95,98,119,129,148,165,176,201,223,237,241,12,23,44,53,66,94,104,114,131,149,163,191,204,208,227,253,6,28,39,62,76,83,101,124,142,155,175,186,197,217,228,254,9,16,43,60,69,91,107,113,133,156,173,179,198,218,238,240,3,21,36,59,74,85,108,122,130,151,160,185,195,212,231,251,2,30,34,63,78,82,100,126,136,146,174,184,192,213,229,243,15,25,40,49,71,92,111,121,132,158,169,182,206,211,235,248};

unsigned short unpis[] = { 14,3,4,8,1,12,10,15,7,13,9,6,11,2,0,5 };
int pip[] = { 0,4,8,12,1,5,9,13,2,6,10,14,3,7,11,15 };


unsigned short k1 = 0x3a94;
unsigned short k2 = 0xa94d;
unsigned short k3 = 0x94d6;
unsigned short k4 = 0x4d63;
unsigned short k5 = 0xd63f;
unsigned short xtest[] = { 0xad21,0x5fff,0x6432 };
unsigned short ytest[] = { 0x91e9,0x58ee,0xeff4 };



void pishalf(unsigned short *a)
{
	*a = (pis[(*a >> 12)] << 12) + (pis[(*a >> 8) & 0x000f] << 8) + (pis[(*a >> 4) & 0x000f] << 4) + pis[*a & 0x000f];
}

void xor_plus(unsigned char * a,unsigned char * key)
{
	unsigned char * b = new unsigned char[16];
	for (int i = 0; i < 16; i++)
	{
		b[i] = a[i]^key[i];
	}
	memcpy(a, b, 16);
	delete(b);
};


void pishalf_plus(unsigned char * a)
{
	unsigned char * b = new unsigned char[16];
	for (int i = 0; i < 16; i++)
	{
		b[i] = pisp[a[i]];
	}
	memcpy(a, b, 16);
	delete(b);
};

void piswitch_plus(unsigned char * a)
{
	unsigned char * b = new unsigned char[16];
	memset(b, 0, 16);
	for (int i = 0; i <= 7; i++)
	{
		for (int j = 0; j <= 7; j++)
		{
			bitcopy_plus(a + j, b + 2 * i, 7 - i, 7 - j);
		}
	};
	for (int i = 0; i <= 7; i++)
	{
		for (int j = 0; j <= 7; j++)
		{
			bitcopy_plus(a + j + 8, b + 2 * i + 1, 7 - i, 7 - j);
		}
	};
	memcpy(a, b, 16);
	delete(b);
}

void spn_plus(unsigned char * a, unsigned char * key)
{
	unsigned char * b = new unsigned char[16];
	memcpy(b, a, 16);
	for (int i = 0; i <= 7; i++)
	{
		xor_plus(b, key);
		pishalf_plus(b);
		piswitch_plus(b);
	}
	xor_plus(b, key);
	pishalf_plus(b);
	xor_plus(b,key);
	memcpy(a, b, 16);
	delete(b);
}



void unpishalf(unsigned short *a)
{
	*a = (unpis[(*a >> 12)] << 12) + (unpis[(*a >> 8) & 0x000f] << 8) + (unpis[(*a >> 4) & 0x000f] << 4) + unpis[*a & 0x000f];
}
void piswitch(unsigned short *a)
{
	pishalf(a);
	unsigned short b;
	for (int i = 0; i <= 15; i++)
	{
		bitcopy(a, &b, 15 - i, 15 - pip[i]);
	};
	*a = b;
}
void unpiswitch(unsigned short *a)
{
	unsigned short b;
	for (int i = 0; i <= 15; i++)
	{
		bitcopy(a, &b, 15 - i, 15 - pip[i]);
	};
	*a = b;
	unpishalf(a);
}
unsigned short spn(unsigned short x)
{
	unsigned short x1 = x;
	x1 ^= k1;
	piswitch(&x1);
	x1 ^= k2;
	piswitch(&x1);
	x1 ^= k3;
	piswitch(&x1);
	x1 ^= k4;
	pishalf(&x1);
	return x1 ^ k5;
}
unsigned short unspn(unsigned short x)
{
	unsigned short x1 = x;
	x1 ^= k5;
	unpishalf(&x1);
	x1 ^= k4;
	unpiswitch(&x1);
	x1 ^= k3;
	unpiswitch(&x1);
	x1 ^= k2;
	unpiswitch(&x1);
	return x1 ^ k1;
}
unsigned short lineratk()
{
	unsigned short y, v2, v4, u2, u4;
	unsigned short tL1 = 0, tL2 = 0, z = 0;
	unsigned short xmax = 65535;
	for (unsigned short x = 1; x <= 40000; x++)
	{
		y = spn(x);
		for (unsigned short L1 = 0; L1 <= 15; L1++)
		{
			for (unsigned short L2 = 0; L2 <= 15; L2++)
			{
				v2 = L1 ^ (0xf & (y >> 8));
				v4 = L2 ^ (y & 0xf);
				u2 = unpis[v2];
				u4 = unpis[v4];
				z = getbit(x, 11) ^ getbit(x, 9) ^ getbit(x, 8) ^ getbit(u2, 0) ^ getbit(u2, 2) ^ getbit(u4, 0) ^ getbit(u4, 2);
				if (z == 0) c[L1][L2]++;
			}
		}
	}
	int max = -1;
	for (unsigned short L1 = 0; L1 <= 15; L1++)
	{
		for (unsigned short L2 = 0; L2 <= 15; L2++)
		{
			c[L1][L2] = abs(c[L1][L2] - 20000);
			if (c[L1][L2] > max)
			{
				max = c[L1][L2];
				tL1 = L1; tL2 = L2;
			};
		}
	}
	return (tL1 << 8) + tL2;
}
unsigned int difatk()
{
	for (int i = 0; i <= 15; i++)
	{
		for (int j = 0; j <= 15; j++)
		{
			c[i][j] = 0;
		}
	}
	unsigned int y1 = 0, y2 = 0;
	int i = 0;
	for (unsigned short x1 = 0; i <= 0xffff; x1++, i++)
	{
		unsigned short x2 = x1 ^ 0x0b00;
		y1 = spn(x1);
		y2 = spn(x2);
		if (((y1^y2) & 0xf0f0) != 0) continue;
		else for (unsigned short L1 = 0; L1 <= 15; L1++)
		{
			for (unsigned short L2 = 0; L2 <= 15; L2++)
			{
				unsigned short v2 = L1 ^ ((y1 >> 8) & 0xf);
				unsigned short v4 = L2 ^ (y1 & 0xf);
				unsigned short u2 = unpis[v2];
				unsigned short u4 = unpis[v4];
				unsigned short v22 = L1 ^ ((y2 >> 8) & 0xf);
				unsigned short v42 = L2 ^ (y2 & 0xf);
				unsigned short u22 = unpis[v22];
				unsigned short u42 = unpis[v42];
				if (((u2 ^ u22) == 6) && ((u4 ^ u42) == 6))
				{
					c[L1][L2]++;
				}
			}
		}
	}
	int max = -1;
	unsigned short tL1, tL2;
	for (unsigned short L1 = 0; L1 <= 15; L1++)
	{
		for (unsigned short L2 = 0; L2 <= 15; L2++)
		{
			if (c[L1][L2] > max)
			{
				max = c[L1][L2];
				tL1 = L1;
				tL2 = L2;
			}
		}
	}
	return ((tL1 << 8) + tL2);
};
unsigned int forceatk(unsigned short key)
{
	unsigned short uk1, uk2, uk3, uk4, uk5;
	int i = 0;

	for (int k1 = 0; k1 <= 15; k1++)
	{
		for (int k2 = 0; k2 <= 15; k2++)
		{
			for (int k3 = 0; k3 <= 15; k3++)
			{
				for (int k4 = 0; k4 <= 15; k4++)
				{
					for (int k5 = 0; k5 <= 15; k5++)
					{
						for (int k6 = 0; k6 <= 15; k6++)
						{
							int countnum[] = { 0,0,0 };
							uk1 = (k1 << 12) + (k2 << 8) + (k3 << 4) + k4;
							uk2 = (k2 << 12) + (k3 << 8) + (k4 << 4) + k5;
							uk3 = (k3 << 12) + (k4 << 8) + (k5 << 4) + (key >> 8);
							uk4 = (k4 << 12) + (k5 << 8) + ((key >> 8) << 4) + k6;
							uk5 = (k5 << 12) + key + (k6 << 4);
							//#pragma omp for
							for (int x_1 = 0; x_1 <= 2; x_1++)
							{
								unsigned short x1 = xtest[x_1];
								x1 ^= uk1;
								piswitch(&x1);
								x1 ^= uk2;
								piswitch(&x1);
								x1 ^= uk3;
								piswitch(&x1);
								x1 ^= uk4;
								pishalf(&x1);
								if ((x1 ^ uk5) != ytest[x_1]) break;
								else countnum[x_1]++;
							}
							if (countnum[0] && countnum[1] && countnum[2]) return ((k1 << 28) + (k2 << 24) + (k3 << 20) + (k4 << 16) + (k5 << 12) + (k6 << 4) + key);
						}
					}
				}
			}
		}
	}
}
int main()
{
	//unsigned short x = 0x3131;
	//unsigned short y = spn(x);
	//x = unspn(y);

	unsigned char msg[16] = { 0 };
	FILE*  fp;
	fopen_s(&fp, "D:\\spnp.txt", "wb");
	unsigned char kp0[16] = { 0 };
	unsigned char * p = msg;
	for (int i = 0; i < 640000; i++)
	{
		spn_plus(p,kp0);
		fwrite(p, 16 * sizeof(unsigned char), 1, fp);
	};
	fclose(fp);
	//unsigned short L = difatk();
	//unsigned int i = forceatk(L);
	return 0;
}
