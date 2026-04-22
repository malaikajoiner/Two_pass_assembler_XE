.  SOURCE CODE FOR THE XE VERSION OF THE SIC FAMILY OF COMPUTER
   0  000000  TEST3     START     0                   
   0  000000            LDT       ELEVEN              77201F
   0  000003            LDX       #0                  050000
   0  000006  MOVECH    LDCH      STR1,               
   0  000009            STCH      STR2,               
   0  00000C            TIXR      T                   B850
   0  00000E            JLT       MOVECH              3B2FF5
   0  000011            RSUB                          4F0000
   0  000014  STR1      BYTE      C'TEST              
   0  000017  STR2      RESB      11                  
   0  000022  ELEVEN    WORD      11                  00000B
   0  000025            END       TEST3               
