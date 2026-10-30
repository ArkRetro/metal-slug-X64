/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x055478.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_55478(void)

{
  int iVar1;
  undefined1 uVar2;
  ushort in_D0w;
  uint uVar3;
  int unaff_A6;

  uVar3 = (uint)in_D0w;
  if (0xc000 < in_D0w) {
    uVar3 = 0xc000;
  }
  iVar1 = (int)(short)(((ushort)(uVar3 / 0xec4) & 0xf) * 2);
  *(undefined1 *)(unaff_A6 + 0x60) = *(undefined1 *)(iVar1 + 0x5545c);
  uVar2 = *(undefined1 *)(iVar1 + 0x5545d);
  *(undefined1 *)(unaff_A6 + 0x12) = uVar2;
  *(undefined1 *)(unaff_A6 + 0x13) = uVar2;
  return;
}
