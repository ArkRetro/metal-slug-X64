/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a8602.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a8602(void)

{
  undefined4 *puVar1;
  uint *puVar2;
  ushort uVar3;
  uint *puVar4;
  short sVar5;
  int iVar6;
  undefined4 *unaff_A6;
  undefined8 uVar7;

  candidate_9ea8();
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0;
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 4;
  *unaff_A6 = 0xa8622;
  *(undefined2 *)(unaff_A6 + 0xf) = 0xa0;
  *(undefined2 *)(unaff_A6 + 0x10) = 0x160;
  uVar3 = *(ushort *)((int)unaff_A6 + 0x1e);
  uVar7 = (*pcRam0010bf64)();
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
    puVar1[2] = 0x1d5b22;
    *puVar2 = CONCAT22(uVar3,sVar5) << 0x10 | (uint)uVar3;
    *puVar4 = (uint)(puVar2 + 1);
  }
  return;
}
