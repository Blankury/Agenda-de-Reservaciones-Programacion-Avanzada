#include<Windows.h>
#include <CommCtrl.h>
#include "Resource.h"
#include <string>
#include<time.h>
#include<fstream>

using namespace std;

fstream archivo;
fstream archivo00;

#define ID_Timer 90000
time_t strctfech;
struct tm *hoy;

struct Registro {
	string nombre;
	string telefono;
	string costo;
	string habitacion;
	string fechai2;
	string fechat2;
	string fechaaux;
	Registro *siguiente;
	Registro *anterior;
}*inicio, *actual, *fin;

struct Fecha {
	string Dia;
	string Mes;
	string Año;
}tim;

string Factual;

#pragma region callbacks y funciones
BOOL CALLBACK Admin(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK Perfil(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK Admin_login(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK Ventana_De_Mensajes(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK Reservar(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK Modificar(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK Eliminar(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK Buscar(HWND, UINT, WPARAM, LPARAM);
void Guardar(Registro *inicio);
void Cargar_Archivo();
#pragma endregion

string nombre_admin, contraseña, cedula;

#pragma region VARIABLES GLOBALES
HINSTANCE hGlobalInst;

bool isClosed = false, guardado = false, listo = false;
HWND cbCombo1; 

char foto[MAX_PATH], szFileName[MAX_PATH];
int NumClientes = 0, contador = 0;
#pragma endregion


int WINAPI WinMain(HINSTANCE instancia, HINSTANCE hprev, LPSTR comandos, int cShow) {

	hGlobalInst = instancia;

	inicio = actual = fin = NULL;

	Cargar_Archivo();

	if (guardado == false) {
		HWND Administrador = CreateDialog(instancia, MAKEINTRESOURCE(AdminVentana), NULL, Admin);
		ShowWindow(Administrador, cShow);
	}
	else {
		HWND Administrador = CreateDialog(instancia, MAKEINTRESOURCE(AdminVentana_login), NULL, Admin_login);
		ShowWindow(Administrador, cShow);
	}


	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

}

BOOL CALLBACK Perfil(HWND handler, UINT msg, WPARAM wParam, LPARAM lParam) {
	bool correcto = false;
	switch (msg) {
	case WM_INITDIALOG: {
		HBITMAP hDirImagen;
		hDirImagen = (HBITMAP)LoadImage(NULL, foto, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
		HWND hPictureControl = GetDlgItem(handler, Picture);
		SendMessage(hPictureControl, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hDirImagen);

		SendDlgItemMessage(handler, IDC_EDIT1, WM_SETTEXT, sizeof(nombre_admin.c_str()), (LPARAM)nombre_admin.c_str());
		SendDlgItemMessage(handler, IDC_EDIT3, WM_SETTEXT, sizeof(cedula.c_str()), (LPARAM)cedula.c_str());
	}
	case WM_COMMAND: {
		if (LOWORD(wParam) == IDC_BUTTON2 && HIWORD(wParam) == BN_CLICKED) {
			OPENFILENAME Img;
			ZeroMemory(&Img, sizeof(OPENFILENAME));
			Img.hwndOwner = handler;
			Img.nMaxFile = MAX_PATH;
			Img.lpstrDefExt = "bmp";
			Img.lStructSize = sizeof(OPENFILENAME);
			Img.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR;
			Img.lpstrFile = foto;
			Img.lpstrFilter = "Selecciona una imagen en formato bmp\0*.bmp";
			if (GetOpenFileName(&Img) == TRUE) {
				HBITMAP hDirImagen;
				hDirImagen = (HBITMAP)LoadImage(NULL, foto, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				HWND hPictureControl = GetDlgItem(handler, Picture);
				SendMessage(hPictureControl, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hDirImagen);
			}
		}
		else if (LOWORD(wParam) == IDC_BUTTON1 && HIWORD(wParam) == BN_CLICKED) {
			HWND hnombre = GetDlgItem(handler, IDC_EDIT1);
			HWND hcedula = GetDlgItem(handler, IDC_EDIT3);
			int lnombre = GetWindowTextLength(hnombre);
			int lcedula = GetWindowTextLength(hcedula);
			char Temp_N[MAX_PATH];
			char Temp_Cedula[MAX_PATH];
			if (lnombre < 1 || lcedula < 1) {
				correcto = false;
			}
			else {
				correcto = true;
			}
			GetWindowText(hnombre, Temp_N, ++lnombre);
			GetWindowText(hcedula, Temp_Cedula, ++lcedula);

			if (correcto == true) {
				int i;
				for (i = 0; i < lcedula - 1; i++) {
					if (!(isalpha(Temp_Cedula[i]) == 0) && i <= 1) {
						correcto = true;

					}
					else {
						if ((i > 1) && (Temp_Cedula[i] >= 48 && Temp_Cedula[i] <= 57)) {
							correcto = true;
						}
						else {
							correcto = false;
							break;
						}
					}
				}
				if (i == 5) correcto = true;
				else correcto = false;
			}
			if (correcto == true) {
				for (int i = 0; i < lnombre - 1; i++) {
					if (isalpha(Temp_N[i]) == 0) { correcto = false;}
					else correcto = true;
				}
			}
			if (correcto != true) {
				MessageBox(handler, "Se ha producido un error que impide continuar. Revise los siguientes puntos: \n\nHay numeros en el en el nombre. \nHay espacios vacíos. \nLa cedula debe tener el formato LETRA LETRA NUMERO NUMERO NUMERO.\nLa cedula debe tener 5 de longitud.", "Error", MB_OK | MB_ICONERROR);
			}
			else {
				nombre_admin = Temp_N, cedula = Temp_Cedula;
				HWND Mi_ventana = CreateDialog(hGlobalInst, MAKEINTRESOURCE(VENTANA1), NULL, Ventana_De_Mensajes);
				ShowWindow(Mi_ventana, SW_SHOW);
				DestroyWindow(handler);
			}
		}
		break;
	}
	default:
		DefWindowProc(handler, msg, wParam, lParam);
	}
	return FALSE;
}

BOOL CALLBACK Admin_login(HWND handler, UINT msg, WPARAM wParam, LPARAM lParam) {	
	switch (msg) {
	case WM_INITDIALOG: {
		HBITMAP hDirImagen;
		hDirImagen = (HBITMAP)LoadImage(NULL, foto, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
		HWND hPictureControl = GetDlgItem(handler, Picture);
		SendMessage(hPictureControl, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hDirImagen);
		break;
	}
	case WM_COMMAND:
		if (LOWORD(wParam) == IDC_BUTTON1 && HIWORD(wParam) == BN_CLICKED) {
			string comparar;
			bool correcto = false;
			HWND hcontraseña = GetDlgItem(handler, IDC_EDIT1);
			int lcontraseña = GetWindowTextLength(hcontraseña);
			char Temp_Cont[MAX_PATH];
			if (lcontraseña < 1) {
				correcto = false;
				break;
			}
			else {
				correcto = true;
			}
			GetWindowText(hcontraseña, Temp_Cont, ++lcontraseña);

			comparar = Temp_Cont;
			if (correcto == true) {
				if (comparar.compare(contraseña) != 0) {
					correcto = false;
					MessageBox(handler, "Contraseña incorrecta", "ERROR", MB_OK | MB_ICONEXCLAMATION);
					break;
				}
				else {
					correcto = true;
					HWND Mi_ventana = CreateDialog(hGlobalInst, MAKEINTRESOURCE(VENTANA1), NULL, Ventana_De_Mensajes);
					ShowWindow(Mi_ventana, SW_SHOW);
					DestroyWindow(handler);
				}
			}
		}
		break;

	default:
		DefWindowProc(handler, msg, wParam, lParam);
	}

	return FALSE;
}

BOOL CALLBACK Admin(HWND handler, UINT msg, WPARAM wParam, LPARAM lParam) {
	bool correcto = false;
	
	switch (msg) {
	case WM_COMMAND:
		if (LOWORD(wParam) == IDC_BUTTON1 && HIWORD(wParam) == BN_CLICKED) {
			HWND hnombre = GetDlgItem(handler, IDC_EDIT1);
			HWND hcontraseña = GetDlgItem(handler, IDC_EDIT2);
			HWND hcedula = GetDlgItem(handler, IDC_EDIT3);
			int lnombre = GetWindowTextLength(hnombre);
			int lcontraseña = GetWindowTextLength(hcontraseña);
			int lcedula = GetWindowTextLength(hcedula);
			char Temp_N[MAX_PATH];
			char Temp_Cont[MAX_PATH];
			char Temp_Cedula[MAX_PATH];
			if (lnombre < 1 || lcontraseña < 1 || lcedula < 1) {
				correcto = false;
			}
			else {
				correcto = true;
			}
			GetWindowText(hnombre, Temp_N, ++lnombre);
			GetWindowText(hcontraseña, Temp_Cont, ++lcontraseña);
			GetWindowText(hcedula, Temp_Cedula, ++lcedula);

			if (correcto == true) {
				int i;
				for (i = 0; i < lcedula - 1; i++) {
					if (!(isalpha(Temp_Cedula[i]) == 0) && i <= 1) {
							correcto = true;
						
					}
					else {
						if ((i > 1) && (Temp_Cedula[i] >=48 && Temp_Cedula[i] <= 57)) {
							correcto = true;
						}
						else { correcto = false;
							break; }
					}
				}
				if (i == 5) correcto = true;
				else correcto = false;
			}
			if (correcto == true) {
				for (int i = 0; i < lnombre - 1; i++) {
					if (isalpha(Temp_N[i]) == 0) {
						correcto = false;
					}
					else
						correcto = true;
				}
			}
			if (correcto == true) {
				nombre_admin = Temp_N;
				contraseña = Temp_Cont;
				cedula = Temp_Cedula;
			}
			
			if (correcto != true) {
				MessageBox(handler, "Se ha producido un error que impide continuar. Revise los siguientes puntos: \n\nHay numeros en el en el nombre. \nHay espacios vacíos. \nLa cedula debe tener el formato LETRA LETRA NUMERO NUMERO NUMERO.\nLa cedula debe tener 5 de longitud.", "Error", MB_OK | MB_ICONERROR);
			}
			else {
				HWND Mi_ventana = CreateDialog(hGlobalInst, MAKEINTRESOURCE(VENTANA1), NULL, Ventana_De_Mensajes);
				ShowWindow(Mi_ventana, SW_SHOW);
				DestroyWindow(handler);
			}
		}
		else if (LOWORD(wParam) == IDC_BUTTON2 && HIWORD(wParam) == BN_CLICKED) {
			OPENFILENAME Img;
			ZeroMemory(&Img, sizeof(OPENFILENAME));
			Img.hwndOwner = handler;
			Img.nMaxFile = MAX_PATH;
			Img.lpstrDefExt = "bmp";
			Img.lStructSize = sizeof(OPENFILENAME);
			Img.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR;
			Img.lpstrFile = foto;
			Img.lpstrFilter = "Selecciona una imagen en formato bmp\0*.bmp";
			if (GetOpenFileName(&Img) == TRUE) {
				HBITMAP hDirImagen;
				hDirImagen = (HBITMAP)LoadImage(NULL,foto, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				HWND hPictureControl = GetDlgItem(handler, Picture);
				SendMessage(hPictureControl, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hDirImagen);
			}
		}

		break;
	case WM_CLOSE:
		break;
	default:
		DefWindowProc(handler, msg, wParam, lParam);
	}

	return FALSE;
}

BOOL CALLBACK Ventana_De_Mensajes(HWND handler, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_INITDIALOG: {
		HWND hLista = GetDlgItem(handler, IDC_LIST1);
		SendMessage(hLista, LB_RESETCONTENT, NULL, NULL);
		for (int i = 0; i < NumClientes;i++) {
				SendMessage(hLista, LB_ADDSTRING, NULL, (LPARAM)actual->fechai2.c_str());
				SendMessage(hLista, LB_ADDSTRING, NULL, (LPARAM)actual->fechat2.c_str());
				SendMessage(hLista, LB_ADDSTRING, NULL , (LPARAM)actual->nombre.c_str());
				SendMessage(hLista, LB_ADDSTRING, NULL, (LPARAM)actual->telefono.c_str());
				SendMessage(hLista, LB_ADDSTRING, NULL, (LPARAM)actual->habitacion.c_str());
				SendMessage(hLista, LB_ADDSTRING, NULL, (LPARAM)actual->costo.c_str());
				SendMessage(hLista, LB_ADDSTRING, NULL, (LPARAM)"");
				actual = actual->siguiente;
			}
			actual = inicio;
			break;
		}
		case WM_COMMAND:
			if (LOWORD(wParam) == IDC_BUTTON8 && HIWORD(wParam) == BN_CLICKED) {
				HWND Mi_Ventana2 = CreateDialog(hGlobalInst, MAKEINTRESOURCE(VENTANA2), NULL, Reservar);
				ShowWindow(Mi_Ventana2, SW_SHOW);
				SetTimer(Mi_Ventana2, ID_Timer, 1000, (TIMERPROC)NULL);
				DestroyWindow(handler);
			}
			else if (LOWORD(wParam) == IDC_BUTTON10 && HIWORD(wParam) == BN_CLICKED) {
				if (inicio == NULL) {
					MessageBox(handler, "No hay reservaciones", "Error", MB_OK | MB_ICONERROR);
					break;
				}
				else {
					HWND Mi_Ventana4 = CreateDialog(hGlobalInst, MAKEINTRESOURCE(VENTANA5), NULL, Buscar);
					ShowWindow(Mi_Ventana4, SW_SHOW);
					DestroyWindow(handler);
				}
			}
			else if (LOWORD(wParam) == IDC_BUTTON11 && HIWORD(wParam) == BN_CLICKED) {
				if (inicio == NULL) {
					MessageBox(handler, "No hay reservaciones", "Error", MB_OK | MB_ICONERROR);
					break;
				}
				else {
					HWND Mi_Ventana5 = CreateDialog(hGlobalInst, MAKEINTRESOURCE(VENTANA5), NULL, Eliminar);
					ShowWindow(Mi_Ventana5, SW_SHOW);
					DestroyWindow(handler);
				}
			}
			else if (LOWORD(wParam) == ID_PERFIL && HIWORD(wParam) == BN_CLICKED) {
				HWND Mi_Ventana1 = CreateDialog(hGlobalInst, MAKEINTRESOURCE(Ventana_Perfil), NULL, Perfil);
				ShowWindow(Mi_Ventana1, SW_SHOW);
				DestroyWindow(handler);
			}
			else if (LOWORD(wParam) == ID_SALIR && HIWORD(wParam) == BN_CLICKED) {
				Guardar(inicio);
				isClosed = true;
				DestroyWindow(handler);
			}
			else if (LOWORD(wParam) == CSV && HIWORD(wParam) == BN_CLICKED) {
				OPENFILENAME csv;
				ZeroMemory(&csv, sizeof(OPENFILENAME));
				csv.hwndOwner = handler;
				csv.nMaxFile = MAX_PATH;
				csv.lpstrDefExt = "CSV";
				csv.lpstrFile = szFileName;
				csv.lStructSize = sizeof(OPENFILENAME);
				csv.Flags =  OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;
				csv.lpstrFilter = "All Files\0*.*\0\0";
				ofstream CSVA;
				if (GetSaveFileName(&csv) == TRUE) {
					actual = inicio;
					string nombrefinal = szFileName;
					CSVA.open(nombrefinal);
					CSVA << "Fecha de inicio" << "," << "Fecha final de reserva" << "," << "NOMBRE" << "," << "TELEFONO" << "," << "TAMAÑO DE HABITACIÓN"  << "," << "COSTO"<< endl;
					for (int i = 0; i < NumClientes; i++) {
						CSVA <<actual->fechai2<< "," << actual->fechat2 << "," << actual->nombre<< "," << actual->telefono<< "," << actual->habitacion << "," << "$" << actual->costo<< endl;
						actual = actual->siguiente;
					}
					CSVA.close();
				}
			};
			break;
		case WM_CLOSE:
			break;
		case WM_DESTROY:
			if (isClosed)
				PostQuitMessage(712);
			break;
		default:
			DefWindowProc(handler, msg, wParam, lParam);
	}
	return FALSE;
}

BOOL CALLBACK Reservar(HWND handler, UINT msg, WPARAM wParam, LPARAM lParam) {
	bool correcto = false;
#pragma region PROCESO
	HWND hDTP11 = GetDlgItem(handler, IDC_DATETIMEPICKER1);
	HWND hDTP21 = GetDlgItem(handler, IDC_DATETIMEPICKER2);
	HWND hprecio1 = GetDlgItem(handler, PRECIOH);
	int lDTP11 = GetWindowTextLength(hDTP11);
	int lDTP21 = GetWindowTextLength(hDTP21);
	int lprecio1 = GetWindowTextLength(hprecio1);
	char Temp_DTP11[MAX_PATH];
	char Temp_DTP21[MAX_PATH];
	char Temp_precio1[MAX_PATH];
	GetWindowText(hDTP11, Temp_DTP11, ++lDTP11);
	GetWindowText(hDTP21, Temp_DTP21, ++lDTP21);
	GetWindowText(hprecio1, Temp_precio1, ++lprecio1);
	int dia1 = 50;
	int cantdias1 = atoi(Temp_DTP21) - atoi(Temp_DTP11);
	int preciopordias1 = cantdias1 * dia1;
#pragma endregion

	switch (msg) {
	case WM_INITDIALOG: {
		cbCombo1 = GetDlgItem(handler, IDC_COMBO1);
		SendMessage(cbCombo1, CB_ADDSTRING, 0, (LPARAM) "Selecciona el tamaño");
		SendMessage(cbCombo1, CB_ADDSTRING, 0, (LPARAM) "SENCILLA");
		SendMessage(cbCombo1, CB_ADDSTRING, 0, (LPARAM) "DOBLE");
		SendMessage(cbCombo1, CB_ADDSTRING, 0, (LPARAM) "FAMILIAR");
		SendMessage(cbCombo1, CB_SETCURSEL, 0, 0);
		break;
	}
	case WM_TIMER: {
		if (listo == false) {
			time(&strctfech);
			hoy = localtime(&strctfech);
			char dia[5], mes[5], año[5];
			_itoa(hoy->tm_mday, dia, 10);
			_itoa(++hoy->tm_mon, mes, 10);
			_itoa((hoy->tm_year + 1900), año, 10);
			tim.Dia = dia, tim.Mes = mes, tim.Año = año;
			if (tim.Dia.length() < 2) {
				tim.Dia.clear();
				tim.Dia.append("0");
				tim.Dia.append(dia);
			}
			if (tim.Mes.length() < 2) {
				tim.Mes.clear();
				tim.Mes.append("0");
				tim.Mes.append(mes);
			}

			Factual.append(tim.Año);
			Factual.append(tim.Mes);
			Factual.append(tim.Dia);
			listo = true;
		}
		break;
	}
	case WM_COMMAND: {
		if (LOWORD(wParam) == IDC_BUTTON1 && HIWORD(wParam) == BN_CLICKED) {
			HWND hnombre = GetDlgItem(handler, IDC_EDIT1);
			HWND htelefono = GetDlgItem(handler, IDC_EDIT2);
			HWND hhab = GetDlgItem(handler, IDC_COMBO1);
			HWND hDTP1 = GetDlgItem(handler, IDC_DATETIMEPICKER1);
			HWND hDTP2 = GetDlgItem(handler, IDC_DATETIMEPICKER2);
			HWND hprecio = GetDlgItem(handler, PRECIOH);
			int lnombre = GetWindowTextLength(hnombre);
			int ltelefono = GetWindowTextLength(htelefono);
			int lhab = GetWindowTextLength(hhab);
			int lDTP1 = GetWindowTextLength(hDTP1);
			int lDTP2 = GetWindowTextLength(hDTP2);
			int lprecio = GetWindowTextLength(hprecio);
			char Temp_N[MAX_PATH];
			char Temp_Tel[MAX_PATH];
			char Temp_hab[MAX_PATH];
			char Temp_DTP1[MAX_PATH];
			char Temp_DTP2[MAX_PATH];
			char Temp_precio[MAX_PATH];
			if (lnombre < 1 || ltelefono < 1 || lhab < 1 || lDTP1 < 1 || lDTP2 < 1  || lhab < 1) {
				correcto = false;
			}
			else {
				correcto = true;
			}
			GetWindowText(hnombre, Temp_N, ++lnombre);
			GetWindowText(htelefono, Temp_Tel, ++ltelefono);
			GetWindowText(hDTP1, Temp_DTP1, ++lDTP1);
			GetWindowText(hDTP2, Temp_DTP2, ++lDTP2);
			GetWindowText(hprecio, Temp_precio, ++lprecio);
			GetWindowText(hhab, Temp_hab, ++lhab);
			_strupr(Temp_N);
			string aux1 = Temp_DTP1, aux2 = "";
			char aux3[8] = "0";

			for (int i = 0; aux1[i]; i++) {
				if (aux1.c_str()[i] != '/') {
					aux2 += aux1[i];
				}
			}

			aux3[0] = aux2[4], aux3[1] = aux2[5], aux3[2] = aux2[6], aux3[3] = aux2[7], aux3[4] = aux2[2], aux3[5] = aux2[3], aux3[6] = aux2[0], aux3[7] = aux2[1];

			string aux4 = Temp_DTP2, aux5 = "";
			char aux6[8] = "0";

			for (int i = 0; aux4[i]; i++) {
				if ((aux1.c_str()[i] != '/')) {
					aux5 += aux4[i];
				}
			}
			aux6[0] = aux5[4], aux6[1] = aux5[5], aux6[2] = aux5[6], aux6[3] = aux5[7], aux6[4] = aux5[2], aux6[5] = aux5[3], aux6[6] = aux5[0], aux6[7] = aux5[1];

			int fecha1 = atoi(aux3), fecha2 = atoi(aux6);

			if (fecha2 < fecha1 ||  fecha1 == fecha2) {
				correcto = false;
				MessageBox(handler, "No puedes poner una fecha de fin antes o igual que la de inicio", "Error", MB_OK | MB_ICONWARNING);
				break;
			}
			else if (fecha1 < atoi(Factual.c_str()) || fecha2 < atoi(Factual.c_str())) {
				correcto = false;
				MessageBox(handler, "No puedes poner una fecha anterior al día de hoy.", "Error", MB_OK | MB_ICONWARNING);
				break;
			}
			
			int dia = 50;
			int cantdias = fecha2 - fecha1;
			int preciopordias = cantdias * dia;
			int preciofinal = preciopordias * 1 * 100;
			_itoa(preciofinal, Temp_precio, 10);

			if (correcto == true) {
				for (int i = 0; i < lnombre - 1; i++) {
					if (isalpha(Temp_N[i]) == 0 && (Temp_N[i + 1] == 32)) {
						correcto = false;
						break;
					}
					else
						correcto = true;
				}
			}
			if (correcto == true) {
				int i = 0;
				for (i = 0; i < ltelefono - 1; i++) {
					if (!isalpha(Temp_Tel[i]) == 0) {
						correcto = false;
						break;
					}
					else
						correcto = true;
				}
				if (i == 10) {
					correcto = true;
				}
				else
					correcto = false;
			}
			int combo = SendMessage(cbCombo1, CB_GETCURSEL, 0, 0);
			if (combo == 0) {
				MessageBox(NULL, "Seleccione el tamaño de la habitación", "Error", MB_OK|MB_ICONWARNING);
				break;
			}
			else {char textoEdit[50];	int length = GetWindowTextLength(cbCombo1);	GetWindowText(cbCombo1, textoEdit, length);}

			if (correcto == true) {
				if (inicio == NULL) {
					inicio = new Registro;
					inicio->siguiente = inicio;
					inicio->anterior = inicio;
					inicio->nombre = Temp_N;
					inicio->telefono = Temp_Tel;
					inicio->fechaaux = aux3; inicio->fechaaux = inicio->fechaaux.substr(0, 8);
					inicio->costo = Temp_precio;
					inicio->habitacion = Temp_hab;
					inicio->fechai2 = Temp_DTP1;
					inicio->fechat2 = Temp_DTP2;
					actual = fin = inicio;
					NumClientes++;
				}
				else {
					while (actual->siguiente != inicio != NULL) {
						actual = actual->siguiente;
					}
					actual->siguiente = NULL;
					actual->siguiente = new Registro;
					actual->siguiente->anterior = actual;
					actual = actual->siguiente;
					actual->siguiente = inicio;
					inicio->anterior = actual;
					actual->nombre = Temp_N;
					actual->telefono = Temp_Tel;
					actual->fechaaux = aux3; actual->fechaaux = actual->fechaaux.substr(0, 8);
					actual->costo = Temp_precio;
					actual->habitacion = Temp_hab;
					actual->fechai2 = Temp_DTP1;
					actual->fechat2 = Temp_DTP2;
					fin = actual;
					actual = inicio;
					NumClientes++;
				}
			}
			
			if (correcto != true) {
				MessageBox(handler, " Se ha producido un error que impide continuar.Revise los siguientes puntos : \n\nHay letras en el teléfono o números en el nombre. \n Revisa que no hayan quedado datos vacíos.\nEl teléfono diebe tener 10 números.","Error", MB_OK|MB_ICONERROR);
			}
			else {
				if (NumClientes > 1) {
					for (int i = 1; i < NumClientes; i++) {
						for (int j = 1; j < NumClientes; j++) {
							if (actual->fechaaux > actual->siguiente->fechaaux) {
								swap(actual->nombre, actual->siguiente->nombre);
								swap(actual->telefono, actual->siguiente->telefono);
								swap(actual->habitacion, actual->siguiente->habitacion);
								swap(actual->fechaaux, actual->siguiente->fechaaux);
								swap(actual->costo, actual->siguiente->costo);
								swap(actual->fechai2, actual->siguiente->fechai2);
								swap(actual->fechat2, actual->siguiente->fechat2);
							}
							actual = actual->siguiente;
						}
						actual = inicio;
					}
					actual = inicio;
				}
				HWND Mi_ventana = CreateDialog(hGlobalInst, MAKEINTRESOURCE(VENTANA1), NULL, Ventana_De_Mensajes);
				ShowWindow(Mi_ventana, SW_SHOW);
				DestroyWindow(handler);
			}
		}
		else if (LOWORD(wParam) == IDC_BUTTON3 && HIWORD(wParam) == BN_CLICKED) {
			HWND H = GetDlgItem(handler, IDC_COMBO1);
			int iCuarto = GetWindowTextLength(H);
			char cCuarto[MAX_PATH];
			GetWindowText(H, cCuarto, ++iCuarto);
			HWND texto = GetDlgItem(handler, PRECIOH);
			string Cuarto = cCuarto;
			if (Cuarto.compare("SENCILLA") == 0) {
				int preciofinal1 = preciopordias1 * 1 * 100;
				_itoa(preciofinal1, Temp_precio1, 10);
				SetWindowText(texto, Temp_precio1);
			}
			else if (Cuarto.compare("DOBLE") == 0) {
				int preciofinal1 = preciopordias1 * 2 * 100;
				_itoa(preciofinal1, Temp_precio1, 10);
				SetWindowText(texto, Temp_precio1);
			}
			else if (Cuarto.compare("FAMILIAR") == 0) {
				int preciofinal1 = preciopordias1 * 4 * 100;
				_itoa(preciofinal1, Temp_precio1, 10);
				SetWindowText(texto, Temp_precio1);
			}
		}
		break; 
	}
	case WM_CLOSE:
		break;
	default:
		DefWindowProc(handler, msg, wParam, lParam);
	}
	return FALSE;
}

BOOL CALLBACK Modificar(HWND handler, UINT msg, WPARAM wParam, LPARAM lParam) {
	bool correcto = false, encontrado = true;
#pragma region PROCESO
	HWND hDTP11 = GetDlgItem(handler, IDC_DATETIMEPICKER1);
	HWND hDTP21 = GetDlgItem(handler, IDC_DATETIMEPICKER2);
	HWND hprecio1 = GetDlgItem(handler, PRECIOH);
	int lDTP11 = GetWindowTextLength(hDTP11);
	int lDTP21 = GetWindowTextLength(hDTP21);
	int lprecio1 = GetWindowTextLength(hprecio1);
	char Temp_DTP11[MAX_PATH];
	char Temp_DTP21[MAX_PATH];
	char Temp_precio1[MAX_PATH];
	GetWindowText(hDTP11, Temp_DTP11, ++lDTP11);
	GetWindowText(hDTP21, Temp_DTP21, ++lDTP21);
	GetWindowText(hprecio1, Temp_precio1, ++lprecio1);
	int dia1 = 50;
	int cantdias1 = atoi(Temp_DTP21) - atoi(Temp_DTP11);
	int preciopordias1 = cantdias1 * dia1;
#pragma endregion

	switch(msg)
	{
	case WM_INITDIALOG:
		for (int i = 0; i < contador; i++ )
			actual = actual->siguiente;

		cbCombo1 = GetDlgItem(handler, IDC_COMBO1);
		SendMessage(cbCombo1, CB_ADDSTRING, 0, (LPARAM) "SENCILLA");
		SendMessage(cbCombo1, CB_ADDSTRING, 0, (LPARAM) "DOBLE");
		SendMessage(cbCombo1, CB_ADDSTRING, 0, (LPARAM) "FAMILIAR");
		SendMessage(cbCombo1, CB_SETCURSEL, 0, 0);

		SendDlgItemMessage(handler, IDC_EDIT1, WM_SETTEXT, sizeof(actual->nombre.c_str()), (LPARAM)actual->nombre.c_str());
		SendDlgItemMessage(handler, IDC_EDIT2, WM_SETTEXT, sizeof(actual->telefono.c_str()), (LPARAM)actual->telefono.c_str());
		SendDlgItemMessage(handler, PRECIOH, WM_SETTEXT, sizeof(actual->costo.c_str()), (LPARAM)actual->costo.c_str());
		SendDlgItemMessage(handler, IDC_COMBO1, WM_SETTEXT, sizeof(actual->habitacion.c_str()), (LPARAM)actual->habitacion.c_str());
		SendDlgItemMessage(handler, f1, WM_SETTEXT, sizeof(actual->fechai2.c_str()), (LPARAM)actual->fechai2.c_str());
		SendDlgItemMessage(handler, f2, WM_SETTEXT, sizeof(actual->fechat2.c_str()), (LPARAM)actual->fechat2.c_str());
		break;
	case WM_COMMAND: {
		if (LOWORD(wParam) == IDC_BUTTON1 && HIWORD(wParam) == BN_CLICKED) {
			for (int i = 0; i < contador; i++)
				actual = actual->siguiente;
			HWND hnombre = GetDlgItem(handler, IDC_EDIT1);
			HWND htelefono = GetDlgItem(handler, IDC_EDIT2);
			HWND hhab = GetDlgItem(handler, IDC_COMBO1);
			HWND hDTP1 = GetDlgItem(handler, IDC_DATETIMEPICKER1);
			HWND hDTP2 = GetDlgItem(handler, IDC_DATETIMEPICKER2);
			HWND hprecio = GetDlgItem(handler, PRECIOH);
			int lnombre = GetWindowTextLength(hnombre);
			int ltelefono = GetWindowTextLength(htelefono);
			int lhab = GetWindowTextLength(hhab);
			int lDTP1 = GetWindowTextLength(hDTP1);
			int lDTP2 = GetWindowTextLength(hDTP2);
			int lprecio = GetWindowTextLength(hprecio);
			char Temp_N[MAX_PATH];
			char Temp_Tel[MAX_PATH];
			char Temp_hab[MAX_PATH];
			char Temp_DTP1[MAX_PATH];
			char Temp_DTP2[MAX_PATH];
			char Temp_precio[MAX_PATH];
			if (lnombre < 1 || ltelefono < 1 || lhab < 1 || lDTP1 < 1 || lDTP2 < 1 || lhab < 1) {
				correcto = false;
			}
			else {
				correcto = true;
			}
			GetWindowText(hnombre, Temp_N, ++lnombre);
			GetWindowText(htelefono, Temp_Tel, ++ltelefono);
			GetWindowText(hDTP1, Temp_DTP1, ++lDTP1);
			GetWindowText(hDTP2, Temp_DTP2, ++lDTP2);
			GetWindowText(hprecio, Temp_precio, ++lprecio);
			GetWindowText(hhab, Temp_hab, ++lhab);
			_strupr(Temp_N);
			string aux1 = Temp_DTP1, aux2 = "";
			char aux3[8] = "0";

			for (int i = 0; aux1[i]; i++) {
				if ((aux1.c_str()[i] != '/')) {
					aux2 += aux1[i];
				}
			}
			aux3[0] = aux2[4], aux3[1] = aux2[5], aux3[2] = aux2[6], aux3[3] = aux2[7], aux3[4] = aux2[2], aux3[5] = aux2[3], aux3[6] = aux2[0], aux3[7] = aux2[1];

			string aux4 = Temp_DTP2, aux5 = "";
			char aux6[8] = "0";

			for (int i = 0; aux4[i]; i++) {
				if ((aux1.c_str()[i] != '/')) {
					aux5 += aux4[i];
				}
			}
			aux6[0] = aux5[4], aux6[1] = aux5[5], aux6[2] = aux5[6], aux6[3] = aux5[7], aux6[4] = aux5[2], aux6[5] = aux5[3], aux6[6] = aux5[0], aux6[7] = aux5[1];

			int fecha1 = atoi(aux3), fecha2 = atoi(aux6);

			if (fecha2 < fecha1 || fecha1 == fecha2) {
				correcto = false;
				MessageBox(handler, "No puedes poner una fecha de fin antes o igual que la de inicio", "Error", MB_OK | MB_ICONWARNING);
				break;
			}
			else if (fecha1 < atoi(Factual.c_str()) || fecha2 < atoi(Factual.c_str())) {
				correcto = false;
				MessageBox(handler, "No puedes poner una fecha anterior al día de hoy.", "Error", MB_OK | MB_ICONWARNING);
				break;
			}
		
			int dia = 50;
			int cantdias = fecha2 - fecha1;
			int preciopordias = cantdias * dia;
			int preciofinal = preciopordias * 1 * 100;
			_itoa(preciofinal, Temp_precio, 10);

			if (correcto == true) {
				for (int i = 0; i < lnombre - 1; i++) {
					if (isalpha(Temp_N[i]) == 0 && (Temp_N[i + 1] == 32)) {
						correcto = false;
						break;
					}
					else
						correcto = true;
				}
			}
			if (correcto == true) {
				int i = 0;
				for (i = 0; i < ltelefono - 1; i++) {
					if (!isalpha(Temp_Tel[i]) == 0) {
						correcto = false;
						break;
					}
					else
						correcto = true;
				}
				if (i == 10) {
					correcto = true;
				}
				else
					correcto = false;
			}
			if (correcto == true) {
				actual->nombre = Temp_N;
				actual->telefono = Temp_Tel;
				actual->fechaaux = aux3; actual->fechaaux = actual->fechaaux.substr(0, 8);
				actual->costo = Temp_precio;
				actual->habitacion = Temp_hab;
				actual->fechai2 = Temp_DTP1;
				actual->fechat2 = Temp_DTP2;
				actual = inicio;
			}

			if (correcto != true) {
				MessageBox(handler, " Se ha producido un error que impide continuar.Revise los siguientes puntos : \n\nHay letras en el teléfono o números en el nombre. \n Revisa que no hayan quedado datos vacíos.\nEl teléfono diebe tener 10 números.", "Error", MB_OK | MB_ICONERROR);
			}
			else {
				if (NumClientes > 1) {
					for (int i = 1; i < NumClientes; i++) {
						for (int j = 1; j < NumClientes; j++) {
							if (actual->fechaaux > actual->siguiente->fechaaux) {
								swap(actual->nombre, actual->siguiente->nombre);
								swap(actual->telefono, actual->siguiente->telefono);
								swap(actual->habitacion, actual->siguiente->habitacion);
								swap(actual->fechaaux, actual->siguiente->fechaaux);
								swap(actual->costo, actual->siguiente->costo);
							}
							actual = actual->siguiente;
						}
						actual = inicio;
					}
					actual = inicio;
				}
				HWND Mi_ventana = CreateDialog(hGlobalInst, MAKEINTRESOURCE(VENTANA1), NULL, Ventana_De_Mensajes);
				ShowWindow(Mi_ventana, SW_SHOW);
				DestroyWindow(handler);
			}
		}
		else if (LOWORD(wParam) == IDC_BUTTON3 && HIWORD(wParam) == BN_CLICKED) {
			HWND H = GetDlgItem(handler, IDC_COMBO1);
			int iCuarto = GetWindowTextLength(H);
			char cCuarto[MAX_PATH];
			GetWindowText(H, cCuarto, ++iCuarto);
			HWND texto = GetDlgItem(handler, PRECIOH);
			string Cuarto = cCuarto;
			if (Cuarto.compare("SENCILLA") == 0) {
				int preciofinal1 = preciopordias1 * 1 * 100;
				_itoa(preciofinal1, Temp_precio1, 10);
				SetWindowText(texto, Temp_precio1);
			}
			else if (Cuarto.compare("DOBLE") == 0) {
				int preciofinal1 = preciopordias1 * 2 * 100;
				_itoa(preciofinal1, Temp_precio1, 10);
				SetWindowText(texto, Temp_precio1);
			}
			else if (Cuarto.compare("FAMILIAR") == 0) {
				int preciofinal1 = preciopordias1 * 4 * 100;
				_itoa(preciofinal1, Temp_precio1, 10);
				SetWindowText(texto, Temp_precio1);
			}
		}
		break;
	}
	default:
		DefWindowProc(handler, msg, wParam, lParam);
	}
return FALSE;
}

BOOL CALLBACK Buscar(HWND handler, UINT msg, WPARAM wParam, LPARAM lParam) {
	bool correcto = false, encontrado = true;
	contador = 0;

	switch (msg) {
	case WM_COMMAND:
		if (LOWORD(wParam) == IDC_BUTTON1 && HIWORD(wParam) == BN_CLICKED) {
			HWND hnombre = GetDlgItem(handler, IDC_EDIT1);
			int lnombre = GetWindowTextLength(hnombre);
			char Temp_N[MAX_PATH];
			if (lnombre < 1) {
				correcto = false;
			}
			else {
				correcto = true;
			}
			GetWindowText(hnombre, Temp_N, ++lnombre);

			_strupr(Temp_N);

			string Temp_Ns = Temp_N;
			while (Temp_Ns.compare(actual->nombre) != 0) {
				if (actual->siguiente == inicio) {
					encontrado = false;
					break;
				}
				actual = actual->siguiente;
				contador++;
			}
			if (encontrado == false) {
				MessageBox(handler, "No se encontró la reservación", "Ocurrió un problema", MB_OK);
				break;
			}

			if (Factual.c_str() > actual->fechaaux.c_str()) {
				correcto = false;
				MessageBox(handler, "Esta reservación está en curso o ya finalizó.", "Error", MB_OK | MB_ICONWARNING);
				break;
			}
			
			DestroyWindow(handler);
			HWND Mi_ventana4 = CreateDialog(hGlobalInst, MAKEINTRESOURCE(VENTANA4), NULL, Modificar);
			ShowWindow(Mi_ventana4, SW_SHOW);
		}
		if (LOWORD(wParam) == IDC_BUTTON2 && HIWORD(wParam) == BN_CLICKED) {
			DestroyWindow(handler);
			HWND Mi_ventana = CreateDialog(hGlobalInst, MAKEINTRESOURCE(VENTANA1), NULL, Ventana_De_Mensajes);
			ShowWindow(Mi_ventana, SW_SHOW);
		}
		break;
	default:
		DefWindowProc(handler, msg, wParam, lParam);
	}
	return FALSE;
}

BOOL CALLBACK Eliminar(HWND handler, UINT msg, WPARAM wParam, LPARAM lParam) {
	bool correcto = false, encontrado = true;
	switch (msg) {
	case WM_COMMAND:
		if (LOWORD(wParam) == IDC_BUTTON1 && HIWORD(wParam) == BN_CLICKED) {
			HWND hnombre = GetDlgItem(handler, IDC_EDIT1);
			int lnombre = GetWindowTextLength(hnombre);
			char Temp_N[MAX_PATH];
			if (lnombre < 1) {
				correcto = false;
			}
			else {
				correcto = true;
			}
			GetWindowText(hnombre, Temp_N, ++lnombre);
			
			string Temp_Ns = Temp_N;

			while (Temp_Ns.compare(actual->nombre) != 0) {
				if (actual->siguiente == inicio) {
					encontrado = false;
					break;
				}
				actual = actual->siguiente;
			}
			if (encontrado == false) {
				MessageBox(handler, "No se encontró la reservación", "Ocurrió un problema", MB_OK);
				actual = inicio;
				break;
			}
			
			if (Factual>actual->fechaaux) {
				MessageBox(handler, "Esta reservación está en curso o ya pasó.", "", MB_OK);
				actual = inicio;
				break;
			}
			else {
				if (actual->siguiente == inicio && actual->anterior == inicio && fin == inicio) {
					delete actual;
					inicio = actual = fin = NULL;
					NumClientes--;
				}
				else if (actual->anterior == fin) {
					inicio = actual->siguiente;
					inicio->anterior = fin;
					inicio->anterior->siguiente = inicio;
					delete actual;
					actual = inicio;
					NumClientes--;
				}
				else if (actual->siguiente == inicio) {
					fin = actual->anterior;
					actual->anterior->siguiente = inicio;
					inicio->anterior = actual->anterior;
					delete actual;
					actual = inicio;
					NumClientes--;
				}
				else {
					actual->anterior->siguiente = actual->siguiente;
					actual->siguiente->anterior = actual->anterior;
					delete actual;
					actual = inicio;
					NumClientes--;
				}
				MessageBox(handler, "La baja se realizó con éxito", "", MB_OK);
				DestroyWindow(handler);
				HWND Mi_ventana = CreateDialog(hGlobalInst, MAKEINTRESOURCE(VENTANA1), NULL, Ventana_De_Mensajes);
				ShowWindow(Mi_ventana, SW_SHOW);
			}
		}
		if (LOWORD(wParam) == IDC_BUTTON2 && HIWORD(wParam) == BN_CLICKED) {
			DestroyWindow(handler);
			HWND Mi_ventana = CreateDialog(hGlobalInst, MAKEINTRESOURCE(VENTANA1), NULL, Ventana_De_Mensajes);
			ShowWindow(Mi_ventana, SW_SHOW);
		}
		break;
	default:
		DefWindowProc(handler, msg, wParam, lParam);
	}
	return FALSE;
}

void Cargar_Archivo() {
	archivo.open("Reservaciones.bin", ios::binary | ios::in | ios::ate);
	if (archivo.is_open()) {
		int TotalSize = archivo.tellg();
		if (TotalSize < 1) {
			archivo.close();
			guardado = false;
			return;
		}
		for (int i = 0; i < TotalSize / sizeof(Registro); i++) {
			guardado = true;
			archivo.seekg(i * sizeof(Registro));
			if (inicio == NULL) {
				inicio = new Registro;
				inicio->siguiente = inicio;
				inicio->anterior = inicio;
				Registro *temp = new Registro;
				archivo.read(reinterpret_cast<char*>(temp), sizeof(Registro));
				inicio->nombre = temp->nombre;
				inicio->telefono = temp->telefono;
				inicio->fechaaux = temp->fechaaux;
				inicio->costo = temp->costo;
				inicio->habitacion = temp->habitacion;
				inicio->fechai2 = temp->fechai2;
				inicio->fechat2 = temp->fechat2;
				actual = fin = inicio;
				NumClientes++;
				delete reinterpret_cast<char*>(temp);		
			}
			else {
				while (actual->siguiente != inicio)
					actual = actual->siguiente;
				Registro *temp = new Registro;
				actual->siguiente = new Registro;
				archivo.read(reinterpret_cast<char*>(temp), sizeof(Registro));
				actual->siguiente->anterior = actual;
				actual = actual->siguiente;
				actual->siguiente = inicio;
				inicio->anterior = actual;
				actual->nombre = temp->nombre;
				actual->telefono = temp->telefono;
				actual->fechaaux = temp->fechaaux;
				actual->costo = temp->costo;
				actual->habitacion = temp->habitacion;
				actual->fechai2 = temp->fechai2;
				actual->fechat2 = temp->fechat2;
				NumClientes++;
				fin = actual;
				actual = inicio;
				delete reinterpret_cast<char*>(temp);
			}
		}
		archivo.close();
	}
	string linea;
	int j = 0;

	archivo00.open("Datos.txt", ios::in);
	if (archivo00.is_open()) {
		while (!archivo00.eof()) {
			getline(archivo00, linea);
			switch (j) {
			case 0: nombre_admin = linea; break;
			case 1: contraseña = linea; break;
			case 2: cedula = linea; break;
			case 3:	strcpy_s(foto, linea.c_str());
			}
			j++;
		}
	}
	archivo00.close();

	return;
}
void Guardar(Registro *inicio) {
	archivo.open("Reservaciones.bin", ios::binary | ios::out | ios::trunc);
	if (archivo.is_open()) {
		for (int i = 0; i < NumClientes; i++) {
			archivo.write(reinterpret_cast<char*>(inicio), sizeof(Registro));
			Registro* temp = inicio;
			temp = inicio;
			inicio = inicio->siguiente;
			delete temp;
		}
	}
	archivo.close();

	archivo00.open("Datos.txt", ios::out);

	if (archivo00.is_open()) {
		archivo00 << nombre_admin<<endl;
		archivo00 << contraseña<<endl;
		archivo00 << cedula<<endl;
		archivo00 << foto << endl;
	}

	archivo00.close();
	
	return;
}