/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07fe02.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7fe02(void)

{
  byte bVar1;
  byte bVar2;
  short sVar4;
  int iVar3;
  ushort uVar5;
  int iVar6;
  ushort uVar7;
  int extraout_A0;
  undefined4 *unaff_A6;
  undefined1 in_CF;
  bool bVar8;
  undefined8 uVar9;

  uVar9 = candidate_7fc5e();
  if ((bool)in_CF) {
    sVar4 = candidate_29c6();
    iVar3 = (uint)(ushort)(sVar4 >> 8) << 0x10;
    iVar6 = (uint)(ushort)(short)(char)sVar4 << 0x10;
  }
  else {
    iVar3 = (int)((ulonglong)uVar9 >> 0x20) - unaff_A6[0xf];
    iVar6 = (int)uVar9 - unaff_A6[0x10];
  }
  *(undefined4 *)((int)unaff_A6 + 0x7a) = unaff_A6[0xf];
  *(undefined4 *)((int)unaff_A6 + 0x7e) = unaff_A6[0x10];
  *(int *)((int)unaff_A6 + 0x7a) = iVar3 + *(int *)((int)unaff_A6 + 0x7a);
  *(int *)((int)unaff_A6 + 0x7e) = iVar6 + *(int *)((int)unaff_A6 + 0x7e);
  candidate_2eb4();
  uVar9 = candidate_2e7c();
  *(undefined8 *)((int)unaff_A6 + 0x72) = uVar9;
  candidate_1b1dc();
  *(undefined2 *)((int)unaff_A6 + 0x86) = *(undefined2 *)((int)unaff_A6 + 0x8a);
  *unaff_A6 = 0x7fe70;
  unaff_A6[0xc] = (*(int *)((int)unaff_A6 + 0x72) - unaff_A6[0xc] >> 2) + unaff_A6[0xc];
  unaff_A6[0xd] = (*(int *)((int)unaff_A6 + 0x76) - unaff_A6[0xd] >> 2) + unaff_A6[0xd];
  func_0x000143e4();
  candidate_427a();
  uVar5 = *(short *)((int)unaff_A6 + 0x7a) - *(short *)(unaff_A6 + 0xf);
  if ((int)((uint)uVar5 << 0x10) < 0) {
    uVar5 = -uVar5;
  }
  uVar7 = *(short *)((int)unaff_A6 + 0x7e) - *(short *)(unaff_A6 + 0x10);
  if ((int)((uint)uVar7 << 0x10) < 0) {
    uVar7 = -uVar7;
  }
  if (((short)uVar5 < 9) && ((short)uVar7 < 9)) {
    *unaff_A6 = candidate_7fedc;
  }
  uVar5 = *(ushort *)((int)unaff_A6 + 0x86);
  uVar7 = uVar5 - 1;
  *(ushort *)((int)unaff_A6 + 0x86) = uVar7;
  bVar8 = SBORROW2(uVar5,1) == (int)((uint)uVar7 << 0x10) < 0;
  if (uVar7 == 0 || !bVar8) {
    *unaff_A6 = candidate_7fedc;
  }
  bVar8 = (uVar7 != 0 && bVar8) && uVar5 == 0;
  candidate_4c24();
  if ((bVar8) &&
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
  bVar1 = *(byte *)(unaff_A6 + 0x24);
  bVar2 = bVar1 - 1;
  *(byte *)(unaff_A6 + 0x24) = bVar2;
  if (bVar2 == 0 || SBORROW1(bVar1,'\x01') != (int)((uint)bVar2 << 0x18) < 0) {
    bVar1 = *(byte *)((int)unaff_A6 + 0x91);
    *(byte *)((int)unaff_A6 + 0x91) = bVar1 ^ 1;
    if ((bVar1 & 1) == 0) {
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
