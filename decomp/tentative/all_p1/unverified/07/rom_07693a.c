/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07693a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7693a(void)

{
  undefined4 uVar1;
  undefined4 uVar2;
  int in_A0;
  int unaff_A6;

  if ((*(byte *)(in_A0 + 0x84) & 1) == 0) {
    uVar1 = 0xfff80000;
    uVar2 = 0xfff00000;
    if ((*(byte *)(unaff_A6 + 0x15) & 1) != 0) {
      uVar1 = 0x80000;
    }
  }
  else {
    uVar1 = 0;
    uVar2 = 0;
    if (*(char *)(unaff_A6 + 0x70) == '\x10') {
      uVar1 = 0xfff80000;
      uVar2 = 0xfff00000;
      if ((*(byte *)(unaff_A6 + 0x15) & 1) != 0) {
        uVar2 = 0x100000;
      }
    }
    if (*(char *)(unaff_A6 + 0x70) == '\0') {
      uVar1 = 0x80000;
      uVar2 = 0xfff00000;
      if ((*(byte *)(unaff_A6 + 0x15) & 1) == 0) {
        uVar2 = 0x100000;
      }
    }
  }
  *(undefined4 *)(unaff_A6 + 0x28) = uVar1;
  *(undefined4 *)(unaff_A6 + 0x2c) = uVar2;
  return;
}
