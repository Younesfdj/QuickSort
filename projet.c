#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
    new->suiv = Pile->Sommet;
    Pile->Sommet = new;
}

Article SuppArti(Aliste *Pile) /// --- CA MARCHE COMME DEPILER ---
{
    Article *new = malloc(sizeof(*new));
    new->Designation = Pile->Sommet->Designation;
    new->Prix = Pile->Sommet->Prix;
    new->QTC = Pile->Sommet->QTC;
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
        printf("%s\tQTC:%d\tPrix:%d da\n", temp->Designation, temp->QTC, temp->Prix);
        temp = temp->suiv;
    }
}
/// --- SUPPRIMER UN ARTICLE DEFINIE PAR DESIGNIATION ---
void SuppArti2(Aliste **Pile,Article Art_a_supp)
{
    Article x;
    Aliste *Pilei = malloc(sizeof(Aliste));
    Pilei=initAliste();
    int trouve=0;
    while ((*Pile)->Sommet != NULL && trouve==0)
    {
        x=SuppArti(*Pile);
        if (strcmp(x.Designation,Art_a_supp.Designation)==0)
        {
            trouve=1;
        }
        else
        {
            AjouArti(Pilei,x);
        }
    }
    // --- PARTIE RECONSTRUCION ---
    while (Pilei->Sommet->suiv!=NULL)
    {
        x=SuppArti(Pilei);
        AjouArti(*Pile,x);
    }
}

void main()
{
    Aliste *AA=initAliste();
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

    Article produitSupp;

    AjouArti(AA, produit1);
    AjouArti(AA, produit2);
    AjouArti(AA, produit3);
    affArtiList(AA);
    // ----TEST DE SuppArti(AA)---
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

    SuppArti2(&AA,produit1);
    affArtiList(AA);
}
