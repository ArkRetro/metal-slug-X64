/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x011ff6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_11ff6(void)

{
  undefined4 *puVar1;
  uint *puVar2;
  ushort uVar3;
  uint *puVar4;
  short sVar5;
  int unaff_D7;
  int iVar6;
  int unaff_A6;
  undefined8 uVar7;

  uVar3 = *(ushort *)(unaff_A6 + 0x1e);
  uVar7 = (*pcRam0010bf64)();
  if (unaff_D7 != 0) {
    iVar6 = 0x109948;
    if (cRam00109946 < '\0') {
      iVar6 = 0x10a94e;
    }
    puVar4 = (uint *)(iVar6 + 0x400);
    puVar2 = (uint *)*puVar4;
    if (puVar2 < puVar4) {
      sVar5 = *(short *)(candidate_1004 + iVar6) + -0xc;
      *(short *)(candidate_1004 + iVar6) = sVar5;
      puVar1 = (undefined4 *)(iVar6 + sVar5);
      *puVar1 = (int)((ulonglong)uVar7 >> 0x20);
      puVar1[1] = (uint)uVar7 & 0xffffff07;
      puVar1[2] = unaff_D7;
      *puVar2 = CONCAT22(uVar3,sVar5) << 0x10 | (uint)uVar3;
      *puVar4 = (uint)(puVar2 + 1);
    }
  }
  return;
}
