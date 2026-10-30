/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03ac8a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Possible PIC construction at 0x0003ac8e: Changing call to branch */
/* WARNING: Removing unreachable block (ram,0x0003ac92) */

void candidate_3ac8a(void)

{
  int iVar1;
  int iVar2;
  ushort uVar3;
  int iVar4;
  ushort uVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  byte bVar8;
  int extraout_A0;
  int unaff_A6;

  candidate_40bc();
  *(undefined4 *)(extraout_A0 + 0x6e) =
       **(undefined4 **)((short)(*(short *)(unaff_A6 + 0x72) * 4) + 0x8b6);
  *(int *)(extraout_A0 + 0x38) = unaff_A6 + 0x28;
  *(int *)(extraout_A0 + 0x28) = 0;
  *(undefined4 *)(extraout_A0 + 0x2c) = 0;
  *(undefined2 *)(extraout_A0 + 0x4c) = 0;
  uVar6 = *(undefined4 *)(extraout_A0 + 0x3c);
  uVar7 = *(undefined4 *)(extraout_A0 + 0x40);
  iVar1 = *(int *)(extraout_A0 + 0x38);
  iVar2 = *(int *)(extraout_A0 + 0x28);
  iVar4 = *(int *)(extraout_A0 + 0x2c);
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
  *(byte *)(extraout_A0 + 0x4d) = bVar8;
  *(int *)(extraout_A0 + 0x3c) = iVar2;
  *(int *)(extraout_A0 + 0x40) = iVar4;
  if (*(char *)(extraout_A0 + 0x4c) == '\0') {
    *(char *)(extraout_A0 + 0x4c) = cRam0010c33a;
    *(int *)(extraout_A0 + 0x44) = iVar2;
    *(int *)(extraout_A0 + 0x48) = iVar4;
    return;
  }
  if (cRam0010c33a != *(char *)(extraout_A0 + 0x4c)) {
    *(char *)(extraout_A0 + 0x4c) = cRam0010c33a;
    *(undefined4 *)(extraout_A0 + 0x44) = uVar6;
    *(undefined4 *)(extraout_A0 + 0x48) = uVar7;
  }
  return;
}
