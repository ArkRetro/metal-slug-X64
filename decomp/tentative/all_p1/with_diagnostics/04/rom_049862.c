/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x049862.
 * Status: tentative_c_with_diagnostics. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Removing unreachable block (ram,0x000498db) */

void candidate_49862(void)

{
  short sVar1;
  short sVar2;
  byte bVar3;
  int unaff_A6;
  bool bVar4;

  if (*(ushort *)(unaff_A6 + 0x6e) < 0x14) {
    *(ushort *)(unaff_A6 + 0x7a) = *(ushort *)(unaff_A6 + 0x6e) * 0x10 + 0x100;
    bVar4 = false;
    sVar1 = candidate_1ae8e();
    if (bVar4) {
      *(undefined2 *)(unaff_A6 + 0x78) = 0x20;
    }
    else if (sVar1 == 3) {
      sVar1 = sRam00101500 + -0xa0;
      bVar4 = sVar1 < 0;
      if (bVar4) {
        sVar1 = -sVar1;
      }
      sVar2 = sRam001015b0 + -0xa0;
      bVar3 = bVar4;
      if (sVar2 < 0) {
        sVar2 = -sVar2;
        bVar3 = 2;
      }
      if (sVar1 < sVar2) {
        if ((bVar3 & 2) == 0) {
          *(undefined2 *)(unaff_A6 + 0x78) = 0x20;
        }
        else {
          *(undefined2 *)(unaff_A6 + 0x78) = 0x120;
        }
      }
      else if (bVar4) {
        *(undefined2 *)(unaff_A6 + 0x78) = 0x120;
      }
      else {
        *(undefined2 *)(unaff_A6 + 0x78) = 0x20;
      }
    }
    else if (sVar1 == 1) {
      if (sRam00101500 < 0xa0) {
        *(undefined2 *)(unaff_A6 + 0x78) = 0x120;
      }
      else {
        *(undefined2 *)(unaff_A6 + 0x78) = 0x20;
      }
    }
    else if (sRam001015b0 < 0xa0) {
      *(undefined2 *)(unaff_A6 + 0x78) = 0x120;
    }
    else {
      *(undefined2 *)(unaff_A6 + 0x78) = 0x20;
    }
    return;
  }
  __m68k_trap(0xf);
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}
