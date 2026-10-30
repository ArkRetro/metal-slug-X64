/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08ea46.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_8ea46(void)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  undefined1 uVar4;
  undefined4 *unaff_A6;
  undefined1 uVar5;

  if (*(char *)((int)unaff_A6 + 0x92) == '\0') {
    uVar5 = 2;
    uVar4 = 0xff;
  }
  else {
    uVar5 = 3;
    uVar4 = 0;
  }
  *(undefined1 *)((int)unaff_A6 + 0x8d) = uVar5;
  *(undefined1 *)((int)unaff_A6 + 0x92) = uVar4;
  *(undefined1 *)((int)unaff_A6 + 0x8e) = 0;
  candidate_4256();
  *unaff_A6 = 0x8ea82;
  *(undefined2 *)((int)unaff_A6 + 0x26) = 0x100;
  uVar5 = 0;
  candidate_1ac38();
  candidate_427a();
  if ((bool)uVar5) {
    *unaff_A6 = candidate_8e9b8;
  }
  if (*(char *)((int)unaff_A6 + 0x8e) != '\0') {
    *(undefined1 *)((int)unaff_A6 + 0x8e) = 0;
    *(undefined1 *)((int)unaff_A6 + 0x8d) = 5;
  }
  if (((*(short *)((int)unaff_A6 + 0x82) != -0x40) && (*(short *)(unaff_A6 + 0x21) != 0)) &&
     (*(short *)(unaff_A6 + 0x21) < _DAT_0010e99e)) {
    *(undefined2 *)((int)unaff_A6 + 0x82) = 0xffc0;
  }
  uVar1 = *(uint *)((int)unaff_A6 + 0x62);
  if (*(short *)((int)unaff_A6 + 0x82) < *(short *)(unaff_A6 + 0xf)) {
    uVar3 = -uVar1;
    unaff_A6[0xc] = unaff_A6[0xc] - (uVar1 >> 3);
    uVar2 = unaff_A6[0xc];
    if (-uVar2 != uVar1 && (int)uVar3 >= (int)uVar2) {
      unaff_A6[0xc] = uVar3;
    }
    uVar5 = (-uVar2 == uVar1 || (int)uVar3 < (int)uVar2) && uVar3 < uVar2;
  }
  else {
    unaff_A6[0xc] = (uVar1 >> 3) + unaff_A6[0xc];
    uVar5 = uVar1 < (uint)unaff_A6[0xc];
    if ((int)uVar1 < (int)unaff_A6[0xc]) {
      unaff_A6[0xc] = uVar1;
      uVar5 = false;
    }
  }
  candidate_8e15a();
  candidate_8f67c();
  *(byte *)((int)unaff_A6 + 0x93) = *(byte *)((int)unaff_A6 + 0x93) & 0xbf;
  candidate_4c24();
  if ((bool)uVar5) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar5 = 0;
    *(byte *)((int)unaff_A6 + 0x93) = *(byte *)((int)unaff_A6 + 0x93) | 0x40;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_8e0d4;
    uVar5 = 0;
  }
  candidate_8f432();
  if ((bool)uVar5) {
    *unaff_A6 = candidate_8e128;
  }
  return;
}
