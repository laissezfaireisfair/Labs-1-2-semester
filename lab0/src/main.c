#include <stdio.h>

#define SYMBOLS_MAX 50
#define IN_BUFER_MAX (SYMBOLS_MAX-2) /// минус два так как запятая и ноль в конце
#define TRUEC 1
#define FALSEC 0
#define MAX_SYSTEM 16
#define MIN_SYSTEM 2

	///функция переворачивающая первые n элементов массива
	void revers(int a[],int n)
	{
		int i;
		for(i=0; i<(n/2); i++)
		{
			a[i] ^= a[n-i-1];
			a[n-i-1] ^= a[i];
			a[i] ^= a[n-i-1];
		}
	}

	///функция вычисляющая a в степени b
	double stepen(int a, int b)
	{
		int i;
		double output = 1;
		if(b>=0)
			for(i=1;i<=b;i++)
			 	output *= a;
		else
			for(i=-1;i>=b;i--)
			 	output /= a;
		return output;
	}

	///функция перевода в десятичную, возвращает число в десятичной
	double to10sys(int *Input,int sys1,int n, int m)
	{
		double prom10 = 0;
		int i;
		for (i = 0; i < n+m; i++)
		{
			prom10 += (*(Input+i))*stepen(sys1,(n-i-1));
		}
		return prom10;
	}

	///функция перевода целого числа в нужную систему счисления, возвращает количество символов в новой системе
	int toOtherSys(long long int prom10,int sys2,int *Out)
	{
		int counter = 0;
		do
		{
			*Out = prom10%sys2;
			Out++;
			prom10 /= sys2;
			counter++;
		}
		while(prom10>=sys2);
		if(prom10!=0)
		{
			*Out = prom10;
			counter++;
		}
		return counter;
	}

	///Функция для перевода дробной части из 10 в любую систему счисления
	int toOtherSysDrob(double prom10, int sys2, int *Out)
	{
		int i,  celoe = 0, symbols = 0;
		double promnow = prom10;
		for(i=0;i<12 && promnow != 0;i++)
		{
			promnow = promnow*sys2;
			celoe = promnow;
			Out[i] = celoe;
			promnow -= celoe;
			symbols++;
		}
		return symbols;
	}


	///функция для перевода из массива интов в массив чаров (цифры до 15)
	void intToChar(int *in, int n, int m, char *out, int *flag)
	{
		int i;
		for (i=0;i<n && *flag == TRUEC;i++)
			{
				if(in[i] >= 0 && in[i] <= 9)
					out[i] = in[i] + '0';
				else
					if(in[i] >= 10 && in[i] <= 35)
						out[i] = in[i]-10+'a';
					else
						*flag = FALSEC;
			}
		if(*flag == TRUEC && m != 0)
		{
			out[n] = '.';
			for (i=n;i<n+m && *flag == TRUEC;i++)
				{
					if(in[i] >= 0 && in[i] <= 9)
						out[i+1] = in[i] + '0'; /// здесь и далее в функции i+1 так как сдвиг не 1 из-за точки занимающей место в массиве
					else
						if(in[i] >= 10 && in[i] <= 35)
							out[i+1] = in[i]-10+'a';
						else
							*flag = FALSEC;
				}
		}
	}

	///функция для преобразования char[] -> int[]
	int charToInt(char *in, int *out, int num, int *flag, int *n, int *m)
	{
		int *drobnost, sdvigOut = 0, maxSys = 1, i, charAfterPoint = 0;
		drobnost = n;
		for(i=0;i<num && *flag==TRUEC && charAfterPoint < 6;i++)
		{
			if(in[i] == '.' || in[i] == ',')
			{
				drobnost = m;
				if(sdvigOut == 0)
					sdvigOut++;
				else
					*flag = FALSEC;
			}
			else
			   {
			    	if(in[i] >= '0' && in[i] <= '9')
			    	{
			    		out[i-sdvigOut] = in[i] - '0';
			    		*drobnost = *drobnost + 1;
			    		if(drobnost == m)
			    		    charAfterPoint++;
			    		if(out[i-sdvigOut]>maxSys)
			    			maxSys = out[i];
			    	}
			    	else
			    	{
			    		if(in[i] >= 'a' && in[i] <= 'z')
			    		{
			    			out[i-sdvigOut] = in[i] - 'a' + 10;
			    			*drobnost = *drobnost + 1;
			    			if(out[i-sdvigOut]>maxSys)
			    				maxSys = out[i];
			    		    if(drobnost == m)
			    		        charAfterPoint++;
			    		}
			    		else
			    			*flag = FALSEC;
			    	}
			   }
		}
		if(drobnost == m && *m == 0)
		    *flag = FALSEC;
		if(drobnost == m && *n == 0)
		    *flag = FALSEC;
		return maxSys+1;
	}

	///функция для подсчётa количества символов в строке
	int counter(char *In1)
	{
		int n1 = 0, i = 0;
		do{
		n1++;
		i++;
		}while(*(In1+i) != 0);
		return n1;
	}
int main()
{
	///инициализация переменных
	char inAndOut[SYMBOLS_MAX];
	int maxSys, i, n = 0, m = 0, flag = TRUEC, num1, sys1, sys2, chislo[IN_BUFER_MAX];
	double promIn10;
			long long int prom10Cel;

	///открытие файлов
	FILE *fin;
	FILE *fout;
	fin = fopen("in.txt","r");
	fout = fopen("out.txt","w");

	///получение входных данных
	fscanf(fin,"%d %d", &sys1, &sys2);
	if(sys1>MAX_SYSTEM || sys1<MIN_SYSTEM || sys2>MAX_SYSTEM ||sys2<MIN_SYSTEM)
	{
		flag = FALSEC;
		fprintf(fout, "bad input");
	     	return 1;
	}
	fscanf(fin, "%s", inAndOut);


	///преобразование в массив целых
	num1 = counter(inAndOut);
	maxSys = charToInt(inAndOut, chislo, num1, &flag, &n, &m);
	if(maxSys>sys1)
	{
		flag == FALSEC;
		fprintf(fout, "bad input");
		return 2;
	}
	///получение промежуточного числа в десятичной системе счисления
	promIn10 = to10sys(chislo, sys1, n, m);

	if(sys2 != 10)
	{
		///получение числа в нужной системе
		prom10Cel = promIn10;
		promIn10 -= prom10Cel;
		n = toOtherSys(prom10Cel, sys2, &chislo[0]);
		revers(chislo, n);
		m = toOtherSysDrob(promIn10, sys2, &chislo[0]+n);

		///вывод результата
		intToChar(chislo, n, m, inAndOut, &flag);
		if(flag == FALSEC)
		{
			fprintf(fout, "bad input");
			return 3;
		}
		if(m!=0)
			for(i=0;i<n+m+1;i++)
				fprintf(fout,"%c",inAndOut[i]);
		else
		{
			for(i=0;i<n;i++)
				fprintf(fout,"%c",inAndOut[i]);
				fprintf(fout,".000000000000");
		}
	}
	else
	{
		fprintf(fout, "%f", promIn10);
	}
	///закрытие файлов
	fclose(fin);
	fclose(fout);
	return 0;
}
