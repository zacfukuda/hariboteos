[BITS 32]
  MOV   AL, 'A'
  CALL  2*8:0xc64
fin:
  HLT
  JMP   fin
