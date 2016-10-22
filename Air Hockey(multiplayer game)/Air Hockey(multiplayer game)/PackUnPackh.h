//Macros for packing and unpacking data into and out of a buffer

//************************************************************
//PACKING
//************************************************************

#define PackBytes(bf,bytes,len)\
	memcpy(bf, bytes, len);\
	bf += len;

//Pack a long(32 bits).... network byte order... most sig value added to buffer first

#define PackLong(bf,l)\
	*bf++ = (l >> 24) & 0xFF;\
	*bf++ = (l >> 16) & 0xFF;\
	*bf++ = (l >> 8) & 0xFF;\
	*bf++ = l & 0xff;

//Pack a short(16 bits)
#define PackShort(bf,sh)\
	*bf++ = (sh >> 8) & 0xFF;\
	*bf++ = sh & 0xFF;

//Pack a byte (8 bits) ... just of consistency
#define PackByte(bf,by)\
	*bf++ = by;


//********************************************************
//UNPACKING
//*******************************************************

#define UnPackBytes(bf, bytes, len)\
	memcpy(bytes, bf, len);\
	bf += len;


//UnPack a long (32 bit)
#define UnPackLong(bf,lo)\
	lo = (*bf++ << 24);\
	lo += (*bf++ << 16);\
	lo += (*bf++ << 8);\
	lo += *bf++;


//UnPack a short (16 bits)
#define UnPackShort(bf,sh)\
	sh = (*bf++ << 8);\
	sh += *bf++;

// UnPack a byte(8 bits).. just for consistency
#define UnPackByte(bf,by)\
	by = *bf++;// shouldn't it be *bf+

#define MANTISMUL 10000//for 4 decimal placess
// was to 6 but loss of accuracy