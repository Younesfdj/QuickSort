#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
int clear_input_buffer(void)
{
    int ch;
    while (((ch = getchar()) != EOF) && (ch != '\n'))
        ;
    return ch;
}
//--------------------------Declaration des structures--------------------------//
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
    float Prix;
    int Critique; // si 1 qlors oui sinon 0 alors non
    Date date_per;
    int NBvente;
    Article *suiv;
};

typedef struct Aliste Aliste;
struct Aliste
{
    Article *Sommet;
};

typedef struct Commande Commande;
struct Commande
{
    int IDclient;
    int Type;      // si 0 alors Achat sinon si 1 alors Vente
    int NFacture;  // numero de la facture
    int NCommande; // numero de la commande
    // si il y a une facture alors c'est un achat sinon c'est un vente et il y a une commande et non pas une facture
    Date date;
    float MontantHT;
    float MontantTTC;
    Aliste *ListeArticle;
    Commande *suiv;
};

typedef struct ComListe ComListe;
struct ComListe
{
    Commande *SommetC;
};

typedef struct Personne Personne;
struct Personne
{
    int ID; // numero client
    char Nom[15];
    char Prenom[15];
    int Phone;
    char Adresse[30];
    int Etat;  // 1 -> client   2->fournisseur   3->diver
    int NBCom; // nombre de commandes
    Personne *suiv;
};
typedef struct Pliste Pliste;
struct Pliste
{
    Personne *TeteP;
    Personne *QueueP;
};

//----------------------Initialisation -------------------//
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
    new->NBvente = 0;
    new->suiv = NULL;
    Pile->Sommet = new;
    return Pile;
}

ComListe *initComListe()
{
    ComListe *P = malloc(sizeof(*P));
    Commande *new = malloc(sizeof(*new));
    new->date.jj = 0;
    new->date.mm = 0;
    new->date.aaaa = 0;
    new->ListeArticle = NULL;
    new->MontantHT = 0;
    new->MontantTTC = 0;
    new->NCommande = 0;
    new->NFacture = 0;
    new->Type = 2;
    new->IDclient = 0;
    new->suiv = NULL;
    P->SommetC = new;
    return P;
}

Pliste *initPListe()
{
    Pliste *P = malloc(sizeof(*P));
    Personne *new = malloc(sizeof(*new));
    new->ID = 0;
    new->Etat = 0;
    new->Phone = 0;
    new->NBCom = 0;
    strcpy(new->Nom, " ");
    strcpy(new->Prenom, " ");
    strcpy(new->Adresse, " ");
    new->suiv = NULL;
    P->TeteP = new;
    P->QueueP = new;
    return P;
}
//-------------------------------- Crud Article ----------------------------------------------//

void EmpilerArti(Aliste *P, Article NewArt)
{
    Article *new = malloc(sizeof(*new));
    new->date_per.jj = NewArt.date_per.jj;
    new->date_per.mm = NewArt.date_per.mm;
    new->date_per.aaaa = NewArt.date_per.aaaa;
    new->Critique = NewArt.Critique;
    strcpy(new->Designation, NewArt.Designation);
    new->Prix = NewArt.Prix;
    new->QTC = NewArt.QTC;
    new->NBvente = NewArt.NBvente;
    new->suiv = P->Sommet;
    P->Sommet = new;
}
void AjouArti(Aliste *Pile) /// NewArt = nouveau Article a ajouter --- CA MARCHE COMME EMPILER ---
{
    int n;
    Article *new = malloc(sizeof(*new));
    printf("donner la Designation de cette article: ");
    clear_input_buffer();
    gets(new->Designation);
    printf("le prix: ");
    scanf("%f", &new->Prix);
    printf("la quantity: ");
    scanf("%d", &new->QTC);
    printf("la date d'expiration: ");
    scanf("%d/%d/%d", &new->date_per.jj, &new->date_per.mm, &new->date_per.aaaa);
    printf("Estce que c'est un article critique?(1-oui/2-non) ");
    scanf("%d", &n);
    if (n == 1)
    {
        new->Critique = 1;
    }
    else if (n == 2)
    {
        new->Critique = 0;
    }
    new->NBvente = 0;
    new->suiv = Pile->Sommet;
    Pile->Sommet = new;
}

Article DepilerArticle(Aliste *Pile) /// --- CA MARCHE COMME DEPILER et elle renvoi un article ---
{
    Article *T;
    Article *new = malloc(sizeof(*new));
    strcpy(new->Designation, Pile->Sommet->Designation);
    new->Prix = Pile->Sommet->Prix;
    new->QTC = Pile->Sommet->QTC;
    new->date_per.aaaa = Pile->Sommet->date_per.aaaa;
    new->date_per.jj = Pile->Sommet->date_per.jj;
    new->date_per.mm = Pile->Sommet->date_per.mm;
    new->Critique = Pile->Sommet->Critique;
    new->NBvente = Pile->Sommet->NBvente;
    new->suiv = NULL;
    T = Pile->Sommet;
    Pile->Sommet = Pile->Sommet->suiv;
    free(T);
    return *new;
}
void ModifierArt(Aliste *Pile, char *referance)
{
    Aliste *TempP = initAliste();
    Article x;
    int R, n;   // R pour test du boocle DO WHILE / n pout le test du switch
    char ND[5]; // ND variable intermediaire pour changement des chaines

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
            EmpilerArti(TempP, x);
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
            scanf("%f", &x.Prix);

            break;

        case 3:
            printf("donner la nouvelle Designation: ");
            clear_input_buffer();
            gets(x.Designation);
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
    EmpilerArti(Pile, x);
    while (TempP->Sommet->suiv != NULL)
    {
        x = DepilerArticle(TempP);
        EmpilerArti(Pile, x);
    }
}
void SupprimerArticle(Aliste *Pile) //----suppression d'un article precis---
{
    char D[20];
    Article x;
    Aliste *Pilei = initAliste();
    int trouve = 0;
    printf("\ndonner la designation de l'article a supprimer: ");
    gets(D);
    gets(D);
    while (Pile->Sommet != NULL && trouve == 0)
    {
        x = DepilerArticle(Pile);
        if (strcmp(x.Designation, D) == 0)
        {
            trouve = 1;
        }
        else
        {
            EmpilerArti(Pilei, x);
        }
    }
    // --- PARTIE RECONSTRUCION ---
    while (Pilei->Sommet->suiv != NULL)
    {
        x = DepilerArticle(Pilei);
        EmpilerArti(Pile, x);
    }
}
//-------------------------les fonctions D'affichage---------------------------------------------------------//

void affArtiList(Aliste *Pile)
{
    Aliste *TempPile = initAliste();
    Article temp;

    while (Pile->Sommet->suiv != NULL) // affichage
    {
        temp = DepilerArticle(Pile);
        if (temp.Critique == 1)
        {
            printf("\n%s   QTC:%d   Prix:%.2f da Critique Date %d/%d/%d", temp.Designation, temp.QTC, temp.Prix, temp.date_per.jj, temp.date_per.mm, temp.date_per.aaaa);
        }
        else if (temp.Critique == 0)
        {
            printf("\n%s   QTC:%d   Prix:%.2f da Non-Critique Date %d/%d/%d", temp.Designation, temp.QTC, temp.Prix, temp.date_per.jj, temp.date_per.mm, temp.date_per.aaaa);
        }
        EmpilerArti(TempPile, temp);
    }
    while (TempPile->Sommet->suiv != NULL)
    {
        temp = DepilerArticle(TempPile);
        EmpilerArti(Pile, temp);
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
            printf("\n%s\tQTC:%d\tPrix:%.2fda  Date %d/%d/%d", temp.Designation, temp.QTC, temp.Prix, temp.date_per.jj, temp.date_per.mm, temp.date_per.aaaa);
        }
        EmpilerArti(TempPile, temp);
    }
    while (TempPile->Sommet->suiv != NULL)
    {
        temp = DepilerArticle(TempPile);
        EmpilerArti(Pile, temp);
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
            printf("\n%s\tQTC:%d\tPrix:%.2fda Date %d/%d/%d", temp.Designation, temp.QTC, temp.Prix, temp.date_per.jj, temp.date_per.mm, temp.date_per.aaaa);
        }
        EmpilerArti(TempPile, temp);
    }
    while (TempPile->Sommet->suiv != NULL)
    {
        temp = DepilerArticle(TempPile);
        EmpilerArti(Pile, temp);
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
            EmpilerArti(I, x);
            switch (option)
            {
            case '1': // 1 pour Designation
                if (strcmp(x.Designation, Art_a_rech.Designation) == 0)
                {
                    if (exist == 0)
                        exist++;
                    printf("%s\tQTC:%.2f\tPrix:%d da\n", x.Designation, x.QTC, x.Prix);
                }
                break;
            case '2': // 2 pour date de peremption
                if (x.date_per.aaaa == Art_a_rech.date_per.aaaa && x.date_per.jj == Art_a_rech.date_per.jj && x.date_per.mm == Art_a_rech.date_per.mm)
                {
                    if (exist == 0)
                        exist++;
                    printf("%s\tQTC:%.2f\tPrix:%d da\n Date:%d/%d/%d", x.Designation, x.QTC, x.Prix, x.date_per.jj, x.date_per.mm, x.date_per.aaaa);
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
            EmpilerArti(P, x);
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
        EmpilerArti(I, x);
        if (DateCmp(current_date, x.date_per) == 1 || DateCmp(current_date, x.date_per) == 0)
            printf("%s\tQTC:%.2f\tPrix:%d da Date:%d/%d/%d\n", x.Designation, x.QTC, x.Prix, x.date_per.jj, x.date_per.mm, x.date_per.aaaa);
    }
    // --- PARTIE RECONSTRUCION ---
    while (I->Sommet->suiv != NULL)
    {
        x = DepilerArticle(I);
        EmpilerArti(P, x);
    }
}

//---------------------------------- Crud Personne -------------------------------------//
void AjouPer(Pliste *P)
{

    Personne *new = malloc(sizeof(*new));
    if (P->TeteP->ID == 0)
    {
        printf("ID: ");
        scanf("%d", &new->ID);
        printf("Nom: ");
        clear_input_buffer();
        gets(new->Nom);
        printf("Prenom: ");
        clear_input_buffer();
        gets(new->Prenom);
        printf("Addresse: ");
        clear_input_buffer();
        gets(new->Adresse);
        printf("Phone: ");
        scanf("%d", &new->Phone);
        printf("Etat:   client [1]  fournisseur [2]  diver [3]");
        scanf("%d", &new->Etat);
        new->NBCom = 0;
        new->suiv = P->TeteP;
        P->TeteP = new;
        P->QueueP = new;
        return;
    }
    if (P->TeteP->ID != 0)
    {
        printf("ID:");
        scanf("%d", &new->ID);
        printf("Nom: ");
        clear_input_buffer();
        gets(new->Nom);
        printf("Prenom: ");
        clear_input_buffer();
        gets(new->Prenom);
        printf("Addresse: ");
        clear_input_buffer();
        gets(new->Adresse);
        printf("Phone: ");
        scanf("%d", &new->Phone);
        printf("Etat:   client [1]  fournisseur [2]  diver [3]");
        scanf("%d", &new->Etat);
        new->NBCom = 0;
        new->suiv = P->QueueP->suiv;
        P->QueueP->suiv = new;
        P->QueueP = new;
    }
}

void EnfilerPer(Pliste *F, Personne newPer)
{
    Personne *new = malloc(sizeof(*new));
    if (F->TeteP->ID != 0)
    {
        new->ID = newPer.ID;
        new->Etat = newPer.Etat;
        new->NBCom = newPer.NBCom;
        new->Phone = newPer.Phone;
        strcpy(new->Adresse, newPer.Adresse);
        strcpy(new->Nom, newPer.Nom);
        strcpy(new->Prenom, newPer.Prenom);
        new->suiv = F->QueueP->suiv;
        F->QueueP->suiv = new;
        F->QueueP = new;
    }
    if (F->TeteP->ID == 0)
    {
        new->ID = newPer.ID;
        new->Etat = newPer.Etat;
        new->NBCom = newPer.NBCom;
        new->Phone = newPer.Phone;
        strcpy(new->Adresse, newPer.Adresse);
        strcpy(new->Nom, newPer.Nom);
        strcpy(new->Prenom, newPer.Prenom);
        new->suiv = F->TeteP;
        F->TeteP = new;
        F->QueueP = new;
    }
}

Personne DefilerPer(Pliste *F)
{
    Personne *T;
    Personne *new = malloc(sizeof(*new));
    new->ID = F->TeteP->ID;
    new->NBCom = F->TeteP->NBCom;
    new->Etat = F->TeteP->Etat;
    new->Phone = F->TeteP->Phone;
    strcpy(new->Adresse, F->TeteP->Adresse);
    strcpy(new->Nom, F->TeteP->Nom);
    strcpy(new->Prenom, F->TeteP->Prenom);
    new->suiv = NULL;
    T = F->TeteP;
    F->TeteP = F->TeteP->suiv;
    free(T);
    return *new;
}
void SupprimerPersonne(Pliste *File) //----suppression d'une Personne precis---
{
    int n;
    Personne x;
    Pliste *Filei = initPListe();
    int trouve = 0;
    printf("\ndonner ID de la personne qui vous voulez supprimer: ");
    scanf("%d", &n);
    while (File->TeteP->suiv != NULL && trouve == 0)
    {
        x = DefilerPer(File);
        if (x.ID == n)
        {
            trouve = 1;
        }
        else
        {
            EnfilerPer(Filei, x);
        }
    }
    // --- PARTIE RECONSTRUCION ---
    while (Filei->TeteP->suiv != NULL)
    {
        x = DefilerPer(Filei);
        EnfilerPer(File, x);
    }
}

// ----------------------------------- Crud Commande -----------------------------------//

void EmpilerCom(ComListe *P, Commande NewCom)
{
    Commande *new = malloc(sizeof(*new));
    new->date.jj = NewCom.date.jj;
    new->date.mm = NewCom.date.mm;
    new->date.aaaa = NewCom.date.aaaa;
    new->ListeArticle = NewCom.ListeArticle;
    new->MontantHT = NewCom.MontantHT;
    new->MontantTTC = NewCom.MontantTTC;
    new->NCommande = NewCom.NCommande;
    new->NFacture = NewCom.NFacture;
    new->Type = NewCom.Type;
    new->IDclient = NewCom.IDclient;
    new->suiv = P->SommetC;
    P->SommetC = new;
}
void AjouCom(ComListe *P, Aliste *ListArti, Pliste *ListP)
{
    int trouve, n, NC, TC; // NC pour scane ID clien // TC pour tester dans la file des client
    Article x, y;
    char test[20];
    int tt = 1;
    int R, gg;
    Aliste *TempPile = initAliste();

    Commande *new = malloc(sizeof(*new));
    new->ListeArticle = initAliste();

    Personne zz;
    Pliste *TempPer = initPListe();
    printf("donner l'ID du client: ");
    scanf("%d", &NC);
    TC = 0;
    do
    {
        while (ListP->TeteP->suiv != NULL)
        {
            zz = DefilerPer(ListP);
            if (zz.ID == NC)
            {
                TC = 1;

                zz.NBCom = zz.NBCom + 1;
            }
            EnfilerPer(TempPer, zz);
        }
        while (TempPer->TeteP->suiv != NULL)
        {
            zz = DefilerPer(TempPer);
            EnfilerPer(ListP, zz);
        }

        if (TC == 0)
        {
            printf("Ce client n'existe pas,veuiller l'ajouter:\n");
            AjouPer(ListP);
        }
    } while (TC == 0);
    new->IDclient = NC;
    printf("Numero de la commande: ");
    scanf("%d", &new->NCommande);
    printf("Numero de la facture: ");
    scanf("%d", &new->NFacture);
    printf("La date: ");
    scanf("%d/%d/%d", &new->date.jj, &new->date.mm, &new->date.aaaa);
    printf("donner le type de cette commande: (0->achat/1->vente) ");
    scanf("%d", &new->Type);
    if (new->Type == 0)
    {
        do
        {
            printf("donner l'article que vous voulez ajouter a la commande: ");
            scanf("%s", &test);
            trouve = 0;
            while (ListArti->Sommet->suiv != NULL && trouve == 0)
            {
                x = DepilerArticle(ListArti);
                if (strcmp(x.Designation, test) == 0)
                {
                    trouve = 1;
                }
                else
                {
                    EmpilerArti(TempPile, x);
                }
            }
            if (trouve == 1)
            {
                printf("cet article existe deja !\n");
                y.Critique = x.Critique;
                y.date_per.jj = x.date_per.jj;
                y.date_per.mm = x.date_per.mm;
                y.date_per.aaaa = x.date_per.aaaa;
                strcpy(y.Designation, x.Designation);
                y.Prix = x.Prix;
                printf("combien vous voulez ajouter:");
                scanf("%d", &n);
                y.QTC = n;
                x.QTC = x.QTC + n;
                EmpilerArti(new->ListeArticle, y);
            }

            EmpilerArti(ListArti, x); // reconstruction de la liste des article
            while (TempPile->Sommet->suiv != NULL)
            {
                x = DepilerArticle(TempPile);
                EmpilerArti(ListArti, x);
            }
            if (trouve == 0)
            {
                AjouArti(ListArti);

                x = DepilerArticle(ListArti);
                y.Critique = x.Critique;
                y.date_per.jj = x.date_per.jj;
                y.date_per.mm = x.date_per.mm;
                y.date_per.aaaa = x.date_per.aaaa;
                strcpy(y.Designation, x.Designation);
                y.Prix = x.Prix;
                y.QTC = x.QTC;
                x.NBvente = 0;
                EmpilerArti(ListArti, x);
                EmpilerArti(new->ListeArticle, y);
            }
            printf("voulez-vous ajouter un autre article ?(1-oui / 2-non)");
            scanf("%d", &R);
        } while (R == 1);
    }
    if (new->Type == 1)
    {
        do
        {
            printf("donner l'article que vous voulez ajouter a la commande: ");
            scanf("%s", &test);
            trouve = 0;
            while (ListArti->Sommet->suiv != NULL && trouve == 0)
            {
                x = DepilerArticle(ListArti);
                if (strcmp(x.Designation, test) == 0)
                {
                    trouve = 1;
                }
                else
                {
                    EmpilerArti(TempPile, x);
                }
            }
            if (trouve == 1) // s'il l'article existe
            {

                y.Critique = x.Critique;
                y.date_per.jj = x.date_per.jj;
                y.date_per.mm = x.date_per.mm;
                y.date_per.aaaa = x.date_per.aaaa;
                strcpy(y.Designation, x.Designation);
                y.Prix = x.Prix;
                tt = 1;
                while (tt == 1)
                {
                    printf("combien vous voulez:");
                    scanf("%d", &n);
                    if (x.QTC >= n)
                    {
                        y.QTC = n;
                        x.NBvente = x.NBvente + n;
                        x.QTC = x.QTC - n;

                        EmpilerArti(new->ListeArticle, y);
                        tt = 0;
                    }
                    else if (x.QTC < n)
                    {
                        printf("la quantite restante de cet article est: %d\n", x.QTC);
                        printf("voulez vous prendre cette quantite?(1-oui / 2-non)");
                        scanf("%d", &tt);
                    }
                }
            }
            else if (trouve == 0) // l'article n'existe pas
            {
                printf("cet article n'existe pas.\n");
            }
            EmpilerArti(ListArti, x); // reconstruction de la liste des article
            while (TempPile->Sommet->suiv != NULL)
            {
                x = DepilerArticle(TempPile);
                EmpilerArti(ListArti, x);
            }
            printf("voulez-vous ajouter un autre article ?(1-oui / 2-non)");
            scanf("%d", &R);
            if (new->ListeArticle->Sommet->suiv == NULL)
            {
                printf("!!ERREUR!! :veuillez ajouter au moins un article pour completer cette commande.\n");
                printf("Voulez vous annuler cette commande ?(1-oui / 2-non)");
                scanf("%d", &gg);
                if (gg == 1)
                {
                    while (ListP->TeteP->suiv != NULL)
                    {
                        zz = DefilerPer(ListP);
                        if (zz.ID == NC)
                        {
                            zz.NBCom = zz.NBCom - 1;
                        }
                        EnfilerPer(TempPer, zz);
                    }
                    while (TempPer->TeteP->suiv != NULL)
                    {
                        zz = DefilerPer(TempPer);
                        EnfilerPer(ListP, zz);
                    }
                    free(new);
                    return;
                }
                else
                {
                    R = 1;
                }
            }

        } while (R == 1);
    }
    new->MontantHT = 0;                             // initialisation du montant Ht a 0
    while (new->ListeArticle->Sommet->suiv != NULL) // boucle pour conter les prix de tous les article dans la commande
    {
        x = DepilerArticle(new->ListeArticle);
        new->MontantHT = new->MontantHT + (x.Prix * x.QTC);
        EmpilerArti(TempPile, x);
    }
    while (TempPile->Sommet->suiv != NULL) // reconstruction de la liste d'article
    {
        x = DepilerArticle(TempPile);
        EmpilerArti(new->ListeArticle, x);
    }
    new->MontantTTC = (new->MontantHT * 0.19) + new->MontantHT;
    new->suiv = P->SommetC;
    P->SommetC = new;
}
Commande DepilerCommande(ComListe *Pile)
{
    Commande *T;
    Commande *new = malloc(sizeof(*new));
    new->IDclient = Pile->SommetC->IDclient;
    new->ListeArticle = Pile->SommetC->ListeArticle;
    new->MontantHT = Pile->SommetC->MontantHT;
    new->MontantTTC = Pile->SommetC->MontantTTC;
    new->date.aaaa = Pile->SommetC->date.aaaa;
    new->date.jj = Pile->SommetC->date.jj;
    new->date.mm = Pile->SommetC->date.mm;
    new->NCommande = Pile->SommetC->NCommande;
    new->NFacture = Pile->SommetC->NFacture;
    new->Type = Pile->SommetC->Type;
    new->suiv = NULL;
    T = Pile->SommetC;
    Pile->SommetC = Pile->SommetC->suiv;
    free(T);
    return *new;
}

void SupprimerCommande(ComListe *Pile) //----suppression d'une coommande precis---
{
    int n;
    Commande x;
    ComListe *Pilei = initComListe();
    int trouve = 0;
    printf("\ndonner le numero de la commande a supprimer: ");
    scanf("%d", &n);
    while (Pile->SommetC != NULL && trouve == 0)
    {
        x = DepilerCommande(Pile);
        if (x.NCommande == n)
        {
            trouve = 1;
        }
        else
        {
            EmpilerCom(Pilei, x);
        }
    }
    // --- PARTIE RECONSTRUCION ---
    while (Pilei->SommetC->suiv != NULL)
    {
        x = DepilerCommande(Pilei);
        EmpilerCom(Pile, x);
    }
}
void ModifierCommande(ComListe *Pile, Aliste *ListArti)
{
    ComListe *TempP = initComListe();
    Aliste *TempPile = initAliste();
    Commande x;
    Article y, z;
    int R, n, h, tt, NCom; // R pour test du boocle DO WHILE / n pout le test du switch / h pour test
    char test[20];
    int trouve;
    trouve = 0;
    printf("donner le numero de commande que vous voulez changer: ");
    scanf("%d", &NCom);
    while (Pile->SommetC->suiv != NULL && trouve == 0)
    {
        x = DepilerCommande(Pile);
        if (x.NCommande == NCom)
        {
            trouve = 1;
        }
        else
        {
            EmpilerCom(TempP, x);
        }
    }
    do
    {
        printf("\nQue voulez vous modifier ?(choisissez par numero)\n");
        printf("1-numero de la commande\n");
        printf("2-Liste des Articles\n");
        printf("3-Numero de la facture\n");
        printf("4-Date\n");
        printf("5-MontantHT\n");
        printf("6-Type\n");
        scanf("%d", &n);
        switch (n)
        {
        case 1:
            printf("donner la nouveau numero de commande: \n");
            scanf("%d", &x.NCommande);

            break;

        case 2:
            printf("\t\tQue voulez vous faire: \n");
            printf("\t\t  Ajouter Article [1]\n");
            printf("\t\t Supprimer Article [2]\n");
            scanf("%d", &h);
            if (h == 1)
            {
                printf("donner l'article que vous voulez ajouter a la commande: ");
                scanf("%s", &test);
                trouve = 0;
                while (x.ListeArticle->Sommet->suiv != NULL && trouve == 0)
                {
                    z = DepilerArticle(ListArti);
                    if (strcmp(z.Designation, test) == 0)
                    {
                        trouve = 1;
                    }
                    else
                    {
                        EmpilerArti(TempPile, z);
                    }
                }
                if (trouve == 1) // s'il l'article existe
                {

                    y.Critique = z.Critique;
                    y.date_per.jj = z.date_per.jj;
                    y.date_per.mm = z.date_per.mm;
                    y.date_per.aaaa = z.date_per.aaaa;
                    strcpy(y.Designation, z.Designation);
                    y.Prix = z.Prix;

                    while (tt == 1)
                    {
                        printf("combien vous voulez:");
                        scanf("%d", &n);
                        z.QTC = z.QTC + y.QTC;
                        z.NBvente = z.NBvente - y.QTC;
                        if (z.QTC >= n)
                        {
                            y.QTC = n;
                            z.QTC = z.QTC - n;
                            z.NBvente = z.NBvente + n;
                            EmpilerArti(x.ListeArticle, y);
                            tt = 0;
                        }
                        else if (z.QTC < n)
                        {
                            printf("la quantite restante de cet article est: %d\n", z.QTC);
                            printf("voulez vous prendre cette quantite?(1-oui / 2-non)");
                            scanf("%d", &tt);
                        }
                    }
                }
                else if (trouve == 0) // l'article n'existe pas
                {
                    printf("cet article n'existe pas.\n");
                }
                EmpilerArti(ListArti, z); // reconstruction de la liste des article
                while (TempPile->Sommet->suiv != NULL)
                {
                    z = DepilerArticle(TempPile);
                    EmpilerArti(ListArti, z);
                }
            }
            if (h == 2)
            {
                SupprimerArticle(x.ListeArticle);
            }

            break;

        case 3:
            printf("donner le nouveau numero de facture: ");
            scanf("%s", &x.NFacture);
            break;

        case 4:
            printf("donner la nouvelle Date:");
            scanf("%d", &x.date.jj);
            scanf("%d", &x.date.mm);
            scanf("%d", &x.date.aaaa);

            break;

        case 5:
            printf("le nouveau montant HT:");
            scanf("%d", x.MontantHT);
            x.MontantTTC = (x.MontantHT * 19) / 100 + x.MontantHT;
            break;
        case 6:
            printf("le nouveau Type:");
            scanf("%d", x.Type);
            break;

        default:
            printf("SVP choisir un numero de 1 a 6 !");
            break;
        }
        printf("\n voulez vous faire un autre changement ?(Si oui tapez 1)");
        scanf("%d", &R);
    } while (R == 1);
    EmpilerCom(Pile, x);
    while (TempP->SommetC->suiv != NULL)
    {
        x = DepilerCommande(TempP);
        EmpilerCom(Pile, x);
    }
}

///////////////// ----------- Affichage Commande --------------------///////////////////
void AffComList(ComListe *P)
{
    ComListe *TemP = initComListe();
    Commande T;

    while (P->SommetC->suiv != NULL)
    {
        printf("\n");
        printf("\n");
        T = DepilerCommande(P);
        if (T.Type == 0)
        {
            printf("Numero Commande: %d\n", T.NCommande);
            printf("ID client: %d\n", T.IDclient);
            printf("Numero de la facture: %d\n", T.NFacture);
            printf("Date: %d/%d/%d\n", T.date.jj, T.date.mm, T.date.aaaa);
            printf("Type: Achat\n");
            printf("Montant HT: %.2f\n", T.MontantHT);
            printf("Montant TTC: %.2f\n", T.MontantTTC);
            printf("Liste des articles:");
            affArtiList(T.ListeArticle);
        }
        if (T.Type == 1)
        {
            printf("Numero Commande: %d\n", T.NCommande);
            printf("ID client: %d\n", T.IDclient);
            printf("Numero de la facture: %d\n", T.NFacture);
            printf("Date: %d/%d/%d\n", T.date.jj, T.date.mm, T.date.aaaa);
            printf("Type: Vente\n");
            printf("Montant HT: %.2f\n", T.MontantHT);
            printf("Montant TTC: %.2f\n", T.MontantTTC);
            printf("Liste des articles:");
            affArtiList(T.ListeArticle);
        }
        EmpilerCom(TemP, T);
    }
    while (TemP->SommetC->suiv != NULL)
    {
        T = DepilerCommande(TemP);
        EmpilerCom(P, T);
    }
}
void Affichage_cmd(ComListe *P, int Ncom)
{
    ComListe *I = initComListe();
    Commande temp;
    short int exist = 0;
    while (P->SommetC->suiv != NULL)
    {
        temp = DepilerCommande(P);
        EmpilerCom(I, temp);
        if (temp.NCommande == Ncom)
        {
            switch (temp.Type)
            {
            case 0:
                printf("ID Fournisseur : %d\n", temp.IDclient);
                printf("N Facture : %d\n", temp.NFacture);
                printf("N Commande : %d\n", temp.NCommande);
                printf("Date : %d/%d/%d\n", temp.date.jj, temp.date.mm, temp.date.aaaa);
                printf("La liste des articles achetes : ");
                affArtiList(temp.ListeArticle);
                break;
            case 1:
                printf("ID Client : %d\n", temp.IDclient);
                printf("N Facture : %d\n", temp.NFacture);
                printf("N Commande : %d\n", temp.NCommande);
                printf("Date : %d/%d/%d\n", temp.date.jj, temp.date.mm, temp.date.aaaa);
                printf("La liste des articles vendus : ");
                affArtiList(temp.ListeArticle);

                break;
            }
            exist++;
        }
    }
    if (exist == 0)
        printf("Cette Commande n'appartient pas a la liste des Commandes\n");

    // --- PARTIE RECONSTRUCION ---
    while (I->SommetC->suiv != NULL)
    {
        temp = DepilerCommande(I);
        EmpilerCom(P, temp);
    }
}
void Rech_par_client(ComListe *P)
{
    ComListe *I = initComListe();
    Commande temp;
    system("cls");
    int IDclient;
    printf("Entrer ID du client :");
    scanf("%d", &IDclient);
    short int exist = 0;
    while (P->SommetC->suiv != NULL)
    {
        temp = DepilerCommande(P);
        EmpilerCom(I, temp);
        if (temp.IDclient == IDclient)
        {
            Affichage_cmd(I, temp.NCommande);
            exist++;
        }
    }
    if (exist == 0)
        printf("Cet Client n'existe pas\n");

    // --- PARTIE RECONSTRUCION ---
    while (I->SommetC->suiv != NULL)
    {
        temp = DepilerCommande(I);
        EmpilerCom(P, temp);
    }
}
void Rech_par_cmd(ComListe *P, int Ncom)
{
    ComListe *I = initComListe();
    Commande temp;
    short int exist = 0;
    while (P->SommetC->suiv != NULL)
    {
        temp = DepilerCommande(P);
        EmpilerCom(I, temp);
        if (temp.NCommande == Ncom)
            exist++;
    }
    if (exist == 0)
        printf("Cette Commande n'appartient pas a la liste des Commandes\n");
    else
        printf("Cette Commande figure dans la liste des commandes\n");

    // --- PARTIE RECONSTRUCION ---
    while (I->SommetC->suiv != NULL)
    {
        temp = DepilerCommande(I);
        EmpilerCom(P, temp);
    }
}

//------------------------------- Statistiques ------------------------------//
void ArtPlusVendu(Aliste *P)
{
    Aliste *tempP = initAliste();
    Article x, y, z;
    y = DepilerArticle(P);
    z = y;
    while (P->Sommet->suiv != NULL)
    {

        x = DepilerArticle(P);
        if (z.NBvente < x.NBvente)
        {
            z = x;
        }
        EmpilerArti(tempP, x);
    }
    while (tempP->Sommet->suiv != NULL)
    {
        x = DepilerArticle(tempP);
        EmpilerArti(P, x);
    }
    EmpilerArti(P, y);
    printf("l'article le plus vendu: %s", &z.Designation);
}

void NBComClient(Pliste *F, int IDclient)
{
    Pliste *tempF = initPListe();
    Personne x;

    while (F->TeteP->suiv != NULL)
    {
        x = DefilerPer(F);
        if (x.ID = IDclient)
        {
            printf("Nombre des commande de %s %s est: %d", &x.Nom, &x.Prenom, x.NBCom);
        }
        EnfilerPer(tempF, x);
    }
    while (tempF->TeteP->suiv != NULL)
    {
        x = DefilerPer(tempF);
        EnfilerPer(F, x);
    }
}
void PireClient(Pliste *F)
{
    Pliste *tempP = initPListe();
    Personne x, y, z;
    int trouve = 0;
    while (F->TeteP->suiv != NULL && trouve == 0)
    {
        y = DefilerPer(F);
        if (y.Etat == 0)
        {
            z = y;
            trouve = 1;
        }
        EnfilerPer(tempP, y);
    }
    while (F->TeteP->suiv != NULL)
    {
        x = DefilerPer(F);
        if (x.Etat == 0 && z.NBCom > x.NBCom)
        {
            z = x;
        }
        EnfilerPer(tempP, x);
    }
    while (tempP->TeteP->suiv != NULL)
    {
        x = DefilerPer(tempP);
        EnfilerPer(F, x);
    }
    EnfilerPer(F, y);
    printf("\nle Pire Client est: %s %s", &z.Nom, &z.Prenom);
}
//----------------------------------------- MAIN -----------------------------------//
void main()
{
    Aliste *AA = initAliste();
    ComListe *CC = initComListe();
    Pliste *PP = initPListe();

    Article produit1;
    strcpy(produit1.Designation, "iPhoneXS");
    produit1.Prix = 130;
    produit1.QTC = 20;
    produit1.Critique = 0;
    produit1.NBvente = 4;

    Article produit2;
    strcpy(produit2.Designation, "SS");
    produit2.Prix = 10;
    produit2.QTC = 10;
    produit2.Critique = 1;
    produit2.NBvente = 2;

    Article produit3;
    strcpy(produit3.Designation, "Redmi7i");
    produit3.Prix = 25;
    produit3.QTC = 44;
    produit3.Critique = 1;
    produit3.NBvente = 3;

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
    produit4.NBvente = 10;

    produit4.date_per.jj = 0;
    produit4.date_per.mm = 0;
    produit4.date_per.aaaa = 0;

    Article produitSupp;

    EmpilerArti(AA, produit1);
    EmpilerArti(AA, produit2);
    EmpilerArti(AA, produit3);
    EmpilerArti(AA, produit4);

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

    // Affiche_arti_exp(AA);

    affArtiList(AA);
    printf("\n");
    AffArtCrit(AA);
    printf("\n");
    // affArtiList(AA);
    AffArtNonCrit(AA);
    printf("\n");
    Article hh;
    hh = DepilerArticle(AA);

    affArtiList(AA);
    // AjouArti(AA);

    // ModifierArt(AA, "iPhoneXS");
    // affArtiList(AA);
    //  SupprimerArticle(AA);

    // AjouPer(PP);
    AjouCom(CC, AA, PP);
    AffComList(CC);
    printf("\n");
    // ArtPlusVendu(AA);
    printf("\n");
    affArtiList(AA);
    printf("\n");
    Rech_par_cmd(CC, 1);
    printf("\n");
    // Rech_par_client(CC);
    NBComClient(PP, 123);
}
