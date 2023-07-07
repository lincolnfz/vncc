
/*****************************/  
/**** Function Prototypes ****/  
/*****************************/  

unsigned long int ft(  
	int t,  
	unsigned long int x,  
	unsigned long int y,  
	unsigned long int z  
	);  

unsigned long int k(int t);

void sha1   (  
	unsigned char *message,  
	int message_length,  
	unsigned char *digest  
	);  

void hmac_sha1(  
	unsigned char *key,  
	int key_length,  
	unsigned char *data,  
	int data_length,  
	unsigned char *digest  
	)  ;

unsigned long int rotl(int bits, unsigned long int a) ;

unsigned long int rotr(int bits, unsigned long int a);