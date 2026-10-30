/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c7c70.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_c7c70(void)

{
  int iVar1;
  ushort uVar3;
  undefined4 in_D0;
  int iVar2;
  ushort uVar5;
  undefined4 in_D1;
  int iVar4;
  undefined4 uVar6;
  undefined4 uVar7;
  byte bVar8;
  int unaff_A5;

  *(undefined4 *)(unaff_A5 + 0x698e) = in_D0;
  *(undefined4 *)(unaff_A5 + 0x6992) = in_D1;
  iVar2 = 0;
  iVar4 = 0x1000000;
  iVar1 = *(int *)(unaff_A5 + 27000);
  if (iVar1 != 0) {
    iVar2 = -*(int *)(iVar1 + 0x14);
    iVar4 = 0x1000000 - *(int *)(iVar1 + 0x18);
  }
  *(int *)(unaff_A5 + 27000) = iVar1;
  *(int *)(unaff_A5 + 0x6968) = iVar2 - *(int *)(unaff_A5 + 0x698e);
  *(int *)(unaff_A5 + 0x696c) = iVar4 - *(int *)(unaff_A5 + 0x6992);
  *(undefined2 *)(candidate_698c + unaff_A5) = 0;
  uVar6 = *(undefined4 *)(unaff_A5 + 0x697c);
  uVar7 = *(undefined4 *)(unaff_A5 + 0x6980);
  iVar1 = *(int *)(unaff_A5 + 27000);
  iVar4 = *(int *)(unaff_A5 + 0x6968);
  iVar2 = *(int *)(unaff_A5 + 0x696c);
  bVar8 = 0;
  if (iVar1 != 0) {
    uVar5 = (ushort)iVar2;
    uVar3 = (ushort)iVar4;
    iVar4 = *(int *)(iVar1 + 0x14) + iVar4;
    iVar2 = *(int *)(iVar1 + 0x18) + iVar2;
    bVar8 = *(byte *)(iVar1 + 0x25) |
            CARRY2((ushort)*(int *)(iVar1 + 0x18),uVar5) * '\x02' +
            CARRY2((ushort)*(int *)(iVar1 + 0x14),uVar3);
  }
  *(byte *)(unaff_A5 + 0x698d) = bVar8;
  *(int *)(unaff_A5 + 0x697c) = iVar4;
  *(int *)(unaff_A5 + 0x6980) = iVar2;
  if (candidate_698c[unaff_A5] == (code)0x0) {
    candidate_698c[unaff_A5] = cRam0010c33a;
    *(int *)(unaff_A5 + 0x6984) = iVar4;
    *(int *)(unaff_A5 + 0x6988) = iVar2;
    return;
  }
  if (cRam0010c33a != candidate_698c[unaff_A5]) {
    candidate_698c[unaff_A5] = cRam0010c33a;
    *(undefined4 *)(unaff_A5 + 0x6984) = uVar6;
    *(undefined4 *)(unaff_A5 + 0x6988) = uVar7;
  }
  return;
}
