/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04d798.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4d798(void)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  ushort uVar5;
  char cVar6;
  short sVar7;
  short sVar8;
  int extraout_A0;
  int iVar9;
  int unaff_A6;
  bool bVar10;

  uVar1 = *(undefined4 *)(unaff_A6 + 0x3c);
  uVar2 = *(undefined4 *)(unaff_A6 + 0x40);
  sVar7 = *(short *)(*(int *)(unaff_A6 + 0xc) + 0x98);
  *(short *)(unaff_A6 + 0x98) = sVar7;
  sVar7 = sVar7 << 2;
  *(short *)(unaff_A6 + 0x3c) = *(short *)(sVar7 + 0xe0f9a) + *(short *)(unaff_A6 + 0x3c);
  *(short *)(unaff_A6 + 0x40) = *(short *)(sVar7 + 0xe0f9c) + *(short *)(unaff_A6 + 0x40);
  if (*(char *)(unaff_A6 + 0x9d) == '\0') {
    sVar7 = candidate_29f2();
    bVar10 = *(ushort *)(unaff_A6 + 0x3c) < (ushort)(sVar7 + 0x10U);
    sVar7 = *(ushort *)(unaff_A6 + 0x3c) - (sVar7 + 0x10U);
  }
  else {
    uVar5 = *(ushort *)(*(int *)(unaff_A6 + 0x6e) + 0x3c);
    bVar10 = *(ushort *)(unaff_A6 + 0x3c) < uVar5;
    sVar7 = *(ushort *)(unaff_A6 + 0x3c) - uVar5;
  }
  if (sVar7 < 0) {
    bVar10 = sVar7 != 0;
  }
  sVar7 = candidate_4d718();
  if (bVar10) {
    iVar9 = 0x4d92a;
  }
  else {
    iVar9 = 0x4d91a;
  }
  sVar8 = 0;
  do {
    if (sVar7 <= *(short *)(iVar9 + sVar8)) {
      *(undefined2 *)(unaff_A6 + 0x72) = *(undefined2 *)(iVar9 + 2 + (int)sVar8);
LAB_0004d83e:
      *(short *)(unaff_A6 + 0x72) = *(short *)(unaff_A6 + 0x3c) - *(short *)(unaff_A6 + 0x72);
      *(undefined2 *)(unaff_A6 + 0x74) = 0x120;
      uVar3 = *(undefined4 *)(extraout_A0 + 0x40);
      uVar4 = *(undefined4 *)(extraout_A0 + 0x3c);
      *(uint *)(extraout_A0 + 0x3c) = (uint)*(ushort *)(unaff_A6 + 0x72) << 0x10;
      *(undefined4 *)(extraout_A0 + 0x40) = 0x1200000;
      cVar6 = candidate_1b12e();
      *(undefined4 *)(extraout_A0 + 0x3c) = uVar4;
      *(undefined4 *)(extraout_A0 + 0x40) = uVar3;
      *(char *)(unaff_A6 + 0xa2) = -cVar6;
      for (sVar7 = 0; *(short *)(sVar7 + 0x4d8c6) < (short)(ushort)(byte)-cVar6; sVar7 = sVar7 + 4)
      {
      }
      *(char *)(unaff_A6 + 0x9c) = (char)*(undefined2 *)(sVar7 + 0x4d8c8);
      sVar7 = *(short *)(unaff_A6 + 0x3c) - *(short *)(unaff_A6 + 0x72);
      if (sVar7 < 0) {
        sVar7 = -sVar7;
      }
      *(short *)(unaff_A6 + 0x8c) = sVar7;
      *(undefined4 *)(unaff_A6 + 0x40) = uVar2;
      *(undefined4 *)(unaff_A6 + 0x3c) = uVar1;
      return;
    }
    if (*(short *)(iVar9 + sVar8) == 0xff) {
      *(undefined2 *)(unaff_A6 + 0x72) = *(undefined2 *)(iVar9 + 2 + (int)sVar8);
      goto LAB_0004d83e;
    }
    sVar8 = sVar8 + 4;
  } while( true );
}
