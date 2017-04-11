#include <DxLib.h>

#define SAFE_RELEASE(x) if( x != NULL ){ x->Release(); x = NULL; }
#define SAFE_DESTROY(x) if( x != NULL ){ x->Destroy(); x = nullptr; }
#define SAFE_DELETE(x)  if( x != NULL ){ delete x;  x = NULL; }
#define SAFE_DELETE_ARRAY(x)  if( x != NULL ){ delete[] x;  x = NULL; }
#define SAFE_FREE(x)  if( x != NULL ){ free( x );  x = NULL; }