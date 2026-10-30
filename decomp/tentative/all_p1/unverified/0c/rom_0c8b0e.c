/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c8b0e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_c8b0e(void)

{
  short sVar1;
  short sVar2;
  short sVar3;
  short sVar4;
  short sVar5;
  ushort uVar6;
  ushort uVar7;
  int in_A0;
  int unaff_A5;

  sVar2 = 1;
  sVar4 = 0x26;
  do {
    DAT_00300001 = (char)sVar2;
    sVar3 = 6;
    sVar5 = 0x19;
    do {
      uVar7 = (ushort)*(byte *)(in_A0 + 0x88 +
                               (int)(short)((sVar3 + (*(short *)(in_A0 + 0x18) >> 3) & 0x3fU) +
                                           (sVar2 + (-*(short *)(in_A0 + 0x14) >> 3) & 0x3fU) * 0x40
                                           ));
      uVar6 = (short)uVar7 >> 5;
      if (*(int *)(unaff_A5 + 0x400e) != 0) {
        uVar6 = (ushort)*(byte *)(*(int *)(unaff_A5 + 0x400e) + (int)(short)uVar6);
      }
      uVar7 = uVar7 & 0xf;
      if (uVar6 == 0xb) {
        uVar7 = uVar7 + 0x10;
      }
      sVar1 = *(short *)((short)(uVar7 * 2) + 0xc8bac);
      if (sVar1 != 0xff) {
        _DAT_003c0000 = CONCAT22(sVar3 + sVar2 * 0x20 + 0x7000,sVar1);
      }
      sVar3 = sVar3 + 1;
      sVar5 = sVar5 + -1;
    } while (sVar5 != 0);
    sVar2 = sVar2 + 1;
    sVar4 = sVar4 + -1;
  } while (sVar4 != 0);
  return;
}
