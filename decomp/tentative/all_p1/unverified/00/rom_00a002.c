/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x00a002.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a002(void)

{
  short in_D0w;
  ushort in_D1w;
  ushort unaff_D2w;
  undefined4 *puVar1;
  int unaff_A5;

  puVar1 = (undefined4 *)(unaff_A5 + 0x3954);
  if (in_D0w != 0) {
    puVar1 = (undefined4 *)(unaff_A5 + 0x395c);
  }
  if (0x17b < in_D1w) {
    in_D1w = 0xffff;
  }
  if (unaff_D2w <= in_D1w) {
    unaff_D2w = 0xffff;
  }
  *puVar1 = CONCAT22(in_D1w,unaff_D2w);
  puVar1[1] = CONCAT22(in_D1w,unaff_D2w);
  return;
}
