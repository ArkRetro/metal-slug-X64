/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08e9b8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_8e9b8(void)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  undefined2 uVar4;
  undefined1 uVar5;
  undefined4 *unaff_A6;

  candidate_4256();
  if (*(char *)((int)unaff_A6 + 0x8f) < '\x01') {
    uVar4 = candidate_260c8();
    *(undefined2 *)((int)unaff_A6 + 0x7e) = uVar4;
    uVar5 = candidate_260c8();
    *(undefined1 *)((int)unaff_A6 + 0x8f) = uVar5;
    *(undefined1 *)((int)unaff_A6 + 0x8d) = 1;
  }
  else {
    uVar4 = candidate_260c8();
    *(undefined2 *)((int)unaff_A6 + 0x7e) = uVar4;
    *(char *)((int)unaff_A6 + 0x8f) = *(char *)((int)unaff_A6 + 0x8f) + -1;
  }
  *unaff_A6 = 0x8ea1c;
  *(undefined2 *)((int)unaff_A6 + 0x26) = 0x400;
  candidate_1ac38();
  candidate_427a();
  *(short *)((int)unaff_A6 + 0x7e) = *(short *)((int)unaff_A6 + 0x7e) + -1;
  if (*(short *)((int)unaff_A6 + 0x7e) < 1) {
    *unaff_A6 = candidate_8ea46;
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
