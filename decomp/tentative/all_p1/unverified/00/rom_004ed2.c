/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x004ed2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4ed2(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  char cVar8;
  short sVar7;
  int *piVar9;
  int unaff_A6;
  undefined1 in_CF;
  undefined8 uVar10;

  thunk_FUN_000c8cfc();
  if ((bool)in_CF) {
    return;
  }
  candidate_1abc8();
  piVar9 = (int *)0x106756;
  cVar8 = '\0';
  do {
    iVar1 = *piVar9;
    if ((iVar1 != 0) && (cVar8 == *(char *)(iVar1 + 0x6b))) {
      iVar2 = piVar9[1];
      iVar4 = *(int *)(unaff_A6 + 0x3c) - *(int *)(iVar1 + 0x3c);
      iVar5 = *(int *)(unaff_A6 + 0x30);
      iVar6 = *(int *)(unaff_A6 + 0x34);
      iVar3 = *(int *)(unaff_A6 + 0x38);
      if (iVar3 != 0) {
        iVar5 = (*(int *)(iVar3 + 0x14) + iVar5) - *(int *)(iVar3 + 0x1c);
        iVar6 = (*(int *)(iVar3 + 0x18) + iVar6) - *(int *)(iVar3 + 0x20);
      }
      iVar5 = iVar5 - *(int *)(iVar1 + 0x30);
      iVar6 = iVar6 - *(int *)(iVar1 + 0x34);
      iVar3 = *(int *)(iVar1 + 0x38);
      if (iVar3 != 0) {
        iVar5 = *(int *)(iVar3 + 0x1c) + (iVar5 - *(int *)(iVar3 + 0x14));
        iVar6 = *(int *)(iVar3 + 0x20) + (iVar6 - *(int *)(iVar3 + 0x18));
      }
      if ((ushort)((short)((uint)((*(int *)(unaff_A6 + 0x40) - *(int *)(iVar1 + 0x40)) + iVar6) >>
                          0x10) - *(short *)(iVar2 + 6)) < *(ushort *)(iVar2 + 10)) {
        sVar7 = *(short *)(iVar2 + 4);
        if ((*(byte *)(iVar1 + 0x15) & 1) != 0) {
          sVar7 = -(*(ushort *)(iVar2 + 8) + sVar7);
        }
        if ((ushort)((short)((uint)(iVar4 + iVar5) >> 0x10) - sVar7) < *(ushort *)(iVar2 + 8)) {
          if ((short)(((short)((uint)iVar4 >> 0x10) - sVar7) - *(short *)(iVar2 + 2)) < 0) {
            *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) | 8;
            *(byte *)(iVar1 + 0x15) = *(byte *)(iVar1 + 0x15) | 0x10;
          }
          else {
            *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) | 0x10;
            *(byte *)(iVar1 + 0x15) = *(byte *)(iVar1 + 0x15) | 8;
          }
          candidate_13968();
          uVar10 = candidate_1ac4c();
          *(undefined8 *)(unaff_A6 + 0x28) = uVar10;
        }
      }
    }
    cVar8 = cVar8 + '\x01';
    piVar9 = piVar9 + 2;
  } while (piVar9 < (int *)0x1067d6);
  return;
}
