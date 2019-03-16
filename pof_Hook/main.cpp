#include "hook.h"
#include <stdio.h>

using namespace std;

/* 
   Dependiendo de la configuración del UNICODE coge W o A.
   Para esta prueba de concepto lo he desabilitado (A).
*/
int main()
{
	string titulo = "Informacion";
	string body = "Hola Mundo";

	Hook * hook = new Hook();
	
	MessageBox(NULL, body.c_str() , titulo.c_str(), MB_ICONASTERISK);
	hook->Enable();
	MessageBox(NULL, body.c_str() , titulo.c_str(), MB_ICONASTERISK);
	hook->Disable();
	MessageBox(NULL, body.c_str() , titulo.c_str(), MB_ICONASTERISK);
	hook->Enable();
	MessageBox(NULL, body.c_str(), titulo.c_str(), MB_ICONASTERISK);
	hook->~Hook();
}