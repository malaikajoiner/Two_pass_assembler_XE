.  SOURCE CODE FOR THE XE VERSION OF THE SIC FAMILY OF COMPUTER
   0  000000  SUM       START     0                   
   0  000000  FIRST     LDX       #0                  050000
   0  000003            LDA       #0                  010000
   0  000006            +LDB      #TABLE2             69101794
   0  00000A            BASE      TABLE2              
   0  00000A  LOOP      ADD       TABLE,X             1BA017
   0  00000D            ADD       TABLE2,X            1BC000
   0  000010            TIX       COUNT               2F200E
   0  000013            JLT       LOOP                3B2FF4
   0  000016            +STA      TOTAL               0F102F04
   0  00001A            RSUB                          4F0000
   0  00001D  MYLIT     +LDA      =C'EOF'             
   0  000021  COUNT     RESW      1                   
   0  000024  TABLE     RESW      2000                
   0  001794  TABLE2    RESW      2000                
   0  002F04  TOTAL     RESW      1                   
   0  002F07  *         =C'EOF'                       454F46
   0  002F0B            END       FIRST               
