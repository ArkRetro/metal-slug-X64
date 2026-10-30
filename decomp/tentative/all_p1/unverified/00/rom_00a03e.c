/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x00a03e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a03e(void)

{
  undefined4 *puVar1;
  uint *puVar2;
  uint *puVar3;
  short sVar4;
  ushort unaff_D5w;
  int unaff_D7;
  int iVar5;
  int unaff_A5;
  undefined8 uVar6;

  uVar6 = (**(code **)(unaff_A5 + 0x3f64))();
  if (unaff_D7 != 0) {
    iVar5 = unaff_A5 + 0x1948;
    if (*(char *)(unaff_A5 + 0x1946) < '\0') {
      iVar5 = unaff_A5 + 0x294e;
    }
    puVar3 = (uint *)(iVar5 + 0x400);
    puVar2 = (uint *)*puVar3;
    if (puVar2 < puVar3) {
      sVar4 = *(short *)(candidate_1004 + iVar5) + -0xc;
      *(short *)(candidate_1004 + iVar5) = sVar4;
      puVar1 = (undefined4 *)(iVar5 + sVar4);
      *puVar1 = (int)((ulonglong)uVar6 >> 0x20);
      puVar1[1] = (uint)uVar6 & 0xffffff07;
      puVar1[2] = unaff_D7;
      *puVar2 = CONCAT22(unaff_D5w,sVar4) << 0x10 | (uint)unaff_D5w;
      *puVar3 = (uint)(puVar2 + 1);
    }
  }
  return;
}
