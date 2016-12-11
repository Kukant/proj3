/**
 *@file
 */

/*
 * Soubor:  proj3.h
 * Datum:   11. 12. 2016
 * Autor:   Kukan Tomas
 * Projekt: Shlukova analyza
 * Popis:   https://wis.fit.vutbr.cz/FIT/st/cwk.php?title=IZP:Projekt3&csid=623120&id=11499 
 */

/**
 * @defgroup Struktury
 * @{
 */

/**
 * @brief Struktura udrzujici informace o jednou objektu. 
 */
struct obj_t {
	/// id je jednoznacne, celociselne oznaceni objektu.
    int id;
    /// x je prvni souradnice bodu.
    float x;
    /// y je druha souradnice bodu.
    float y;
};

/**
 * @brief Struktura udrzujici ukazatel na pole objektu do ni nalezici, svoji kapacitu a velikost.
 */
struct cluster_t {
	/// size je aktualni pocet pouzivanych objektu ve shluku.
    int size;
    /// capacity urcuje celkovou kapacitu shluku.
    int capacity
    /// obj je ukazatel na pole objektu nalezici do shluku.
    struct obj_t *obj;
};
/** @} */ //konec struktur

/**
 * @defgroup Konstanty
 * @{
 */

/// Konstanta pouzita k rozsireni shluku.
extern const int CLUSTER_CHUNK;

/** @} */ //konec konstant

/**
 * @defgroup Funkce
 * @{
 */

/**
 * @brief Funkce init_cluster inicializuje shluk, uvolni pamet pro pole objektu.
 * 
 * @pre c musi ukazovat nekam do pameti, nesmi mit hodnotu NULL.
 * @post v *c je adresa prvniho prvku pole shluku.
 *
 * @param c     Ukazatel na shluk.
 * @param cap   Kapacitu pro objekty, kterou ma funkce uvolnit.
 */
void init_cluster(struct cluster_t *c, int cap);

/**
 * @brief Funkce clear_cluster uvolni pamet pro pole objektu ve shluku.
 * 
 * @pre c musi ukazovat nekam do pameti, nesmi mit hodnotu NULL.
 * @post pamet je uvolnena
 *
 * @param c     Ukazatel na strukturu - shluk.
 */
void clear_cluster(struct cluster_t *c);

/**
 * @brief Funkce zmeni kapacitu shluku ze stavajici na new_cap.
 * 
 * @pre c musi ukazovat nekam do pameti, nesmi mit hodnotu NULL. newcap >= 0
 * @post kapacita je zmenena
 *
 * @param c        Ukazatel na strukturu - shluk.
 * @param new_cap  Nova kapacita shluku
 *
 * @return Funkce vraci ukazatel na strukuturu - shluk, s novou kapacitou.
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap);

/**
 * @brief Na konec pole objektu shluku prida objekt obj.
 * 
 * @pre c musi ukazovat nekam do pameti, nesmi mit hodnotu NULL.
 * @post objekt je pridan na konec pole objektu shluku c
 *
 * @param c     Ukazatel na strukturu - shluk.
 * @param obj   Objekt na pridani.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj);

/**
 * @brief Funkce vlozi vsechny objekty z druheho shluku do prvniho a seradi je.
 * 
 * @pre c1 a c2 musi ukazovat nekam do pameti, nesmi mit hodnotu NULL.
 * @post objekty 2. shluku jsou pridany v 1. shluku.
 *
 * @param c1    Ukazatel na 1. strukturu - shluk.
 * @param c2    Ukazatel na 2. strukturu - shluk.
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2);

/**
 * Funkce odstrani pole objektu a inicializuje kapacitu shluku na 0.
 * 
 * @pre carr musi ukazovat nekam do pameti, nesmi mit hodnotu NULL, idx > 0, narr > 0
 * @post pole je odstraneno a kapacita je nastavena na 0.
 *
 * @param carr  Ukazatel na pole shluku.
 * @param narr  Pocet shluku v poli.
 * @param idx   Index shluku k odstaneni.
 *
 * @return Funkce vraci aktualni pocet shluku v poli.
 */
int remove_cluster(struct cluster_t *carr, int narr, int idx);

/**
 * @brief Funkce zmeri euklidovskou vzalenost dvou objektu.
 * 
 * @pre o1 a o2 musi ukazovat nekam do pameti, nesmi mit hodnotu NULL.
 *
 * @param o1    Ukazatel na 1. objekt.
 * @param o2    Ukazatel na 2. objekt.
 *
 * @return Funkce vraci vypoctenou vzdalenost.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2);

/**
 * @brief Funkce zmeri vzdalenost dvou shluku podle algoritmu
 *        nejvzdalenejsich objektu.
 *        
 * @pre c1 a c2 musi ukazovat nekam do pameti, nesmi mit hodnotu NULL.
 *
 * @param c1    Ukazatel na prvni shluk.
 * @param c2    Ukazatel na druhy shluk.
 *
 * @return Funkce vraci vzdalenost dvou objektu.
 */
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2);

/**
 * @brief Funkce najde dva sobe nejblizsi shluky.
 * 
 * @pre carr, c1, c2 musi ukazovat nekam do pameti, nesmi mit hodnotu NULL. narr > 0. 
 *
 * @param carr  Ukazatel na pole shluku.
 * @param narr  Pocet shluku v poli.
 * @param c1    Ukazatel na index prvniho ze dvojice nalezenych shluku.
 * @param c2    Ukazatel na index druheho ze dvojice nalezenych shluku.
 */
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2);

/**
 * @brief Funkce seradi objekty ve shluku podle jeho ID.
 * 
 * @pre c musi ukazovat nekam do pameti, nesmi mit hodnotu NULL.
 * @post shluk je serazen dle id objektu.
 *
 * @param c     Ukazatel na shluk k serazeni.
 */
void sort_cluster(struct cluster_t *c);

/**
 * @brief Funkce vytiskne vsechny objekty v shluku.
 * 
 * @pre Ve shluku musi byt nejaky objekty.
 * @post Objekty jsou vytisknuty na stan. vystup
 *
 * @param c     Ukazatel na shluk k vytisknuti.
 */
void print_cluster(struct cluster_t *c);

/**
 * @brief Funke nacte objekty ze souboru, kazdemu priradi jeden shluk.
 * 
 * @pre filename a arr musi ukazovat nekam do pameti, nesmi mit hodnotu NULL, soubor musi existovat, 
 * @post *arr obsahuje ukazatel na prvni polozku shluku
 *
 * @param filename  Jmeno souboru.
 * @param arr       Ukazatel na ukazatel na pole shluku.
 *
 * @return Funkce vraci pocet uspesne nactenych objektu.
 */
int load_clusters(char *filename, struct cluster_t **arr);

/**
 * @brief Funkce vytisne vsechny shluky v poli shluku.
 * 
 * @pre carr musi ukazovat nekam do pameti, nesmi mit hodnotu NULL, narr > 0
 * @post shluky jsou vytisknuty na standartni vystup.
 *
 * @param carr  Ukazatel na prvni shluk v poli.
 * @param narr  Pocet shluku v poli.
 */
void print_clusters(struct cluster_t *carr, int narr);

/**
 * @brief Funkce vypise napovedu.
 * 
 * @post Napoveda je vytisknuta na standartni vystup.
 */
void help();

/**
 * @brief Funkce vypise na stderr chybovou hlasku.
 * 
 * @pre err_num musi byt validni cislo ze seznamu chyb
 * @post Chybova hlaska je vytisknuta na standartni vystup.
 *
 * @param err_num  Cislo chyby.
 */
void error(int err_num);

/**
 * @brief Rozhodne zda je v rezeci ulozeno kladne cislo.
 * 
 * @pre s musi ukazovat nekam do pameti, nesmi mit hodnotu NULL.
 *
 * @param s Ukazatel na prvni polozku v rezeci.
 *
 * @return True pokud je pravda, jinak False.
 */
int is_number(char* s);

/**
 * @brief Uvolni vsechny shluky a jejich objekty.
 * 
 * @pre clusters musi ukazovat nekam do pameti, nesmi mit hodnotu NULL, n_clusters > 0
 * @post pole shluku a jejich objekty jsou uvolneny z pameti.
 *
 * @param clusters    Ukazatel na prvni shluk v poli.
 * @param n_clusters  Pocet shluku v poli.
 */
void free_all(struct cluster_t *clusters, int n_clusters);

/** @} */ //konec funkci 
