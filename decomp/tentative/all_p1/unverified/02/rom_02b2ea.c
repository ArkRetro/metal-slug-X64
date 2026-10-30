/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x02b2ea.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_2b2ea(void)

{
  uint uVar1;
  short sVar2;
  undefined4 *unaff_A6;
  bool bVar3;

  if (((*(short *)((int)unaff_A6 + 0x7e) != -0x40) && (*(short *)(unaff_A6 + 0x20) != 0)) &&
     (*(short *)(unaff_A6 + 0x20) < _DAT_0010e99e)) {
    *(undefined2 *)((int)unaff_A6 + 0x7e) = 0xffc0;
    *unaff_A6 = 0x2b316;
  }
  candidate_1ac38();
  candidate_1ad8c();
  candidate_427a();
  uVar1 = *(uint *)((int)unaff_A6 + 0x62);
  if (*(short *)((int)unaff_A6 + 0x7e) < *(short *)(unaff_A6 + 0xf)) {
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
  if ((*(byte *)((int)unaff_A6 + 0x91) & 4) != 0) {
    sVar2 = *(short *)(unaff_A6 + 0xf) - *(short *)((int)unaff_A6 + 0x7e);
    if (sVar2 < 0) {
      sVar2 = -sVar2;
    }
    if (sVar2 < 8) {
      *(byte *)((int)unaff_A6 + 0x91) = *(byte *)((int)unaff_A6 + 0x91) & 0xfb;
    }
  }
  if ((*(byte *)((int)unaff_A6 + 0x91) & 1) != 0) {
    candidate_2c170();
  }
  *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
  candidate_4c24();
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_2ae46;
  }
  bVar3 = false;
  candidate_1945c();
  if (bVar3) {
    *unaff_A6 = candidate_2b138;
  }
  return;
}
