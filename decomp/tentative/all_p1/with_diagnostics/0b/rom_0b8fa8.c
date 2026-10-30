/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0b8fa8.
 * Status: tentative_c_with_diagnostics. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Control flow encountered bad instruction data */

void candidate_b8fa8(void)

{
  code *pcVar1;
  int iVar2;
  char cVar3;
  short sVar4;
  int unaff_A5;
  undefined4 *unaff_A6;

  sVar4 = *(short *)(unaff_A6 + 0x20) + -1;
  if (sVar4 == 0 || *(short *)(unaff_A6 + 0x20) < 1) {
    iVar2 = *(int *)((int)unaff_A6 + 0x6e);
    sVar4 = (ushort)*(byte *)(iVar2 + 1 + (int)*(short *)((int)unaff_A6 + 0x76)) +
            *(short *)((int)unaff_A6 + 0x76);
    cVar3 = *(char *)(iVar2 + sVar4);
    if (cVar3 == -1) {
      candidate_4126();
      iVar2 = *(int *)(unaff_A5 + -0x1c24);
      do {
        unaff_A6 = (undefined4 *)unaff_A6[2];
        pcVar1 = (code *)*unaff_A6;
        *(undefined4 *)(iVar2 + -4) = 0x424a;
        (*pcVar1)();
      } while( true );
    }
    if (cVar3 == '\x01') {
      *(short *)((int)unaff_A6 + 0x76) = sVar4;
      *(undefined2 *)(unaff_A6 + 0x20) = *(undefined2 *)(iVar2 + 2 + (int)sVar4);
      candidate_c70fc();
    }
    else if (cVar3 == '\x01') {
      __m68k_trap(0xf);
                    /* WARNING: Bad instruction - Truncating control flow here */
      halt_baddata();
    }
  }
  else {
    *(short *)(unaff_A6 + 0x20) = sVar4;
  }
  return;
}
