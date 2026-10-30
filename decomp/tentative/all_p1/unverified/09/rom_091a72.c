/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x091a72.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_91a72(void)

{
  char in_D0b;
  byte bVar2;
  ushort uVar1;
  int unaff_A6;

  bVar2 = in_D0b + 0x20;
  if ((char)bVar2 < '\0') {
    bVar2 = 0;
  }
  uVar1 = (ushort)bVar2;
  if (0x40 < uVar1) {
    uVar1 = 0x40;
  }
  if ((*(byte *)(unaff_A6 + 0x15) & 1) == 0) {
    uVar1 = 0x40 - uVar1;
  }
  *(char *)(unaff_A6 + 0x90) = (char)(uVar1 >> 2);
  return;
}
