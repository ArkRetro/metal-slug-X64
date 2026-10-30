/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0cbd9a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_cbd9a(void)

{
  undefined4 *puVar1;
  code *pcVar2;
  uint *puVar3;
  int iVar4;
  ushort uVar5;
  uint *puVar6;
  short sVar7;
  int iVar8;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined8 uVar9;

  candidate_1ad6e();
  *unaff_A6 = 0xcbdae;
  if ((*(byte *)(unaff_A6[3] + 0x5f) & 0x80) != 0) {
    candidate_4192();
    candidate_4126();
    iVar4 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar2 = (code *)*unaff_A6;
      *(undefined4 *)(iVar4 + -4) = 0x424a;
      (*pcVar2)();
    } while( true );
  }
  iVar4 = unaff_A6[3];
  *(undefined2 *)(unaff_A6 + 0xb) = *(undefined2 *)(iVar4 + 0x76);
  *(undefined2 *)((int)unaff_A6 + 0x1e) = *(undefined2 *)(iVar4 + 0x1e);
  if (*(short *)(iVar4 + 0x78) != *(short *)(unaff_A6 + 0x1e)) {
    *(short *)(unaff_A6 + 0x1e) = *(short *)(iVar4 + 0x78);
    candidate_9f88();
    candidate_9ea8();
  }
  candidate_1abc8();
  iVar4 = *(int *)(unaff_A6[3] + 0x72);
  uVar5 = *(ushort *)((int)unaff_A6 + 0x1e);
  uVar9 = (*pcRam0010bf64)();
  if (iVar4 != 0) {
    iVar8 = 0x109948;
    if (cRam00109946 < '\0') {
      iVar8 = 0x10a94e;
    }
    puVar6 = (uint *)(iVar8 + 0x400);
    puVar3 = (uint *)*puVar6;
    if (puVar3 < puVar6) {
      sVar7 = *(short *)(candidate_1004 + iVar8) + -0xc;
      *(short *)(candidate_1004 + iVar8) = sVar7;
      puVar1 = (undefined4 *)(iVar8 + sVar7);
      *puVar1 = (int)((ulonglong)uVar9 >> 0x20);
      puVar1[1] = (uint)uVar9 & 0xffffff07;
      puVar1[2] = iVar4;
      *puVar3 = CONCAT22(uVar5,sVar7) << 0x10 | (uint)uVar5;
      *puVar6 = (uint)(puVar3 + 1);
    }
  }
  return;
}
