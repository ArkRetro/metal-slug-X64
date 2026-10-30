/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04d9de.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4d9de(void)

{
  undefined4 uVar1;
  undefined4 uVar2;
  ushort uVar3;
  ushort uVar4;
  short sVar5;
  int unaff_A6;
  bool bVar6;

  candidate_44f6();
  uVar1 = *(undefined4 *)(unaff_A6 + 0x3c);
  uVar2 = *(undefined4 *)(unaff_A6 + 0x40);
  sVar5 = *(short *)(*(int *)(unaff_A6 + 0xc) + 0x98);
  *(short *)(unaff_A6 + 0x98) = sVar5;
  sVar5 = sVar5 << 2;
  *(short *)(unaff_A6 + 0x3c) = *(short *)(sVar5 + 0xe0f9a) + *(short *)(unaff_A6 + 0x3c);
  uVar4 = *(ushort *)(sVar5 + 0xe0f9c);
  uVar3 = *(ushort *)(unaff_A6 + 0x40);
  bVar6 = CARRY2(uVar4,uVar3);
  *(ushort *)(unaff_A6 + 0x40) = uVar4 + uVar3;
  candidate_427a();
  if (bVar6) {
    *(undefined4 *)(unaff_A6 + 0x40) = uVar2;
    *(undefined4 *)(unaff_A6 + 0x3c) = uVar1;
  }
  else {
    *(undefined4 *)(unaff_A6 + 0x40) = uVar2;
    *(undefined4 *)(unaff_A6 + 0x3c) = uVar1;
  }
  return;
}
