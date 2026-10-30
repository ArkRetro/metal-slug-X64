/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0b9d58.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_b9d58(void)

{
  short sVar1;
  ushort uVar2;
  ushort *unaff_A4;
  int unaff_A5;

  *unaff_A4 = *unaff_A4 + 1;
  if (0x1a < *unaff_A4) {
    *unaff_A4 = 0;
  }
  sVar1 = 0x1a;
  uVar2 = *unaff_A4;
  do {
    if (*(char *)(unaff_A5 + 0x46e6 + (int)(short)uVar2) == '\0') break;
    uVar2 = uVar2 + 1;
    if (0x1a < uVar2) {
      uVar2 = 0;
    }
    sVar1 = sVar1 + -1;
  } while (sVar1 != 0);
  *unaff_A4 = uVar2;
  return;
}
