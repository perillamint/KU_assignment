typedef struct tagFIS_DATA
{
	// DWORD 0
	BYTE	fis_type;	// FIS_TYPE_DATA
 
	BYTE	pmport:4;	// Port multiplier
	BYTE	rsv0:4;		// Reserved
 
	BYTE	rsv1[2];	// Reserved
 
	// DWORD 1 ~ N
	DWORD	data[1];	// Payload
} FIS_DATA;
