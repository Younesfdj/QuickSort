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
    char Designation[20];
    int Prix;
    int Critique; // si 1 qlors oui sinon 0 alors non
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
    strcpy(new->Designation, " ");
    new->Prix = 0;
    new->QTC = 0;
    new->date_per.aaaa = 0;
    new->date_per.jj = 0;
    new->date_per.mm = 0;
    new->Critique = 0;
    new->suiv = NULL;
    Pile->Sommet = new;
    return Pile;
}

void AjouArti(Aliste *Pile, Article NewArt) /// NewArt = nouveau Article a ajouter --- CA MARCHE COMME EMPILER ---
{
    Article *new = malloc(sizeof(*new));
    strcpy(new->Designation, NewArt.Designation);
    new->Prix = NewArt.Prix;
    new->QTC = NewArt.QTC;
    new->date_per.aaaa = NewArt.date_per.aaaa;
    new->date_per.mm = NewArt.date_per.mm;
    new->date_per.jj = NewArt.date_per.jj;
    new->Critique = NewArt.Critique;
    new->suiv = Pile->Sommet;
    Pile->Sommet = new;
}

Article DepilerArticle(Aliste *Pile) /// --- CA MARCHE COMME DEPILER et elle renvoi un article ---
{
    Article *new = malloc(sizeof(*new));
    strcpy(new->Designation, Pile->Sommet->Designation);
    new->Prix = Pile->Sommet->Prix;
    new->QTC = Pile->Sommet->QTC;
    new->date_per.aaaa = Pile->Sommet->date_per.aaaa;
    new->date_per.jj = Pile->Sommet->date_per.jj;
    new->date_per.mm = Pile->Sommet->date_per.mm;
    new->Critique = Pile->Sommet->Critique;
    new->suiv = NULL;
    Pile->Sommet = Pile->Sommet->suiv;
    return *new;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//-------------------------les fonctions D'affichage---------------------------------------------------------//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// AFFICHAGE CE FAIT COMME UNE LISTE !!
void affArtiList(Aliste *Pile)
{
    Aliste *TempPile = initAliste();
    Article temp;

    while (Pile->Sommet->suiv != NULL) // affichage
    {
        temp = DepilerArticle(Pile);
        if (temp.Critique == 1)
        {
            printf("\n%s\tQTC:%d\tPrix:%dda Critique Date %d/%d/%d", temp.Designation, temp.QTC, temp.Prix, temp.date_per.jj, temp.date_per.mm, temp.date_per.aaaa);
        }
        else if (temp.Critique == 0)
        {
            printf("\n%s\tQTC:%d\tPrix:%dda Non-Critique Date %d/%d/%d", temp.Designation, temp.QTC, temp.Prix, temp.date_per.jj, temp.date_per.mm, temp.date_per.aaaa);
        }
        AjouArti(TempPile, temp);
    }
    while (TempPile->Sommet->suiv != NULL)
    {
        temp = DepilerArticle(TempPile);
        AjouArti(Pile, temp);
    }
}

// AFFICHAGE DES ARTICLES CRITIQUES
void AffArtCrit(Aliste *Pile)
{
    Aliste *TempPile = initAliste();
    Article temp;

    while (Pile->Sommet->suiv != NULL) // affichage
    {
        temp = DepilerArticle(Pile);
        if (temp.Critique == 1)
        {
            printf("\n%s\tQTC:%d\tPrix:%dda  Date %d/%d/%d", temp.Designation, temp.QTC, temp.Prix, temp.date_per.jj, temp.date_per.mm, temp.date_per.aaaa);
        }
        AjouArti(TempPile, temp);
    }
    while (TempPile->Sommet->suiv != NULL)
    {
        temp = DepilerArticle(TempPile);
        AjouArti(Pile, temp);
    }
}

// AFFICHAGE DES ARTICLES NON CRITIQUES
void AffArtNonCrit(Aliste *Pile)
{
    Aliste *TempPile = initAliste();
    Article temp;

    while (Pile->Sommet->suiv != NULL) // affichage
    {
        temp = DepilerArticle(Pile);
        if (temp.Critique == 0)
        {
            printf("\n%s\tQTC:%d\tPrix:%dda Date %d/%d/%d", temp.Designation, temp.QTC, temp.Prix, temp.date_per.jj, temp.date_per.mm, temp.date_per.aaaa);
        }
        AjouArti(TempPile, temp);
    }
    while (TempPile->Sommet->suiv != NULL)
    {
        temp = DepilerArticle(TempPile);
        AjouArti(Pile, temp);
    }
}

/////////////////////// --- SUPPRIMER UN ARTICLE DEFINIE PAR DESIGNIATION ---//////////////////////////////

void SupprimerArticle(Aliste *Pile, Article Art_a_supp) //----suppression d'un article precis---
{
    Article x;
    Aliste *Pilei = initAliste();
    int trouve = 0;
    while (Pile->Sommet != NULL && trouve == 0)
    {
        x = DepilerArticle(Pile);
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
        x = DepilerArticle(Pilei);
        AjouArti(Pile, x);
    }
}

//////////////////------------ Rechereche par caractéristiques-------------////////////

void Rech_carac(Aliste *P, Article Art_a_rech, char option)
{
    if ((int)option == 49 || (int)option == 50)
    { // force the user to give only 1 or 2 by checking ascii number of option
        Aliste *I = initAliste();
        Article x;
        short int exist = 0;
        while (P->Sommet != NULL)
        {
            x = DepilerArticle(P);
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
            x = DepilerArticle(I);
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
        x = DepilerArticle(P);
        AjouArti(I, x);
        if (DateCmp(current_date, x.date_per) == 1 || DateCmp(current_date, x.date_per) == 0)
            printf("%s\tQTC:%d\tPrix:%d da Date:%d/%d/%d\n", x.Designation, x.QTC, x.Prix, x.date_per.jj, x.date_per.mm, x.date_per.aaaa);
    }
    // --- PARTIE RECONSTRUCION ---
    while (I->Sommet->suiv != NULL)
    {
        x = DepilerArticle(I);
        AjouArti(P, x);
    }
}

//////////////////////---------------- MODIFICATION ARTICLE -------------------/////////////
void ModifierArt(Aliste *Pile, char *referance)
{
    Aliste *TempP = initAliste();
    Article x;
    int R, n, NB;   // R pour test du boocle DO WHILE / n pout le test du switch / NB variable intermediaire pour les chengement entier
    char ND[5];     // ND variable intermediaire pour changement des chaines
    int NJ, NM, NA; // variables intermediaires pour changement du date
    int trouve;

    trouve = 0;
    while (Pile->Sommet->suiv != NULL && trouve == 0)
    {
        x = DepilerArticle(Pile);
        if (strcmp(x.Designation, referance) == 0)
        {
            trouve = 1;
        }
        else
        {
            AjouArti(TempP, x);
        }
    }
    do
    {
        printf("\nQue voulez vous modifier ?(choisissez par numero)\n");
        printf("1-Quantite\n");
        printf("2-Prix\n");
        printf("3-Designation\n");
        printf("4-Date d'expiration\n");
        printf("5-Critique ou non\n");
        scanf("%d", &n);
        switch (n)
        {
        case 1:
            printf("donner la nouvelle quantite: \n");
            scanf("%d", &x.QTC);

            break;

        case 2:
            printf("donner le nouveau Prix:");
            scanf("%d", &x.Prix);

            break;

        case 3:
            printf("donner la nouvelle Designation: ");
            scanf("%s", &x.Designation);
            break;

        case 4:
            printf("donner la nouvelle Date d'expiration:");
            scanf("%d", &x.date_per.jj);
            scanf("%d", &x.date_per.mm);
            scanf("%d", &x.date_per.aaaa);

            break;

        case 5:
            printf("Critique ou Non Critique(oui/non):");
            scanf("%s", &ND);
            if (strcmp(ND, "oui") == 0)
            {
                x.Critique = 1;
            }
            else
            {
                x.Critique = 0;
            }
            break;

        default:
            printf("SVP choisir un numero de 1 a 5 !");
            break;
        }
        printf("\n voulez vous faire un autre changement ?(Si oui tapez 1)");
        scanf("%d", &R);
    } while (R == 1);
    AjouArti(Pile, x);
    while (TempP->Sommet->suiv != NULL)
    {
        x = DepilerArticle(TempP);
        AjouArti(Pile, x);
    }
}

void main()
{
    Aliste *AA = initAliste();

    Article produit1;
    strcpy(produit1.Designation, "iPhoneXS");
    produit1.Prix = 130;
    produit1.QTC = 20;
    produit1.Critique = 0;

    Article produit2;
    strcpy(produit2.Designation, "SamsungA10");
    produit2.Prix = 10;
    produit2.QTC = 10;
    produit2.Critique = 1;

    Article produit3;
    strcpy(produit3.Designation, "Redmi7i");
    produit3.Prix = 25;
    produit3.QTC = 44;
    produit3.Critique = 1;

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
    strcpy(produit4.Designation, "Redmi7ii");
    produit4.Prix = 25;
    produit4.QTC = 44;
    produit4.Critique = 0;

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
    printf("\n");
    AffArtCrit(AA);
    printf("\n");
    affArtiList(AA);
    printf("\n");
    AffArtNonCrit(AA);

    ModifierArt(AA, "iPhoneXS");
    affArtiList(AA);
}

// ya youneeeessssssssss ouiiii
