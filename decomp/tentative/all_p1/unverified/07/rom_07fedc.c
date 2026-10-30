/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07fedc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7fedc(void)

{
  ushort uVar1;
  byte bVar2;
  byte bVar3;
  ushort uVar4;
  uint uVar5;
  uint uVar6;
  int extraout_A0;
  undefined4 *unaff_A6;
  undefined1 uVar7;

  uVar5 = (int)unaff_A6[0xc] >> 2;
  unaff_A6[0xc] = unaff_A6[0xc] - uVar5;
  uVar6 = (int)unaff_A6[0xd] >> 2;
  unaff_A6[0xd] = unaff_A6[0xd] - uVar6;
  uVar7 = CARRY4(uVar5,uVar6);
  if (uVar5 + uVar6 == 0) {
    uVar1 = *(ushort *)(unaff_A6 + 0x23);
    uVar4 = uVar1 - 1;
    *(ushort *)(unaff_A6 + 0x23) = uVar4;
    if (uVar4 == 0 || SBORROW2(uVar1,1) != (int)((uint)uVar4 << 0x10) < 0) {
      *unaff_A6 = candidate_7ffb2;
      uVar7 = false;
    }
    else {
      *unaff_A6 = candidate_7fdb8;
      uVar7 = false;
    }
  }
  func_0x000143e4();
  candidate_427a();
  candidate_4c24();
  if (((bool)uVar7) &&
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
