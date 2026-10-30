/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0263b6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_263b6(void)

{
  short sVar1;
  undefined4 *unaff_A6;
  bool bVar2;

  unaff_A6[0xd] = 0;
  candidate_1b1dc();
  candidate_27848();
  *(undefined2 *)(unaff_A6 + 0x1d) = 6;
  candidate_4256();
  *unaff_A6 = 0x263e0;
  sVar1 = *(short *)(unaff_A6 + 0x1d);
  bVar2 = sVar1 == 0;
  *(short *)(unaff_A6 + 0x1d) = sVar1 + -1;
  candidate_27890();
  if ((bVar2) && (*(short *)(unaff_A6 + 0x1d) < 1)) {
    *unaff_A6 = candidate_26400;
  }
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
