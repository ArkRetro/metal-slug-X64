/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x066ca4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_66ca4(void)

{
  uint uVar1;
  int extraout_A0;
  int iVar2;
  undefined4 in_A1;
  undefined4 extraout_A1;
  undefined4 extraout_A1_00;
  undefined4 extraout_A1_01;
  int *unaff_A2;
  bool bVar3;

  if (*(byte *)(unaff_A2 + 1) <= (byte)(*(char *)(unaff_A2 + 2) + 1U)) {
    return;
  }
  iVar2 = *(int *)((int)(short)((ushort)*(byte *)(unaff_A2 + 2) * 8) + *unaff_A2 + 0x10);
  if (iVar2 == -2) {
    if (1 < *(byte *)((int)unaff_A2 + 0xb)) {
      candidate_66c3e();
      in_A1 = extraout_A1;
    }
  }
  else {
    if ((*(ushort *)(*unaff_A2 + 0xe) & 0xe) == 0) {
      bVar3 = false;
      uVar1 = candidate_66f7a();
      if (!bVar3) {
        _DAT_003c0000 = uVar1 << 0x10 | uVar1 >> 0x10;
        uVar1 = uVar1 & 0xffff;
      }
      bVar3 = 0xffdf < (ushort)uVar1;
      uVar1 = candidate_66f7a();
      iVar2 = extraout_A0;
      in_A1 = extraout_A1_00;
      if ((!bVar3) &&
         (_DAT_003c0000 = uVar1 << 0x10 | uVar1 >> 0x10,
         *(char *)(unaff_A2 + 2) == *(char *)((int)unaff_A2 + 7))) {
        _DAT_003c0000 = CONCAT31((int3)(_DAT_003c0000 >> 8),(char)(uVar1 >> 0x10) + '\x01');
      }
    }
    if (iVar2 != 0) {
      candidate_66eda();
      candidate_3d25a();
      in_A1 = extraout_A1_01;
    }
  }
  candidate_66aac(in_A1);
  return;
}
