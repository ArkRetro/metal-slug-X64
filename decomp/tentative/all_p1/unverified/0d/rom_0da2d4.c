/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0da2d4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_da2d4(void)

{
  short sVar1;
  int unaff_A6;
  undefined8 uVar2;

  sVar1 = 0x99;
  do {
    *(undefined1 *)(unaff_A6 + sVar1) = *(undefined1 *)(unaff_A6 + -1 + (int)sVar1);
    sVar1 = sVar1 + -1;
  } while (0x8a < sVar1);
  *(char *)(unaff_A6 + 0x8a) = (char)*(undefined2 *)(unaff_A6 + 0x88);
  candidate_da42c();
  uVar2 = candidate_2e7c();
  *(undefined8 *)(unaff_A6 + 0x30) = uVar2;
  *(undefined2 *)(unaff_A6 + 0x80) = 0;
  if ((*(byte *)(unaff_A6 + 0x99) & 0x10) != 0) {
    *(undefined2 *)(unaff_A6 + 0x80) = 0xffa0;
  }
  return;
}
