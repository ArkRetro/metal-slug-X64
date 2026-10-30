/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0265e2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_265e2(void)

{
  undefined2 uVar1;
  short sVar2;
  undefined4 *unaff_A6;

  if ((*(byte *)((int)unaff_A6 + 0x8f) & 0x20) == 0) {
    uVar1 = candidate_260c8();
    *(undefined2 *)(unaff_A6 + 0x1d) = uVar1;
    uVar1 = candidate_260c8();
    *(undefined2 *)((int)unaff_A6 + 0x76) = uVar1;
    sVar2 = candidate_260c8();
  }
  else {
    uVar1 = candidate_260c8();
    *(undefined2 *)(unaff_A6 + 0x1d) = uVar1;
    uVar1 = candidate_260c8();
    *(undefined2 *)((int)unaff_A6 + 0x76) = uVar1;
    sVar2 = candidate_260c8();
  }
  unaff_A6[0xd] = (int)sVar2 << 8;
  candidate_27a32();
  if ((*(byte *)((int)unaff_A6 + 0x8f) & 0x80) == 0) {
    candidate_4256();
  }
  else {
    candidate_4256();
  }
  candidate_120f8();
  sVar2 = *(short *)(unaff_A6 + 0x10);
  candidate_1abc8();
  *(short *)(unaff_A6 + 0xb) = (sVar2 - *(short *)(unaff_A6 + 0x10)) + *(short *)(unaff_A6 + 0xb);
  candidate_1abc8();
  *unaff_A6 = 0x266b2;
  candidate_1ac38();
  if (*(short *)(unaff_A6 + 0x10) < 0xf0) {
    candidate_1abc8();
    *(short *)(unaff_A6 + 0xb) = (0xf0 - *(short *)(unaff_A6 + 0x10)) + *(short *)(unaff_A6 + 0xb);
    candidate_1abc8();
  }
  *(short *)(unaff_A6 + 0x1d) = *(short *)(unaff_A6 + 0x1d) + -1;
  if (*(short *)(unaff_A6 + 0x1d) < 1) {
    *unaff_A6 = candidate_26714;
  }
  if (*(short *)((int)unaff_A6 + 0x76) <= *(short *)(unaff_A6 + 0x10)) {
    *unaff_A6 = candidate_26714;
  }
  if (0x15f < *(short *)(unaff_A6 + 0x10)) {
    *unaff_A6 = candidate_26714;
  }
  candidate_49e8();
  candidate_427a();
  if (*(short *)((int)unaff_A6 + 0x4e) < *(short *)((int)unaff_A6 + 0x7e)) {
    *(byte *)((int)unaff_A6 + 0x8f) = *(byte *)((int)unaff_A6 + 0x8f) | 0x80;
  }
  if ((*(byte *)((int)unaff_A6 + 0x8f) & 0x10) == 0) {
    _DAT_0010e9ba = 0x1a0;
  }
  else {
    _DAT_0010e9ba = 0x110;
  }
  candidate_19388();
  if (_DAT_0010e99a == 0) {
    *(byte *)((int)unaff_A6 + 0x8f) = *(byte *)((int)unaff_A6 + 0x8f) & 0xf7;
    *(undefined1 *)((int)unaff_A6 + 0x8d) = 0;
  }
  else {
    *(byte *)((int)unaff_A6 + 0x8f) = *(byte *)((int)unaff_A6 + 0x8f) | 8;
    *(char *)((int)unaff_A6 + 0x8d) = *(char *)((int)unaff_A6 + 0x8d) + '\x01';
  }
  candidate_4c24();
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  if (0x7e00 < _DAT_0010e99e) {
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x80;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_26c30;
  }
  candidate_27bd4();
  *(undefined1 *)((int)unaff_A6 + 0x8e) = *(undefined1 *)((int)unaff_A6 + 0x5e);
  *(undefined1 *)((int)unaff_A6 + 0x5e) = 0;
  return;
}
