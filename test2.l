.  SOURCE CODE FOR THE XE VERSION OF THE SIC FAMILY OF COMPUTER
   0  000000  TEST2     START     0                   
   0  000000            LDB       #TABLE              69200A
   0  000003            BASE      TABLE               
   0  000003            LDA       @VALUE              022FFA
   0  000006            NOBASE                        
   0  000006            FIX                           C4
   0  000007            RSUB                          4F0000
   0  00000A  VALUE     WORD      5                   000005
   0  00000D  TABLE     RESW      1                   
   0  000010            END       TEST2               
