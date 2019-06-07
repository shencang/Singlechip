
                  ORG    2000H
                  MOV    SP,#40H
                  MOV    A,#30H
                  LCALL  2500H
                  ADD     A,#10H
                  MOV    B,A
                  SJMP    $
                  ORG    2500H
                  MOV    DPTR,#200AH
                  PUSH    DPL
                  PUSH    DPH
                  RET
