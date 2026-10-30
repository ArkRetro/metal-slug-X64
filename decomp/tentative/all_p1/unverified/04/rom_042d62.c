/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x042d62.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_42d62(void)

{
  int iVar1;
  int iVar2;
  ushort uVar3;
  int iVar4;
  ushort uVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  byte bVar8;
  int unaff_A6;

  *(undefined1 *)(*(int *)(unaff_A6 + 0x72) + 0x87) = 0;
  *(undefined2 *)(unaff_A6 + 0x66) = 100;
  *(undefined2 *)(unaff_A6 + 0x1e) = 100;
  candidate_9ea8();
  candidate_9ea8();
  candidate_9ea8();
  candidate_4520();
  *(int *)(unaff_A6 + 0x38) = *(int *)(unaff_A6 + 0xc) + 0x28;
  *(int *)(unaff_A6 + 0x28) = 0;
  *(undefined4 *)(unaff_A6 + 0x2c) = 0xffc80000;
  *(undefined2 *)(unaff_A6 + 0x4c) = 0;
  uVar6 = *(undefined4 *)(unaff_A6 + 0x3c);
  uVar7 = *(undefined4 *)(unaff_A6 + 0x40);
  iVar1 = *(int *)(unaff_A6 + 0x38);
  iVar2 = *(int *)(unaff_A6 + 0x28);
  iVar4 = *(int *)(unaff_A6 + 0x2c);
  bVar8 = 0;
  if (iVar1 != 0) {
    uVar5 = (ushort)iVar4;
    uVar3 = (ushort)iVar2;
    iVar2 = *(int *)(iVar1 + 0x14) + iVar2;
    iVar4 = *(int *)(iVar1 + 0x18) + iVar4;
    bVar8 = *(byte *)(iVar1 + 0x25) |
            CARRY2((ushort)*(int *)(iVar1 + 0x18),uVar5) * '\x02' +
            CARRY2((ushort)*(int *)(iVar1 + 0x14),uVar3);
  }
  *(byte *)(unaff_A6 + 0x4d) = bVar8;
  *(int *)(unaff_A6 + 0x3c) = iVar2;
  *(int *)(unaff_A6 + 0x40) = iVar4;
  if (*(char *)(unaff_A6 + 0x4c) == '\0') {
    *(char *)(unaff_A6 + 0x4c) = cRam0010c33a;
    *(int *)(unaff_A6 + 0x44) = iVar2;
    *(int *)(unaff_A6 + 0x48) = iVar4;
    return;
  }
  if (cRam0010c33a != *(char *)(unaff_A6 + 0x4c)) {
    *(char *)(unaff_A6 + 0x4c) = cRam0010c33a;
    *(undefined4 *)(unaff_A6 + 0x44) = uVar6;
    *(undefined4 *)(unaff_A6 + 0x48) = uVar7;
  }
  return;
}
