/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x029728.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_29728(void)

{
  uint uVar1;
  short sVar2;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  if (((*(short *)((int)unaff_A6 + 0x7a) != -0x40) && (*(short *)(unaff_A6 + 0x1f) != 0)) &&
     (*(short *)(unaff_A6 + 0x1f) < _DAT_0010e99e)) {
    *(undefined2 *)((int)unaff_A6 + 0x7a) = 0xffc0;
    *unaff_A6 = 0x29754;
  }
  candidate_1ac38();
  candidate_427a();
  *(undefined2 *)((int)unaff_A6 + 0x26) = 0x400;
  uVar1 = *(uint *)((int)unaff_A6 + 0x62);
  if (*(short *)((int)unaff_A6 + 0x7a) < *(short *)(unaff_A6 + 0xf)) {
    unaff_A6[0xc] = unaff_A6[0xc] - (uVar1 >> 3);
    if (-unaff_A6[0xc] != uVar1 && (int)unaff_A6[0xc] <= (int)-uVar1) {
      unaff_A6[0xc] = -uVar1;
    }
  }
  else {
    unaff_A6[0xc] = (uVar1 >> 3) + unaff_A6[0xc];
    if ((int)uVar1 < (int)unaff_A6[0xc]) {
      unaff_A6[0xc] = uVar1;
    }
  }
  if ((*(byte *)(unaff_A6 + 0x21) & 4) != 0) {
    sVar2 = *(short *)(unaff_A6 + 0xf) - *(short *)((int)unaff_A6 + 0x7a);
    if (sVar2 < 0) {
      sVar2 = -sVar2;
    }
    if (sVar2 < 8) {
      *(byte *)(unaff_A6 + 0x21) = *(byte *)(unaff_A6 + 0x21) & 0xfb;
    }
  }
  if (((*(byte *)(unaff_A6 + 0x21) & 0x40) == 0) &&
     (*(short *)((int)unaff_A6 + 0x4e) < *(short *)((int)unaff_A6 + 0x7e))) {
    candidate_4256();
    *(byte *)(unaff_A6 + 0x21) = *(byte *)(unaff_A6 + 0x21) | 0x40;
    candidate_29ae4();
  }
  candidate_299e6();
  uVar3 = false;
  if (unaff_A6[0x14] != 0) {
    candidate_4c24();
    if ((bool)uVar3) {
      if (*(char *)((int)unaff_A6 + 0x5a) == '\x05') {
        *unaff_A6 = candidate_294a2;
      }
      *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
      uVar3 = 0;
    }
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
      *unaff_A6 = candidate_294a2;
      uVar3 = 0;
    }
  }
  candidate_c8bec();
  if ((bool)uVar3) {
    *unaff_A6 = 0x295bc;
  }
  return;
}
