/**
 * Kostra programu pro 3. projekt IZP 2015/16
 *
 * Jednoducha shlukova analyza
 * Complete linkage
 * http://is.muni.cz/th/172767/fi_b/5739129/web/web/clsrov.html
 * 
 * Tomas Kukan, xkukan00
 *
 * 
 * 5. 12. 2016
 *
 * Napovedu najdete na:
 * https://wis.fit.vutbr.cz/FIT/st/cwk.php?title=IZP:Projekt3&csid=623120&id=11499 
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h> // sqrtf
#include <limits.h> // INT_MAX

/*****************************************************************
 * Ladici makra. Vypnout jejich efekt lze definici makra
 * NDEBUG, napr.:
 *   a) pri prekladu argumentem prekladaci -DNDEBUG
 *   b) v souboru (na radek pred #include <assert.h>
 *      #define NDEBUG
 */
#ifdef NDEBUG
#define debug(s)
#define dfmt(s, ...)
#define dint(i)
#define dfloat(f)
#else

// vypise ladici retezec
#define debug(s) printf("- %s\n", s)

// vypise formatovany ladici vystup - pouziti podobne jako printf
#define dfmt(s, ...) printf(" - "__FILE__":%u: "s"\n",__LINE__,__VA_ARGS__)

// vypise ladici informaci o promenne - pouziti dint(identifikator_promenne)
#define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)

// vypise ladici informaci o promenne typu float - pouziti
// dfloat(identifikator_promenne)
#define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)

#endif

#define WRONG_ARGS 1
#define TOO_LONG_NAME 2
#define NOT_NUM 3
#define FILE_ERR 4

/*****************************************************************
 * Deklarace potrebnych datovych typu:
 *
 * TYTO DEKLARACE NEMENTE
 *
 *   struct obj_t - struktura objektu: identifikator a souradnice
 *   struct cluster_t - shluk objektu:
 *      pocet objektu ve shluku,
 *      kapacita shluku (pocet objektu, pro ktere je rezervovano
 *          misto v poli),
 *      ukazatel na pole shluku.
 */

struct obj_t {
    int id;
    float x;
    float y;
};

struct cluster_t {
    int size;
    int capacity;
    struct obj_t *obj;
};

/*****************************************************************
 * Deklarace potrebnych funkci.
 *
 * PROTOTYPY FUNKCI NEMENTE
 *
 * IMPLEMENTUJTE POUZE FUNKCE NA MISTECH OZNACENYCH 'TODO'
 *
 */

/**
*   Funkci prototypy nekterych dalsich, vlastnich funkci.
*/

void help();
void error(int err_num);
int is_number(char* s);
void free_all(struct cluster_t *clusters, int n_clusters);
/*
 Inicializace shluku 'c'. Alokuje pamet pro cap objektu (kapacitu).
 Ukazatel NULL u pole objektu znamena kapacitu 0.
*/
void init_cluster(struct cluster_t *c, int cap)
{
    assert(c != NULL);
    assert(cap >= 0);
    c -> obj = malloc(cap * sizeof(struct obj_t));
	c->capacity = cap;
}

/*
 Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 */
void clear_cluster(struct cluster_t *c)
{
        free(c->obj);
        c->size = 0;
        c->capacity = 0;
}

/// Chunk of cluster objects. Value recommended for reallocation.
const int CLUSTER_CHUNK = 10;

/*
 Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap) /*bylo cluster*/
{
    // TUTO FUNKCI NEMENTE
    assert(c);
    assert(c->capacity >= 0);
    assert(new_cap >= 0);

    if (c->capacity >= new_cap)
        return c;

    size_t size = sizeof(struct obj_t) * new_cap;

    void *arr = realloc(c->obj, size);
    if (arr == NULL)
        return NULL;

    c->obj = arr;
    c->capacity = new_cap;
    return c;
}

/*
 Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt
 nevejde.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj)
{
    if (c->size + 1 > c->capacity)
    {
        struct cluster_t *p = resize_cluster(c, c->size + CLUSTER_CHUNK);

        if(p == NULL)
            fprintf(stderr, "Nepodarila se zmenit kapacita clusteru.\n");
    }

    c->obj[c->size] = obj;
    c->size = c->size + 1;
}

/*
 Seradi objekty ve shluku 'c' vzestupne podle jejich identifikacniho cisla.
 */
void sort_cluster(struct cluster_t *c);

/*
 Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade nutnosti rozsiren.
 Objekty ve shluku 'c1' budou serazny vzestupne podle identifikacniho cisla.
 Shluk 'c2' bude nezmenen.
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c2 != NULL);

    for(int i = 0 ; i < c2->size; i++)
        append_cluster(c1,c2->obj[i]);    

    sort_cluster(c1);
}

/**********************************************************************/
/* Prace s polem shluku */

/*
 Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek
 (shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy
 pocet shluku v poli.
*/
int remove_cluster(struct cluster_t *carr, int narr, int idx) 
{
    assert(idx < narr);
    assert(narr > 0);

    free(carr[idx].obj);
    carr[idx] = carr[narr - 1]; // posledni cluster si prekopiruji do clusteru s indexem idx a posledni uvolnim

    return narr - 1;
}

/*
 Pocita Euklidovskou vzdalenost mezi dvema objekty.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2)
{
    assert(o1 != NULL);
    assert(o2 != NULL);

    float a1 = o1->x;
    float a2 = o1->y;
    float b1 = o2->x;
    float b2 = o2->y;

    return sqrtf( (a1-b1)*(a1-b1) + (a2-b2)*(a2-b2) );
}

/*
 Pocita vzdalenost dvou shluku.
*/
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c1->size > 0);
    assert(c2 != NULL);
    assert(c2->size > 0);

    float max = 0;
    float distance = 0;

    for (int i = 0; i < c1->size; i++)
    {
        for(int j = 0; j < c2->size; j++)
        {
            distance = obj_distance( &c1->obj[i], &c2->obj[j] );
            if (distance > max)
                max = distance;
        }
    }

    return max;
}

/*
 Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 hleda dva nejblizsi shluky. Nalezene shluky identifikuje jejich indexy v poli
 'carr'. Funkce nalezene shluky (indexy do pole 'carr') uklada do pameti na
 adresu 'c1' resp. 'c2'.
*/
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2)
{
    assert(narr > 0);

    // TODO

    float min = 1420; // protoze nejvzdalenejsi body v poli 1000x1000 jsou 1415
    float prom = 0;

    for(int i = 0; i < narr; i++) // /2 protoze nechci testovat 2 stejne znovu
        for(int j = 0;j < narr; j++)
            if(i != j) // nebude testovat 2 stejne clustery
            {
                prom = cluster_distance(&carr[i], &carr[j]);
                if (prom < min)
                {
                    min = prom;
                    *c1 = i;
                    *c2 = j;
                }
            }
}

// pomocna funkce pro razeni shluku
static int obj_sort_compar(const void *a, const void *b)
{
    // TUTO FUNKCI NEMENTE
    const struct obj_t *o1 = a;
    const struct obj_t *o2 = b;
    if (o1->id < o2->id) return -1;
    if (o1->id > o2->id) return 1;
    return 0;
}

/*
 Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
*/
void sort_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    qsort(c->obj, c->size, sizeof(struct obj_t), &obj_sort_compar);
}

/*
 Tisk shluku 'c' na stdout.
*/
void print_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE

    for (int i = 0; i < c->size; i++)
    {
        if (i) putchar(' ');
        printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
    }
    putchar('\n');
}

/*
 Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
 jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni
 polozku pole (ukalazatel na prvni shluk v alokovanem poli) ulozi do pameti,
 kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych objektu (shluku).
 V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL.
*/
int load_clusters(char *filename, struct cluster_t **arr)
{
    assert(arr != NULL);

    // TODO

    FILE *fr;
    int count;

    if ((fr = fopen(filename, "r")) == NULL)
    {
        fprintf(stderr,"Soubor %s se nepodarilo otevrit.\n", filename);
        help();
		return -1;
    }

    if (fscanf(fr, "count=%d", &count) == 0)
    {
        error(FILE_ERR);
        (*arr) = NULL;
        return -1;
    }

    if (count < 1)
    {
        error(FILE_ERR);
        (*arr) = NULL;
        return -1;
    }

    float x,y;
    int id;

    int loaded = 0;

    *arr = (struct cluster_t *) malloc(count * sizeof(struct cluster_t));

    if(*arr == NULL)
        return -1;

    for(int i = 0; i < count; i++) // cyklus pro nacitani objektu ze souboru a jejich prepisovani
    {
	init_cluster(&(*arr)[i],1);
        (*arr)[i].size = 1;

        if (fscanf(fr, "%d %f %f", &id, &x, &y) == 3)
        {
            if(x < 0 || x > 1000 || y < 0 || y > 1000) // pokud bude neplatny format souradnic
            {
                error(FILE_ERR);
                (*arr) = NULL;
                return -1;
            }

            for (int j = 0; j < loaded; j++) // testuji zdali v souboru nejsou 2 objekty se stejnym id
            {
                if((*arr)[j].obj->id == id)
                {
                    error(FILE_ERR);
                    (*arr) = NULL;
                    return -1;
                }
            }

            loaded++;

            (*arr)[i].obj->id = id;
            (*arr)[i].obj->x = x;
            (*arr)[i].obj->y = y;
        }

        else
        {
            error(FILE_ERR);
            (*arr) = NULL;
            return -1;
        }
    }
    fclose(fr);

    return loaded;
}

/*
 Tisk pole shluku. Parametr 'carr' je ukazatel na prvni polozku (shluk).
 Tiskne se prvnich 'narr' shluku.
*/
void print_clusters(struct cluster_t *carr, int narr)
{
    printf("Clusters:\n");
    for (int i = 0; i < narr; i++)
    {
        printf("cluster %d: ", i);
        print_cluster(&carr[i]);

    }
}

/****************************** MAIN ********************************************/

int main(int argc, char *argv[])
{
    struct cluster_t *clusters;

    if(argc != 2 && argc != 3)
    {
        error(WRONG_ARGS);
        return 1;
    }

    int final_cluster_num; /* Deklarovani promennych.*/
    char file_name[100];
    int c1, c2;

    if(strlen(argv[1]) + 1 > 100) /* Nasleduje testovani a definice argumentu. */
    {
        error(TOO_LONG_NAME);
        return 1;
    }

    strcpy(file_name, argv[1]);

    if (argc == 2)
        final_cluster_num = 1;

    else if (!is_number(argv[2]))
    {
        error(NOT_NUM);
        return 1;
    }

    else
        final_cluster_num = atoi(argv[2]); /*Konec inicializace promennych z argumentu.*/

    int n_clusters = load_clusters(file_name, &clusters);

    if (n_clusters == -1)
    {
        free_all(clusters, n_clusters);
        return 1;
    }

    else if (n_clusters < final_cluster_num)
    {
        fprintf(stderr, "V souboru je pouze %d objektu, tudiz nemuzu udelat %d shluku. \n", n_clusters, final_cluster_num);
		help();
        free_all(clusters, n_clusters);
        return 1;
    }


    while (n_clusters > final_cluster_num)
    {
       find_neighbours(clusters, n_clusters, &c1, &c2);
       merge_clusters(&clusters[c1], &clusters[c2]);
       n_clusters = remove_cluster(clusters, n_clusters, c2);
       //print_clusters(clusters, n_clusters);
       //printf("\n\n\n");
    }

    print_clusters(clusters, n_clusters);

    free_all(clusters, n_clusters);

    return 0;
}

/********************** DEFINICE DALSICH FUNKCI ************************************/

/*
*   Funkce vypise chybovou hlasku na stderr.
*   err_num je cislo chyby
*
*/
void error(int err_num)
{
    switch(err_num)
    {
        case WRONG_ARGS :
            fprintf(stderr, "Chybne zadani argumentu.\n");
			help();
            break;

        case TOO_LONG_NAME :
            fprintf(stderr, "Prilis dlouhy nazev dokumentu.\n");
			help();
            break;

        case NOT_NUM :
            fprintf(stderr, "Druhy argument funkce neni kladne cislo.\n");
			help();
            break;

        case FILE_ERR :
            fprintf(stderr, "Chyba v souboru.\n");
			help();
            break;
    }
}

/*
*   Otestuje retezec zdali je kladne cislo. Vrati 1 pokud ano, 0 pokud ne.
*/
int is_number(char *s)
{
    char *endptr;
    strtod(s, &endptr);

    if(*endptr != '\0')
        return 0;

    double x = atof(s);

    return x < 1 ? 0 : 1;
}

/*
*   Funkce uvolni vsechny pole objektu v poli clusters
*   a samotne pole clusteru.
*/

void free_all(struct cluster_t *clusters, int n_clusters)
{
    for(int i = 0; i < n_clusters; i++)
        clear_cluster(&clusters[i]);

    free(clusters);
}

/*
*	Vypise napovedu.
*/

void help()
{
	fprintf(stderr, "Tomas Kukan, xkukan00 \nJednoducha shlukova analyza\nnapoveda: https://wis.fit.vutbr.cz/FIT/st/cwk.php?title=IZP:Projekt3&csid=623120&id=11499\n");
}
