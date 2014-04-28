extern int height;
extern int width;
extern char * map;
extern int difficulty;
extern int is_pacman_powered_up;
extern struct pacghost pacman;
/* This function return a direction of pacman from ghost point of view
   -1 mean no face to face
   0 - top, 1 - right, 2 - bottom, 3 - left
   @param map the char pointer to teh map array
   @pacghost ghost the ghost
   @int difficulty the difficulty of ghost
   @int isPower 1 - ghost is save, 2 - ghost is in danger
 */
int face2face (struct pacghost * ghost);
/* This function find the better direction to the pacman
   @param map the char pointer to teh map array
   @pacghost ghost the ghost
   @int difficulty the difficulty of ghost
   @int isPower 1 - ghost is save, 2 - ghost is in danger
 */
void directToPacman(struct pacghost * ghost);
int findRelativePosition(struct pacghost * ghost);
int isSurround(struct pacghost * ghost);
void nguyenvinhlinh_ai(struct pacghost * ghost);
