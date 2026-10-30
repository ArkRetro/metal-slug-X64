/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08f4ce.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8f4ce(void)

{
  char cVar1;
  int unaff_A5;
  int unaff_A6;

  cVar1 = (char)*(undefined2 *)(unaff_A5 + -0x7ff2);
  if (cVar1 == *(char *)(unaff_A6 + 0x91)) {
    return;
  }
  *(char *)(unaff_A6 + 0x91) = cVar1;
  candidate_1436a();
  if ((*(byte *)(unaff_A6 + 0x15) & 0x40) == 0) {
    if (-0x20000 < *(int *)(unaff_A6 + 0x34)) {
      *(undefined4 *)(unaff_A6 + 0x34) = 0xfffe0000;
    }
    candidate_143dc();
  }
  else {
    *(undefined4 *)(unaff_A6 + 0x30) = 0;
    *(undefined4 *)(unaff_A6 + 0x34) = 0;
  }
  return;
}
