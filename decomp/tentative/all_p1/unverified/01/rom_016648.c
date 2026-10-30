/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x016648.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_16648(void)

{
  char cVar1;
  byte bVar2;
  char unaff_D2b;
  char unaff_D7b;
  code *extraout_A1;
  int unaff_A6;

  candidate_1a900();
  cVar1 = *(char *)(unaff_A6 + 0x71);
  *(char *)(unaff_A6 + 0x71) = unaff_D7b;
  if (unaff_D7b != cVar1) {
    *(undefined1 *)(unaff_A6 + 0x70) = 0xff;
  }
  bVar2 = *(char *)(unaff_A6 + 0x70) + 1;
  *(byte *)(unaff_A6 + 0x70) = bVar2;
  if (((bVar2 & 0x1f) != 0) && ((bVar2 & 0x1f) != 0x18)) {
    return;
  }
  while (candidate_3d1b8(), unaff_D2b != '\0') {
    (*extraout_A1)();
  }
  return;
}
