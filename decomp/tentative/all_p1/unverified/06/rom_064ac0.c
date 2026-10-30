/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x064ac0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_64ac0(void)

{
  undefined4 *puVar1;
  uint *puVar2;
  int iVar3;
  ushort uVar4;
  uint *puVar5;
  short sVar6;
  int iVar7;
  undefined4 *unaff_A6;
  undefined8 uVar8;

  candidate_9ea8();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0;
  *unaff_A6 = 0x64ae0;
  func_0x00009cce();
  iVar3 = unaff_A6[0x20];
  uVar4 = *(ushort *)((int)unaff_A6 + 0x1e);
  uVar8 = (*pcRam0010bf64)();
  if (iVar3 != 0) {
    iVar7 = 0x109948;
    if (cRam00109946 < '\0') {
      iVar7 = 0x10a94e;
    }
    puVar5 = (uint *)(iVar7 + 0x400);
    puVar2 = (uint *)*puVar5;
    if (puVar2 < puVar5) {
      sVar6 = *(short *)(candidate_1004 + iVar7) + -0xc;
      *(short *)(candidate_1004 + iVar7) = sVar6;
      puVar1 = (undefined4 *)(iVar7 + sVar6);
      *puVar1 = (int)((ulonglong)uVar8 >> 0x20);
      puVar1[1] = (uint)uVar8 & 0xffffff07;
      puVar1[2] = iVar3;
      *puVar2 = CONCAT22(uVar4,sVar6) << 0x10 | (uint)uVar4;
      *puVar5 = (uint)(puVar2 + 1);
    }
  }
  return;
}
