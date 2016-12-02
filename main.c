/**
 * Kostra programu pro 3. projekt IZP 2015/16
 *
 * Jednoducha shlukova analyza
 * Complete linkage
 * http://is.muni.cz/th/172767/fi_b/5739129/web/web/clsrov.html
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

void error(int err_num);
int is_number(char* s);

/*
 Inicializace shluku 'c'. Alokuje pamet pro cap objektu (kapacitu).
 Ukazatel NULL u pole objektu znamena kapacitu 0.
*/
void init_cluster(struct cluster_t *c, int cap)
{
    assert(c != NULL);
    assert(cap >= 0);
    c = malloc(sizeof(struct cluster_t));
    c -> obj = malloc(cap * sizeof(struct obj_t));
}

/*
 Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 */
void clear_cluster(struct cluster_t *c)
{
        free(c->obj);
        c->size = 0;
}

/// Chunk of cluster objects. Value recommended for reallocation.
const int CLUSTER_CHUNK = 10;

/*
 Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap)
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
    // TODO

    if (c->size + 1 > c->capacity)
        resize_cluster(c, c->size + 1);

    c->obj[c->size] = obj;
    c->size ++;
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

    resize_cluster(c1, c1->size + c2->size);

    for(int i = c1->size; i < c1->size + c2->size; i++)
        c1->obj[i] = c2->obj[i - c1->size];

    c1->size += c2->size;
    sort_cluster(c1);

}

/**********************************************************************/
/* Prace s polem shluku */

/*
 Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek
 (shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy
 pocet shluku v poli.
*/
int remove_cluster(struct cluster_t *carr, int narr, int idx) /**------------------------------------------------ TODO ----------------------------------------------------*/
{
    assert(idx < narr);
    assert(narr > 0);

    // TODO
    //free(&(carr[idx].obj[0]));
    carr[idx] = carr[narr - 1]; /*posledni cluster si prekopiruji do clusteru s indexem idx a posledni uvolnim */

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

    return sqrt( (a1-b1)*(a1-b1) + (a2-b2)*(a2-b2) );
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

    return distance;
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

    float min = 1415; // protoze nejvzdalenejsi body v poli 1000x1000 jsou 1415
    float prom = 0;

    for(int i = 0; i < narr/2; i++) // /2 protoze nechci testovat 2 stejne znovu
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
    return ;
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
        return -1;
    }

    if (fscanf(fr, "count=%d", &count) == 0)
    {
        error(FILE_ERR);
        (*arr) = NULL;
        return 0;
    }

    float x,y;
    int id;

    int loaded = 0;

    *arr = (struct cluster_t *) malloc(count * sizeof(struct cluster_t));

    if(*arr == NULL)
        return -1;

    for(int i = 0; i < count; i++)
    {
        (*arr)[i].obj = malloc(sizeof(struct obj_t));
        (*arr)[i].capacity = 1;
        (*arr)[i].size = 1;

        if (fscanf(fr, "%d %f %f", &id, &x, &y) == 3)
        {
            loaded++;

            (*arr)[i].obj->id = id;
            (*arr)[i].obj->x = x;
            (*arr)[i].obj->y = y;
        }

        else
            error(FILE_ERR);
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
//    int n_clusters = 0;

    // TODO

    if(argc != 2 && argc != 3)
    {
        error(WRONG_ARGS);
        return 1;
    }

    int cluster_num; /* Deklarovani promennych.*/
    char file_name[100];

    if(strlen(argv[1]) + 1 > 100) /* Nasleduje testovani a definice argumentu. */
    {
        error(TOO_LONG_NAME);
        return 1;
    }

    strcpy(file_name, argv[1]);

    if (argc == 1)
        cluster_num = 1;

    else if (!is_number(argv[2]))
    {
        error(NOT_NUM);
        return 1;
    }

    else
        cluster_num = atoi(argv[2]); /*Konec inicializace promennych z argumentu.*/

    int loaded = load_clusters(file_name, &clusters);

    merge_clusters(&clusters[5], &clusters[6]);
    merge_clusters(&clusters[2], &clusters[5]);
    merge_clusters(&clusters[8], &clusters[2]);
    append_cluster(&clusters[8], clusters[19].obj[0]);
    print_clusters(clusters, loaded);

    for(int i = 0; i < loaded; i++)
        free(&clusters[i].obj[0]);

    free(clusters);



    return 0;
}

/********************** DEFINICE DALSICH FUNKCI ************************************/

/*
*   Funkce vypise chybovou hlasku na stderr.
*   err_num je cislo chyby
*/
void error(int err_num)
{
    switch(err_num)
    {
        case WRONG_ARGS :
            fprintf(stderr, "Chybne zadani argumentu.\n");
            break;

        case TOO_LONG_NAME :
            fprintf(stderr, "Prilis dlouhy nazev dokumentu.\n");
            break;

        case NOT_NUM :
            fprintf(stderr, "Druhy argument funkce neni kladne cislo.\n");
            break;

        case FILE_ERR :
            fprintf(stderr, "Chyba v souboru.\n");
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
