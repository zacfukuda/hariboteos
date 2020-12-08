[BITS 32]
  MOV   AL, 'A'
  CALL  0xc64
fin:
  HLT
  JMP   fin
