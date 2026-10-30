/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x038148.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_38148(void)

{
  short sVar1;
  char cVar2;
  int unaff_A6;

  *(undefined1 *)(unaff_A6 + 0x9c) = 1;
  cVar2 = *(char *)(unaff_A6 + 0x9c) + -1;
  *(char *)(unaff_A6 + 0x9c) = cVar2;
  if (cVar2 == '\0') {
    sVar1 = candidate_29f2();
    *(short *)(unaff_A6 + 0x8a) = sVar1 + 0x14;
    cVar2 = candidate_29f2();
    *(char *)(unaff_A6 + 0x9c) = cVar2 + '\x01';
  }
  else {
    *(undefined2 *)(unaff_A6 + 0x8a) = 3;
  }
  return;
}
