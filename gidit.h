#ifndef GIDIT_H
#define GIDIT_H
#include "chimera.h"

#define PUSHOBJ_DIR "pushobjects"
#define BUNDLES_DIR "bundles"

#define END_SHA1 "0000000000000000000000000000000000000000"

#define INCLUDE_TAGS 0x0001
#define SIGN 0x0004

#define DEFAULT_CHIMERA_PORT 2323
#define DEFAULT_LITSEN_PORT 9898

#define GIDIT_PUSH_MSG 0
#define GIDIT_PUSHF_MSG 1

#define GIDIT_OK 0
#define GIDIT_UNKNOWN_PROJ 1

struct gidit_projdir {
	char * basepath;
	int pgp_len;
	unsigned char * pgp;
	unsigned char pgp_sha1[20];
	char * userdir;
	char * projdir;
	char * projname;
	char head[41];
};

typedef struct chat_m {
	int pid;
	Key source;
	char message[1000];
} chat_message;

typedef struct push_m {
	char force;
	uint32_t pid;
	Key source;
	uint32_t name_len;
	uint32_t pgp_len;
	char buf[];
} push_message;

typedef struct return_m {
	uint32_t pid;
	uint32_t return_val;
	unsigned char pgp[20];
	uint32_t buf_len;
	char buf[];
} return_message;

struct gidit_pushobj {
	int lines;
	char ** refs;
	char * signature;
	char head[41];
	char prev[41];
};

#define PO_INIT { 0, NULL, NULL, "\0" }

struct gidit_projdir * new_projdir(const char * basepath, const char * sha1_hex, const char * projname);

void free_projdir(struct gidit_projdir* pd);

/**
 * Generate a pushobj, which is a list of all refs including HEAD, and
 * excluding stashes and remotes
 */
int gidit_pushobj(FILE *fp, char * signingkey, unsigned int flags);

/**
 * Initialize a gidit directory, that means creating the PUSHOBJ_DIR and 
 * BUNDLES_DIR
 */
int gidit_init(const char *path);

/**
 * Initialize a user's project directory
 */
int gidit_proj_init(const char * basepath, int pgp_len, const unsigned char * pgp_key, const char * projname, unsigned int flags);
int gidit_proj_init_stream(FILE *fp, const char * basepath, unsigned int flags);

/**
 * Read pushobj from fp and save, will need to take
 */
int gidit_update_pl(FILE *fp, const char * basepath, unsigned int flags);

/**
 */
char * gidit_po_list(const char * basepath, const char * pgp_sha1, const char * projname);

/**
 * Generate pushobject list, file stream version
 */
int gidit_po_list_stream(FILE *fp, const char * basepath, unsigned int flags);

/**
 * save a bundle
 */
int gidit_store_bundle(FILE *fp, const char * basepath, unsigned int flags);

/**
 * get a bundle
 */
int gidit_get_bundle(FILE *fp,  FILE * out, const char * basepath, unsigned int flags);

/**
 * Verify if a pushobject would apply cleanly to repo
 */
int gidit_verify_pushobj(FILE *fp, unsigned int flags);


/**
 * Generate a bundle read from stdin
 */
int gidit_gen_bundle(FILE *fp, unsigned int flags);

/**
 * push stuff out to DHT
 */
int gidit_push(const char * url, int refspec_nr, const char ** refspec, unsigned int flags);

/**
 * Function for reading pushobjects
 */
int gidit_read_pushobj(FILE * fp, struct gidit_pushobj *po);

/**
 * Function takes projdir, number of pushobjects, and polist
 *
 * Updates projdirs polist, and verifies that it has a subset
 */
int gidit_update_pushobj_list(struct gidit_projdir * pd, int num_po, struct gidit_pushobj ** polist);

/**
 * Given a buffer containing a pushobj, construct po, and return pointer in buf after pushobj
 */
const char * str_to_pushobj(const char *buf, struct gidit_pushobj * po);

/**
 * Take a buf, and a pointer to a **polist, from buf, construct a whole bunch of polist items
 * return the number created
 */
int str_to_polist(const char * buf, struct gidit_pushobj ***polist);

#endif		// GIDIT_H
