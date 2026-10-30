/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05fad8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

short candidate_5fad8(void)

{
  ushort uVar1;
  int unaff_A6;

  uVar1 = (ushort)((*(byte *)(unaff_A6 + 0x15) & 1) != 0);
  if (*(char *)(unaff_A6 + 0x88) != '\0') {
    uVar1 = uVar1 | 2;
  }
  if (-1 < *(int *)(unaff_A6 + 0x62)) {
    uVar1 = uVar1 | 4;
  }
  return uVar1 << 2;
}
