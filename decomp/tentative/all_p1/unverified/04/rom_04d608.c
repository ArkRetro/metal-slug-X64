/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04d608.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4d608(void)

{
  short sVar1;
  undefined4 *unaff_A6;
  bool bVar2;

  sVar1 = candidate_49e8();
  if (((sVar1 == 0) && (*(char *)(unaff_A6[3] + 0x61) == '\0')) &&
     (bVar2 = false, *(char *)(unaff_A6[3] + 0x9f) == '\0')) {
    candidate_4c24();
    if (bVar2) {
      if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) goto LAB_0004d654;
      *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    }
    return;
  }
  *(undefined1 *)(unaff_A6[3] + 0x9f) = 0xff;
LAB_0004d654:
  *(short *)(unaff_A6[3] + 0x86) = *(short *)(unaff_A6[3] + 0x86) + -1;
  candidate_120f8();
  *unaff_A6 = candidate_1c6ee;
  return;
}
