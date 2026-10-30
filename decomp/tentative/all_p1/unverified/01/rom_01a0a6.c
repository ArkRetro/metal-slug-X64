/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01a0a6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Possible PIC construction at 0x0001a24e: Changing call to branch */
/* WARNING: Possible PIC construction at 0x0001a29e: Changing call to branch */
/* WARNING: Possible PIC construction at 0x0001a20a: Changing call to branch */
/* WARNING: Possible PIC construction at 0x0001a412: Changing call to branch */
/* WARNING: Removing unreachable block (ram,0x0001a20e) */
/* WARNING: Removing unreachable block (ram,0x0001a2a2) */
/* WARNING: Removing unreachable block (ram,0x0001a252) */
/* WARNING: Removing unreachable block (ram,0x0001a416) */
/* WARNING: Removing unreachable block (ram,0x0001a2b0) */

uint candidate_1a0a6(void)

{
  char *pcVar1;
  short sVar2;
  int iVar3;
  byte bVar5;
  uint uVar4;
  byte *in_A0;
  int extraout_A0;
  int extraout_A0_00;
  int extraout_A0_01;
  byte *pbVar6;
  char *extraout_A0_02;
  byte *extraout_A0_03;
  int extraout_A0_04;
  byte *extraout_A0_05;
  undefined2 *puVar7;
  undefined2 *puVar8;
  int unaff_A5;
  int unaff_A6;
  undefined4 *puVar9;
  bool bVar10;
  short sStack_8;
  undefined2 uStack_6;
  undefined4 uStack_4;

  do {
    bVar5 = *in_A0;
    uVar4 = (uint)bVar5;
    if (0x11 < bVar5) goto LAB_0001a052;
    uVar4 = (uint)bVar5 << 2;
    uStack_4._2_2_ = SUB42(in_A0,0);
    uStack_4._0_2_ = (undefined2)((uint)in_A0 >> 0x10);
                    /* WARNING: Switch is manually overridden */
    switch((ushort)bVar5 << 2) {
    case 0:
      goto LAB_0001a052;
    case 4:
      goto LAB_0001a052;
    case 8:
      goto LAB_0001a052;
    case 0xc:
      goto LAB_0001a052;
    case 0x10:
      goto LAB_0001a052;
    case 0x14:
      goto LAB_0001a052;
    case 0x18:
      return (uint)in_A0[1];
    case 0x1c:
      *(undefined1 *)(unaff_A6 + 0x74) = 0xff;
      sStack_8 = (short)((uint)*(undefined4 *)(*(short *)(in_A0 + 4) + 0x197dc) >> 0x10);
      uStack_6 = (undefined2)*(undefined4 *)(*(short *)(in_A0 + 4) + 0x197dc);
      candidate_40bc();
      iVar3 = CONCAT22(uStack_4._0_2_,uStack_4._2_2_);
      bVar5 = *(byte *)(iVar3 + 1);
      puVar7 = (undefined2 *)(extraout_A0 + 0x6e);
      for (puVar8 = (undefined2 *)(iVar3 + 10);
          puVar8 < (undefined2 *)(iVar3 + (short)(ushort)bVar5); puVar8 = puVar8 + 1) {
        *puVar7 = *puVar8;
        puVar7 = puVar7 + 1;
      }
      if ((-1 < sStack_8) && (bVar5 = *(byte *)(iVar3 + 2), bVar5 < 9)) {
        *(byte *)(extraout_A0 + 0x69) = bVar5;
        pcVar1 = (char *)(unaff_A5 + 0x4326 + (int)(short)(ushort)bVar5);
        *pcVar1 = *pcVar1 + '\x01';
      }
      *(byte *)(extraout_A0 + 0x68) = *(byte *)(iVar3 + 3) & 7;
      sStack_8 = 1;
      uStack_6 = 0xa1dc;
      candidate_1abb6();
      return (uint)*(byte *)(CONCAT22(uStack_4._0_2_,uStack_4._2_2_) + 1);
    case 0x20:
      sStack_8 = 1;
      uStack_6 = 0xa1fa;
      candidate_40bc();
      iVar3 = CONCAT22(uStack_4._0_2_,uStack_4._2_2_);
      *(int *)(extraout_A0_00 + 0x6e) = iVar3;
      pbVar6 = (byte *)((short)(ushort)*(byte *)(iVar3 + 1) + iVar3);
      register0x0000003c = (BADSPACEBASE *)&uStack_4;
      uStack_4 = (byte *)0x1a20e;
      goto SUB_0001a372;
    case 0x24:
      uStack_4._0_2_ = 1;
      uStack_4._2_2_ = 0xa260;
      uVar4 = candidate_1a056();
      if (*extraout_A0_02 == '\x05') {
        return (uint)(byte)extraout_A0_02[1];
      }
      goto LAB_0001a052;
    case 0x28:
      pbVar6 = in_A0 + (short)(ushort)in_A0[1];
      bVar5 = *pbVar6;
      uVar4 = (uint)bVar5;
      if (bVar5 == 4) {
        return (uint)pbVar6[1];
      }
      if ((0x11 < bVar5) || (BYTE_ARRAY_0001a074[(short)(ushort)bVar5] == 0)) goto LAB_0001a052;
      register0x0000003c = (BADSPACEBASE *)&sStack_8;
      sStack_8 = 1;
      uStack_6 = 0xa2a2;
      uStack_4 = pbVar6;
      goto SUB_0001a372;
    case 0x30:
      uStack_4._0_2_ = 1;
      uStack_4._2_2_ = 0xa2f6;
      candidate_c7c12();
      return (uint)*(byte *)(extraout_A0_04 + 1);
    case 0x34:
      sStack_8 = 1;
      uStack_6 = 0xa352;
      candidate_c7b4e();
      return (uint)*(byte *)(CONCAT22(uStack_4._0_2_,uStack_4._2_2_) + 1);
    case 0x38:
      sStack_8 = 1;
      uStack_6 = 0xa366;
      candidate_c7bba();
      return (uint)*(byte *)(CONCAT22(uStack_4._0_2_,uStack_4._2_2_) + 1);
    case 0x3c:
      if (*(short *)(in_A0 + 2) != -0x8000) {
        *(short *)(unaff_A5 + 0x69b8) = *(short *)(in_A0 + 2);
      }
      if (*(short *)(in_A0 + 4) != -0x8000) {
        *(short *)(unaff_A5 + 0x69ba) = *(short *)(in_A0 + 4) + 0x100;
      }
      return (uint)in_A0[1];
    case 0x40:
      *(undefined2 *)(unaff_A5 + 0x4330) = *(undefined2 *)(in_A0 + 2);
      *(short *)(unaff_A5 + 0x4332) = *(short *)(in_A0 + 4) + 0x100;
      return (uint)in_A0[1];
    case 0x44:
      sStack_8 = 1;
      uStack_6 = 0xa222;
      candidate_40bc();
      iVar3 = CONCAT22(uStack_4._0_2_,uStack_4._2_2_);
      sVar2 = *(short *)(iVar3 + 2);
      bVar5 = (byte)sVar2;
      if (bVar5 < 9) {
        *(byte *)(extraout_A0_01 + 0x69) = bVar5;
        pcVar1 = (char *)(unaff_A5 + 0x4326 + (int)sVar2);
        *pcVar1 = *pcVar1 + '\x01';
      }
      *(int *)(extraout_A0_01 + 0x6e) = iVar3 + 4;
      pbVar6 = (byte *)((short)(ushort)*(byte *)(iVar3 + 1) + iVar3);
      register0x0000003c = (BADSPACEBASE *)&uStack_4;
      uStack_4 = (byte *)0x1a252;
      goto SUB_0001a372;
    }
    bVar10 = false;
    uStack_4._0_2_ = 1;
    uStack_4._2_2_ = 0xa2e8;
    candidate_1a5ca();
    pbVar6 = extraout_A0_03;
    in_A0 = extraout_A0_03;
  } while (bVar10);
SUB_0001a372:
  bVar5 = *pbVar6;
  uVar4 = (uint)bVar5;
  if (0x11 < bVar5) {
LAB_0001a052:
    __m68k_trap(0xf);
    return uVar4;
  }
  uVar4 = (uint)bVar5 << 2;
                    /* WARNING: Switch is manually overridden */
  switch((ushort)bVar5 << 2) {
  case 0:
    goto LAB_0001a052;
  case 4:
    goto LAB_0001a052;
  case 8:
    goto LAB_0001a052;
  case 0xc:
    goto LAB_0001a052;
  case 0x10:
    goto LAB_0001a052;
  case 0x14:
    goto LAB_0001a052;
  case 0x18:
    break;
  case 0x1c:
    break;
  case 0x20:
    goto LAB_0001a3d8;
  case 0x24:
    pbVar6 = pbVar6 + (short)(ushort)pbVar6[1];
    while (*pbVar6 != 5) {
      *(undefined4 *)((int)register0x0000003c + -4) = 0x1a3f6;
      func_0x0001a372();
      pbVar6 = extraout_A0_05;
    }
    return (uint)pbVar6[1];
  case 0x28:
    goto LAB_0001a402;
  case 0x2c:
    goto LAB_0001a3d8;
  case 0x30:
    break;
  case 0x34:
    break;
  case 0x38:
    break;
  case 0x3c:
    break;
  case 0x40:
    break;
  case 0x44:
LAB_0001a3d8:
    pbVar6 = pbVar6 + (short)(ushort)pbVar6[1];
    goto SUB_0001a372;
  }
  return (uint)pbVar6[1];
LAB_0001a402:
  pbVar6 = pbVar6 + (short)(ushort)pbVar6[1];
  if (*pbVar6 == 4) {
    return (uint)pbVar6[1];
  }
  puVar9 = (undefined4 *)((int)register0x0000003c + -4);
  register0x0000003c = (BADSPACEBASE *)((int)register0x0000003c + -4);
  *puVar9 = 0x1a416;
  goto SUB_0001a372;
}
