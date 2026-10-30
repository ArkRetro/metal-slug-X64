/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04940c.
 * Status: tentative_c_with_diagnostics. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Removing unreachable block (ram,0x00049485) */

void candidate_4940c(void)

{
  ushort uVar1;
  short sVar2;
  int unaff_A6;
  bool bVar3;

  if (0x13 < *(ushort *)(unaff_A6 + 0x6e)) {
    __m68k_trap(0xf);
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
  *(ushort *)(unaff_A6 + 0x7a) = *(ushort *)(unaff_A6 + 0x6e) * 0x10 + 0x100;
  bVar3 = false;
  uVar1 = candidate_1ae8e();
  if (bVar3) {
    return;
  }
  bVar3 = uVar1 < 3;
  if (uVar1 == 3) {
    candidate_1af3c();
    if (!bVar3) {
      sVar2 = sRam00101500 - sRam001015b0;
      if (-1 < sVar2) {
        if (sVar2 < 0x60) {
          sVar2 = sRam00101500 + 0x30;
          if (0x120 < sVar2) {
            sVar2 = sRam001015b0 + -0x30;
          }
        }
        else {
          if ((*(byte *)(unaff_A6 + 0x15) & 1) == 0) {
            sVar2 = sRam00101500 + -0x30;
          }
          else {
            sVar2 = sRam00101500 + 0x30;
          }
          if (sVar2 < 0x20) {
            sVar2 = sRam00101500 + 0x30;
          }
          if (0x120 < sVar2) {
            sVar2 = sRam00101500 + -0x30;
          }
        }
        *(short *)(unaff_A6 + 0x78) = sVar2;
        if ((*(byte *)(unaff_A6 + 0x15) & 1) == 0) {
          if (*(short *)(unaff_A6 + 0x3c) < sVar2) {
            return;
          }
        }
        else if (sVar2 < *(short *)(unaff_A6 + 0x3c)) {
          return;
        }
        return;
      }
      if (-sVar2 < 0x60) {
        sVar2 = sRam00101500 + -0x30;
        if (sVar2 < 0x20) {
          sVar2 = sRam001015b0 + 0x30;
        }
      }
      else {
        if ((*(byte *)(unaff_A6 + 0x15) & 1) == 0) {
          sVar2 = sRam00101500 + -0x30;
        }
        else {
          sVar2 = sRam00101500 + 0x30;
        }
        if (sVar2 < 0x20) {
          sVar2 = sRam00101500 + 0x30;
        }
        if (0x120 < sVar2) {
          sVar2 = sRam00101500 + -0x30;
        }
      }
      *(short *)(unaff_A6 + 0x78) = sVar2;
      if ((*(byte *)(unaff_A6 + 0x15) & 1) == 0) {
        if (*(short *)(unaff_A6 + 0x3c) < sVar2) {
          return;
        }
      }
      else if (sVar2 < *(short *)(unaff_A6 + 0x3c)) {
        return;
      }
      return;
    }
    candidate_1af3c();
    if (!bVar3) {
      sVar2 = sRam001015b0 - sRam00101500;
      if (-1 < sVar2) {
        if (sVar2 < 0x60) {
          sVar2 = sRam001015b0 + 0x30;
          if (0x120 < sVar2) {
            sVar2 = sRam00101500 + -0x30;
          }
        }
        else {
          if ((*(byte *)(unaff_A6 + 0x15) & 1) == 0) {
            sVar2 = sRam001015b0 + -0x30;
          }
          else {
            sVar2 = sRam001015b0 + 0x30;
          }
          if (sVar2 < 0x20) {
            sVar2 = sRam001015b0 + 0x30;
          }
          if (0x120 < sVar2) {
            sVar2 = sRam001015b0 + -0x30;
          }
        }
        *(short *)(unaff_A6 + 0x78) = sVar2;
        if ((*(byte *)(unaff_A6 + 0x15) & 1) == 0) {
          if (*(short *)(unaff_A6 + 0x3c) < sVar2) {
            return;
          }
        }
        else if (sVar2 < *(short *)(unaff_A6 + 0x3c)) {
          return;
        }
        return;
      }
      if (-sVar2 < 0x60) {
        sVar2 = sRam001015b0 + -0x30;
        if (sVar2 < 0x20) {
          sVar2 = sRam00101500 + 0x30;
        }
      }
      else {
        if ((*(byte *)(unaff_A6 + 0x15) & 1) == 0) {
          sVar2 = sRam001015b0 + -0x30;
        }
        else {
          sVar2 = sRam001015b0 + 0x30;
        }
        if (sVar2 < 0x20) {
          sVar2 = sRam001015b0 + 0x30;
        }
        if (0x120 < sVar2) {
          sVar2 = sRam001015b0 + -0x30;
        }
      }
      *(short *)(unaff_A6 + 0x78) = sVar2;
      if ((*(byte *)(unaff_A6 + 0x15) & 1) == 0) {
        if (*(short *)(unaff_A6 + 0x3c) < sVar2) {
          return;
        }
      }
      else if (sVar2 < *(short *)(unaff_A6 + 0x3c)) {
        return;
      }
      return;
    }
  }
  else {
    bVar3 = uVar1 == 0;
    if (uVar1 == 1) {
      candidate_1af3c();
      if (!bVar3) {
        candidate_4976a();
        return;
      }
    }
    else {
      candidate_1af3c();
      if (!bVar3) {
        candidate_4976a();
        return;
      }
    }
  }
  return;
}
