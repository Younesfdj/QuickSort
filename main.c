#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

typedef struct Date Date;
struct Date
{
    int jj, mm, aaaa;
};

typedef struct Article Article;
struct Article
{
    int QTC; // Quantity
    char *Designation;
    int Prix;
    Date date_per;
    Article *suiv;
};

typedef struct Aliste Aliste;
struct Aliste
{
    Article *Sommet;
};

typedef struct AchatVente AchatVente;
struct AchatVente
{
    int Type;      // si 0 alors Achat sinon si 1 alors Vente
    int NFacture;  // numero de la facture
    int NCommande; // numero de la commande
    // si il y a une facture alors c'est un achat sinon c'est un vente et il y a une commande et non pas une facture
    Date date;
    int MontantHT;
    int MontantTTC;
    Aliste *ListeArticle;
};

Aliste *initAliste() /// -- MARCHE COMME INITPILE --
{
    Aliste *Pile = malloc(sizeof(*Pile));
    Article *new = malloc(sizeof(*new));
    new->Designation = " ";
    new->Prix = 0;
    new->QTC = 0;
    new->date_per.aaaa = 0;
    new->date_per.jj = 0;
    new->date_per.mm = 0;
    new->suiv = NULL;
    Pile->Sommet = new;
    return Pile;
}

void AjouArti(Aliste *Pile, Article NewArt) /// NewArt = nouveau Article a ajouter --- CA MARCHE COMME EMPILER ---
{
    Article *new = malloc(sizeof(*new));
    new->Designation = NewArt.Designation;
    new->Prix = NewArt.Prix;
    new->QTC = NewArt.QTC;
    new->date_per.aaaa = NewArt.date_per.aaaa;
    new->date_per.mm = NewArt.date_per.mm;
    new->date_per.jj = NewArt.date_per.jj;
    new->suiv = Pile->Sommet;
    Pile->Sommet = new;
}

Article SuppArti(Aliste *Pile) /// --- CA MARCHE COMME DEPILER ---
{
    Article *new = malloc(sizeof(*new));
    new->Designation = Pile->Sommet->Designation;
    new->Prix = Pile->Sommet->Prix;
    new->QTC = Pile->Sommet->QTC;
    new->date_per.aaaa = Pile->Sommet->date_per.aaaa;
    new->date_per.jj = Pile->Sommet->date_per.jj;
    new->date_per.mm = Pile->Sommet->date_per.mm;
    new->suiv = NULL;
    Pile->Sommet = Pile->Sommet->suiv;
    return *new;
}
/// AFFICHAGE CE FAIT COMME UNE LISTE !!
void affArtiList(Aliste *Pile)
{
    Article *temp = Pile->Sommet;
    while (temp->suiv != NULL)
    {
        printf("\n%s\tQTC:%d\tPrix:%dda ", temp->Designation, temp->QTC, temp->Prix);
        if (temp->date_per.aaaa != 0)
            printf("Date %d/%d/%d", temp->date_per.jj, temp->date_per.mm, temp->date_per.aaaa);
        temp = temp->suiv;
    }
}
/// --- SUPPRIMER UN ARTICLE DEFINIE PAR DESIGNIATION ---
void SuppArti2(Aliste *Pile, Article Art_a_supp)
{
    Article x;
    Aliste *Pilei = initAliste();
    int trouve = 0;
    while (Pile->Sommet != NULL && trouve == 0)
    {
        x = SuppArti(Pile);
        if (strcmp(x.Designation, Art_a_supp.Designation) == 0)
        {
            trouve = 1;
        }
        else
        {
            AjouArti(Pilei, x);
        }
    }
    // --- PARTIE RECONSTRUCION ---
    while (Pilei->Sommet->suiv != NULL)
    {
        x = SuppArti(Pilei);
        AjouArti(Pile, x);
    }
}

/// Rechereche par caractéristiques
void Rech_carac(Aliste *P, Article Art_a_rech, char option)
{
    if ((int)option == 49 || (int)option == 50)
    { // force the user to give only 1 or 2 by checking ascii number of option
        Aliste *I = initAliste();
        Article x;
        short int exist = 0;
        while (P->Sommet != NULL)
        {
            x = SuppArti(P);
            AjouArti(I, x);
            switch (option)
            {
            case '1': // 1 pour Designation
                if (strcmp(x.Designation, Art_a_rech.Designation) == 0)
                {
                    if (exist == 0)
                        exist++;
                    printf("%s\tQTC:%d\tPrix:%d da\n", x.Designation, x.QTC, x.Prix);
                }
                break;
            case '2': // 2 pour date de peremption
                if (x.date_per.aaaa == Art_a_rech.date_per.aaaa && x.date_per.jj == Art_a_rech.date_per.jj && x.date_per.mm == Art_a_rech.date_per.mm)
                {
                    if (exist == 0)
                        exist++;
                    printf("%s\tQTC:%d\tPrix:%d da\n Date:%d/%d/%d", x.Designation, x.QTC, x.Prix, x.date_per.jj, x.date_per.mm, x.date_per.aaaa);
                }
                break;
            }
        }
        if (exist == 0)
            printf("Cet article n'appartient pas a la liste des articles\n");
        // --- PARTIE RECONSTRUCION ---
        while (I->Sommet->suiv != NULL)
        {
            x = SuppArti(I);
            AjouArti(P, x);
        }
    }
}

int DateCmp(struct tm *D1, Date D2)
{
    if (D2.jj > 31 || D2.jj < 1 || D2.mm > 12 || D2.mm < 1)
        return 2;
    else
    {
        if (D1->tm_year + 1900 > D2.aaaa)
        {
            /* D1>D2 --> 1 */ return 1;
        }
        else if (D1->tm_year + 1900 < D2.aaaa)
        {
            /* D1<D2 --> -1 */ return -1;
        }
        else if (D1->tm_mon + 1 > D2.mm)
        {
            /* D1>D2 --> 1 */ return 1;
        }
        else if (D1->tm_mon + 1 < D2.mm)
        {
            /* D1<D2 --> -1 */ return -1;
        }
        else if (D1->tm_mday, D2.jj)
        {
            /* D1>D2 --> 1 */ return 1;
        }
        else if (D1->tm_mday < D2.jj)
        {
            /* D1<D2 --> -1 */ return -1;
        }
        else
        {
            /* D1=D2 --> 0 */ return 0;
        }
    }
}

void Affiche_arti_exp(Aliste *P)
{
    Aliste *I = initAliste();
    Article x;
    time_t dt; // to get the current date
    time(&dt);
    struct tm *current_date = localtime(&dt);

    while (P->Sommet != NULL)
    {
        x = SuppArti(P);
        AjouArti(I, x);
        if (DateCmp(current_date, x.date_per) == 1 || DateCmp(current_date, x.date_per) == 0)
            printf("%s\tQTC:%d\tPrix:%d da Date:%d/%d/%d\n", x.Designation, x.QTC, x.Prix, x.date_per.jj, x.date_per.mm, x.date_per.aaaa);
    }
    // --- PARTIE RECONSTRUCION ---
    while (I->Sommet->suiv != NULL)
    {
        x = SuppArti(I);
        AjouArti(P, x);
    }
}

void main()
{
    Aliste *AA = initAliste();

    Article produit1;
    produit1.Designation = "iPhoneXS";
    produit1.Prix = 130;
    produit1.QTC = 20;

    Article produit2;
    produit2.Designation = "SamsungA10";
    produit2.Prix = 10;
    produit2.QTC = 10;

    Article produit3;
    produit3.Designation = "Redmi7i";
    produit3.Prix = 25;
    produit3.QTC = 44;

    produit3.date_per.jj = 12;
    produit3.date_per.mm = 12;
    produit3.date_per.aaaa = 2012;

    produit2.date_per.jj = 12;
    produit2.date_per.mm = 12;
    produit2.date_per.aaaa = 2023;

    produit1.date_per.jj = 12;
    produit1.date_per.mm = 12;
    produit1.date_per.aaaa = 2013;

    Article produit4;
    produit4.Designation = "Redmi7ii";
    produit4.Prix = 25;
    produit4.QTC = 44;

    produit4.date_per.jj = 0;
    produit4.date_per.mm = 0;
    produit4.date_per.aaaa = 0;

    Article produitSupp;

    AjouArti(AA, produit1);
    AjouArti(AA, produit2);
    AjouArti(AA, produit3);
    AjouArti(AA, produit4);

    // affArtiList(AA);
    //  ----TEST DE SuppArti(AA)---
    /*printf("on supprime un element: \n");
    produitSupp = SuppArti(AA);
    affArtiList(AA);
    printf("\n");
    printf("on affiche les proprietes du l'element supprimer:\n");
    printf("%s QTC:%d\tPrix:%d da", produitSupp.Designation, produitSupp.QTC, produitSupp.Prix);*/

    // ----TEST DE AjouArti(AA,Produit_a_ajouter)---
    /*printf("\n-----on ajoute un element :\n");
    Article Produit_a_ajouter;
    Produit_a_ajouter.QTC=20;
    Produit_a_ajouter.Prix=230;
    Produit_a_ajouter.Designation="Huawei";
    AjouArti(AA,Produit_a_ajouter);*/

    // Rech_carac(AA,produit4,'3'); // n'affiche rien
    // Rech_carac(AA,produit3,'1'); // affiche redmi

    Affiche_arti_exp(AA);

    affArtiList(AA);
}

// ya youneeeessssssssss
