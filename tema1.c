/* FIDEL Andreea 325CB*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "info.h"

TLista AlocCelula(void *e, size_t d)
{
	TLista aux = (TLista)malloc(sizeof(Celula));
	if(!aux) return NULL;
	aux->info = malloc (sizeof(d));
	memcpy(aux->info, e, d);
	aux->urm = NULL;
	return aux;
}

TLista InitLDI()
{
	TLista aux = (TLista)malloc(sizeof(Celula));
	if(aux)
	{
		aux->urm = aux;
		aux->info = NULL;
	}
	return aux;
}

int InsJucator(ALista aL, Jucator ae, size_t d)
{
	TLista aux = (TLista)malloc(sizeof(Celula));
	if(!aux) return 0;
	aux->info = malloc(d);
	if(!aux->info)
	{
		free(aux);
		return 0;
	}
	memcpy(aux->info, ae, d);
	
	TLista p = (*aL)->urm, ultim = NULL;
	while (p != (*aL))
	{
		ultim = p;
		p = p->urm;
	}
	if(ultim) 
	{
		ultim->urm = aux;
		aux->urm = *aL;
	}
	else 
	{
		(*aL)->urm = aux;
		aux->urm = *aL;
	}
	
	return 1;
} 

int InsMasa(ALista aL, Masa ae, size_t d)
{
	TLista aux = (TLista)malloc(sizeof(Celula));
	if(!aux) return 0;
	aux->info = malloc(d);
	aux->urm = NULL;
	if(!aux->info)
	{
		free(aux);
		return 0;
	}
	memcpy(aux->info, ae, d);
	
	TLista p = *aL, ultim = NULL;
	while(p != NULL) 
	{
		ultim = p; 
		p = p->urm;	
	}
	if(ultim) ultim->urm = aux;
	else *aL = aux;
	return 1;
}

void AfisareJucatori (ALista jucatori, FILE * fout)
{
	TLista p = (*jucatori)->urm;
	if(!p) fprintf(fout, "nu sunt jucatori");
	while (p != (*jucatori))
	{
		fprintf(fout, "%s - %d", ((Jucator)(p->info))->nume, ((Jucator)(p->info))->nrMaini);
		p = p->urm;
		if (p == (*jucatori)) fprintf(fout, ".");
		else fprintf(fout, "; ");
	}
	fprintf(fout, "\n"); 
	
}

void Afisare (ALista masa, FILE * fout)
{
	TLista p = *masa;
	
	if(!p) fprintf(fout, "Sala este inchisa!");
	while(p != NULL)
	{
		
		fprintf(fout, "%s: ", ((Masa)(p->info))->numeMasa);
		AfisareJucatori (&(((Masa)(p->info))->jucatori), fout);
		p = p->urm;
	}
	
}


void Noroc(ALista masa, char* numemasa, char* numejucator, int grad_noroc, FILE *fout)
{
	TLista p = *masa;
	int OKj = 0;
	int OKm = 0; 
	while(p != NULL)
	{
		if (strcmp (((Masa)(p->info))->numeMasa, numemasa) == 0)
		{
			OKm = 1;
			TLista j = ((Masa)(p->info))->jucatori;
			j = j->urm;
			if (((Masa)(p->info))->jucatori != NULL)
			{
				do
				{
					if(strcmp (((Jucator)(j->info))->nume, numejucator) == 0)
					{	
						((Jucator)(j->info))->nrMaini += grad_noroc;
						OKj = 1; //am gasit jucatorul cu numele numejucator
					}
				j = j->urm;
				}while(j != ((Masa)(p->info))->jucatori);
			}
			
			
		}
		p = p->urm;
	}
	if(!OKm) {fprintf(fout, "Masa %s nu exista!\n", numemasa); return;}
	if(!OKj) fprintf(fout, "Jucatorul %s nu exista la masa %s!\n", numejucator, numemasa);
}

void EliminareJucator(ALista jucatori, char* nume)
{
	TLista p = (*jucatori)->urm;
	TLista ant = NULL;
	while (p != (*jucatori))
	{
		if(strcmp(((Jucator)(p->info))->nume, nume) == 0) break;
		if(p == (*jucatori)) return;
		ant = p;
		p = p->urm;
	}
	if (ant == NULL) (*jucatori)->urm = p->urm;
	else ant->urm = p->urm;
	free(p);
	
	
}

void EliminareMasa(ALista masa, char* numemasa)
{
	TLista p = *masa;
	TLista ant = NULL;
	while(p != NULL)
	{
		if(strcmp (((Masa)(p->info))->numeMasa, numemasa) == 0) break;
		if(p == NULL) return;
		ant = p;
		p = p->urm;
	}
	if(ant == NULL) *masa = p->urm;
	else ant->urm = p->urm;
	free(p);
}

void Ghinion(Sala *sala_joc, ALista masa, char* numemasa, char* numejucator, int grad_ghinion, FILE *fout)
{
	TLista p = *masa;
	int OKj = 0;
	int OKm = 0;
	for(;p != NULL; p=p->urm)
	{
		
		if (strcmp (((Masa)(p->info))->numeMasa, numemasa) == 0)
		{
			OKm = 1;
			TLista j = ((Masa)(p->info))->jucatori;
			j = j->urm;

			do
			{
				if(strcmp (((Jucator)(j->info))->nume, numejucator) == 0)
				{	
					OKj = 1; //am gasit jucatorul cu numele numejucator
					((Jucator)(j->info))->nrMaini -= grad_ghinion;
					if(((Jucator)(j->info))->nrMaini <= 0)
					{
						EliminareJucator(&(((Masa)(p->info))->jucatori), numejucator);
						((Masa)(p->info))->nrCrtJucatori--;
						(*sala_joc)->nrLocCrt--;
					}
				}	

				
				j = j->urm;
			}
			while(j != ((Masa)(p->info))->jucatori);
			
			if (((Masa)(p->info))->nrCrtJucatori == 0)
			{
				(*sala_joc)->nrLocMax -= ((Masa)(p->info))->nrMaxJucatori;
				EliminareMasa(masa, numemasa );
				
			}
			break;
		}
	
	}
	if(!OKm) {fprintf(fout, "Masa %s nu exista!\n", numemasa); return;}
	if(!OKj) fprintf(fout, "Jucatorul %s nu exista la masa %s!\n", numejucator, numemasa);
}

void AfisareJucatoriClasament (ALista jucatori, FILE * fout)
{
	TLista p = (*jucatori)->urm;
	if(!p) fprintf(fout, "nu sunt jucatori");
	while (p != (*jucatori))
	{
		fprintf(fout, "%s %d\n", ((Jucator)(p->info))->nume, ((Jucator)(p->info))->nrMaini);
		p = p->urm;
		
	}
	
	
}

int InsOrd(ALista aL, Jucator e)
{
	TLista p = *aL, ant = NULL, aux;
	if(p->urm == *aL)
	{
		if(InsJucator(aL, e, sizeof(struct jucator))) return 1;
		
		
	}
	p = p->urm;
	while(p != (*aL) && (((Jucator)(p->info))->nrMaini >= e->nrMaini))
	{	
		if ((((Jucator)(p->info))->nrMaini == e->nrMaini) && (strcmp(((Jucator)(p->info))->nume, e->nume) < 0))
			break;
		ant = p;
		p = p->urm;
	}
	
	aux = AlocCelula(e, sizeof(struct jucator));
	if(!aux) return 0;
	if(ant == NULL) 
	{
		(*aL)->urm = aux;
		aux->urm = p;
	}
	else
	{
		ant->urm = aux;
		aux->urm = p;
	}
	
	return 1;
}

void Clasament(ALista masa, char *numemasa, FILE* fout)
{
	
	TLista p = *masa, ant = NULL, aux;
	TLista jord = InitLDI();
	int OKm = 0;
	
	for(; p != NULL; p = p->urm)
	{
		if (strcmp (((Masa)(p->info))->numeMasa, numemasa) == 0)
		{
			OKm = 1;
			fprintf(fout, "Clasament %s:\n", numemasa);
			TLista j = ((Masa)(p->info))->jucatori;
			j = j->urm;
			
			while(j != ((Masa)(p->info))->jucatori)
			{
				InsOrd(&jord, ((Jucator)(j->info)));
				j = j->urm;
			}
			AfisareJucatoriClasament(&jord, fout);
			break;
		}
	}
	if(!OKm) fprintf(fout, "Masa %s nu exista!\n", numemasa);
}

void Tura_aux(ALista jucatori)
{
	TLista p = (*jucatori)->urm; //primul jucator din dreapta santinelei
	InsJucator(jucatori, ((Jucator)(p->info)), sizeof(struct jucator));
	EliminareJucator(jucatori, ((Jucator)(p->info))->nume);
	
}

void Tura (ALista masa, char *numemasa, Sala *sala_joc, FILE *fout)
{
	TLista p = *masa;
	int OKm = 0;
	for(; p != NULL; p = p->urm)
	{
		if (strcmp (((Masa)(p->info))->numeMasa, numemasa) == 0)
		{
			OKm = 1;
			
			Tura_aux(&(((Masa)(p->info))->jucatori));
			TLista j = ((Masa)(p->info))->jucatori;
			j = j->urm;
			while(j != ((Masa)(p->info))->jucatori)
			{
				((Jucator)(j->info))->nrMaini--;
				if (((Jucator)(j->info))->nrMaini <= 0) 
				{
					EliminareJucator(&(((Masa)(p->info))->jucatori), ((Jucator)(j->info))->nume );
					(*sala_joc)->nrLocCrt--;
					((Masa)(p->info))->nrCrtJucatori--;
				}
				j = j->urm;
			}

			if (((Masa)(p->info))->nrCrtJucatori == 0)
			{
				(*sala_joc)->nrLocMax -= ((Masa)(p->info))->nrMaxJucatori;
				EliminareMasa(masa, numemasa );
				
			}
			break;
		}
	}
	if(!OKm) fprintf(fout, "Masa %s nu exista!\n", numemasa);
}

void Tura_completa(ALista masa, Sala *sala_joc)
{
	TLista p = *masa;
	for(; p != NULL; p = p->urm)
	{
		
		Tura_aux(&(((Masa)(p->info))->jucatori));
		TLista j = ((Masa)(p->info))->jucatori;
		j = j->urm;
		while(j != ((Masa)(p->info))->jucatori)
		{
			((Jucator)(j->info))->nrMaini--;
			if (((Jucator)(j->info))->nrMaini <= 0) 
			{
				EliminareJucator(&(((Masa)(p->info))->jucatori), ((Jucator)(j->info))->nume );
				(*sala_joc)->nrLocCrt--;
				((Masa)(p->info))->nrCrtJucatori--;
			}
			j = j->urm;
		}

		if (((Masa)(p->info))->nrCrtJucatori == 0)
		{
			(*sala_joc)->nrLocMax -= ((Masa)(p->info))->nrMaxJucatori;
			EliminareMasa(masa, ((Masa)(p->info))->numeMasa );
				
		}
	}
}

void Inchide(Sala* sala_joc, ALista masa, char *numemasa, FILE *fout)
{
	TLista p = *masa;
	TLista l = *masa;
	TLista j;
	int iter;
	int OKm = 0;

	for(; p != NULL; p = p->urm)
	{
		if (strcmp (((Masa)(p->info))->numeMasa, numemasa) == 0)
		{
			OKm = 1;
			int nr_locuri_libere_sala = (*sala_joc)->nrLocMax - (*sala_joc)->nrLocCrt - ((Masa)(p->info))->nrCrtJucatori;
			j = ((Masa)(p->info))->jucatori;
			j = j->urm;
			if(nr_locuri_libere_sala < ((Masa)(p->info))->nrCrtJucatori) 
			{
				fprintf(fout, "Nu exista suficiente locuri in sala!\n");
				return;
			}
			break;
		}
	}
	if(!OKm) 
	{
		fprintf(fout,"Masa %s nu exista!\n", numemasa);
		return;
	}
	for(; l != NULL; l = l->urm)
	{
		if (strcmp (((Masa)(l->info))->numeMasa, numemasa) == 0) continue;
		int nr_locuri_libere_masa = ((Masa)(l->info))->nrMaxJucatori - ((Masa)(l->info))->nrCrtJucatori;
		
		
		for(iter = 0; iter < nr_locuri_libere_masa; iter++)
		{
			if(j != ((Masa)(p->info))->jucatori)
		 	{
				InsJucator(&(((Masa)(l->info))->jucatori), (Jucator)(j->info), sizeof(struct jucator));
				((Masa)(l->info))->nrCrtJucatori++;
				j = j->urm;
			}
		}
	}
	(*sala_joc)->nrLocMax -= ((Masa)(p->info))->nrMaxJucatori;
	EliminareMasa(masa, numemasa);
}

void DistrugeJ(TLista aL)
{
	TLista p = aL->urm, aux;
	while (p !=aL)
	{
		aux = p;
		p = p->urm;
		
		free(aux);
	}
	aL->urm = aL;
}

void DistrugeM(ALista aL)
{
	TLista aux;
	while(*aL)
	{
		aux = *aL;
		*aL = aux->urm;
		DistrugeJ(((Masa)(aux->info))->jucatori);
		free(aux);
	}

	*aL = NULL;
}



int main(int argc, char *argv[])
{
	FILE *fin, *fconf, *fout;

	fin = fopen(argv[2], "rt");
	if (fin == NULL) {
        	fprintf(stderr, "ERROR: Can't open file %s", argv[2]);
        	return -1;
	}
	
	fconf = fopen(argv[1], "rt");
	if (fconf == NULL) {
        	fprintf(stderr, "ERROR: Can't open file %s", argv[1]);
        	return -1;
	}
	
	fout = fopen (argv[3], "w+");
	if (fout == NULL) {
        	fprintf(stderr, "ERROR: Can't open file %s", argv[3]);
        	return -1;
	}

	
	Sala sala_joc = malloc(sizeof(Sala)) ;
	TLista masa = NULL;
	
	
	fscanf(fconf, "%d", &sala_joc->nrMese);
	sala_joc->nrLocCrt = 0;
	sala_joc->nrLocMax = 0;
	
	int i,j;
	
	char numemasa[7], numejucator[20];
	

	for(i = 0; i < sala_joc->nrMese; i++)
	{

		TLista jucatori = InitLDI();
		Masa aux_masa = malloc(sizeof(struct masa));
		aux_masa->numeMasa = malloc(sizeof(char*));
		aux_masa->jucatori = malloc(sizeof(TLista));
		

		Jucator aux_jucatori = malloc(sizeof(struct jucator));
		
		fscanf(fconf, "%s", numemasa);
		
		strcpy(aux_masa->numeMasa, numemasa);
		
		fscanf(fconf, "%d", &aux_masa->nrCrtJucatori);
		fscanf(fconf, "%d", &aux_masa->nrMaxJucatori);
		
		sala_joc->nrLocCrt += aux_masa->nrCrtJucatori;
		sala_joc->nrLocMax += aux_masa->nrMaxJucatori;
		
		for (j = 0; j < aux_masa->nrCrtJucatori; j++)
		{
			aux_jucatori->nume = malloc(sizeof(char*));
			fscanf(fconf, "%s", numejucator);
			strcpy(aux_jucatori->nume, numejucator);
			fscanf(fconf, "%d", &aux_jucatori->nrMaini);

			if(InsJucator((&jucatori), aux_jucatori, sizeof(struct jucator))) continue;
			else printf("inserare %s nu a reusit\n", aux_jucatori->nume);
			
			
		}
			aux_masa->jucatori = jucatori;
			if(InsMasa((&masa), aux_masa, sizeof(struct masa))) continue; 
			else printf("inserare %s nu a reusit", aux_masa->numeMasa);
			sala_joc->masa = masa;
		
	}
	
	
	
	
	char operatie[14];
	int grad_noroc;
	int grad_ghinion;
	while( fscanf(fin, "%s", operatie) != EOF){

		if (strcmp (operatie, "print") == 0)
		{
			
			Afisare(&masa, fout);
			
		}
		else if(strcmp(operatie, "noroc") == 0)
		{
			fscanf(fin, "%s", numemasa);
			fscanf(fin, "%s", numejucator);
			fscanf(fin, "%d", &grad_noroc);
			Noroc(&masa, numemasa, numejucator, grad_noroc, fout);
			
		}
		else if (strcmp(operatie, "ghinion") == 0)
		{
			fscanf(fin, "%s", numemasa);
			fscanf(fin, "%s", numejucator);
			fscanf(fin, "%d", &grad_ghinion);
			Ghinion(&sala_joc, &masa, numemasa, numejucator, grad_ghinion, fout);
			
		}
		else if(strcmp(operatie, "clasament") == 0)
		{
			fscanf(fin, "%s", numemasa);
			Clasament(&masa, numemasa, fout);
				
		}
		else if(strcmp(operatie, "tura") == 0)
		{
			fscanf(fin, "%s", numemasa);
			Tura(&masa, numemasa, &sala_joc, fout);
			
		}
		else if(strcmp(operatie, "tura_completa") == 0)
		{
			Tura_completa(&masa, &sala_joc);
			
		}
		else if(strcmp(operatie, "inchide") == 0)
		{
			fscanf(fin, "%s", numemasa);
			Inchide(&sala_joc, &masa, numemasa, fout);
			
		}
	}
	
	
	DistrugeM(&masa);


	fclose(fin);
	fclose(fout);

	return 0;
}
