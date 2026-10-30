/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x074c82.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_74c82(void)

{
  undefined2 uVar1;
  ushort uVar2;
  undefined1 uVar3;
  short sVar4;
  short extraout_D1w;
  int unaff_A6;
  bool bVar5;

  uVar1 = *(undefined2 *)(unaff_A6 + 0x1e);
  uVar3 = *(undefined1 *)(unaff_A6 + 0x14);
  sVar4 = candidate_74d4e();
  *(short *)(unaff_A6 + 0x3c) = sVar4 + *(short *)(unaff_A6 + 0x3c);
  *(short *)(unaff_A6 + 0x40) = extraout_D1w + *(short *)(unaff_A6 + 0x40);
  *(undefined1 *)(unaff_A6 + 0x14) = *(undefined1 *)(unaff_A6 + 0x1c);
  uVar2 = *(ushort *)(unaff_A6 + 0x1e);
  bVar5 = CARRY2((short)*(char *)(unaff_A6 + 0x90),uVar2);
  *(ushort *)(unaff_A6 + 0x1e) = (short)*(char *)(unaff_A6 + 0x90) + uVar2;
  candidate_427a();
  if (!bVar5) {
    *(byte *)(unaff_A6 + 0x8e) = *(byte *)(unaff_A6 + 0x8e) & 0xf7;
    candidate_1abc8();
    *(undefined1 *)(unaff_A6 + 0x14) = uVar3;
    *(undefined2 *)(unaff_A6 + 0x1e) = uVar1;
    return;
  }
  candidate_1abc8();
  *(undefined1 *)(unaff_A6 + 0x14) = uVar3;
  *(undefined2 *)(unaff_A6 + 0x1e) = uVar1;
  *(byte *)(unaff_A6 + 0x8e) = *(byte *)(unaff_A6 + 0x8e) | 8;
  return;
}
