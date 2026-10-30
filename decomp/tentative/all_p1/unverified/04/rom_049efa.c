/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x049efa.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_49efa(void)

{
  int iVar1;
  ushort uVar2;
  byte bVar3;
  int iVar4;
  undefined1 uVar5;
  ushort *puVar6;
  int unaff_A6;
  undefined8 uVar7;

  iVar1 = *(int *)(*(int *)(unaff_A6 + 0xc) + 0xc);
  bVar3 = *(byte *)(iVar1 + 0x95);
  puVar6 = (ushort *)((short)((ushort)bVar3 * 4 + 0x76) + iVar1);
  iVar4 = candidate_1b29c();
  if (iVar4 * 0x10000 < 0x40001) {
    uVar2 = puVar6[1];
    *(uint *)(unaff_A6 + 0x30) = (uint)*puVar6 * 0x10000 - *(int *)(unaff_A6 + 0x3c);
    *(uint *)(unaff_A6 + 0x34) = (uint)uVar2 * 0x10000 - *(int *)(unaff_A6 + 0x40);
    *(undefined1 *)(unaff_A6 + 0xaa) = *(undefined1 *)((short)(bVar3 + 0x90) + iVar1);
  }
  else {
    uVar5 = candidate_2eb4();
    *(undefined1 *)(unaff_A6 + 0xaa) = uVar5;
    uVar7 = candidate_2e7c();
    *(undefined8 *)(unaff_A6 + 0x30) = uVar7;
  }
  return;
}
