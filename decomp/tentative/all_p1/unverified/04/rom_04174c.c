/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04174c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4174c(void)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  byte bVar4;
  byte *pbVar5;
  int iVar6;
  ushort uVar7;
  int iVar8;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar9;

  candidate_420a8();
  *unaff_A6 = 0x41756;
  if (*(char *)((int)unaff_A6 + 0x81) == '\0') {
    *unaff_A6 = candidate_41768;
  }
  if (*(char *)((int)unaff_A6 + 0xa3) == '\0') {
    candidate_446e8();
  }
  else {
    candidate_446f0();
  }
  candidate_1ac38();
  *(int *)((int)unaff_A6 + 0x86) = *(int *)((int)unaff_A6 + 0x8e) + *(int *)((int)unaff_A6 + 0x86);
  *(int *)((int)unaff_A6 + 0x8a) = *(int *)((int)unaff_A6 + 0x92) + *(int *)((int)unaff_A6 + 0x8a);
  iVar6 = *(int *)((int)unaff_A6 + 0x86) - iRam0010d7b6;
  iVar8 = *(int *)((int)unaff_A6 + 0x8a) - iRam0010d7ba;
  iVar1 = *(int *)((int)unaff_A6 + 0x72);
  bVar4 = *(byte *)(iVar1 + 0x8a);
  *(char *)(iVar1 + 0x8a) = *(char *)(iVar1 + 0x8b) + *(char *)(iVar1 + 0x8a);
  if (bVar4 != 0) {
    *(int *)(iVar1 + 0x74) =
         (int)*(short *)((short)((ushort)bVar4 * 2) + 0x29fc) << (*(byte *)(iVar1 + 0x8c) & 0x3f);
  }
  iVar1 = *(int *)((int)unaff_A6 + 0x72);
  if ((*(byte *)(iVar1 + 0x8d) & 2) != 0) {
    if ((*(byte *)(iVar1 + 0x8d) & 1) == 0) {
      iVar8 = *(int *)(iVar1 + 0x74) + iVar8;
    }
    else {
      iVar6 = *(int *)(iVar1 + 0x74) + iVar6;
    }
  }
  *(int *)(unaff_A5 + 0x694e) = iVar6;
  *(int *)(unaff_A5 + 0x6952) = iVar8;
  if ((*(byte *)(*(int *)((int)unaff_A6 + 0x72) + 0x86) & 1) != 0) {
    candidate_421e4();
  }
  uVar7 = candidate_44926();
  if (uVar7 != *(ushort *)((int)unaff_A6 + 0x26)) {
    if (*(ushort *)((int)unaff_A6 + 0x26) < uVar7) {
      *(short *)((int)unaff_A6 + 0x26) = *(short *)((int)unaff_A6 + 0x26) + 2;
    }
    else {
      *(short *)((int)unaff_A6 + 0x26) = *(short *)((int)unaff_A6 + 0x26) + -2;
    }
  }
  uVar2 = unaff_A6[10];
  uVar3 = unaff_A6[0xb];
  iVar1 = *(int *)((int)unaff_A6 + 0x72);
  if ((*(byte *)(iVar1 + 0x8d) & 2) == 0) {
    if ((*(byte *)(iVar1 + 0x8d) & 1) == 0) {
      unaff_A6[0xb] = *(int *)(iVar1 + 0x74) + unaff_A6[0xb];
    }
    else {
      unaff_A6[10] = *(int *)(iVar1 + 0x74) + unaff_A6[10];
    }
  }
  candidate_1abc8();
  candidate_427a();
  unaff_A6[0xb] = uVar3;
  unaff_A6[10] = uVar2;
  bVar9 = false;
  candidate_4c24();
  if ((bVar9) &&
     (*(undefined1 *)((int)unaff_A6 + 0x5a) = 0, (*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0)) {
    pbVar5 = (byte *)(*(int *)((int)unaff_A6 + 0x72) + 0x86);
    *pbVar5 = *pbVar5 | 2;
    if ((*(byte *)(*(int *)((int)unaff_A6 + 0x72) + 0x86) & 1) == 0) {
      *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
      *unaff_A6 = candidate_41634;
    }
    else {
      *(undefined1 *)(unaff_A5 + 0x4448) = 0xff;
      *unaff_A6 = candidate_4180a;
    }
  }
  return;
}
