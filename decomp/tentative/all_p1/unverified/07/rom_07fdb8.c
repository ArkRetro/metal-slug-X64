/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07fdb8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7fdb8(void)

{
  short sVar1;
  byte bVar2;
  byte bVar3;
  short sVar4;
  undefined4 extraout_D1;
  int extraout_A0;
  undefined4 *unaff_A6;
  bool bVar5;

  candidate_1b1dc();
  *(undefined2 *)((int)unaff_A6 + 0x86) = *(undefined2 *)(unaff_A6 + 0x22);
  *unaff_A6 = 0x7fdca;
  func_0x000143e4();
  candidate_427a();
  *(char *)((int)unaff_A6 + 0x92) = *(char *)((int)unaff_A6 + 0x92) + '\x04';
  candidate_2e7c();
  unaff_A6[0xd] = extraout_D1;
  sVar1 = *(short *)((int)unaff_A6 + 0x86);
  sVar4 = sVar1 + -1;
  *(short *)((int)unaff_A6 + 0x86) = sVar4;
  if (sVar4 == 0 || 0 >= sVar1) {
    *unaff_A6 = candidate_7fe02;
  }
  bVar5 = (sVar4 != 0 && 0 < sVar1) && sVar1 == 0;
  candidate_4c24();
  if ((bVar5) &&
     (*(undefined1 *)((int)unaff_A6 + 0x5a) = 0, (*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0)) {
    *unaff_A6 = candidate_7ffb2;
  }
  candidate_49e8();
  if (extraout_A0 != 0) {
    *unaff_A6 = candidate_7ffb2;
  }
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x80) != 0) {
    *unaff_A6 = candidate_7ffb2;
  }
  bVar2 = *(byte *)(unaff_A6 + 0x24);
  bVar3 = bVar2 - 1;
  *(byte *)(unaff_A6 + 0x24) = bVar3;
  if (bVar3 == 0 || SBORROW1(bVar2,'\x01') != (short)((ushort)bVar3 << 8) < 0) {
    bVar2 = *(byte *)((int)unaff_A6 + 0x91);
    *(byte *)((int)unaff_A6 + 0x91) = bVar2 ^ 1;
    if ((bVar2 & 1) == 0) {
      *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x19);
    }
    else {
      *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x1a);
    }
    *(undefined1 *)(unaff_A6 + 0x24) = 8;
  }
  if ((*(char *)(unaff_A6 + 0x25) != '\0') && (-1 < *(short *)(unaff_A6 + 0xd))) {
    *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ef;
    *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ef;
    *(undefined1 *)(unaff_A6 + 0x25) = 0;
  }
  candidate_80152();
  return;
}
