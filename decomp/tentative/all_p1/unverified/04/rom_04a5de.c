/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04a5de.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4a5de(void)

{
  short sVar1;
  undefined4 *unaff_A6;
  bool bVar2;

  if (0xff < (short)(*(short *)(unaff_A6 + 0xf) + 0x30)) {
    *(undefined2 *)(unaff_A6 + 0x22) = 0;
    unaff_A6[0xc] = 0x30000;
    *(undefined2 *)((int)unaff_A6 + 0x8a) = 0x100;
    if (*(short *)((int)unaff_A6 + 0x8a) < *(short *)(unaff_A6 + 0xf)) {
      unaff_A6[0xc] = -unaff_A6[0xc];
    }
    *unaff_A6 = 0x4a974;
    candidate_4e008();
    candidate_1ac38();
    candidate_427a();
    candidate_4e04e();
    if ((short)(*(short *)((int)unaff_A6 + 0x8a) - *(short *)(unaff_A6 + 0xf)) < 0) {
      if (-1 < (int)unaff_A6[0xc]) {
        *unaff_A6 = candidate_4a380;
      }
    }
    else if ((int)unaff_A6[0xc] < 0) {
      *unaff_A6 = candidate_4a380;
    }
    bVar2 = false;
    candidate_4c24();
    if (bVar2) {
      *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    }
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
      *unaff_A6 = candidate_4a9d4;
    }
    return;
  }
  *(short *)((int)unaff_A6 + 0x8a) = *(short *)(unaff_A6 + 10) + 0x30;
  unaff_A6[0xc] = 0x30000;
  if (*(short *)((int)unaff_A6 + 0x8a) < *(short *)(unaff_A6 + 10)) {
    unaff_A6[0xc] = -unaff_A6[0xc];
  }
  *unaff_A6 = 0x4a618;
  candidate_4e008();
  candidate_1ac38();
  candidate_427a();
  candidate_4e04e();
  sVar1 = *(short *)(unaff_A6 + 10) - *(short *)((int)unaff_A6 + 0x8a);
  if (sVar1 < 0) {
    sVar1 = -sVar1;
  }
  if (sVar1 < 4) {
    *unaff_A6 = candidate_4a668;
  }
  bVar2 = false;
  candidate_4c24();
  if (bVar2) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_4a9d4;
  }
  return;
}
