/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07fd10.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7fd10(void)

{
  uint uVar1;
  byte bVar2;
  byte bVar3;
  int iVar4;
  int extraout_A0;
  undefined4 *unaff_A6;
  bool bVar5;

  func_0x000143e4();
  candidate_427a();
  iVar4 = unaff_A6[0xd] + -0x8000 >> 3;
  if (iVar4 == *(int *)((int)unaff_A6 + 0x82)) {
    *unaff_A6 = candidate_7fdb8;
  }
  unaff_A6[0xd] = unaff_A6[0xd] - iVar4;
  *(int *)((int)unaff_A6 + 0x82) = iVar4;
  uVar1 = unaff_A6[0xc];
  bVar5 = uVar1 < (uint)((int)unaff_A6[0xc] >> 3);
  unaff_A6[0xc] = uVar1 - ((int)unaff_A6[0xc] >> 3);
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
  if (bVar3 == 0 || SBORROW1(bVar2,'\x01') != (int)((uint)bVar3 << 0x18) < 0) {
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
