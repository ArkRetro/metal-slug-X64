/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0307d6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

ushort candidate_307d6(void)

{
  ushort uVar1;
  int extraout_A0;
  int unaff_A6;

  uVar1 = *(byte *)(unaff_A6 + 0x8e) & 0xf;
  if ((*(byte *)(unaff_A6 + 0x8e) & 0xf) != 0) {
    uVar1 = candidate_40bc();
    *(undefined2 *)(extraout_A0 + 0x80) = *(undefined2 *)(unaff_A6 + 0x80);
    *(undefined1 *)(extraout_A0 + 0x8e) = *(undefined1 *)(unaff_A6 + 0x8e);
  }
  return uVar1;
}
