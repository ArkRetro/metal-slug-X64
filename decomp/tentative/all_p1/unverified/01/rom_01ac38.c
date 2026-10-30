/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01ac38.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_1ac38(void)

{
  int iVar1;
  int *piVar2;
  int iVar3;
  ushort uVar4;
  int iVar5;
  ushort uVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  byte bVar9;
  int unaff_A6;

  piVar2 = (int *)(unaff_A6 + 0x28);
  *piVar2 = *(int *)(unaff_A6 + 0x30) + *piVar2;
  *(int *)(unaff_A6 + 0x2c) = *(int *)(unaff_A6 + 0x34) + *(int *)(unaff_A6 + 0x2c);
  uVar7 = *(undefined4 *)(unaff_A6 + 0x3c);
  uVar8 = *(undefined4 *)(unaff_A6 + 0x40);
  iVar1 = *(int *)(unaff_A6 + 0x38);
  iVar3 = *piVar2;
  iVar5 = *(int *)(unaff_A6 + 0x2c);
  bVar9 = 0;
  if (iVar1 != 0) {
    uVar6 = (ushort)iVar5;
    uVar4 = (ushort)iVar3;
    iVar3 = *(int *)(iVar1 + 0x14) + iVar3;
    iVar5 = *(int *)(iVar1 + 0x18) + iVar5;
    bVar9 = *(byte *)(iVar1 + 0x25) |
            CARRY2((ushort)*(int *)(iVar1 + 0x18),uVar6) * '\x02' +
            CARRY2((ushort)*(int *)(iVar1 + 0x14),uVar4);
  }
  *(byte *)(unaff_A6 + 0x4d) = bVar9;
  *(int *)(unaff_A6 + 0x3c) = iVar3;
  *(int *)(unaff_A6 + 0x40) = iVar5;
  if (*(char *)(unaff_A6 + 0x4c) == '\0') {
    *(char *)(unaff_A6 + 0x4c) = cRam0010c33a;
    *(int *)(unaff_A6 + 0x44) = iVar3;
    *(int *)(unaff_A6 + 0x48) = iVar5;
    return;
  }
  if (cRam0010c33a != *(char *)(unaff_A6 + 0x4c)) {
    *(char *)(unaff_A6 + 0x4c) = cRam0010c33a;
    *(undefined4 *)(unaff_A6 + 0x44) = uVar7;
    *(undefined4 *)(unaff_A6 + 0x48) = uVar8;
  }
  return;
}
