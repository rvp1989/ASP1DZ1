#include <stdio.h>
#include <stdlib.h>

typedef struct Elem {
	int row;
	int col;
	int val;
	struct Elem* down;
	struct Elem* right;
} Elem;

/*
	Ova funkcija se poziva kada god je alokacija prostora neuspesna
*/
void greskaAlokacijaProstora() {
	system("cls");
	printf("NEMA DOVOLJNO MEMORIJE. GRESKA U ALOKACIJI PROSTORA.\n");
	exit(1);
}

/*
	Ova funkcija se poziva prilikom incijalizacije matrice i prilikom postavljanja elementa matrice
	Promenljiva jePodrazumevana govori da li se u matricu postavlja podrazumevana vrednost na poziciju i,j.
	Ako je to slucaj potrebno je da se izbaci odgovarajuci element matrice na poziciji i,j jer se podrazumevani elementi ne pamte u matrici.
*/
void postaviElement(Elem** r, Elem **c, int i, int j, int vrednost, int jePodrazumevana, int* brojNepodrazumevanih) {
	Elem* tekuciVrsta, *prethodniVrsta, *tekuciKolona, *prethodniKolona, *novi, *zaBrisanje;

	// PROMENLJIVE KOJE SLUZE ZA PROLAZAK KROZ MATRICU DOK SE NE NADJE POZICIJA ZA ELEMENT KOJI SE UBACUJE
	prethodniVrsta = NULL;
	prethodniKolona = NULL;
	tekuciVrsta = r[i];
	tekuciKolona = c[j];

	// PRI UBACIVANJU ELEMENTA U MATRICU MORA SE NACI NJEGOVA POZICIJA U VRSTI I KOLONI RETKE MATRICE
	while ((tekuciVrsta != NULL && tekuciVrsta->col < j) || (tekuciKolona != NULL && tekuciKolona->row < i)) {
		if (tekuciVrsta != NULL && tekuciVrsta->col < j) {
			prethodniVrsta = tekuciVrsta;
			tekuciVrsta = tekuciVrsta->right;
		}

		if (tekuciKolona != NULL && tekuciKolona->row < i) {
			prethodniKolona = tekuciKolona;
			tekuciKolona = tekuciKolona->down;
		}
	}

	// UKOLIKO ELEMENT NA POZICIJI i,j NE POSTOJI U RETKOJ MATRICI
	if (tekuciVrsta == NULL || tekuciKolona == NULL || (tekuciVrsta->col != j && tekuciKolona->row != i)) {
		// AKO SE UBACUJE PODRAZUMEVANI ELEMENT NE TREBA NISTA URADITI JER SE ONI NE PAMTE U RETKOJ MATRICI
		if (jePodrazumevana) return;

		// AKO SE NE UBACUJE PODRAZUMEVANI ELEMENT
		novi = malloc(sizeof(Elem));

		if (novi == NULL) {
			greskaAlokacijaProstora();
		}

		novi->row = i;
		novi->col = j;
		novi->val = vrednost;

		if (prethodniVrsta == NULL) {
			r[i] = novi;
		}
		else {
			prethodniVrsta->right = novi;
		}

		novi->right = tekuciVrsta;

		if (prethodniKolona == NULL) {
			c[j] = novi;
		}
		else {
			prethodniKolona->down = novi;
		}

		novi->down = tekuciKolona;

		(*brojNepodrazumevanih)++;
	}
	else {
		// UKOLIKO ELEMENT NA POZICIJI i,j POSTOJI U MATRICI, A SADA SE UBACUJE PODRAZUMEVANA VREDNOST ONDA TREBA IZBACITI POSTOJECI ELEMENT IZ MATRICE
		if (jePodrazumevana) {
			if (prethodniVrsta == NULL) {
				r[i] = tekuciVrsta->right;
			}
			else {
				prethodniVrsta->right = tekuciVrsta->right;
			}

			if (prethodniKolona == NULL) {
				c[j] = tekuciKolona->down;
			}
			else {
				prethodniKolona->down = tekuciKolona->down;
			}

			zaBrisanje = tekuciVrsta;

			tekuciVrsta = tekuciVrsta->right;

			free(zaBrisanje);

			(*brojNepodrazumevanih)--;
		}
		else {
			// U SLUCAJU DA SE UBACUJE VREDNOST KOJA NIJE PODRAZUMEVANA SAMO PROMENITI VREDNOST POSTOJECEM ELEMENTU
			tekuciVrsta->val = vrednost;
		}
	}
}

/*
	u funkciju su poslate adrese od r, c, m, n, brojNepodrazumevanih zato sto se njihova vrednost menja u okviru funkcije, a
	promene se trebaju videti i u glavnom programu koji je pozvao ovu funkciju
*/

void formirajMatricu(Elem*** r, Elem*** c, int* m, int* n, int* brojNepodrazumevanih) {
	Elem* tekuciVrsta, *tekuciKolona, *prethodniKolona, *prethodniVrsta, *novi;
	int kraj = 1;
	int i, j;
	int postoji;
	int vrednost;

	system("cls");

	// UNOS BROJA VRSTA I KOLONA MATRICE
	do {
		printf("Unesite broj vrsta: ");
		scanf("%d", m);
		printf("Unesite broj kolona: ");
		scanf("%d", n);

		if (m <= 0 || n <= 0) {
			printf("Broj vrsta i broj kolona moraju biti pozitivni brojevi. Ponovite unos.\n\n");
			kraj = 0;
		}
	} while (!kraj);

	// ALOKACIJA NIZA VRSTA
	*r = calloc(m, sizeof(Elem*));

	if (r == NULL) {
		greskaAlokacijaProstora();
	}

	// ALOKACIJA NIZA KOLONA
	*c = calloc(n, sizeof(Elem*));

	if (c == NULL) {
		greskaAlokacijaProstora();
	}

	kraj = 0;

	do {
		// U PETLJI SE UNOSE ELEMENTI MATRICE SVE DOK SE ZA POZICIJU NE UNESE -1 -1
		printf("\n\nUnesite vrstu i kolonu elementa matrice koji zelite da inicijalizujete (-1 -1 za kraj unosa) : ");
-		scanf("%d %d", &i, &j);

		if (i == -1 && j == -1) {
			kraj = 1;
		}
		else if (i < 0 || i >= *m || j < 0 || j >= *n) {
			printf("\n Greska. Pozicija mora biti unutar opsega matrice. Ponovite unos.\n");
		}
		else{
			printf("Unesite vrednost elementa: ");
			scanf("%d", &vrednost);

			// POZIVA SE FUNKCIJA KOJA UBACUJE ELEMENT U MATRICU
			postaviElement(*r, *c, i, j, vrednost, 0, brojNepodrazumevanih);
		}
	} while (!kraj);
}

void unesiPodrazumevanuVrednost(Elem** r, Elem** c, int m, int n, int* podrazumevana, int* brojNepodrazumevanih) {
	Elem* tekuciVrsta, *prethodniVrsta, *tekuciKolona, *prethodniKolona, *zaBrisanje;
	int i, j;

	system("cls");

	printf("Unesite podrazumevanu vrednost matrice: ");
	scanf("%d", podrazumevana);

	// NAKON UNOSA PODRAZUMEVANE VREDNOSTI TREBA PROVERITI DA LI TA VREDNOST POSTOJI U OKVIRU MATRICE I AKO POSTOJE TAKVI ELEMENTI TREBA IH IZBACITI
	for (i = 0; i < m; i++) {
		prethodniVrsta = NULL;
		tekuciVrsta = r[i];

		for (j = 0; j < n; j++) {
			if (tekuciVrsta != NULL && j == tekuciVrsta->col) {
				prethodniKolona = NULL;
				tekuciKolona = c[j];

				if (tekuciVrsta->val == *podrazumevana) {
					(*brojNepodrazumevanih)--;

					while (tekuciKolona != tekuciVrsta) {
						prethodniKolona = tekuciKolona;
						tekuciKolona = tekuciKolona->down;
					}

					if (prethodniVrsta == NULL) {
						r[i] = tekuciVrsta->right;
					}
					else {
						prethodniVrsta->right = tekuciVrsta->right;
					}

					if (prethodniKolona == NULL) {
						c[j] = tekuciKolona->down;
					}
					else {
						prethodniKolona->down = tekuciKolona->down;
					}

					zaBrisanje = tekuciVrsta;

					tekuciVrsta = tekuciVrsta->right;

					free(zaBrisanje);
				}
				else {
					prethodniVrsta = tekuciVrsta;
					tekuciVrsta = tekuciVrsta->right;
				}
			}

		}
	}
}

void dohvatiElement(Elem** r, Elem** c, int m, int n, int podrazumevana) {
	Elem* tekuciVrsta, *tekuciKolona;
	int i, j;
	int kraj = 1;

	system("cls");

	// UNOS VRSTE I KOLONE SA KOJE SE ELEMENT DOHVATA
	do {
		printf("Unesite vrstu i kolonu elementa koji zelite da dohvatite: ");
		scanf("%d %d", &i, &j);

		if (i < 0 || i >= m || j < 0 || j >= n) {
			printf("\nGreska. Unesite poziciju u opsegu matrice\n");
			kraj = 0;
		}
	} while (!kraj);


	tekuciVrsta = r[i];
	tekuciKolona = c[j];

	// PRONALAZENJE ZAHTEVANOG ELEMENTA
	while (tekuciVrsta != NULL && tekuciKolona != NULL && tekuciVrsta->col < j && tekuciKolona->row < i) {
		tekuciVrsta = tekuciVrsta->right;
		tekuciKolona = tekuciKolona->down;
	}

	if (tekuciVrsta == NULL || tekuciKolona == NULL || ( tekuciVrsta->col != j && tekuciKolona->row != i ) ) {
		printf("\nVrednost elementa na poziciji %d, %d je : %d\n", i, j, podrazumevana);
	}
	else if(tekuciVrsta->col == j){
		printf("\nVrednost elementa na poziciji %d, %d je : %d\n", i, j, tekuciVrsta->val);
	}
	else {
		printf("\nVrednost elementa na poziciji %d, %d je : %d\n", i, j, tekuciKolona->val);
	}
}

void postaviVrednostElementu(Elem** r, Elem** c, int m, int n, int podrazumevana, int* brojNepodrazumevanih) {
	Elem* tekuciVrsta, *tekuciKolona;
	int i, j;
	int vrednost;
	int jePodrazumevana;
	int kraj = 1;

	system("cls");

	// UNOS VRSTE I KOLONE ELEMENTA KOME TREBA POSTAVITI VREDNOST
	do {
		printf("Unesite vrstu i kolonu elementa kome zelite da postavite vrednost: ");
		scanf("%d %d", &i, &j);

		if (i < 0 || i >= m || j < 0 || j >= n) {
			printf("\nGreska. Unesite poziciju u opsegu matrice\n");
			kraj = 0;
		}
	} while (!kraj);

	printf("Unesite vrednost elementa: ");
	scanf("%d", &vrednost);

	// ISPITIVANJE DA LI SE RADI O PODRAZUMEVANOJ VREDNOSTI
	if (vrednost == podrazumevana) {
		jePodrazumevana = 1;
	}
	else {
		jePodrazumevana = 0;
	}

	postaviElement(r, c, i, j, vrednost, jePodrazumevana, brojNepodrazumevanih);
}

/*
	Ova funkcija vrsi dealokaciju prostora potrebnog za skladistenje matrice
*/
void brisanjeMatrice(Elem*** r, Elem*** c, int m) {
	Elem* tekuci, *zaBrisanje;
	int i;
	for (i = 0; i < m; i++) {
		tekuci = (*r)[i];

		while (tekuci != NULL) {
			zaBrisanje = tekuci;
			tekuci = tekuci->right;
			free(zaBrisanje);
		}
	}

	free(*r);
	free(*c);

	*r = NULL;
	*c = NULL;
}

void ispisMatrice(Elem** r, Elem** c, int m, int n, int podrazumevana) {
	int i, j;
	Elem* tekuci;

	system("cls");

	printf("MATRICA: \n\n");

	for (i = 0; i < m; i++) {
		tekuci = r[i];

		// AKO JE ELEMENT NA POZICIJI i,j u RETKOJ MATRICI ISPISUJE SE, U SUPROTNOM SE ISPISUJE PODRAZUMEVANA VREDNOST
		for (j = 0; j < n; j++) {
			if(tekuci == NULL || j < tekuci->col){
				printf(" %d ", podrazumevana);
			} else if (j == tekuci->col) {
				printf(" %d ", tekuci->val);
				tekuci = tekuci->right;
			}
		}

		printf("\n\n");
	}

	_sleep(10000);
}

/*
	Ova funkcija formira transponovanu matricu i pamti njene vrste i kolone u nizovima pokazivaca tr i tc, cija je adresa poslata,
	kako bi transponovana matrica postojala i pri povratku u glavni program.
*/
void formirajTransponovanuMatricu(Elem** r, Elem** c, int m, int n, Elem*** tr, Elem*** tc) {
	// Nizovi pokazivaca poslednjiVrste i poslednjiKolone sadrze pokazivace na poslednji ubaceni element u svakoj od vrsta i svakoj od kolona, kako se na njih mogu nadovezati elementi pri kopiranju iz postojece matrice
	Elem** poslednjiVrste, ** poslednjiKolone;
	Elem* tekuci, *novi;
	int i;

	*tr = calloc(n, sizeof(Elem*));

	if (*tr == NULL) {
		greskaAlokacijaProstora();
	}

	*tc = calloc(m, sizeof(Elem*));

	if (*tr == NULL) {
		greskaAlokacijaProstora();
	}

	poslednjiVrste = calloc(n, sizeof(Elem*));

	if (poslednjiVrste == NULL) {
		greskaAlokacijaProstora();
	}

	poslednjiKolone = calloc(m, sizeof(Elem*));

	if (poslednjiKolone == NULL) {
		greskaAlokacijaProstora();
	}

	for (i = 0; i < m; i++) {
		tekuci = r[i];

		while (tekuci != NULL) {
			novi = malloc(sizeof(Elem));

			if (novi == NULL) {
				greskaAlokacijaProstora();
			}

			// KOPIRANJE ELEMENTA POSTOJECE MATRICE
			novi->row = tekuci->col;
			novi->col = tekuci->row;
			novi->val = tekuci->val;
			novi->right = NULL;
			novi->down = NULL;

			// UBACIVANJE ELEMENTA U NOVOFORMIRANU TRANSPONOVANU MATRICU
			if (poslednjiVrste[novi->row] == NULL) {
				(*tr)[novi->row] = novi;
			}
			else {
				poslednjiVrste[novi->row]->right = novi;
			}

			poslednjiVrste[novi->row] = novi;

			if (poslednjiKolone[novi->col] == NULL) {
				(*tc)[novi->col] = novi;
			}
			else {
				poslednjiKolone[novi->col]->down = novi;
			}

			poslednjiKolone[novi->col] = novi;

			tekuci = tekuci->right;
		}
	}

	// OSLOBADJANJE PROSTORA ZA PRIVREMENE POKAZIVACE JER JE MATRICA KOPIRANA TE VISE NISU POTREBNI
	free(poslednjiVrste);
	free(poslednjiKolone);
}

void main() {
	// r - vrste matrice, c - kolone matrice, tr - vrste transponovane matrice, tc - kolone transponovane matrice
	Elem** r, **c, **tr, **tc;
	// m - broj vrsta matrice, n - broj kolona matrice
	int m, n;
	// Odabirom opcije 0 iz menija program se zavrsava i kraj dobija vrednost 1, program se radi u do-while petlji sve dok je vrednost promenljive kraj 0
	int kraj = 0;
	// Izabrana opcija iz menija
	int opcija;
	// Ukoliko se izabere neka opcija iz menija koja nije ponudjena, korektanUnos promenljiva ce imati vrednost 0 i meni ce se ponovo ispisati sve dok korisnik korektno ne odabere opciju
	int korektanUnos;
	// Podrazumevana vrednost matrice
	int podrazumevana;
	// Promenljive formiranaMatrica i unetaPodrazumevana su logickog tipa i sluze da se zabrane ostale akcije dok ove dve promenljive nemaju vrednost 1, tj. dok se ne unese matrica i njena podrazumevana vrednost (opcije 1 i 2)
	int formiranaMatrica = 0, unetaPodrazumevana = 0;
	// Stalno vodimo evidenciju o broju nepodrazumevanih vrednosti u matrici kako ne bismo morali da prodjemo kroz citavu matricu kada korisnik zatrazi dohvatanje broja nepodrazumevanih vrednosti
	int brojNepodrazumevanih = 0;

	do {
		// brisanje konzole
		system("cls");

		printf("==============================MENI=============================\n\n");

		printf("1) Formiranje matrice\n");
		printf("2) Postavljanje podrazumevanog elementa\n");
		printf("3) Dohvatanje elementa\n");
		printf("4) Postavljanje vrednosti elementu\n");
		printf("5) Dohvatanje broja nepodrazumevanih elemenata\n");
		printf("6) Ispis matrice\n");
		printf("7) Brisanje matrice\n\n");
		printf("8) Transponovanje matrice\n\n");
		printf("0) Prekid programa\n");

		do {
			korektanUnos = 1;

			printf("\nUnesite opciju: ");
			scanf("%d", &opcija);

			switch (opcija) {
			case 0:
				kraj = 1;
				break;
			case 1:
				if (formiranaMatrica) {
					brisanjeMatrice(&r, &c, m);
					formiranaMatrica = 0;
					unetaPodrazumevana = 0;
					brojNepodrazumevanih = 0;
					m = 0;
					n = 0;
				}

				formirajMatricu(&r, &c, &m, &n, &brojNepodrazumevanih);
				formiranaMatrica = 1;
				break;
			case 2:
				if (formiranaMatrica) {
					unesiPodrazumevanuVrednost(r, c, m, n, &podrazumevana, &brojNepodrazumevanih);
					unetaPodrazumevana = 1;
				}
				else {
					printf("Morate formirati matricu pre koriscenja ove funkcije!\n");
					_sleep(2000);
				}
				break;
			case 3:
				if (formiranaMatrica && unetaPodrazumevana) {
					dohvatiElement(r, c, m, n, podrazumevana);
				}
				else {
					printf("Morate formirati matricu i uneti njenu podrazumevanu vrednost pre koriscenja ove funkcije!\n");
					_sleep(2000);
				}
				_sleep(4000);
				break;
			case 4:
				if (formiranaMatrica && unetaPodrazumevana) {
					postaviVrednostElementu(r, c, m, n, podrazumevana, &brojNepodrazumevanih);
				}
				else {
					printf("Morate formirati matricu i uneti njenu podrazumevanu vrednost pre koriscenja ove funkcije!\n");
					_sleep(2000);
				}
				break;
			case 5:
				if (formiranaMatrica && unetaPodrazumevana) {
					system("cls");
					printf("Broj nepodrazumevanih elemenata je: %d\n", brojNepodrazumevanih);
					_sleep(3000);
				}
				else {
					printf("Morate formirati matricu i uneti njenu podrazumevanu vrednost pre koriscenja ove funkcije!\n");
					_sleep(2000);
				}
				break;
			case 6:
				if (formiranaMatrica && unetaPodrazumevana) {
					ispisMatrice(r, c, m, n, podrazumevana);
				}
				else {
					printf("Morate formirati matricu i uneti njenu podrazumevanu vrednost pre koriscenja ove funkcije!\n");
					_sleep(2000);
				}
				break;
			case 7:
				if (formiranaMatrica) {
					brisanjeMatrice(&r, &c, m);
					formiranaMatrica = 0;
					unetaPodrazumevana = 0;
					brojNepodrazumevanih = 0;
					m = 0;
					n = 0;
				}
				else {
					printf("Morate formirati matricu pre koriscenja ove funkcije!\n");
					_sleep(2000);
				}
				break;
			case 8:
				if (formiranaMatrica && unetaPodrazumevana) {
					formirajTransponovanuMatricu(r, c, m, n, &tr, &tc);
					ispisMatrice(tr, tc, n, m, podrazumevana);
					brisanjeMatrice(&tr, &tc, n);
				}
				else {
					printf("Morate formirati matricu i uneti njenu podrazumevanu vrednost pre koriscenja ove funkcije!\n");
					_sleep(2000);
				}
				break;
			default:
				korektanUnos = 0;
				break;
			}
		} while (!korektanUnos);

	} while (!kraj);

	if (formiranaMatrica) {
		brisanjeMatrice(&r, &c, m);
	}
}
