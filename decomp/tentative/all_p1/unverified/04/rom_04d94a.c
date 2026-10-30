/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04d94a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4d94a(void)

{
  undefined4 uVar1;
  undefined4 uVar2;
  int iVar3;
  undefined4 uVar4;
  short sVar5;
  short sVar6;
  int unaff_A6;

  uVar1 = *(undefined4 *)(unaff_A6 + 0x3c);
  uVar2 = *(undefined4 *)(unaff_A6 + 0x40);
  sVar6 = *(short *)(*(int *)(unaff_A6 + 0xc) + 0x98);
  *(short *)(unaff_A6 + 0x98) = sVar6;
  sVar6 = sVar6 << 2;
  *(short *)(unaff_A6 + 0x3c) = *(short *)(sVar6 + 0xe0f9a) + *(short *)(unaff_A6 + 0x3c);
  *(short *)(unaff_A6 + 0x40) = *(short *)(sVar6 + 0xe0f9c) + *(short *)(unaff_A6 + 0x40);
  iVar3 = *(int *)(unaff_A6 + 0x6e);
  uVar4 = *(undefined4 *)(iVar3 + 0x40);
  *(short *)(iVar3 + 0x40) = *(short *)(iVar3 + 0x40) + 0x18;
  sVar5 = candidate_1b12e();
  *(undefined4 *)(iVar3 + 0x40) = uVar4;
  for (sVar6 = 0; *(short *)(sVar6 + 0x4d9ba) < sVar5; sVar6 = sVar6 + 4) {
  }
  *(char *)(unaff_A6 + 0x9c) = (char)*(undefined2 *)(sVar6 + 0x4d9bc);
  *(undefined4 *)(unaff_A6 + 0x40) = uVar2;
  *(undefined4 *)(unaff_A6 + 0x3c) = uVar1;
  return;
}
