/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x058752.
 * Status: tentative_c_with_diagnostics. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Instruction at (ram,0x00058915) overlaps instruction at (ram,0x00058912)
    */

void candidate_58752(void)

{
  char *pcVar1;
  char cVar2;
  char cVar3;
  undefined2 uVar4;
  short unaff_D3w;
  uint *extraout_A0;
  int unaff_A2;
  int unaff_A4;
  undefined4 *unaff_A6;

  *(byte *)(unaff_A4 + -1) = *(byte *)(unaff_A4 + -1) | 0x74;
  *(byte *)(unaff_A4 + -2) = *(byte *)(unaff_A4 + -2) | 0x34;
  *(byte *)(unaff_A4 + -3) = *(byte *)(unaff_A4 + -3) | 0xa0;
  *(byte *)(unaff_A4 + -4) = *(byte *)(unaff_A4 + -4) | 0xba;
  *(byte *)(unaff_A4 + -5) = *(byte *)(unaff_A4 + -5) | 200;
  *(byte *)(unaff_A4 + -6) = *(byte *)(unaff_A4 + -6) | 0xe2;
  *(byte *)(unaff_A4 + -7) = *(byte *)(unaff_A4 + -7) | 0xc2;
  *(byte *)(unaff_A4 + -8) = *(byte *)(unaff_A4 + -8) | 0xf6;
  *(byte *)(unaff_A4 + -9) = *(byte *)(unaff_A4 + -9) | 0x62;
  *(byte *)(unaff_A4 + -10) = *(byte *)(unaff_A4 + -10) | 0xd4;
  *(byte *)(unaff_A4 + -0xb) = *(byte *)(unaff_A4 + -0xb) | 0xea;
  *(byte *)(unaff_A4 + -0xc) = *(byte *)(unaff_A4 + -0xc) | 0x48;
  *(byte *)(unaff_A4 + -0xd) = *(byte *)(unaff_A4 + -0xd) | 0xdc;
  *(byte *)(unaff_A4 + -0xe) = *(byte *)(unaff_A4 + -0xe) | 0x5c;
  *(byte *)(unaff_A4 + -0xf) = *(byte *)(unaff_A4 + -0xf) | 0x5c;
  *(byte *)(unaff_A4 + -0x10) = *(byte *)(unaff_A4 + -0x10) | 0x5a;
  *(byte *)(unaff_A4 + -0x11) = *(byte *)(unaff_A4 + -0x11) | 0x76;
  candidate_146e8();
  *(undefined2 *)(unaff_A6 + 0x1e) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x8f) = 0;
  candidate_1abc8();
  if (*(short *)(unaff_A6 + 0xf) < 0xa0) {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
  }
  if (*(short *)((int)unaff_A6 + 0x6e) < 2) {
    if (0x200 < *(short *)(unaff_A6 + 0x1c)) goto code_r0x0005886e;
    if (*(short *)((int)unaff_A6 + 0x72) < 2) {
      unaff_A6[0x1f] = candidate_58752;
      *(undefined1 *)(unaff_A6 + 0x24) = 0;
      *(undefined1 *)((int)unaff_A6 + 0x93) = 0;
      goto code_r0x000588c8;
    }
    __m68k_trap(0xf);
  }
  else {
    __m68k_trap(0xf);
    pcVar1 = (char *)(*(int *)(unaff_A2 + 0x204f7074) + unaff_D3w * 4);
    cVar2 = *pcVar1;
    cVar3 = cVar2 + '\x02';
    *pcVar1 = cVar3;
    if (SCARRY1(cVar2,'\x02')) {
                    /* WARNING: Bad instruction - Truncating control flow here */
      halt_baddata();
    }
    if (cVar3 == '\0' || SCARRY1(cVar2,'\x02') != cVar3 < '\0') {
                    /* WARNING: Bad instruction - Truncating control flow here */
      halt_baddata();
    }
code_r0x0005886e:
    __m68k_trap(0xf);
    pcVar1 = (char *)(*(int *)(unaff_A2 + 0x204f7074) + unaff_D3w * 4);
    cVar2 = *pcVar1;
    cVar3 = cVar2 + '\x02';
    *pcVar1 = cVar3;
    if (SCARRY1(cVar2,'\x02')) {
                    /* WARNING: Bad instruction - Truncating control flow here */
      halt_baddata();
    }
    if (cVar3 == '\0' || SCARRY1(cVar2,'\x02') != cVar3 < '\0') {
                    /* WARNING: Bad instruction - Truncating control flow here */
      halt_baddata();
    }
  }
  pcVar1 = (char *)(*(int *)(unaff_A2 + 0x204f7074) + unaff_D3w * 4);
  cVar2 = *pcVar1;
  cVar3 = cVar2 + '\x02';
  *pcVar1 = cVar3;
  if (SCARRY1(cVar2,'\x02')) {
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
  if (cVar3 == '\0' || SCARRY1(cVar2,'\x02') != cVar3 < '\0') {
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
code_r0x000588c8:
  *extraout_A0 = *extraout_A0 | 0x422e0093;
  *(undefined1 *)((int)unaff_A6 + 0x91) = 0;
  *(undefined1 *)(unaff_A6 + 0x25) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x8800;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x8800;
  uVar4 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x4e) = uVar4;
  unaff_A6[0x14] = 0x5868e;
  candidate_4e76();
  candidate_9ea8();
  candidate_4520();
  *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
  candidate_1456a();
  candidate_1abc8();
  if (*(short *)(unaff_A6 + 0x1e) != 2) {
    candidate_40bc();
  }
  if (*(short *)(unaff_A6 + 0x1e) == 1) {
    *(undefined2 *)((int)unaff_A6 + 0x86) = 1;
    if (*(short *)(unaff_A6 + 0x1d) == *(short *)((int)unaff_A6 + 0x76)) {
      candidate_1abc8();
      if (*(short *)(unaff_A6 + 0xf) < 0xa0) {
        *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
      }
      *unaff_A6 = candidate_5899a;
    }
    else {
      *unaff_A6 = candidate_58cbe;
    }
  }
  else {
    *unaff_A6 = candidate_5899a;
  }
  candidate_3c532();
  return;
}
