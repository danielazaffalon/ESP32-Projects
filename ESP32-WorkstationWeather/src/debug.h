#define DEBUG true

#if DEBUG
  #define DEBUG_PRINT if(1) //Cambiar por DEBUGLOG
#else
  #define DEBUG_PRINT if(0)
#endif