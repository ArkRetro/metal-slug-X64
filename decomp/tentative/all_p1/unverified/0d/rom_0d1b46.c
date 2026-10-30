/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d1b46.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

ushort candidate_d1b46(void)

{
  ushort in_D0w;
  ushort uVar1;
  undefined2 uVar2;
  int unaff_A5;
  int unaff_A6;

  if (*(short *)(unaff_A5 + 0x699e) != 0x1000) {
    return in_D0w;
  }
  uVar1 = candidate_29c6();
  uVar2 = 0xffff;
  if ((uVar1 & 3) == 0) {
    uVar2 = 0;
  }
  *(undefined2 *)(unaff_A6 + 0x2c) = uVar2;
  return uVar1 & 3;
}
