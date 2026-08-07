;
; WSTUB.ASM
;

;DOSSEG    Directiva ignorada pel compilador
.MODEL SMALL
.STACK 80h

.DATA

INCLUDE RES\STUBMSG.ASI

.CODE

  MOV  AX,@data
  MOV  DS,AX
  MOV  AH,9
  MOV  DX,OFFSET Message
  INT  21h
  MOV  AH,4Ch
  INT  21h

END
