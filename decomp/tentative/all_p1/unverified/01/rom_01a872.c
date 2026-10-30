/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01a872.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

undefined4 candidate_1a872(void)

{
  uint in_D0;
  uint uVar1;
  int iVar2;
  int unaff_A5;

  if (*(char *)(unaff_A5 + 0x7d82) == '\0') {
    iVar2 = unaff_A5 + -0x7ffc;
  }
  else {
    iVar2 = 0xd00034;
    if (*(char *)(unaff_A5 + 0x7e80) != '\0') {
      iVar2 = unaff_A5 + 0x7e00;
    }
  }
  uVar1 = in_D0 & 0xffff0001;
  if (DAT_00100003 == '\0') {
    uVar1 = 0;
  }
  return CONCAT31((int3)(uVar1 >> 8),*(undefined1 *)(iVar2 + (short)uVar1));
}
