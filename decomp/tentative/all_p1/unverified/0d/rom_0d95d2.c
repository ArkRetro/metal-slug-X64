/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d95d2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d95d2(void)

{
  ushort uVar1;
  char cVar2;
  int unaff_A6;

  uVar1 = (ushort)*(byte *)(unaff_A6 + 0xaa);
  if (0x7f < uVar1) {
    uVar1 = 0xff - uVar1;
  }
  if (0x3f < uVar1) {
    uVar1 = 0x7f - uVar1;
  }
  if (uVar1 < 0x18) {
    uVar1 = 0;
  }
  cVar2 = (char)(uVar1 << 1) + -0x80;
  *(char *)(unaff_A6 + 0x12) = cVar2;
  *(char *)(unaff_A6 + 0x13) = cVar2;
  *(short *)(unaff_A6 + 0x1e) = uVar1 + *(short *)(unaff_A6 + 0x1e);
  return;
}
