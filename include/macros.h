#ifndef MYMACROS
#define MYMACROS
#define BASE_NODE char name[32];\
    unsigned int age;\
    BloodType bt;\
    float dntbld;
#define BDSWITCH(BDT,T,NT,A,NA,H) switch(BDT){ \
	case tree:\
		T\
		break;\
	case ntree:\
		NT\
		break;\
	case avl:\
		A\
		break;\
	case navl:\
		NA\
		break;\
	case hashtable:\
		H\
		break;\
}
#endif