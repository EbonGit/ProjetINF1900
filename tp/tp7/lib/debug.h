/* Auteurs: Kephren Delannay-Sampany, Jad Ben Rabhi, Amine Ghabia, Amine Zerouali
 * Description: Si la règle debug est utilisé lors de la compilation alors DEBUG_PRINT(x) appel printRS232(x) sinon ne fait rien, code mort pas pris en compte par
 le compilateur.
 -> la règle debug appelle install, dans notre version il faut donc avoir le robot branché pour utiliser debug
 */

//#define DEBUG // à mettre en commentaire ou non au besoin

#ifdef DEBUG

# define DEBUG_PRINT(x)  printRS232(x)// ou par RS-232

#else 

#define DEBUG_PRINT(x) do {} while (0) // code mort

#endif

void printRS232(int);

void printRS232(char[]);