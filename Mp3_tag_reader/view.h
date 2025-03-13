
typedef struct 
{
    char *version; /**< Version of the ID3 tag */
    char *title;   /**< Title of the song */
    char *artist;  /**< Artist of the song */
    char *album;   /**< Album name */
    char *year;    /**< Year of release */
    char *comment; /**< Comment */
    char *content;   /**< content */
    FILE*fptr_image; 
} TagData;
typedef enum
{
    success,
    failure
} Status;

/*To check the TAG*/
int check_ID3_present(TagData*data);

void display_help();

/*To validate a tags*/
Status view_tags(char*argv[],TagData*data);

/*To read and validate*/
int read_validate(char**argv,TagData*data);

/*To check the version*/
int check_TIT2_tag(TagData*data);

/*To convert the size big endian to little endian*/
int convert_big_little_endian(int ,char*);

/*To check TPE1 tag*/
int check_TPE1_tag(TagData*data);

/*TO check TALB is tag*/
int check_TALB_tag(TagData*data);

/*To check TYER TAG*/
int check_TYER_tag(TagData*data);

/*To check TCON Tag*/
int check_TCON_tag(TagData*data);

/*To check COMM TAG*/
int check_COMM_tag(TagData*data);

