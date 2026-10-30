/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x069d4a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_69d4a(void)

{
  undefined4 uVar1;
  short sVar2;
  byte bVar3;
  int unaff_A6;

  bVar3 = *(char *)(unaff_A6 + 0x82) + 0xc;
  *(byte *)(unaff_A6 + 0x82) = bVar3;
  sVar2 = *(short *)((short)((ushort)bVar3 * 2) + 0x29fc);
  uVar1 = *(undefined4 *)(unaff_A6 + 0x40);
  *(int *)(unaff_A6 + 0x40) = (short)((sVar2 >> 1) + sVar2) * 0x100 + *(int *)(unaff_A6 + 0x40);
  candidate_69d18();
  *(undefined4 *)(unaff_A6 + 0x40) = uVar1;
  return;
}
