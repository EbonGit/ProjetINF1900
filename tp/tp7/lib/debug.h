
//#define DEBUG // à mettre en commentaire ou non au besoin

#ifdef DEBUG

# define DEBUG_PRINT(x)  printRS232(x)// ou par RS-232

#else 

#define DEBUG_PRINT(x) do {} while (0) // code mort

#endif

void printRS232(int);

void printRS232(char[]);